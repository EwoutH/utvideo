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
//#include <windows.h>
//#include <algorithm>
//using namespace std;
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

bool generate_code_length(BYTE *codelen, const struct hufftree *node, BYTE curlen)
{
	if (node->left == NULL) {
		codelen[node->symbol] = curlen;
		return (curlen > 24);
	} else {
		return
			generate_code_length(codelen, node->left, curlen+1) ||
			generate_code_length(codelen, node->right, curlen+1);
	}
}

static void GenerateLengthLimitedHuffmanCodeLengthTable(BYTE *pCodeLengthTable)
{
	// �Ƃ肠��������œ�����B
	memset(pCodeLengthTable, 8, 256);
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

	if (generate_code_length(pCodeLengthTable, huffsort[0], 0))
		GenerateLengthLimitedHuffmanCodeLengthTable(pCodeLengthTable);
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

// IA-32 �� BSR ����
// �{���� BSR ���߂ł͓��͂� 0 �̏ꍇ�ɏo�͂��s��ɂȂ�B
// �����ł́i�o�O�����o����̂�e�Ղɂ��邽�߂Ɂj���b�Z�[�W�{�b�N�X���J�������Ƃɕs��Ȓl��Ԃ��Ă���B
inline int bsr(DWORD curcode)
{
	for (int i = 31; i >= 0; i--)
		if (curcode & (1 << i))
			return i;
	MessageBox(NULL, "BSR NullPo", "Ut Video Codec", MB_ICONSTOP|MB_OK);
	return rand() % 32;
}

void GenerateHuffmanDecodeTable(HUFFMAN_DECODE_TABLE *pDecodeTable, const BYTE *pCodeLengthTable)
{
	struct CODE_LENGTH_SORT cls[256];
	int nLastIndex;
	int j;
	int base;
	DWORD curcode;
	int nextfillidx;
	int prevbsrval;

	for (int i = 0; i < 256; i++)
	{
		cls[i].symbol = i;
		cls[i].codelen = pCodeLengthTable[i];
	}

	sort(cls, cls+256, codelengthsort_lt);

	if (cls[0].codelen == 0)
	{
		memset(pDecodeTable, 0, sizeof(HUFFMAN_DECODE_TABLE));
		pDecodeTable->dwSymbol[0] = cls[0].symbol;
		return;
	}

	// �ł����������������V���{���� cls ��ł̈ʒu�����߂�
	for (nLastIndex = 255; nLastIndex >= 0; nLastIndex--)
	{
		if (cls[nLastIndex].codelen != 255)
			break;
	}

	curcode = 1;
	j = 0;
	base = 0;
	nextfillidx = 0;
	prevbsrval = 0;
	for (int i = nLastIndex; i >= 0; i--)
	{
		int bsrval = bsr(curcode);
		if (bsrval != prevbsrval)
		{
			base = nextfillidx - (curcode >> (32 - cls[i].codelen));
		}
		for (; j <= bsrval; j++)
		{
			pDecodeTable->nCodeShift[j] = 32 - cls[i].codelen;
			pDecodeTable->dwSymbolBase[j] = base;
		}
		for (int k = 0; k < (1 << (32 - pDecodeTable->nCodeShift[bsrval] - cls[i].codelen)); k++)
		{
			pDecodeTable->dwSymbol[nextfillidx] = cls[i].symbol;
			pDecodeTable->nCodeLength[nextfillidx] = cls[i].codelen;
			nextfillidx++;
		}
		curcode += 0x80000000 >> (cls[i].codelen - 1);
		prevbsrval = bsrval;
	}
}

DWORD HuffmanEncode(BYTE *pDstBegin, const BYTE *pSrcBegin, const BYTE *pSrcEnd, const HUFFMAN_ENCODE_TABLE *pEncodeTable)
{
	int nBits;
	DWORD dwEncoded;
	DWORD *pDst;
	const BYTE *p;

	if (pEncodeTable->dwTableMux[0] == 0)
		return 0;

	nBits = 0;
	dwEncoded = 0;
	pDst = (DWORD *)pDstBegin;

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

	return ((BYTE *)pDst) - pDstBegin;
}

void HuffmanDecode(BYTE *pDstBegin, BYTE *pDstEnd, const BYTE *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable)
{
	int nBits;
	DWORD *pSrc;
	BYTE *p;
	DWORD code;

	if (pDecodeTable->nCodeLength[0] == 0)
	{
		memset(pDstBegin, pDecodeTable->dwSymbol[0], pDstEnd-pDstBegin);
		return;
	}

	nBits = 0;
	pSrc = (DWORD *)pSrcBegin;

	for (p = pDstBegin; p < pDstEnd; p++)
	{
		if (nBits == 0)
			code = (*pSrc) | 0x00000001;
		else
			code = ((*pSrc) << nBits) | ((*(pSrc+1)) >> (32 - nBits)) | 0x00000001;
		int bsrval = bsr(code);
		int codeshift = pDecodeTable->nCodeShift[bsrval];
		code >>= codeshift;
		DWORD symbol = pDecodeTable->dwSymbol[pDecodeTable->dwSymbolBase[bsrval] + code];
		*p = symbol;
		int codelen = pDecodeTable->nCodeLength[pDecodeTable->dwSymbolBase[bsrval] + code];
		nBits += codelen;
		if (nBits >= 32)
		{
			nBits -= 32;
			pSrc++;
		}
	}
}
