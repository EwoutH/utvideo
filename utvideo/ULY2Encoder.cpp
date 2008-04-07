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
#include "ULY2Encoder.h"
#include "utvideo.h"
#include "HuffmanCode.h"
#include "Predict.h"

CULY2Encoder::CULY2Encoder(void)
{
}

CULY2Encoder::~CULY2Encoder(void)
{
}

DWORD CULY2Encoder::Compress(ICCOMPRESS *icc, DWORD dwSize)
{
	CFrameBuffer *pCurFrame;
	CFrameBuffer *pMedianPredicted;
	FRAMEHEADER *pfh;
	BYTE *p;

	if (icc->lpckid != NULL)
		*icc->lpckid = FCC('dcdc');

	pCurFrame = new CFrameBuffer();
	pCurFrame->AddPlane(m_dwYPlaneSize, m_dwYPlaneStride); // Y
	pCurFrame->AddPlane(m_dwCPlaneSize, m_dwCPlaneStride); // U
	pCurFrame->AddPlane(m_dwCPlaneSize, m_dwCPlaneStride); // V

	pMedianPredicted = new CFrameBuffer();
	pMedianPredicted->AddPlane(m_dwYPlaneSize, m_dwYPlaneStride); // Y
	pMedianPredicted->AddPlane(m_dwCPlaneSize, m_dwCPlaneStride); // U
	pMedianPredicted->AddPlane(m_dwCPlaneSize, m_dwCPlaneStride); // V

	pfh = (FRAMEHEADER *)icc->lpOutput;
	memset(pfh, 0, sizeof(FRAMEHEADER));
	p = (BYTE *)(pfh + 1);

	ConvertFromYUY2ToPlanar(pCurFrame, (BYTE *)icc->lpInput, m_dwFrameSize);

	PredictMedian(pMedianPredicted->GetPlane(0), pCurFrame->GetPlane(0), pCurFrame->GetPlane(0) + m_dwYPlaneSize, m_dwYPlaneStride);
	PredictMedian(pMedianPredicted->GetPlane(1), pCurFrame->GetPlane(1), pCurFrame->GetPlane(1) + m_dwCPlaneSize, m_dwCPlaneStride);
	PredictMedian(pMedianPredicted->GetPlane(2), pCurFrame->GetPlane(2), pCurFrame->GetPlane(2) + m_dwCPlaneSize, m_dwCPlaneStride);

	p += EncodePlane(p, pMedianPredicted->GetPlane(0), pMedianPredicted->GetPlane(0) + m_dwYPlaneSize, m_dwYPlaneStride);
	p += EncodePlane(p, pMedianPredicted->GetPlane(1), pMedianPredicted->GetPlane(1) + m_dwCPlaneSize, m_dwCPlaneStride);
	p += EncodePlane(p, pMedianPredicted->GetPlane(2), pMedianPredicted->GetPlane(2) + m_dwCPlaneSize, m_dwCPlaneStride);
	memset(p, 0, 8);
	p += 8;

	pfh->dwFlags0 = FH_FLAGS0_INTRAFRAME_PREDICT_MEDIAN;

	icc->lpbiOutput->biSizeImage = p - ((BYTE *)icc->lpOutput);
	*icc->lpdwFlags = AVIIF_KEYFRAME;

	delete pCurFrame;
	delete pMedianPredicted;

	return ICERR_OK;
}

DWORD CULY2Encoder::CompressBegin(BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	m_dwFrameStride = ROUNDUP(pbihIn->biWidth, 2) * 2;
	m_dwFrameSize = m_dwFrameStride * pbihIn->biHeight;

	m_dwYPlaneStride = ROUNDUP(pbihIn->biWidth, 2);
	m_dwYPlaneSize = m_dwYPlaneStride * pbihIn->biHeight;

	m_dwCPlaneStride = ROUNDUP(pbihIn->biWidth, 2) / 2;
	m_dwCPlaneSize = m_dwCPlaneStride * pbihIn->biHeight;

	return ICERR_OK;
}

DWORD CULY2Encoder::CompressEnd(void)
{
	return ICERR_OK;
}

DWORD CULY2Encoder::CompressGetFormat(BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	BITMAPINFOHEADER_EXTRA *pbiheOut;

	if (pbihOut == NULL)
		return sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFOHEADER_EXTRA);

	memset(pbihOut, 0, sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFOHEADER_EXTRA));

	pbihOut->biSize          = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFOHEADER_EXTRA);
	pbihOut->biWidth         = pbihIn->biWidth;
	pbihOut->biHeight        = pbihIn->biHeight;
	pbihOut->biPlanes        = 1;
	pbihOut->biBitCount      = 16;
	pbihOut->biCompression   = FCC('ULY2');
	pbihOut->biSizeImage     = pbihIn->biSizeImage;
	//pbihOut->biXPelsPerMeter
	//pbihOut->biYPelsPerMeter
	//pbihOut->biClrUsed
	//pbihOut->biClrImportant

	pbiheOut = (BITMAPINFOHEADER_EXTRA *)(pbihOut + 1);
	pbiheOut->dwEncoderVersion  = UTVIDEO_ENCODER_VERSION;
	pbiheOut->fccOriginalFormat = pbihIn->biCompression;
	pbiheOut->dwFrameHeaderSize = sizeof(FRAMEHEADER);
	pbiheOut->dwFlags0          = BMIHE_FLAGS0_COMPRESS_HUFFMAN_CODE;

	return ICERR_OK;
}

DWORD CULY2Encoder::CompressGetSize(BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	return pbihIn->biSizeImage;
}

DWORD CULY2Encoder::CompressQuery(BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	if (pbihIn->biCompression == FCC('YUY2'))
		return ICERR_OK;
	else
		return ICERR_BADFORMAT;
}

void CULY2Encoder::ConvertFromYUY2ToPlanar(CFrameBuffer *pBuffer, const BYTE *pSrcBegin, DWORD dwFrameSize)
{
	BYTE *y, *u, *v;
	const BYTE *pSrcEnd, *p;

	pSrcEnd = pSrcBegin + dwFrameSize;
	y = pBuffer->GetPlane(0);
	u = pBuffer->GetPlane(1);
	v = pBuffer->GetPlane(2);

	for (p = pSrcBegin; p < pSrcEnd; p += 4)
	{
		*y++ = *p;
		*u++ = *(p+1);
		*y++ = *(p+2);
		*v++ = *(p+3);
	}
}

DWORD CULY2Encoder::EncodePlane(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwFrameStride)
{
	const BYTE *p;
	DWORD count[256];
	BYTE *pCodeLengthTable;
	HUFFMAN_ENCODE_TABLE het;
	DWORD cbEncoded;

	pCodeLengthTable = pDst;

	for (int i = 0; i < 256; i++)
		count[i] = 0;

	for (p = pSrcBegin; p < pSrcEnd; p++)
		count[*p]++;

	GenerateHuffmanCodeLengthTable(pCodeLengthTable, count);
	GenerateHuffmanEncodeTable(&het, pCodeLengthTable);
	cbEncoded = HuffmanEncode(pDst + 256 + 4, pSrcBegin, pSrcEnd, &het);
	*(DWORD *)(pDst + 256) = cbEncoded;
	return cbEncoded + 256 + 4;
}
