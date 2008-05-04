; �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e
; $Id$

; Ut Video Codec Suite
; Copyright (C) 2008  UMEZAWA Takeshi
; 
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
; 
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
; 
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
; 
; 
; Ut Video Codec Suite
; Copyright (C) 2008  �~�V �Ўu
; 
; ���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t
; �g�E�F�A���c�ɂ���Ĕ��s���ꂽ GNU ��ʌ��O���p�����_��(�o�[�W��
; ��2���A��]�ɂ���Ă͂���ȍ~�̃o�[�W�����̂����ǂꂩ)�̒�߂����
; �̉��ōĔЕz�܂��͉��ς��邱�Ƃ��ł��܂��B
; 
; ���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖���
; ��* �ł��B���Ɖ\���̕ۏ؂����̖ړI�ւ̓K�����́A���O�Ɏ����ꂽ
; ���̂��܂ߑS�����݂��܂���B�ڂ�����GNU ��ʌ��O���p�����_�񏑂���
; �����������B
; 
; ���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU ��ʌ��O���p�����_�񏑂̕�������
; �ꕔ�󂯎�����͂��ł��B�����󂯎���Ă��Ȃ���΁A�t���[�\�t�g�E�F
; �A���c�܂Ő������Ă�������(����� the Free Software Foundation,
; Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA)�B

.686
.no87
.xmm

.model	flat

_TEXT_ASM	SEGMENT	page public flat 'CODE'

; void PredictMedian(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride)
public	_sse2_PredictMedian_align16
_sse2_PredictMedian_align16	proc

	push		ebx
	push		esi
	push		edi
	push		ebp

	mov			eax, 80h
	movd		xmm1, eax

	mov			esi, dword ptr [esp + 16 + 4 +  4]	; pSrcBegin
	mov			edi, dword ptr [esp + 16 + 4 +  0]	; pDst
	mov			eax, esi
	mov			edx, dword ptr [esp + 16 + 4 + 12]	; dwStride
	add			eax, edx
	neg			edx

	align		64
label1:
	movdqa		xmm0, oword ptr [esi]
	movdqa		xmm2, xmm0
	pslldq		xmm2, 1
	por			xmm2, xmm1
	movdqa		xmm1, xmm0
	psrldq		xmm1, 15

	psubb		xmm0, xmm2
	movdqa		oword ptr [edi], xmm0

	add			esi, 16
	add			edi, 16
	cmp			esi, eax
	jb			label1

	mov			eax, dword ptr [esp + 16 + 4 +  8]	; pSrcEnd

	pxor		xmm1, xmm1
	pxor		xmm5, xmm5

	align		64
label2:
	movdqa		xmm0, oword ptr [esi]
	movdqa		xmm2, xmm0
	pslldq		xmm2, 1
	por			xmm2, xmm1
	movdqa		xmm1, xmm0
	psrldq		xmm1, 15

	movdqa		xmm4, oword ptr [esi+edx]
	movdqa		xmm6, xmm4
	pslldq		xmm6, 1
	por			xmm6, xmm5
	movdqa		xmm5, xmm4
	psrldq		xmm5, 15

	movdqa		xmm7, xmm2
	paddb		xmm7, xmm4
	psubb		xmm7, xmm6

	movdqa		xmm3, xmm2
	pminub		xmm2, xmm4
	pmaxub		xmm3, xmm4
	pmaxub		xmm7, xmm2
	pminub		xmm7, xmm3	; predicted = min(max(min(left, above), grad), max(left, above))

	psubb		xmm0, xmm7
	movdqa		oword ptr [edi], xmm0

	add			esi, 16
	add			edi, 16
	cmp			esi, eax
	jb			label2

	pop			ebp
	pop			edi
	pop			esi
	pop			ebx
	ret

_sse2_PredictMedian_align16	endp

end
