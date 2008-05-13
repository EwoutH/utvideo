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
#include "Predict.h"

const CPlanarDecoder::OUTPUTFORMAT CULY2Decoder::m_outfmts[8] = {
	{ FCC('YUY2'), 16, TRUE }, { FCC('YUYV'), 16, TRUE }, { FCC('YUNV'), 16, TRUE },
	{ FCC('UYVY'), 16, TRUE }, { FCC('UYNV'), 16, TRUE },
	{ FCC('YVYU'), 16, TRUE },
	{ FCC('VYUY'), 16, TRUE },
	{ BI_RGB, 32, TRUE },
};

CULY2Decoder::CULY2Decoder(void)
{
}

CULY2Decoder::~CULY2Decoder(void)
{
}

CDecoder *CULY2Decoder::CreateInstance(void)
{
	return new CULY2Decoder();
}

void CULY2Decoder::CalcPlaneSizes(const BITMAPINFOHEADER *pbihOut)
{
	m_dwPlaneStride[0] = ROUNDUP(pbihOut->biWidth, 2);
	m_dwPlaneSize[0]   = m_dwPlaneStride[0] * m_dwNumStrides;

	m_dwPlaneStride[1] = ROUNDUP(pbihOut->biWidth, 2) / 2;
	m_dwPlaneSize[1]   = m_dwPlaneStride[1] * m_dwNumStrides;

	m_dwPlaneStride[2] = m_dwPlaneStride[1];
	m_dwPlaneSize[2]   = m_dwPlaneSize[1];
}

void CULY2Decoder::ConvertFromPlanar(DWORD nBandIndex)
{
	DWORD dwPlaneStrideBegin = m_dwNumStrides *  nBandIndex      / m_dwDivideCount;
	DWORD dwPlaneStrideEnd   = m_dwNumStrides * (nBandIndex + 1) / m_dwDivideCount;
	DWORD dwFrameStrideBegin, dwFrameStrideEnd;

	const BYTE *y, *u, *v;
	BYTE *pDstBegin, *pDstEnd, *p;

	if (!m_bBottomUpFrame)
	{
		dwFrameStrideBegin = dwPlaneStrideBegin;
		dwFrameStrideEnd   = dwPlaneStrideEnd;
	}
	else
	{
		dwFrameStrideBegin = m_dwNumStrides - dwPlaneStrideEnd;
		dwFrameStrideEnd   = m_dwNumStrides - dwPlaneStrideBegin;
	}

	pDstBegin = ((BYTE *)m_icd->lpOutput) + dwFrameStrideBegin * m_dwFrameStride;
	pDstEnd   = ((BYTE *)m_icd->lpOutput) + dwFrameStrideEnd   * m_dwFrameStride;
	y = m_pCurFrame->GetPlane(0) + dwPlaneStrideBegin * m_dwPlaneStride[0];
	u = m_pCurFrame->GetPlane(1) + dwPlaneStrideBegin * m_dwPlaneStride[1];
	v = m_pCurFrame->GetPlane(2) + dwPlaneStrideBegin * m_dwPlaneStride[2];

	switch (m_icd->lpbiOutput->biCompression)
	{
	case FCC('YUY2'):
	case FCC('YUYV'):
	case FCC('YUNV'):
		for (p = pDstBegin; p < pDstEnd; p += 4)
		{
			*p     = *y++;
			*(p+1) = *u++;
			*(p+2) = *y++;
			*(p+3) = *v++;
		}
		break;
	case FCC('UYVY'):
	case FCC('UYNV'):
		for (p = pDstBegin; p < pDstEnd; p += 4)
		{
			*p     = *u++;
			*(p+1) = *y++;
			*(p+2) = *v++;
			*(p+3) = *y++;
		}
		break;
	case FCC('YVYU'):
		for (p = pDstBegin; p < pDstEnd; p += 4)
		{
			*p     = *y++;
			*(p+1) = *v++;
			*(p+2) = *y++;
			*(p+3) = *u++;
		}
		break;
	case FCC('VYUY'):
		for (p = pDstBegin; p < pDstEnd; p += 4)
		{
			*p     = *v++;
			*(p+1) = *y++;
			*(p+2) = *u++;
			*(p+3) = *y++;
		}
		break;
	case BI_RGB:
		switch (m_icd->lpbiOutput->biBitCount)
		{
		case 32:
			for (BYTE *pStrideBegin = pDstEnd - m_dwFrameStride; pStrideBegin >= pDstBegin; pStrideBegin -= m_dwFrameStride)
			{
				BYTE *pStrideEnd = pStrideBegin + m_icd->lpbiOutput->biWidth * 4;
				for (p = pStrideBegin; p < pStrideEnd; p += 8)
				{
//R = 1.164(Y-16)                 + 1.596(Cr-128)
//G = 1.164(Y-16) - 0.391(Cb-128) - 0.813(Cr-128)
//B = 1.164(Y-16) + 2.018(Cb-128)
					*(p+1) = min(max(int((*y-16)*1.164 - (*u-128)*0.391 - (*v-128)*0.813), 0), 255);
					*(p+0) = min(max(int((*y-16)*1.164 + (*u-128)*2.018                 ), 0), 255);
					*(p+2) = min(max(int((*y-16)*1.164                  + (*v-128)*1.596), 0), 255);
					*(p+3) = 0;
					y++;
					if (p+4 < pStrideEnd)
					{
						*(p+5) = min(max(int((*y-16)*1.164 - (*u-128)*0.391 - (*v-128)*0.813), 0), 255);
						*(p+4) = min(max(int((*y-16)*1.164 + (*u-128)*2.018                 ), 0), 255);
						*(p+6) = min(max(int((*y-16)*1.164                  + (*v-128)*1.596), 0), 255);
						*(p+7) = 0;
					}
					y++; u++; v++;
				}
			}
			break;
		}
		break;
	}
}
