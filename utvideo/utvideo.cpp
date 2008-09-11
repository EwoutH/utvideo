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

#include "stdafx.h"
#include "utvideo.h"
#include "VCMCodec.h"
#include "TunedFunc.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HMODULE hModule;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		::hModule = hModule;
		InitializeTunedFunc();
	}

	return TRUE;
}

int APIENTRY ICInstallSelf(void)
{
	CVCMCodec::ICInstallAll();
	return 0;
}

int APIENTRY ICRemoveSelf(void)
{
	CVCMCodec::ICRemoveAll();
	return 0;
}

void CALLBACK ICInstallByRundll(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	CVCMCodec::ICInstallAll();
}

void CALLBACK ICRemoveByRundll(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	CVCMCodec::ICRemoveAll();
}

LRESULT CALLBACK DriverProc(DWORD dwDriverId, HDRVR hdrvr,UINT uMsg, LPARAM lParam1, LPARAM lParam2)
{
	CVCMCodec *pCodec = (CVCMCodec *)dwDriverId;

	switch (uMsg)
	{
	/* Driver Messages */
	case DRV_LOAD:
		return TRUE;

	case DRV_FREE:
		return TRUE;

	case DRV_OPEN:
		return (LRESULT)CVCMCodec::Open((ICOPEN *)lParam2);

	case DRV_CLOSE:
		if (pCodec != NULL)
			delete pCodec;
		return TRUE;

	/* Codec Messages */
	case ICM_ABOUT:
		if (lParam1 == -1)
			return pCodec->QueryAbout();
		else
			return pCodec->About((HWND)lParam1);

	case ICM_GETINFO:
		return pCodec->GetInfo((ICINFO *)lParam1, (DWORD)lParam2);

	/* Encoder Messages */
	case ICM_CONFIGURE:
		if (lParam1 == -1)
			return pCodec->QueryConfigure();
		else
			return pCodec->Configure((HWND)lParam1);

	case ICM_GETSTATE:
		if (lParam1 == 0)
			return pCodec->GetStateSize();
		else
			return pCodec->GetState((void *)lParam1, lParam2);

	case ICM_SETSTATE:
		return pCodec->SetState((void *)lParam1, lParam2);

	case ICM_COMPRESS:
		return pCodec->Compress((ICCOMPRESS *)lParam1, (DWORD)lParam2);

	case ICM_COMPRESS_BEGIN:
		return pCodec->CompressBegin((BITMAPINFOHEADER *)lParam1, (BITMAPINFOHEADER *)lParam2);

	case ICM_COMPRESS_END:
		return pCodec->CompressEnd();

	case ICM_COMPRESS_GET_FORMAT:
		return pCodec->CompressGetFormat((BITMAPINFOHEADER *)lParam1, (BITMAPINFOHEADER *)lParam2);

	case ICM_COMPRESS_GET_SIZE:
		return pCodec->CompressGetSize((BITMAPINFOHEADER *)lParam1, (BITMAPINFOHEADER *)lParam2);

	case ICM_COMPRESS_QUERY:
		return pCodec->CompressQuery((BITMAPINFOHEADER *)lParam1, (BITMAPINFOHEADER *)lParam2);

	/* Decoder Messages */
	case ICM_DECOMPRESS:
		return pCodec->Decompress((ICDECOMPRESS *)lParam1, (DWORD)lParam2);

	case ICM_DECOMPRESS_BEGIN:
		return pCodec->DecompressBegin((BITMAPINFOHEADER *)lParam1, (BITMAPINFOHEADER *)lParam2);

	case ICM_DECOMPRESS_END:
		return pCodec->DecompressEnd();

	case ICM_DECOMPRESS_GET_FORMAT:
		return pCodec->DecompressGetFormat((BITMAPINFOHEADER *)lParam1, (BITMAPINFOHEADER *)lParam2);

	case ICM_DECOMPRESS_QUERY:
		return pCodec->DecompressQuery((BITMAPINFOHEADER *)lParam1, (BITMAPINFOHEADER *)lParam2);
	}

	if (uMsg < DRV_USER)
		return DefDriverProc(dwDriverId, hdrvr, uMsg, lParam1, lParam2);
	else
		return ICERR_UNSUPPORTED;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
