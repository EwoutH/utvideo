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
#include "utvideo.h"
#include "Predict.h"
#include "TunedFunc.h"

inline BYTE median(BYTE a, BYTE b, BYTE c)
{
	return max(min(max(a,b),c),min(a,b));
}

void PredictMedian(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride)
{
	if (IS_ALIGNED(pDst, 16) && IS_ALIGNED(pSrcBegin, 16) && IS_ALIGNED(dwStride, 16))
		tfn.pfnPredictMedian_align16(pDst, pSrcBegin, pSrcEnd, dwStride);
	else
		cpp_PredictMedian(pDst, pSrcBegin, pSrcEnd, dwStride);
}

void PredictMedianAndCount(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD *pCountTable)
{
	if (IS_ALIGNED(pDst, 16) && IS_ALIGNED(pSrcBegin, 16) && IS_ALIGNED(dwStride, 16))
		tfn.pfnPredictMedianAndCount_align16(pDst, pSrcBegin, pSrcEnd, dwStride, pCountTable);
	else
		tfn.pfnPredictMedianAndCount_align1(pDst, pSrcBegin, pSrcEnd, dwStride, pCountTable);
}

void cpp_PredictMedian(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride)
{
	const BYTE *p = pSrcBegin;
	BYTE *q = pDst;

	// �ŏ��̃��C���̍ŏ��̃s�N�Z���� 0x80 ��\�����Ă����B
	*q++ = *p++ - 0x80;

	// �ŏ��̃��C���̎c��̃s�N�Z���� predict left �Ɠ����B
	for (; p < pSrcBegin + dwStride; p++, q++)
	{
		*q = *p - *(p - 1);
	}

	// 1 ���C�������Ȃ��ꍇ�͂����ŏI���B
	if (p == pSrcEnd)
		return;

	// ���̃��C���̍ŏ��̃s�N�Z���� predict above�B
	// �������Ă����ƃA�Z���u�����������ɏ������኱�ȒP�ɂȂ�B
	*q = *p - *(p - dwStride);
	p++;
	q++;

	// �c��̃s�N�Z���� predict median �̖{��
	for (; p < pSrcEnd; p++, q++)
	{
		*q = *p - median(*(p - dwStride), *(p - 1), *(p - dwStride) + *(p - 1) - *(p - 1 - dwStride));
	}
}

void cpp_PredictMedianAndCount(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD *pCountTable)
{
	const BYTE *p = pSrcBegin;
	BYTE *q = pDst;

	// �ŏ��̃��C���̍ŏ��̃s�N�Z���� 0x80 ��\�����Ă����B
	*q = *p - 0x80;
	pCountTable[*q]++;
	p++;
	q++;

	// �ŏ��̃��C���̎c��̃s�N�Z���� predict left �Ɠ����B
	for (; p < pSrcBegin + dwStride; p++, q++)
	{
		*q = *p - *(p - 1);
		pCountTable[*q]++;
	}

	// 1 ���C�������Ȃ��ꍇ�͂����ŏI���B
	if (p == pSrcEnd)
		return;

	// ���̃��C���̍ŏ��̃s�N�Z���� predict above�B
	// �������Ă����ƃA�Z���u�����������ɏ������኱�ȒP�ɂȂ�B
	*q = *p - *(p - dwStride);
	pCountTable[*q]++;
	p++;
	q++;

	// �c��̃s�N�Z���� predict median �̖{��
	for (; p < pSrcEnd; p++, q++)
	{
		*q = *p - median(*(p - dwStride), *(p - 1), *(p - dwStride) + *(p - 1) - *(p - 1 - dwStride));
		pCountTable[*q]++;
	}
}

void cpp_RestoreMedian(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride)
{
	const BYTE *p = pSrcBegin;
	BYTE *q = pDst;

	*q++ = *p++ + 0x80;

	for (; p < pSrcBegin + dwStride; p++, q++)
	{
		*q = *p + *(q - 1);
	}

	if (p == pSrcEnd)
		return;

	*q = *p + *(q - dwStride);
	p++;
	q++;

	for (; p < pSrcEnd; p++, q++)
	{
		*q = *p + median(*(q - dwStride), *(q - 1), *(q - dwStride) + *(q - 1) - *(q - 1 - dwStride));
	}
}
