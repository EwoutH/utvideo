/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

template<int F> void tuned_Pack8Sym8(uint8_t *pPacked, size_t *cbPacked, uint8_t *pControl, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd);
template<int F> void tuned_Unpack8Sym8(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pPacked, const uint8_t *pControl);