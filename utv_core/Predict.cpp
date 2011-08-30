/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "utvideo.h"
#include "Predict.h"
#include "TunedFunc.h"

inline uint8_t median(uint8_t a, uint8_t b, uint8_t c)
{
	return max(min(max(a,b),c),min(a,b));
}

void PredictMedianAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, uint32_t *pCountTable)
{
	if (IS_ALIGNED(pDst, 16) && IS_ALIGNED(pSrcBegin, 16) && IS_ALIGNED(dwStride, 16))
		tfn.pfnPredictMedianAndCount_align16(pDst, pSrcBegin, pSrcEnd, dwStride, pCountTable);
	else
		tfn.pfnPredictMedianAndCount_align1(pDst, pSrcBegin, pSrcEnd, dwStride, pCountTable);
}

void cpp_PredictMedianAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, uint32_t *pCountTable)
{
	const uint8_t *p = pSrcBegin;
	uint8_t *q = pDst;

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

void cpp_PredictLeftAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, uint32_t *pCountTable)
{
	const uint8_t *p = pSrcBegin;
	uint8_t *q = pDst;

	// �ŏ��̃��C���̍ŏ��̃s�N�Z���� 0x80 ��\�����Ă����B
	*q = *p - 0x80;
	pCountTable[*q]++;
	p++;
	q++;

	// �c��̃s�N�Z���� predict left �̖{��
	for (; p < pSrcEnd; p++, q++)
	{
		*q = *p - *(p-1);
		pCountTable[*q]++;
	}
}

void cpp_RestoreMedian(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride)
{
	const uint8_t *p = pSrcBegin;
	uint8_t *q = pDst;

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