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
#include "PackedDecoder.h"
#include "Predict.h"

CPackedDecoder::CPackedDecoder(void)
{
}

CPackedDecoder::~CPackedDecoder(void)
{
}

DWORD CPackedDecoder::Decompress(const ICDECOMPRESS *icd, DWORD dwSize)
{
	BITMAPINFOEXT *pbieIn = (BITMAPINFOEXT *)icd->lpbiInput;
	const BYTE *p;

	m_icd = icd;

	memset(&m_fi, 0, sizeof(FRAMEINFO));
	memcpy(&m_fi, ((BYTE *)icd->lpInput) + pbieIn->bih.biSizeImage - pbieIn->dwFrameInfoSize, pbieIn->dwFrameInfoSize);

	p = (BYTE *)icd->lpInput;
	for (int i = 0; i < 3; i++)
	{
		GenerateHuffmanDecodeTable(&m_hdt[i], p);
		p += 256;
	}
	m_pdwTailOffsetTable = (const DWORD *)p;
	p += sizeof(DWORD) * m_dwDivideCount;
	m_pEncodedDataBase = p;

	for (DWORD nStripIndex = 0; nStripIndex < m_dwDivideCount; nStripIndex++)
		m_ptm->SubmitJob(new CDecodeJob(this, nStripIndex), nStripIndex);
	m_ptm->WaitForJobCompletion();

	icd->lpbiOutput->biSizeImage = m_dwFrameSize;

	return ICERR_OK;
}

DWORD CPackedDecoder::DecompressBegin(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut)
{
	DWORD dwRet;
	BITMAPINFOEXT *pbieIn = (BITMAPINFOEXT *)pbihIn;

	dwRet = DecompressQuery(pbihIn, pbihOut);
	if (dwRet != ICERR_OK)
		return dwRet;

	m_dwNumStrides = pbihIn->biHeight;
	m_dwDivideCount = ((pbieIn->dwFlags0 & BIE_FLAGS0_DIVIDE_COUNT_MASK) >> BIE_FLAGS0_DIVIDE_COUNT_SHIFT) + 1;

	_ASSERT(m_dwDivideCount >= 1 && m_dwDivideCount <= 256);
	_RPT1(_CRT_WARN, "divide count = %d\n", m_dwDivideCount);

	m_bBottomUpFrame = FALSE;
	switch (pbihOut->biCompression)
	{
	case BI_RGB:
		switch (pbihOut->biBitCount)
		{
		case 24:
			m_dwStrideSize = ROUNDUP(pbihOut->biWidth * 3, 4);
			break;
		case 32:
			m_dwStrideSize = pbihOut->biWidth * 4;
			break;
		}
		if (pbihIn->biHeight > 0)
			m_bBottomUpFrame = TRUE;
		break;
	case FCC('YUY2'):
	case FCC('YUYV'):
	case FCC('YUNV'):
	case FCC('UYVY'):
	case FCC('UYNV'):
	case FCC('YVYU'):
	case FCC('VYUY'):
		m_dwStrideSize = ROUNDUP(pbihOut->biWidth, 2) * 2;
		break;
	default:
		return ICERR_BADFORMAT;
	}
	m_dwFrameSize = m_dwStrideSize * m_dwNumStrides;

	m_ptm = new CThreadManager();

	SetDecompressionProperty(pbihIn, pbihOut);

	return ICERR_OK;
}

DWORD CPackedDecoder::DecompressEnd(void)
{
	delete m_ptm;

	return ICERR_OK;
}

DWORD CPackedDecoder::DecompressGetFormat(const BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	if (pbihOut == NULL)
		return sizeof(BITMAPINFOHEADER);

	memset(pbihOut, 0, sizeof(BITMAPINFOHEADER));

	pbihOut->biSize          = sizeof(BITMAPINFOHEADER);
	pbihOut->biWidth         = pbihIn->biWidth;
	pbihOut->biHeight        = pbihIn->biHeight;
	pbihOut->biPlanes        = 1;
	pbihOut->biBitCount      = GetSupportedOutputFormats()[0].nBitCount;
	pbihOut->biCompression   = GetSupportedOutputFormats()[0].fcc;
	pbihOut->biSizeImage     = pbihIn->biSizeImage;
	//pbihOut->biXPelsPerMeter
	//pbihOut->biYPelsPerMeter
	//pbihOut->biClrUsed
	//pbihOut->biClrImportant

	return ICERR_OK;
}

DWORD CPackedDecoder::DecompressQuery(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut)
{
	BITMAPINFOEXT *pbieIn = (BITMAPINFOEXT *)pbihIn;

	if (pbihIn->biCompression != GetInputFCC())
		return ICERR_BADFORMAT;

	if (pbihIn->biSize > sizeof(BITMAPINFOEXT))
		return ICERR_BADFORMAT;
	if (pbieIn->dwFrameInfoSize > sizeof(FRAMEINFO))
		return ICERR_BADFORMAT;
	if (pbieIn->dwFlags0 & BIE_FLAGS0_RESERVED)
		return ICERR_BADFORMAT;

	if (pbihOut == NULL)
		return ICERR_OK;

	if (LookupOutputFormat(pbihOut) != NULL && abs(pbihOut->biHeight) == pbihIn->biHeight && pbihOut->biWidth == pbihIn->biWidth)
		return ICERR_OK;
	else
		return ICERR_BADFORMAT;

	return ICERR_BADFORMAT;
}

const CPackedDecoder::OUTPUTFORMAT *CPackedDecoder::LookupOutputFormat(const BITMAPINFOHEADER *pbihOut)
{
	const OUTPUTFORMAT *pfmts;

	pfmts = GetSupportedOutputFormats();
	for (int i = 0; i < GetNumSupportedOutputFormats(); i++)
	{
		if (pbihOut->biCompression == pfmts[i].fcc && pbihOut->biBitCount == pfmts[i].nBitCount)
		{
			if (pbihOut->biHeight > 0 && pfmts[i].bPositiveHeightAllowed)
				return &pfmts[i];
			if (pbihOut->biHeight < 0 && pfmts[i].bNegativeHeightAllowed)
				return &pfmts[i];
		}
	}

	return NULL;
}

void CPackedDecoder::DecodeProc(DWORD nStripIndex)
{
	DWORD dwNumStrideBegin = m_dwNumStrides *  nStripIndex      / m_dwDivideCount;
	DWORD dwNumStrideEnd   = m_dwNumStrides * (nStripIndex + 1) / m_dwDivideCount;
	DWORD dwSrcOffset;

	if (nStripIndex == 0)
		dwSrcOffset = 0;
	else
		dwSrcOffset = m_pdwTailOffsetTable[nStripIndex - 1];

	m_pfnHuffmanDecodeFirstRawWithAccum(
		((BYTE *)m_icd->lpOutput) + m_dwStrideSize * dwNumStrideBegin,
		((BYTE *)m_icd->lpOutput) + m_dwStrideSize * dwNumStrideEnd,
		m_pEncodedDataBase + dwSrcOffset,
		m_hdt);
}
