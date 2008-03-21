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

#pragma once

#define UTVIDEO_ENCODER_VERSION 0x01000000 /* 1.0.0 */

struct BITMAPINFOHEADER_EXTRA
{
	DWORD dwEncoderVersion;
	DWORD fccOriginalFormat;
};

#define BMIHE_VERSION_MASK 0xffffff00

struct FRAMEHEADER
{
	DWORD dwFlags0;
	DWORD dwFlags1;
};

#define FH_FLAGS0_KEYFRAME                        0x00000001

#define FH_FLAGS0_COMPRESS_MASK                   0x0000ff00
#define FH_FLAGS0_COMPRESS_NONE                   0x00000000
#define FH_FLAGS0_COMPRESS_8BITSYMBOL_PREFIX_CODE 0x00000100

#define FH_FLAGS0_INTRAFRAME_PREDICT_MASK         0x00ff0000
#define FH_FLAGS0_INTRAFRAME_PREDICT_NONE         0x00000000
#define FH_FLAGS0_INTRAFRAME_PREDICT_ABOVE        0x00010000

#define FH_FLAGS1_DIVIDE_COUNT_MASK               0x000000ff

#define FH_FLAGS0_RESERVED                        0xff0000fe
#define FH_FLAGS1_RESERVED                        0xffffff00


inline DWORD ROUNDUP(DWORD a, DWORD b)
{
	return ((a + b - 1) / b) * b;
}
