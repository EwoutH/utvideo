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
#include "VCMCodec.h"
#include "UAY2Encoder.h"

CVCMCodec::CVCMCodec(DWORD fccHandler)
{
	m_fccHandler = fccHandler;
	m_pEncoder = new CUAY2Encoder();
}

CVCMCodec::~CVCMCodec(void)
{
	delete m_pEncoder;
}

CVCMCodec *CVCMCodec::Open(ICOPEN *icopen)
{
	DWORD fccHandler;

	if (icopen != NULL)
	{
		if (icopen->fccType != ICTYPE_VIDEO)
			return NULL;
		fccHandler = icopen->fccHandler;
		icopen->dwError = ICERR_OK;
	}
	else
		fccHandler = (DWORD)-1;

	return new CVCMCodec(fccHandler);
}

DWORD CVCMCodec::QueryAbout(void)
{
	return ICERR_OK;
}

DWORD CVCMCodec::About(HWND hwnd)
{
	MessageBox(hwnd, "Ut Video Codec Suite\nCopyright (C) 2008  UMEZAWA Takeshi", "Ut Video Codec Suite", MB_OK);
	return ICERR_OK;
}

DWORD CVCMCodec::GetInfo(ICINFO *icinfo, DWORD dwSize)
{
	if (icinfo == NULL)
		return sizeof(ICINFO);

	if (dwSize < sizeof(ICINFO))
		return 0;

	icinfo->dwSize       = sizeof(ICINFO);
	icinfo->fccType      = ICTYPE_VIDEO;
	icinfo->fccHandler   = m_fccHandler;
	icinfo->dwFlags      = 0;
	icinfo->dwVersion    = 1;
	icinfo->dwVersionICM = ICVERSION;
	wsprintfW(icinfo->szName, L"%S", "Ut Video Codec");
	wsprintfW(icinfo->szDescription, L"%S", "Ut Video Codec Packed YUV422 (VCM)");

	return sizeof(ICINFO);
}

DWORD CVCMCodec::QueryConfigure(void)
{
	return ICERR_UNSUPPORTED;
}

DWORD CVCMCodec::Configure(HWND hwnd)
{
	return ICERR_UNSUPPORTED;
}

DWORD CVCMCodec::Compress(ICCOMPRESS *icc, DWORD dwSize)
{
	return m_pEncoder->Compress(icc, dwSize);
}

DWORD CVCMCodec::CompressBegin(BITMAPINFOHEADER *pbmihIn, BITMAPINFOHEADER *pbmihOut)
{
	return m_pEncoder->CompressBegin(pbmihIn, pbmihOut);
}

DWORD CVCMCodec::CompressEnd(void)
{
	return m_pEncoder->CompressEnd();
}

DWORD CVCMCodec::CompressGetFormat(BITMAPINFOHEADER *pbmihIn, BITMAPINFOHEADER *pbmihOut)
{
	return m_pEncoder->CompressGetFormat(pbmihIn, pbmihOut);
}

DWORD CVCMCodec::CompressGetSize(BITMAPINFOHEADER *pbmihIn, BITMAPINFOHEADER *pbmihOut)
{
	return m_pEncoder->CompressGetSize(pbmihIn, pbmihOut);
}

DWORD CVCMCodec::CompressQuery(BITMAPINFOHEADER *pbmihIn, BITMAPINFOHEADER *pbmihOut)
{
	return m_pEncoder->CompressQuery(pbmihIn, pbmihOut);
}
