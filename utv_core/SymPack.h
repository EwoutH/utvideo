/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

#include "SymbolBits.h"

// �ŏ��� 8 �̓V���{���̌��A�Ō�� 8 �̓V���{���̃r�b�g���i���� 10bit �V���{���Ȃ� Pack8Sym10 �ɂȂ�j
void cpp_Pack8Sym8(uint8_t *pPacked, size_t *cbPacked, uint8_t *pControl, size_t *cbControl, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd);
void cpp_Unpack8Sym8(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pPacked, const uint8_t *pControl, size_t cbControl);
