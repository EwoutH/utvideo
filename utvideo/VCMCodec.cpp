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
#include "VCMCodec.h"
#include "ULY2Encoder.h"
#include "ULY2Decoder.h"

CVCMCodec::CVCMCodec(DWORD fccHandler)
{
	m_fccHandler = fccHandler;
	m_pEncoder = new CULY2Encoder();
	m_pDecoder = new CULY2Decoder();
}

CVCMCodec::~CVCMCodec(void)
{
	delete m_pEncoder;
	delete m_pDecoder;
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
	icinfo->dwVersion    = UTVIDEO_ENCODER_VERSION;
	icinfo->dwVersionICM = ICVERSION;
	wsprintfW(icinfo->szName, L"%S", "Ut Video Codec");
	wsprintfW(icinfo->szDescription, L"%S", "Ut Video Codec YUV422 (VCM)");

	return sizeof(ICINFO);
}

DWORD CVCMCodec::QueryConfigure(void)
{
	return ICERR_OK;
}

DWORD CVCMCodec::Configure(HWND hwnd)
{
	return m_pEncoder->Configure(hwnd);
}

DWORD CVCMCodec::GetStateSize(void)
{
	return sizeof(ENCODERCONF);
}

DWORD CVCMCodec::GetState(void *pState, DWORD dwSize)
{
	_RPT0(_CRT_WARN, "CVCMCodec::GetState()\n");
	return m_pEncoder->GetState(pState, dwSize);
}

DWORD CVCMCodec::SetState(const void *pState, DWORD dwSize)
{
	_RPT0(_CRT_WARN, "CVCMCodec::SetState()\n");
	return m_pEncoder->SetState(pState, dwSize);
}

DWORD CVCMCodec::Compress(const ICCOMPRESS *icc, DWORD dwSize)
{
	return m_pEncoder->Compress(icc, dwSize);
}

DWORD CVCMCodec::CompressBegin(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut)
{
	return m_pEncoder->CompressBegin(pbihIn, pbihOut);
}

DWORD CVCMCodec::CompressEnd(void)
{
	return m_pEncoder->CompressEnd();
}

DWORD CVCMCodec::CompressGetFormat(const BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	return m_pEncoder->CompressGetFormat(pbihIn, pbihOut);
}

DWORD CVCMCodec::CompressGetSize(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut)
{
	return m_pEncoder->CompressGetSize(pbihIn, pbihOut);
}

DWORD CVCMCodec::CompressQuery(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut)
{
	return m_pEncoder->CompressQuery(pbihIn, pbihOut);
}

DWORD CVCMCodec::Decompress(const ICDECOMPRESS *icd, DWORD dwSize)
{
	return m_pDecoder->Decompress(icd, dwSize);
}

DWORD CVCMCodec::DecompressBegin(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut)
{
	return m_pDecoder->DecompressBegin(pbihIn, pbihOut);
}

DWORD CVCMCodec::DecompressEnd(void)
{
	return m_pDecoder->DecompressEnd();
}

DWORD CVCMCodec::DecompressGetFormat(const BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut)
{
	return m_pDecoder->DecompressGetFormat(pbihIn, pbihOut);
}

DWORD CVCMCodec::DecompressQuery(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut)
{
	return m_pDecoder->DecompressQuery(pbihIn, pbihOut);
}
