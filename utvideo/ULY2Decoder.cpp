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
#include "ULY2Decoder.h"
#include "HuffmanCode.h"
#include "Predict.h"

CULY2Decoder::CULY2Decoder(void)
{
}

CULY2Decoder::~CULY2Decoder(void)
{
}

DWORD CULY2Decoder::Decompress(ICDECOMPRESS *icd, DWORD dwSize)
{
	BITMAPINFOEXT *pbieIn = (BITMAPINFOEXT *)icd->lpbiInput;
	CFrameBuffer *pCurFrame;
	CFrameBuffer *pDecodedFrame;
	FRAMEINFO fi;
	const BYTE *p;
	DWORD dwDivideCount;

	pDecodedFrame = new CFrameBuffer();
	pDecodedFrame->AddPlane(m_dwPlaneSize[0], m_dwPlaneStride[0]);
	pDecodedFrame->AddPlane(m_dwPlaneSize[1], m_dwPlaneStride[1]);
	pDecodedFrame->AddPlane(m_dwPlaneSize[2], m_dwPlaneStride[2]);

	memset(&fi, 0, sizeof(FRAMEINFO));
	memcpy(&fi, ((BYTE *)icd->lpInput) + pbieIn->bih.biSizeImage - pbieIn->dwFrameInfoSize, pbieIn->dwFrameInfoSize);

	dwDivideCount =	(fi.dwFlags0 & FI_FLAGS0_DIVIDE_COUNT_MASK) + 1;
	_RPT1(_CRT_WARN, "divide count = %d\n", dwDivideCount);

	p = (BYTE *)icd->lpInput;
	p += DecodePlane(pDecodedFrame->GetPlane(0), pDecodedFrame->GetPlane(0) + m_dwPlaneSize[0], p, m_dwPlaneStride[0]);
	p += DecodePlane(pDecodedFrame->GetPlane(1), pDecodedFrame->GetPlane(1) + m_dwPlaneSize[1], p, m_dwPlaneStride[1]);
	p += DecodePlane(pDecodedFrame->GetPlane(2), pDecodedFrame->GetPlane(2) + m_dwPlaneSize[2], p, m_dwPlaneStride[2]);

	switch (fi.dwFlags0 & FI_FLAGS0_INTRAFRAME_PREDICT_MASK)
	{
	case FI_FLAGS0_INTRAFRAME_PREDICT_NONE:
		pCurFrame = pDecodedFrame;
		break;
	case FI_FLAGS0_INTRAFRAME_PREDICT_MEDIAN:
		{
			pCurFrame = new CFrameBuffer();
			pCurFrame->AddPlane(m_dwPlaneSize[0], m_dwPlaneStride[0]);
			pCurFrame->AddPlane(m_dwPlaneSize[1], m_dwPlaneStride[1]);
			pCurFrame->AddPlane(m_dwPlaneSize[2], m_dwPlaneStride[2]);
			for (DWORD i = 0; i < dwDivideCount; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					DWORD dwPlaneBegin = (m_dwNumStrides *  i      / dwDivideCount) * m_dwPlaneStride[j];
					DWORD dwPlaneEnd   = (m_dwNumStrides * (i + 1) / dwDivideCount) * m_dwPlaneStride[j];

					RestoreMedian(pCurFrame->GetPlane(j) + dwPlaneBegin, pDecodedFrame->GetPlane(j) + dwPlaneBegin, pDecodedFrame->GetPlane(j) + dwPlaneEnd, m_dwPlaneStride[j]);
				}
			}
			delete pDecodedFrame;
		}
		break;
	default:
		return ICERR_ABORT;
	}
	ConvertFromPlanarToYUY2((BYTE *)icd->lpOutput, pCurFrame, m_dwFrameSize);

	icd->lpbiOutput->biSizeImage = m_dwFrameSize;

	delete pCurFrame;

	return ICERR_OK;
}

DWORD CULY2Decoder::DecompressBegin(BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	m_dwNumStrides = pbihIn->biHeight;

	m_dwFrameStride = ROUNDUP(pbihIn->biWidth, 2) * 2;
	m_dwFrameSize = m_dwFrameStride * pbihIn->biHeight;

	m_dwPlaneStride[0] = ROUNDUP(pbihIn->biWidth, 2);
	m_dwPlaneSize[0]   = m_dwPlaneStride[0] * pbihIn->biHeight;

	m_dwPlaneStride[1] = ROUNDUP(pbihIn->biWidth, 2) / 2;
	m_dwPlaneSize[1]   = m_dwPlaneStride[1] * pbihIn->biHeight;

	m_dwPlaneStride[2] = m_dwPlaneStride[1];
	m_dwPlaneSize[2]   = m_dwPlaneSize[1];

	return ICERR_OK;
}

DWORD CULY2Decoder::DecompressEnd(void)
{
	return ICERR_OK;
}

DWORD CULY2Decoder::DecompressGetFormat(BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	if (pbihOut == NULL)
		return sizeof(BITMAPINFOHEADER);

	memset(pbihOut, 0, sizeof(BITMAPINFOHEADER));

	pbihOut->biSize          = sizeof(BITMAPINFOHEADER);
	pbihOut->biWidth         = pbihIn->biWidth;
	pbihOut->biHeight        = pbihIn->biHeight;
	pbihOut->biPlanes        = 1;
	pbihOut->biBitCount      = 16;
	pbihOut->biCompression   = FCC('YUY2');
	pbihOut->biSizeImage     = pbihIn->biSizeImage;
	//pbihOut->biXPelsPerMeter
	//pbihOut->biYPelsPerMeter
	//pbihOut->biClrUsed
	//pbihOut->biClrImportant

	return ICERR_OK;
}

DWORD CULY2Decoder::DecompressQuery(BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	BITMAPINFOEXT *pbieIn = (BITMAPINFOEXT *)pbihIn;

	if (pbihIn->biCompression != FCC('ULY2'))
		return ICERR_BADFORMAT;

	if (pbihOut != NULL && pbihOut->biCompression != FCC('YUY2'))
		return ICERR_BADFORMAT;

	if (pbihIn->biSize > sizeof(BITMAPINFOEXT))
		return ICERR_BADFORMAT;
	if (pbieIn->dwFrameInfoSize > sizeof(FRAMEINFO))
		return ICERR_BADFORMAT;
	if (pbieIn->dwFlags0 & BIE_FLAGS0_RESERVED)
		return ICERR_BADFORMAT;

	return ICERR_OK;
}

void CULY2Decoder::ConvertFromPlanarToYUY2(BYTE *pDstBegin, CFrameBuffer *pBuffer, DWORD dwFrameSize)
{
	const BYTE *y, *u, *v;
	BYTE *pDstEnd, *p;

	pDstEnd = pDstBegin + dwFrameSize;
	y = pBuffer->GetPlane(0);
	u = pBuffer->GetPlane(1);
	v = pBuffer->GetPlane(2);

	for (p = pDstBegin; p < pDstEnd; p += 4)
	{
		*p     = *y++;
		*(p+1) = *u++;
		*(p+2) = *y++;
		*(p+3) = *v++;
	}
}

DWORD CULY2Decoder::DecodePlane(BYTE *pDstBegin, BYTE *pDstEnd, const BYTE *pSrcBegin, DWORD dwFrameStride)
{
	const BYTE *pCodeLengthTable;
	HUFFMAN_DECODE_TABLE hdt;
	DWORD cbEncoded;

	pCodeLengthTable = pSrcBegin;
	cbEncoded = *(DWORD *)(pSrcBegin + 256);

	GenerateHuffmanDecodeTable(&hdt, pCodeLengthTable);
	HuffmanDecode(pDstBegin, pDstEnd, pSrcBegin + 256 + 4, &hdt);
	return cbEncoded + 256 + 4;
}
