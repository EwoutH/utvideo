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

#ifndef FCC
#define FCC(fcc) ( \
	(((DWORD)(fcc) & 0x000000ff) << 24) | \
	(((DWORD)(fcc) & 0x0000ff00) <<  8) | \
	(((DWORD)(fcc) & 0x00ff0000) >>  8) | \
	(((DWORD)(fcc) & 0xff000000) >> 24))
#endif

#define FCC4PRINTF(fcc) \
	(BYTE)(fcc), \
	(BYTE)(fcc >> 8), \
	(BYTE)(fcc >> 16), \
	(BYTE)(fcc >> 24)

#define UTVIDEO_VERSION_MASK                      0xffffff00
#define UTVIDEO_VERSION                           0x07000000 /* 7.0.0 */

/*
 * NOTE:
 *  If you want to implement original version of encoder,
 *  you should register "implementation ID".
 *  Otherwise, you must use 0xff as ID.
 */
#define UTVIDEO_IMPLEMENTATION_MASK               0x000000ff
#define UTVIDEO_IMPLEMENTATION_ORIGINAL_WIN32_X86 0x00000000
#define UTVIDEO_IMPLEMENTATION_ORIGINAL_WIN64_X64 0x00000001
#define UTVIDEO_IMPLEMENTATION_NOT_REGISTERED     0x000000ff

#ifdef _WIN64
 #ifdef _M_X64
  #define UTVIDEO_IMPLEMENTATION UTVIDEO_IMPLEMENTATION_ORIGINAL_WIN64_X64
  #define UTVIDEO_IMPLEMENTATION_STR "x64"
  #ifndef _WIN64_X64
   #define _WIN64_X64
  #endif
 #else
  #error This platform is not supported.
 #endif
#else
#ifdef _WIN32
 #ifdef _M_IX86
  #define UTVIDEO_IMPLEMENTATION UTVIDEO_IMPLEMENTATION_ORIGINAL_WIN32_X86
  #define UTVIDEO_IMPLEMENTATION_STR "x86"
  #ifndef _WIN32_X86
   #define _WIN32_X86
  #endif
 #else
  #error This platform is not supported.
 #endif
#endif
#endif

#define UTVIDEO_VERSION_AND_IMPLEMENTATION (UTVIDEO_VERSION | UTVIDEO_IMPLEMENTATION)

extern HINSTANCE hModule;

struct BITMAPINFOEXT
{
	BITMAPINFOHEADER bih;
	DWORD dwEncoderVersionAndImplementation;
	DWORD fccOriginalFormat;
	DWORD dwFrameInfoSize;
	DWORD dwFlags0;
};

#define BIE_FLAGS0_DIVIDE_COUNT_MASK             0xff000000
#define BIE_FLAGS0_DIVIDE_COUNT_SHIFT            24

#define BIE_FLAGS0_COMPRESS_MASK                 0x00000001
#define BIE_FLAGS0_COMPRESS_NONE                 0x00000000
#define BIE_FLAGS0_COMPRESS_HUFFMAN_CODE         0x00000001

#define BIE_FLAGS0_ASSUME_INTERLACE              0x00000800

#define BIE_FLAGS0_RESERVED                      0x00fff7fe


struct FRAMEINFO
{
	DWORD dwFlags0;
};

#define FI_FLAGS0_INTRAFRAME_PREDICT_MASK        0x00000300
#define FI_FLAGS0_INTRAFRAME_PREDICT_NONE        0x00000000
#define FI_FLAGS0_INTRAFRAME_PREDICT_LEFT        0x00000100
#define FI_FLAGS0_INTRAFRAME_PREDICT_GRADIENT    0x00000200
#define FI_FLAGS0_INTRAFRAME_PREDICT_MEDIAN      0x00000300

#define FI_FLAGS0_RESERVED                       0xfffffcff


struct ENCODERCONF
{
	DWORD dwFlags0;
};

#define EC_FLAGS0_DIVIDE_COUNT_MASK              0x000000ff

#define EC_FLAGS0_INTRAFRAME_PREDICT_MASK        0x00000300
#define EC_FLAGS0_INTRAFRAME_PREDICT_RESERVED    0x00000000
#define EC_FLAGS0_INTRAFRAME_PREDICT_LEFT        0x00000100
#define EC_FLAGS0_INTRAFRAME_PREDICT_MEDIAN      0x00000300

#define EC_FLAGS0_ASSUME_INTERLACE               0x00000800
#define EC_FLAGS0_DIVIDE_COUNT_IS_NUM_PROCESSORS 0x00001000

#define EC_FLAGS0_RESERVED                       0xffffe400


inline DWORD ROUNDUP(DWORD a, DWORD b)
{
	return ((a + b - 1) / b) * b;
}

inline bool IS_ALIGNED(DWORD_PTR v, DWORD_PTR a)
{
	_ASSERT(
		a == 2 ||
		a == 4 ||
		a == 8 ||
		a == 16 ||
		a == 32 ||
		a == 64 ||
		a == 128 ||
		a == 256);
	return (v & (a - 1)) == 0;
}

inline bool IS_ALIGNED(const void *p, DWORD_PTR a)
{
	return IS_ALIGNED((DWORD_PTR)p, a);
}

inline BOOL EnableDlgItem(HWND hwndParent, UINT nID, BOOL bEnable)
{
	return EnableWindow(GetDlgItem(hwndParent, nID), bEnable);
}
