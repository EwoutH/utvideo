/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef __GNUC__
#include <x86intrin.h>
#endif

static inline __m128i _mm_setone_si128()
{
	/*
	 * Clang �ɂ� _mm_undefined_si128 �͖����炵���B
	 * Visual C++ �ł� _mm_setzero_si128 �ɒu����������B
	 * �ǂ��������̑��̕����� all 0 ���g���̂ŁA���̏����ł����Ȃ��낤�B
	 */
	__m128i x = _mm_setzero_si128(); // _mm_undefined_si128();
	return _mm_cmpeq_epi32(x, x);
}

static inline __m128i _mm_set2_epi8(char a, char b)
{
	return _mm_set_epi8(a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b);
}

static inline __m128i _mm_set2_epi16(short a, short b)
{
	return _mm_set_epi16(a, b, a, b, a, b, a, b);
}

static inline __m128i _mm_set4_epi16(short a, short b, short c, short d)
{
	return _mm_set_epi16(a, b, c, d, a, b, c, d);
}

static inline __m128i _mm_set2_epi16_shift(double a, double b, int shift)
{
	short aa = short(a * (1 << shift));
	short bb = short(b * (1 << shift));
	return _mm_set2_epi16(aa, bb);
}

static inline __m128i _mm_set4_epi16_shift(double a, double b, double c, double d, int shift)
{
	short aa = short(a * (1 << shift));
	short bb = short(b * (1 << shift));
	short cc = short(c * (1 << shift));
	short dd = short(d * (1 << shift));
	return _mm_set4_epi16(aa, bb, cc, dd);
}
