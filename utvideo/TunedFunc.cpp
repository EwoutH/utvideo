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
#include "TunedFunc.h"
#include "Predict.h"
#include "HuffmanCode.h"
#include "Convert.h"

const TUNEDFUNC tfnCPP = {
	cpp_PredictMedian,
	cpp_PredictMedianAndCount,
	cpp_PredictMedianAndCount,
	cpp_PredictLeftAndCount,
	cpp_RestoreMedian,
	cpp_HuffmanEncode,
	cpp_HuffmanDecode,
	cpp_HuffmanDecodeAndAccum,
	cpp_ConvertULY2ToBottomupRGB24,
	cpp_ConvertULY2ToBottomupRGB32,
	cpp_ConvertBottomupRGB24ToULY2,
	cpp_ConvertBottomupRGB32ToULY2,
};

#ifdef _WIN64

// nothing

#else

const TUNEDFUNC tfnI686 = {
	cpp_PredictMedian,
	cpp_PredictMedianAndCount,
	cpp_PredictMedianAndCount,
	cpp_PredictLeftAndCount,
	i686_RestoreMedian_align1,
	i686_HuffmanEncode_align1,
	i686_HuffmanDecode_align1,
	i686_HuffmanDecodeAndAccum_align1,
	cpp_ConvertULY2ToBottomupRGB24,
	cpp_ConvertULY2ToBottomupRGB32,
	cpp_ConvertBottomupRGB24ToULY2,
	cpp_ConvertBottomupRGB32ToULY2,
};

const TUNEDFUNC tfnSSE2 = {
	sse2_PredictMedian_align16,
	sse2_PredictMedianAndCount_align16,
	sse2_PredictMedianAndCount_align1,
	sse2_PredictLeftAndCount_align1,
	sse1mmx_RestoreMedian_align1,
	i686_HuffmanEncode_align1,
	i686_HuffmanDecode_align1,
	i686_HuffmanDecodeAndAccum_align1,
	sse2_ConvertULY2ToBottomupRGB24,
	sse2_ConvertULY2ToBottomupRGB32,
	sse2_ConvertBottomupRGB24ToULY2,
	sse2_ConvertBottomupRGB32ToULY2,
};

const TUNEDFUNC tfnSSE3 = {
	sse2_PredictMedian_align16,
	sse2_PredictMedianAndCount_align16,
	sse2_PredictMedianAndCount_align1,
	sse2_PredictLeftAndCount_align1,
	sse1mmx_RestoreMedian_align1,
	i686_HuffmanEncode_align1,
	i686_HuffmanDecode_align1,
	i686_HuffmanDecodeAndAccum_align1,
	sse2_ConvertULY2ToBottomupRGB24,
	sse2_ConvertULY2ToBottomupRGB32,
	sse2_ConvertBottomupRGB24ToULY2,
	sse2_ConvertBottomupRGB32ToULY2,
};

const TUNEDFUNC tfnSSSE3 = {
	sse2_PredictMedian_align16,
	sse2_PredictMedianAndCount_align16,
	sse2_PredictMedianAndCount_align1,
	sse2_PredictLeftAndCount_align1,
	sse1mmx_RestoreMedian_align1,
	i686_HuffmanEncode_align1,
	i686_HuffmanDecode_align1,
	i686_HuffmanDecodeAndAccum_align1,
	sse2_ConvertULY2ToBottomupRGB24,
	sse2_ConvertULY2ToBottomupRGB32,
	sse2_ConvertBottomupRGB24ToULY2,
	sse2_ConvertBottomupRGB32ToULY2,
};

#endif

TUNEDFUNC tfn = tfnCPP;

#ifdef _WIN64

void InitializeTunedFunc(void)
{
	// nothing to do
}

#else

void InitializeTunedFunc(void)
{
	DWORD	cpuid_1_ecx = 0;
	DWORD	cpuid_1_edx = 0;

	__asm
	{
		push	eax
		push	ebx
		push	ecx
		push	edx

		xor		eax, eax
		cpuid
		cmp		eax, 1
		jb		no_feature
		mov		eax, 1
		cpuid
		mov		cpuid_1_ecx, ecx
		mov		cpuid_1_edx, edx

	no_feature:
		pop		edx
		pop		ecx
		pop		ebx
		pop		eax
	};

	_RPT2(_CRT_WARN, "CPUID.EAX=1 ECX=%08X EDX=%08X\n", cpuid_1_ecx, cpuid_1_edx);

	if (cpuid_1_ecx & (1 << 9))
	{
		_RPT0(_CRT_WARN, "supports SSSE3\n");
		tfn = tfnSSSE3;
	}
	else if (cpuid_1_ecx & (1 << 0))
	{
		_RPT0(_CRT_WARN, "supports SSE3\n");
		tfn = tfnSSE3;
	}
	else if (cpuid_1_edx & (1 << 26))
	{
		_RPT0(_CRT_WARN, "supports SSE2\n");
		tfn = tfnSSE2;
	}
	else
	{
		_RPT0(_CRT_WARN, "supports no SSE-integer\n");
		tfn = tfnI686;
	}
}

#endif
