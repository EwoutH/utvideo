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

#include "TunedFunc.h"

#define ConvertULY2ToBottomupRGB24 tfn.pfnConvertULY2ToBottomupRGB24
#define ConvertULY2ToBottomupRGB32 tfn.pfnConvertULY2ToBottomupRGB32
#define ConvertBottomupRGB24ToULY2 tfn.pfnConvertBottomupRGB24ToULY2
#define ConvertBottomupRGB32ToULY2 tfn.pfnConvertBottomupRGB32ToULY2

void cpp_ConvertULY2ToBottomupRGB24(BYTE *pDstBegin, BYTE *pDstEnd, const BYTE *pYBegin, const BYTE *pUBegin, const BYTE *pVBegin, DWORD dwStride, DWORD dwDataStride);
extern "C" void sse2_ConvertULY2ToBottomupRGB24(BYTE *pDstBegin, BYTE *pDstEnd, const BYTE *pYBegin, const BYTE *pUBegin, const BYTE *pVBegin, DWORD dwStride, DWORD dwDataStride);

void cpp_ConvertULY2ToBottomupRGB32(BYTE *pDstBegin, BYTE *pDstEnd, const BYTE *pYBegin, const BYTE *pUBegin, const BYTE *pVBegin, DWORD dwStride, DWORD dwDataStride);
extern "C" void sse2_ConvertULY2ToBottomupRGB32(BYTE *pDstBegin, BYTE *pDstEnd, const BYTE *pYBegin, const BYTE *pUBegin, const BYTE *pVBegin, DWORD dwStride, DWORD dwDataStride);

void cpp_ConvertBottomupRGB24ToULY2(BYTE *pYBegin, BYTE *pUBegin, BYTE *pVBegin, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD dwDataStride);
extern "C" void sse2_ConvertBottomupRGB24ToULY2(BYTE *pYBegin, BYTE *pUBegin, BYTE *pVBegin, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD dwDataStride);

void cpp_ConvertBottomupRGB32ToULY2(BYTE *pYBegin, BYTE *pUBegin, BYTE *pVBegin, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD dwDataStride);
extern "C" void sse2_ConvertBottomupRGB32ToULY2(BYTE *pYBegin, BYTE *pUBegin, BYTE *pVBegin, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD dwDataStride);
