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

#pragma once
#include "Encoder.h"
#include "Decoder.h"

class CVCMCodec
{
private:
	struct CODECLIST
	{
		DWORD fcc;
		const char *pszColorFormatName;
		CEncoder *(*pfnCreateEncoder)(void);
		CDecoder *(*pfnCreateDecoder)(void);
	};

	static const CODECLIST m_codeclist[];

public:
	static void ICInstallAll(void);
	static void ICRemoveAll(void);

private:
	DWORD m_fccHandler;
	const char *m_pszColorFormatName;
	CEncoder *m_pEncoder;
	CDecoder *m_pDecoder;

private:
	CVCMCodec(DWORD fccHandler);
public:
	~CVCMCodec(void);

public:
	static CVCMCodec *Open(ICOPEN *icopen);

	LRESULT QueryAbout(void);
	LRESULT About(HWND hwnd);
	LRESULT GetInfo(ICINFO *icinfo, SIZE_T cb);

	LRESULT QueryConfigure(void);
	LRESULT Configure(HWND hwnd);
	LRESULT GetStateSize(void);
	LRESULT GetState(void *pState, SIZE_T cb);
	LRESULT SetState(const void *pState, SIZE_T cb);
	LRESULT Compress(const ICCOMPRESS *icc, SIZE_T cb);
	LRESULT CompressBegin(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);
	LRESULT CompressEnd(void);
	LRESULT CompressGetFormat(const BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut);
	LRESULT CompressGetSize(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);
	LRESULT CompressQuery(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);

	LRESULT Decompress(const ICDECOMPRESS *icd, SIZE_T cb);
	LRESULT DecompressBegin(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);
	LRESULT DecompressEnd(void);
	LRESULT DecompressGetFormat(const BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut);
	LRESULT DecompressQuery(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);
};
