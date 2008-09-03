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
#include "UAY2Encoder.h"
#include "Predict.h"
#include "Convert.h"
#include "resource.h"

const CPackedEncoder::INPUTFORMAT CUAY2Encoder::m_infmts[] = {
	{ FCC('YUY2'), 16, 16, TRUE, TRUE }, { FCC('YUYV'), 16, 16, TRUE, TRUE }, { FCC('YUNV'), 16, 16, TRUE, TRUE },
	{ FCC('UYVY'), 16, 16, TRUE, TRUE }, { FCC('UYNV'), 16, 16, TRUE, TRUE },
	{ FCC('YVYU'), 16, 16, TRUE, TRUE },
	{ FCC('VYUY'), 16, 16, TRUE, TRUE },
};

CUAY2Encoder::CUAY2Encoder(void)
{
}

CUAY2Encoder::~CUAY2Encoder(void)
{
}

int CUAY2Encoder::GetNumSupportedInputFormats(void)
{
	return _countof(m_infmts);
}

CEncoder *CUAY2Encoder::CreateInstance(void)
{
	return new CUAY2Encoder();
}

void CUAY2Encoder::SetCompressionProperty(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut)
{
	m_dwStrideSize = pbihIn->biWidth*2;
	m_dwNumStrides = abs(pbihIn->biHeight);

	switch(pbihIn->biCompression)
	{
	case FCC('YUY2'):
	case FCC('YUYV'):
	case FCC('YUNV'):
		m_pfnHuffmanEncodeFirstRawWithDiff = HuffmanEncodeFirstRawWithDiffYUY2;
		break;
	case FCC('UYVY'):
	case FCC('UYNV'):
		m_pfnHuffmanEncodeFirstRawWithDiff = HuffmanEncodeFirstRawWithDiffUYVY;
		break;
	case FCC('YVYU'):
		m_pfnHuffmanEncodeFirstRawWithDiff = HuffmanEncodeFirstRawWithDiffYVYU;
		break;
	case FCC('VYUY'):
		m_pfnHuffmanEncodeFirstRawWithDiff = HuffmanEncodeFirstRawWithDiffVYUY;
		break;
	default:
		_ASSERT(false);
	}
}
