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

CULY2Encoder::CULY2Encoder(void)
{
}

CULY2Encoder::~CULY2Encoder(void)
{
}

DWORD CULY2Encoder::Compress(ICCOMPRESS *icc, DWORD dwSize)
{
	CFrameBuffer *pCurFrame;
	FRAMEHEADER *pfh;
	BYTE *p;

	if (icc->lpckid != NULL)
		*icc->lpckid = FCC('dcdc');

	pCurFrame = new CFrameBuffer();
	pCurFrame->AddPlane(m_dwYPlaneSize, m_dwYPlaneStride); // Y
	pCurFrame->AddPlane(m_dwCPlaneSize, m_dwCPlaneStride); // U
	pCurFrame->AddPlane(m_dwCPlaneSize, m_dwCPlaneStride); // V
	pfh = (FRAMEHEADER *)icc->lpOutput;
	memset(pfh, 0, sizeof(FRAMEHEADER));
	p = (BYTE *)(pfh + 1);

	ConvertFromYUY2ToPlanar(pCurFrame, (BYTE *)icc->lpInput, m_dwFrameSize);
	p += EncodePlane(p, pCurFrame->GetPlane(0), pCurFrame->GetPlane(0) + m_dwYPlaneSize, m_dwYPlaneStride);
	p += EncodePlane(p, pCurFrame->GetPlane(1), pCurFrame->GetPlane(1) + m_dwCPlaneSize, m_dwCPlaneStride);
	p += EncodePlane(p, pCurFrame->GetPlane(2), pCurFrame->GetPlane(2) + m_dwCPlaneSize, m_dwCPlaneStride);
	memset(p, 0, 8);
	p += 8;

	icc->lpbiOutput->biSizeImage = p - ((BYTE *)icc->lpOutput);
	*icc->lpdwFlags = AVIIF_KEYFRAME;

	delete pCurFrame;

	return ICERR_OK;
}

DWORD CULY2Encoder::CompressBegin(BITMAPINFOHEADER *pbmihIn, BITMAPINFOHEADER *pbmihOut)
{
	m_dwFrameStride = ROUNDUP(pbmihIn->biWidth, 2) * 2;
	m_dwFrameSize = m_dwFrameStride * pbmihIn->biHeight;

	m_dwYPlaneStride = ROUNDUP(pbmihIn->biWidth, 2);
	m_dwYPlaneSize = m_dwYPlaneStride * pbmihIn->biHeight;

	m_dwCPlaneStride = ROUNDUP(pbmihIn->biWidth, 2) / 2;
	m_dwCPlaneSize = m_dwCPlaneStride * pbmihIn->biHeight;

	return ICERR_OK;
}

DWORD CULY2Encoder::CompressEnd(void)
{
	return ICERR_OK;
}

DWORD CULY2Encoder::CompressGetFormat(BITMAPINFOHEADER *pbmihIn, BITMAPINFOHEADER *pbmihOut)
{
	BITMAPINFOHEADER_EXTRA *pbmiheOut;

	if (pbmihOut == NULL)
		return sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFOHEADER_EXTRA);

	memset(pbmihOut, 0, sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFOHEADER_EXTRA));

	pbmihOut->biSize          = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFOHEADER_EXTRA);
	pbmihOut->biWidth         = pbmihIn->biWidth;
	pbmihOut->biHeight        = pbmihIn->biHeight;
	pbmihOut->biPlanes        = 1;
	pbmihOut->biBitCount      = 16;
	pbmihOut->biCompression   = FCC('ULY2');
	pbmihOut->biSizeImage     = pbmihIn->biSizeImage;
	//pbmihOut->biXPelsPerMeter
	//pbmihOut->biYPelsPerMeter
	//pbmihOut->biClrUsed
	//pbmihOut->biClrImportant

	pbmiheOut = (BITMAPINFOHEADER_EXTRA *)(pbmihOut + 1);
	pbmiheOut->dwEncoderVersion  = UTVIDEO_ENCODER_VERSION;
	pbmiheOut->fccOriginalFormat = pbmihIn->biCompression;
	pbmiheOut->wExtraSize        = sizeof(BITMAPINFOHEADER_EXTRA);
	pbmiheOut->wFrameHeaderSize  = sizeof(FRAMEHEADER);
	pbmiheOut->dwFlags0          = BMIHE_FLAGS0_COMPRESS_HUFFMAN_CODE;

	return ICERR_OK;
}

DWORD CULY2Encoder::CompressGetSize(BITMAPINFOHEADER *pbmihIn, BITMAPINFOHEADER *pbmihOut)
{
	return pbmihIn->biSizeImage;
}

DWORD CULY2Encoder::CompressQuery(BITMAPINFOHEADER *pbmihIn, BITMAPINFOHEADER *pbmihOut)
{
	if (pbmihIn->biCompression == FCC('YUY2'))
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
	*(DWORD *)(pDst + 256) = cbEncoded + 256 + 4;
	return cbEncoded + 256 + 4;
}
