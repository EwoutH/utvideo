/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */
/*
 * Ut Video Codec Suite
 * Copyright (C) 2008-2009  UMEZAWA Takeshi
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
 * Copyright (C) 2008-2009  �~�V �Ўu
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
#include "ULY0Encoder.h"
#include "Predict.h"
#include "resource.h"

const CULY0Encoder::INPUTFORMAT CULY0Encoder::m_infmts[] = {
	{ FCC('YV12'), 12 },
	{ FCC('YUY2'), 16 }, { FCC('YUYV'), 16 }, { FCC('YUNV'), 16 },
	{ FCC('UYVY'), 16 }, { FCC('UYNV'), 16 },
	{ FCC('YVYU'), 16 },
	{ FCC('VYUY'), 16 },
	{ BI_RGB, 32 },
	{ BI_RGB, 24 },
};

CULY0Encoder::CULY0Encoder(void)
{
	memset(&m_ec, 0, sizeof(ENCODERCONF));
	m_ec.dwFlags0 = (CThreadManager::GetNumProcessors() - 1) | EC_FLAGS0_INTRAFRAME_PREDICT_LEFT;
}

CULY0Encoder::~CULY0Encoder(void)
{
}

int CULY0Encoder::GetNumSupportedInputFormats(void)
{
	return _countof(m_infmts);
}

CEncoder *CULY0Encoder::CreateInstance(void)
{
	return new CULY0Encoder();
}

void CULY0Encoder::CalcPlaneSizes(const BITMAPINFOHEADER *pbih)
{
	m_dwPlaneSize[0]          = pbih->biWidth * pbih->biHeight;
	m_dwPlaneSize[1]          = pbih->biWidth * pbih->biHeight / 4;
	m_dwPlaneSize[2]          = pbih->biWidth * pbih->biHeight / 4;

	m_dwPlaneWidth[0]         = pbih->biWidth;
	m_dwPlaneWidth[1]         = pbih->biWidth / 2;
	m_dwPlaneWidth[2]         = pbih->biWidth / 2;

	m_dwPlaneStripeSize[0]    = pbih->biWidth * 2;
	m_dwPlaneStripeSize[1]    = pbih->biWidth / 2;
	m_dwPlaneStripeSize[2]    = pbih->biWidth / 2;

	m_dwPlanePredictStride[0] = pbih->biWidth;
	m_dwPlanePredictStride[1] = pbih->biWidth / 2;
	m_dwPlanePredictStride[2] = pbih->biWidth / 2;
}

void CULY0Encoder::ConvertToPlanar(DWORD nBandIndex)
{
	DWORD dwMacroStrideBegin = m_dwNumStripes *  nBandIndex      / m_dwDivideCount;
	DWORD dwMacroStrideEnd   = m_dwNumStripes * (nBandIndex + 1) / m_dwDivideCount;

	BYTE *pDstYBegin = m_pCurFrame->GetPlane(0) + dwMacroStrideBegin * m_icc->lpbiInput->biWidth * 2;
	BYTE *pDstUBegin = m_pCurFrame->GetPlane(1) + dwMacroStrideBegin * m_icc->lpbiInput->biWidth / 2;
	BYTE *pDstVBegin = m_pCurFrame->GetPlane(2) + dwMacroStrideBegin * m_icc->lpbiInput->biWidth / 2;

	switch (m_icc->lpbiInput->biCompression)
	{
	case FCC('YV12'):
		{
			const BYTE *pSrcYBegin, *pSrcVBegin, *pSrcUBegin;

			pSrcYBegin = ((BYTE *)m_icc->lpInput);
			pSrcVBegin = pSrcYBegin + m_icc->lpbiInput->biWidth * m_icc->lpbiInput->biHeight;
			pSrcUBegin = pSrcVBegin + m_icc->lpbiInput->biWidth * m_icc->lpbiInput->biHeight / 4;

			pSrcYBegin += dwMacroStrideBegin * m_icc->lpbiInput->biWidth * 2;
			pSrcVBegin += dwMacroStrideBegin * m_icc->lpbiInput->biWidth / 2;
			pSrcUBegin += dwMacroStrideBegin * m_icc->lpbiInput->biWidth / 2;

			memcpy(pDstYBegin, pSrcYBegin, (dwMacroStrideEnd - dwMacroStrideBegin) * m_icc->lpbiInput->biWidth * 2);
			memcpy(pDstUBegin, pSrcUBegin, (dwMacroStrideEnd - dwMacroStrideBegin) * m_icc->lpbiInput->biWidth / 2);
			memcpy(pDstVBegin, pSrcVBegin, (dwMacroStrideEnd - dwMacroStrideBegin) * m_icc->lpbiInput->biWidth / 2);
		}
		break;
	default:
		{
			BYTE *y, *u, *v;
			const BYTE *pSrcBegin, *pSrcEnd;

			y = pDstYBegin;
			u = pDstUBegin;
			v = pDstVBegin;

			switch (m_icc->lpbiInput->biCompression)
			{
			case FCC('YVYU'):
				swap(pDstUBegin, pDstVBegin);
				swap(u, v);
				/* FALLTHROUGH */
			case FCC('YUY2'):
			case FCC('YUYV'):
			case FCC('YUNV'):
				{
					DWORD dwSrcStride = m_icc->lpbiInput->biWidth * 2;
					DWORD dwYStride = m_icc->lpbiInput->biWidth;

					pSrcBegin = ((BYTE *)m_icc->lpInput) + dwMacroStrideBegin * dwSrcStride * 2;
					pSrcEnd   = ((BYTE *)m_icc->lpInput) + dwMacroStrideEnd   * dwSrcStride * 2;

					for (const BYTE *pStrideBegin = pSrcBegin; pStrideBegin < pSrcEnd; pStrideBegin += dwSrcStride * 2)
					{
						const BYTE *pStrideEnd = pStrideBegin + dwSrcStride;
						for (const BYTE *p = pStrideBegin; p < pStrideEnd; p += 4)
						{
							const BYTE *q = p + dwSrcStride;
							*(y+0) = *p;
							*(y+1) = *(p+2);
							*(y+dwYStride+0) = *q;
							*(y+dwYStride+1) = *(q+2);
							*u = (*(p+1) + *(q+1)) / 2;
							*v = (*(p+3) + *(q+3)) / 2;

							y+=2; u++; v++;
						}
						y += dwYStride;
					}
				}
				break;
			case FCC('VYUY'):
				swap(pDstUBegin, pDstVBegin);
				swap(u, v);
				/* FALLTHROUGH */
			case FCC('UYVY'):
			case FCC('UYNV'):
				{
					DWORD dwSrcStride = m_icc->lpbiInput->biWidth * 2;
					DWORD dwYStride = m_icc->lpbiInput->biWidth;

					pSrcBegin = ((BYTE *)m_icc->lpInput) + dwMacroStrideBegin * dwSrcStride * 2;
					pSrcEnd   = ((BYTE *)m_icc->lpInput) + dwMacroStrideEnd   * dwSrcStride * 2;

					for (const BYTE *pStrideBegin = pSrcBegin; pStrideBegin < pSrcEnd; pStrideBegin += dwSrcStride * 2)
					{
						const BYTE *pStrideEnd = pStrideBegin + dwSrcStride;
						for (const BYTE *p = pStrideBegin; p < pStrideEnd; p += 4)
						{
							const BYTE *q = p + dwSrcStride;
							*(y+0) = *(p+1);
							*(y+1) = *(p+3);
							*(y+dwYStride+0) = *(q+1);
							*(y+dwYStride+1) = *(q+3);
							*u = (*(p+0) + *(q+0)) / 2;
							*v = (*(p+2) + *(q+2)) / 2;

							y+=2; u++; v++;
						}
						y += dwYStride;
					}
				}
				break;
			case BI_RGB:
				switch (m_icc->lpbiInput->biBitCount)
				{
				case 24:
					{
						DWORD dwDataStride = m_icc->lpbiInput->biWidth * 3;
						DWORD dwSrcStride = ROUNDUP(dwDataStride, 4);
						DWORD dwYStride = m_icc->lpbiInput->biWidth;

						pSrcBegin = ((BYTE *)m_icc->lpInput) + (m_dwNumStripes - dwMacroStrideEnd  ) * dwSrcStride * 2;
						pSrcEnd   = ((BYTE *)m_icc->lpInput) + (m_dwNumStripes - dwMacroStrideBegin) * dwSrcStride * 2;

						for (const BYTE *pStrideBegin = pSrcEnd - dwSrcStride * 2; pStrideBegin >= pSrcBegin; pStrideBegin -= dwSrcStride * 2)
						{
							const BYTE *pStrideEnd = pStrideBegin + dwDataStride;
							for (const BYTE *p = pStrideBegin; p < pStrideEnd; p += 6)
							{
								const BYTE *q = p + dwSrcStride;
								*(y+0)           = min(max(int((*(q+0))*0.098 + (*(q+1))*0.504 + (*(q+2))*0.257 + 16.5), 16), 235);
								*(y+1)           = min(max(int((*(q+3))*0.098 + (*(q+4))*0.504 + (*(q+5))*0.257 + 16.5), 16), 235);
								*(y+dwYStride+0) = min(max(int((*(p+0))*0.098 + (*(p+1))*0.504 + (*(p+2))*0.257 + 16.5), 16), 235);
								*(y+dwYStride+1) = min(max(int((*(p+3))*0.098 + (*(p+4))*0.504 + (*(p+5))*0.257 + 16.5), 16), 235);
								*u               = min(max(int(((*(p+0)+*(p+3)+*(q+0)+*(q+3))*0.439 + (*(p+1)+*(p+4)+*(q+1)+*(q+4))*-0.291 + (*(p+2)+*(p+5)+*(q+2)+*(q+5))*-0.148)/4 + 128.5), 16), 240);
								*v               = min(max(int(((*(p+0)+*(p+3)+*(q+0)+*(q+3))*-0.071 + (*(p+1)+*(p+4)+*(q+1)+*(q+4))*-0.368 + (*(p+2)+*(p+5)+*(q+2)+*(q+5))*0.439)/4 + 128.5), 16), 240);
								y+=2; u++; v++;
							}
							y += dwYStride;
						}
					}
					break;
				case 32:
					{
						DWORD dwDataStride = m_icc->lpbiInput->biWidth * 4;
						DWORD dwSrcStride = ROUNDUP(dwDataStride, 4);
						DWORD dwYStride = m_icc->lpbiInput->biWidth;

						pSrcBegin = ((BYTE *)m_icc->lpInput) + (m_dwNumStripes - dwMacroStrideEnd  ) * dwSrcStride * 2;
						pSrcEnd   = ((BYTE *)m_icc->lpInput) + (m_dwNumStripes - dwMacroStrideBegin) * dwSrcStride * 2;

						for (const BYTE *pStrideBegin = pSrcEnd - dwSrcStride * 2; pStrideBegin >= pSrcBegin; pStrideBegin -= dwSrcStride * 2)
						{
							const BYTE *pStrideEnd = pStrideBegin + dwDataStride;
							for (const BYTE *p = pStrideBegin; p < pStrideEnd; p += 8)
							{
								const BYTE *q = p + dwSrcStride;
								*(y+0)           = min(max(int((*(q+0))*0.098 + (*(q+1))*0.504 + (*(q+2))*0.257 + 16.5), 16), 235);
								*(y+1)           = min(max(int((*(q+4))*0.098 + (*(q+5))*0.504 + (*(q+6))*0.257 + 16.5), 16), 235);
								*(y+dwYStride+0) = min(max(int((*(p+0))*0.098 + (*(p+1))*0.504 + (*(p+2))*0.257 + 16.5), 16), 235);
								*(y+dwYStride+1) = min(max(int((*(p+4))*0.098 + (*(p+5))*0.504 + (*(p+6))*0.257 + 16.5), 16), 235);
								*u               = min(max(int(((*(p+0)+*(p+4)+*(q+0)+*(q+4))*0.439 + (*(p+1)+*(p+5)+*(q+1)+*(q+5))*-0.291 + (*(p+2)+*(p+6)+*(q+2)+*(q+6))*-0.148)/4 + 128.5), 16), 240);
								*v               = min(max(int(((*(p+0)+*(p+4)+*(q+0)+*(q+4))*-0.071 + (*(p+1)+*(p+5)+*(q+1)+*(q+5))*-0.368 + (*(p+2)+*(p+6)+*(q+2)+*(q+6))*0.439)/4 + 128.5), 16), 240);
								y+=2; u++; v++;
							}
							y += dwYStride;
						}
					}
					break;
				}
			}
		}
	}
}
