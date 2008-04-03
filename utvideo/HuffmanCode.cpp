/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */
/*
 * Ut Video Codec Suite
 * Copyright (C) 2008  UMEZAWA Takeshi
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * 
 * Ut Video Codec Suite
 * Copyright (C) 2008  �~�V �Ўu
 * 
 * ���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t
 * �g�E�F�A���c�ɂ���Ĕ��s���ꂽ GNU ��ʌ��O���p�����_��(�o�[�W��
 * ��2���A��]�ɂ���Ă͂���ȍ~�̃o�[�W�����̂����ǂꂩ)�̒�߂����
 * �̉��ōĔЕz�܂��͉��ς��邱�Ƃ��ł��܂��B
 * 
 * ���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖���
 * ��* �ł��B���Ɖ\���̕ۏ؂����̖ړI�ւ̓K�����́A���O�Ɏ����ꂽ
 * ���̂��܂ߑS�����݂��܂���B�ڂ�����GNU ��ʌ��O���p�����_�񏑂���
 * �����������B
 * 
 * ���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU ��ʌ��O���p�����_�񏑂̕�������
 * �ꕔ�󂯎�����͂��ł��B�����󂯎���Ă��Ȃ���΁A�t���[�\�t�g�E�F
 * �A���c�܂Ő������Ă�������(����� the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA)�B
 */

#include "StdAfx.h"
#include "HuffmanCode.h"

struct hufftree {
	struct hufftree *left;
	struct hufftree *right;
	DWORD count;
	DWORD symbol;	// BYTE �ł͂Ȃ��V���{�������邱�Ƃ�����
};

inline bool hufftree_gt(const struct hufftree *a, const struct hufftree *b)
{
	return (a->count > b->count);
}

void generate_code_length(BYTE *codelen, const struct hufftree *node, BYTE curlen)
{
	if (node->left == NULL) {
		codelen[node->symbol] = curlen;
	} else {
		generate_code_length(codelen, node->left, curlen+1);
		generate_code_length(codelen, node->right, curlen+1);
	}
}

void GenerateHuffmanCodeLengthTable(BYTE *pCodeLengthTable, const DWORD *pCountTable)
{
	struct hufftree *huffsort[256];
	struct hufftree huffleaf[256];
	struct hufftree huffnode[256];
	int nsym;

	nsym = 0;
	for (int i = 0; i < 256; i++) {
		if (pCountTable[i] != 0) {
			huffleaf[nsym].left = NULL;
			huffleaf[nsym].right = NULL;
			huffleaf[nsym].count = pCountTable[i];
			huffleaf[nsym].symbol = i;
			huffsort[nsym] = &huffleaf[nsym];
			nsym++;
		}
		else
			pCodeLengthTable[i] = 255;
	}

	sort(huffsort, huffsort+nsym, hufftree_gt);
	for (int i = nsym - 2; i >= 0; i--) {
		huffnode[i].left = huffsort[i];
		huffnode[i].right = huffsort[i+1];
		huffnode[i].count = huffsort[i]->count + huffsort[i+1]->count;

		struct hufftree **insptr = upper_bound(huffsort, huffsort+i, &huffnode[i], hufftree_gt);
		struct hufftree **movptr;
		for (movptr = huffsort+i-1; movptr >= insptr; movptr--)
			*(movptr+1) = *movptr;
		*insptr = &huffnode[i];
	}

	generate_code_length(pCodeLengthTable, huffsort[0], 0);
}

struct CODE_LENGTH_SORT
{
	DWORD symbol;
	BYTE codelen;
};

inline bool codelengthsort_lt(struct CODE_LENGTH_SORT &a, struct CODE_LENGTH_SORT &b)
{
	if (a.codelen != b.codelen)
		return a.codelen < b.codelen;
	else
		return a.symbol < b.symbol;
}

void GenerateHuffmanEncodeTable(HUFFMAN_ENCODE_TABLE *pEncodeTable, const BYTE *pCodeLengthTable)
{
	struct CODE_LENGTH_SORT cls[256];
	DWORD curcode;

	for (int i = 0; i < 256; i++)
	{
		cls[i].symbol = i;
		cls[i].codelen = pCodeLengthTable[i];
	}

	sort(cls, cls+256, codelengthsort_lt);

	if (cls[0].codelen == 0)
	{
		memset(pEncodeTable, 0, sizeof(HUFFMAN_ENCODE_TABLE));
		return;
	}

	curcode = 0;
	for (int i = 255; i >= 0; i--)
	{
		if (cls[i].codelen == 255)
			continue;
		pEncodeTable->dwTableMux[cls[i].symbol] = curcode | cls[i].codelen;
		curcode += 0x80000000 >> (cls[i].codelen - 1);
	}
}

void GenerateHuffmanDecodeTable(HUFFMAN_DECODE_TABLE *pDecodeTable, const BYTE *pCodeLengthTable)
{
}

DWORD HuffmanEncode(BYTE *pDst_, const BYTE *pSrcBegin, const BYTE *pSrcEnd, const HUFFMAN_ENCODE_TABLE *pEncodeTable)
{
	int nBits;
	DWORD dwEncoded;
	DWORD *pDst;
	const BYTE *p;

	nBits = 0;
	dwEncoded = 0;
	pDst = (DWORD *)pDst_;

	for (p = pSrcBegin; p < pSrcEnd; p++)
	{
		int nCurBits = pEncodeTable->dwTableMux[*p] & 0xff;
		DWORD dwCurEncoded = pEncodeTable->dwTableMux[*p] & 0xffffff00;

		dwEncoded |= dwCurEncoded >> nBits;
		nBits += nCurBits;
		if (nBits >= 32)
		{
			*pDst++ = dwEncoded;
			nBits -= 32;
			dwEncoded = dwCurEncoded << (nCurBits - nBits);
		}
	}
	if (nBits != 0)
		*pDst++ = dwEncoded;

	return ((BYTE *)pDst) - pDst_;
}
