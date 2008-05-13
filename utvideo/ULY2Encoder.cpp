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
#include "ULY2Encoder.h"
#include "Predict.h"
#include "resource.h"

const CPlanarEncoder::INPUTFORMAT CULY2Encoder::m_infmts[7] = {
	{ FCC('YUY2'), 16, TRUE }, { FCC('YUYV'), 16, TRUE }, { FCC('YUNV'), 16, TRUE },
	{ FCC('UYVY'), 16, TRUE }, { FCC('UYNV'), 16, TRUE },
	{ FCC('YVYU'), 16, TRUE },
	{ FCC('VYUY'), 16, TRUE },
};

CULY2Encoder::CULY2Encoder(void)
{
}

CULY2Encoder::~CULY2Encoder(void)
{
}

CEncoder *CULY2Encoder::CreateInstance(void)
{
	return new CULY2Encoder();
}

void CULY2Encoder::CalcPlaneSizes(const BITMAPINFOHEADER *pbihIn)
{
	m_dwPlaneStride[0] = ROUNDUP(pbihIn->biWidth, 2);
	m_dwPlaneSize[0]   = m_dwPlaneStride[0] * m_dwNumStrides;

	m_dwPlaneStride[1] = ROUNDUP(pbihIn->biWidth, 2) / 2;
	m_dwPlaneSize[1]   = m_dwPlaneStride[1] * m_dwNumStrides;

	m_dwPlaneStride[2] = m_dwPlaneStride[1];
	m_dwPlaneSize[2]   = m_dwPlaneSize[1];
}

void CULY2Encoder::ConvertToPlanar(DWORD nBandIndex)
{
	DWORD dwStrideBegin = m_dwNumStrides *  nBandIndex      / m_dwDivideCount;
	DWORD dwStrideEnd   = m_dwNumStrides * (nBandIndex + 1) / m_dwDivideCount;
	BYTE *y, *u, *v;
	const BYTE *pSrcBegin, *pSrcEnd, *p;

	pSrcBegin = ((BYTE *)m_icc->lpInput) + dwStrideBegin * m_dwFrameStride;
	pSrcEnd   = ((BYTE *)m_icc->lpInput) + dwStrideEnd   * m_dwFrameStride;
	y = m_pCurFrame->GetPlane(0) + dwStrideBegin * m_dwPlaneStride[0];
	u = m_pCurFrame->GetPlane(1) + dwStrideBegin * m_dwPlaneStride[1];
	v = m_pCurFrame->GetPlane(2) + dwStrideBegin * m_dwPlaneStride[2];

	switch (m_icc->lpbiInput->biCompression)
	{
	case FCC('YUY2'):
	case FCC('YUYV'):
	case FCC('YUNV'):
		for (p = pSrcBegin; p < pSrcEnd; p += 4)
		{
			*y++ = *p;
			*u++ = *(p+1);
			*y++ = *(p+2);
			*v++ = *(p+3);
		}
		break;
	case FCC('UYVY'):
	case FCC('UYNV'):
		for (p = pSrcBegin; p < pSrcEnd; p += 4)
		{
			*u++ = *p;
			*y++ = *(p+1);
			*v++ = *(p+2);
			*y++ = *(p+3);
		}
		break;
	case FCC('YVYU'):
		for (p = pSrcBegin; p < pSrcEnd; p += 4)
		{
			*y++ = *p;
			*v++ = *(p+1);
			*y++ = *(p+2);
			*u++ = *(p+3);
		}
		break;
	case FCC('VYUY'):
		for (p = pSrcBegin; p < pSrcEnd; p += 4)
		{
			*v++ = *p;
			*y++ = *(p+1);
			*u++ = *(p+2);
			*y++ = *(p+3);
		}
		break;
	}
}
