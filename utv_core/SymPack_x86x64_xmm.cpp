/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include <myintrin_x86x64.h>
#include "SymPack_x86x64.h"

#if !defined(GENERATE_SSE2) && !defined(GENERATE_SSSE3) && !defined(GENERATE_SSE41) && !defined(GENERATE_AVX1)
#error
#endif

template<int F>
static inline FORCEINLINE void packer(uint8_t*& q, uint8_t*& r, int& shift, __m128i wa, __m128i wb)
{
	__m128i viszeroa = _mm_cmpeq_epi64(wa, _mm_setzero_si128());
	__m128i viszerob = _mm_cmpeq_epi64(wb, _mm_setzero_si128());
	__m128i viszero = _mm_blend_epi16(viszeroa, viszerob, 0xcc);

	int bits0, bits1, bits2, bits3;

	__m128i za, zb;
	__m128i signsa = _mm_cmpgt_epi8(_mm_setzero_si128(), wa);
	__m128i signsb = _mm_cmpgt_epi8(_mm_setzero_si128(), wb);
	za = _mm_xor_si128(wa, signsa);
	zb = _mm_xor_si128(wb, signsb);
	za = _mm_or_si128(za, _mm_srli_epi64(za, 32));
	zb = _mm_or_si128(zb, _mm_slli_epi64(zb, 32));
	__m128i z = _mm_blend_epi16(za, zb, 0xcc);
	z = _mm_or_si128(_mm_or_si128(z, _mm_set1_epi32(1)), _mm_srli_epi32(z, 16));
	z = _mm_or_si128(z, _mm_srli_epi32(z, 8));
	z = _mm_and_si128(z, _mm_set1_epi32(0xff));


	z = _mm_castps_si128(_mm_cvtepi32_ps(z));
	z = _mm_srli_epi32(z, 23);
	z = _mm_sub_epi32(z, _mm_set1_epi32(0x7d));
	__m128i vbits = z;
	__m128i vrembits = _mm_sub_epi32(_mm_set1_epi32(8), vbits);
	vbits = _mm_andnot_si128(viszero, vbits);
	__m128i vmode = _mm_add_epi32(vbits, _mm_cmpgt_epi32(vbits, _mm_setzero_si128()));

	bits0 = _mm_cvtsi128_si32(vbits);
	bits1 = _mm_extract_epi32(vbits, 2);
	bits2 = _mm_extract_epi32(vbits, 1);
	bits3 = _mm_extract_epi32(vbits, 3);

	__m128i vrembitsa = _mm_and_si128(vrembits, _mm_set1_epi64x(0xffffffff));
	__m128i vrembits0 = vrembitsa;
	__m128i vrembits1 = _mm_srli_si128(vrembitsa, 8);
	__m128i vrembitsb = _mm_srli_epi64(vrembits, 32);
	__m128i vrembits2 = vrembitsb;
	__m128i vrembits3 = _mm_srli_si128(vrembitsb, 8);
	__m128i w0 = _mm_add_epi8(wa, _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits0));
	__m128i w1 = _mm_add_epi8(_mm_srli_si128(wa, 8), _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits1));
	__m128i w2 = _mm_add_epi8(wb, _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits2));
	__m128i w3 = _mm_add_epi8(_mm_srli_si128(wb, 8), _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits3));
	w0 = _mm_or_si128(_mm_and_si128(w0, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w0), vrembits0));
	w1 = _mm_or_si128(_mm_and_si128(w1, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w1), vrembits1));
	w2 = _mm_or_si128(_mm_and_si128(w2, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w2), vrembits2));
	w3 = _mm_or_si128(_mm_and_si128(w3, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w3), vrembits3));
	vrembits0 = _mm_slli_epi64(vrembits0, 1);
	vrembits1 = _mm_slli_epi64(vrembits1, 1);
	vrembits2 = _mm_slli_epi64(vrembits2, 1);
	vrembits3 = _mm_slli_epi64(vrembits3, 1);
	w0 = _mm_or_si128(_mm_and_si128(w0, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w0), vrembits0));
	w1 = _mm_or_si128(_mm_and_si128(w1, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w1), vrembits1));
	w2 = _mm_or_si128(_mm_and_si128(w2, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w2), vrembits2));
	w3 = _mm_or_si128(_mm_and_si128(w3, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w3), vrembits3));
	vrembits0 = _mm_slli_epi64(vrembits0, 1);
	vrembits1 = _mm_slli_epi64(vrembits1, 1);
	vrembits2 = _mm_slli_epi64(vrembits2, 1);
	vrembits3 = _mm_slli_epi64(vrembits3, 1);
	w0 = _mm_or_si128(_mm_and_si128(w0, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w0), vrembits0));
	w1 = _mm_or_si128(_mm_and_si128(w1, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w1), vrembits1));
	w2 = _mm_or_si128(_mm_and_si128(w2, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w2), vrembits2));
	w3 = _mm_or_si128(_mm_and_si128(w3, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w3), vrembits3));
	_mm_storel_epi64((__m128i*)q, w0);
	q += bits0;
	_mm_storel_epi64((__m128i*)q, w1);
	q += bits1;
	_mm_storel_epi64((__m128i*)q, w2);
	q += bits2;
	_mm_storel_epi64((__m128i*)q, w3);
	q += bits3;

	vmode = _mm_shuffle_epi8(vmode, _mm_set_epi8(-1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0));
#ifdef __x86_64__
	uint64_t modes = _mm_cvtsi128_si64(vmode);
	modes |= modes >> (32 - 3);
#else
	vmode = _mm_or_si128(vmode, _mm_srli_epi64(vmode, 32 - 3));
	uint32_t modes = _mm_cvtsi128_si32(vmode);
#endif
	modes |= modes >> (16 - 6);

	*(uint32_t *)r |= (((uint32_t)(uint16_t)modes) << shift);
	shift += 12;
	if (shift == 24)
	{
		r += 3;
		shift = 0;
	}
}

template<int F>
void tuned_Pack8SymAfterPredictPlanarGradient8(uint8_t *pPacked, size_t *cbPacked, uint8_t *pControl, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t cbStride)
{
	int shift = 0;
	auto q = pPacked;
	auto r = pControl;
	memset(pControl, 0, (pSrcEnd - pSrcBegin) / 64 * 3);

	{
		__m128i prev = _mm_set1_epi8((char)0x80);

		for (auto p = pSrcBegin; p != pSrcBegin + cbStride; p += 32)
		{
			__m128i value0 = _mm_loadu_si128((const __m128i *)p);
			__m128i value1 = _mm_loadu_si128((const __m128i *)(p + 16));
			__m128i left0 = _mm_alignr_epi8(value0, prev, 15);
			__m128i left1 = _mm_alignr_epi8(value1, value0, 15);

			__m128i error0 = _mm_sub_epi8(value0, left0);
			__m128i error1 = _mm_sub_epi8(value1, left1);
			prev = value1;
			packer<F>(q, r, shift, error0, error1);
		}
	}

	for (auto pp = pSrcBegin + cbStride; pp != pSrcEnd; pp += cbStride)
	{
		__m128i prev = _mm_set1_epi8((char)0x80);
		__m128i topprev = _mm_set1_epi8((char)0x80);

		for (auto p = pp; p != pp + cbStride; p += 32)
		{
			__m128i value0 = _mm_loadu_si128((const __m128i *)p);
			__m128i value1 = _mm_loadu_si128((const __m128i *)(p + 16));
			__m128i top0 = _mm_loadu_si128((const __m128i *)(p - cbStride));
			__m128i top1 = _mm_loadu_si128((const __m128i *)(p + 16 - cbStride));
			__m128i left0 = _mm_alignr_epi8(value0, prev, 15);
			__m128i left1 = _mm_alignr_epi8(value1, value0, 15);
			__m128i topleft0 = _mm_alignr_epi8(top0, topprev, 15);
			__m128i topleft1 = _mm_alignr_epi8(top1, top0, 15);

			__m128i error0 = _mm_sub_epi8(_mm_add_epi8(value0, topleft0), _mm_add_epi8(left0, top0));
			__m128i error1 = _mm_sub_epi8(_mm_add_epi8(value1, topleft1), _mm_add_epi8(left1, top1));
			prev = value1;
			topprev = top1;
			packer<F>(q, r, shift, error0, error1);
		}
	}

	*cbPacked = q - pPacked;
}

#ifdef GENERATE_SSE41
template void tuned_Pack8SymAfterPredictPlanarGradient8<CODEFEATURE_SSE41>(uint8_t *pPacked, size_t *cbPacked, uint8_t *pControl, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t cbStride);
#endif

#ifdef GENERATE_AVX1
template void tuned_Pack8SymAfterPredictPlanarGradient8<CODEFEATURE_AVX1>(uint8_t *pPacked, size_t *cbPacked, uint8_t *pControl, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t cbStride);
#endif


template<int F>
static inline FORCEINLINE __m128i unpacker(const uint8_t*& q, const uint8_t *& r, int& shift)
{
	__m128i w;
	int mode = ((*(const uint32_t *)r) >> shift) & 7;
	int bits;
	if (mode == 0)
	{
		w = _mm_setzero_si128();
		bits = 0;
	}
	else
	{
		__m128i z, mask;

		w = _mm_loadl_epi64((const __m128i *)q);
		bits = mode + 1;
		int rembits = 8 - bits;
		__m128i vrembits = _mm_cvtsi32_si128(rembits);
		__m128i vbits = _mm_cvtsi32_si128(bits);

		__m128i vrembitsn = _mm_slli_epi64(vrembits, 2);
		z = _mm_sll_epi64(w, vrembitsn);
		w = _mm_blend_epi16(w, z, 0xcc);
		vrembitsn = _mm_srli_epi64(vrembitsn, 1);
		z = _mm_sll_epi64(w, vrembitsn);
		w = _mm_blend_epi16(w, z, 0xaa);
		z = _mm_sll_epi64(w, vrembits);
		w = _mm_blendv_epi8(w, z, _mm_set1_epi16((short)0xff00));
		mask = _mm_sub_epi64(_mm_sll_epi64(_mm_set1_epi8(1), vbits), _mm_set1_epi8(1));
		w = _mm_and_si128(w, mask);
		__m128i offset = _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits);
		w = _mm_sub_epi8(w, offset);
	}

	q += bits;

	shift += 3;
	if (shift == 24)
	{
		r += 3;
		shift = 0;
	}

	return w;
}

template<int F>
void tuned_Unpack8SymAndRestorePlanarGradient8(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pPacked, const uint8_t *pControl, size_t cbStride)
{
	int shift = 0;
	auto q = pPacked;
	auto r = pControl;

	{
		__m128i prev = _mm_set1_epi8((char)0x80);

		for (auto p = pDstBegin; p != pDstBegin + cbStride; p += 16)
		{
			__m128i w0 = unpacker<F>(q, r, shift);
			__m128i w1 = unpacker<F>(q, r, shift);
			__m128i s0 = _mm_unpacklo_epi64(w0, w1);

			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 1));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 2));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 4));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 8));
			s0 = _mm_add_epi8(s0, prev);
			_mm_storeu_si128((__m128i *)p, s0);
			prev = _mm_shuffle_epi8(s0, _mm_set1_epi8(15));
		}
	}

	for (auto pp = pDstBegin + cbStride; pp != pDstEnd; pp += cbStride)
	{
		__m128i prev = _mm_set1_epi8((char)0);

		for (auto p = pp; p != pp + cbStride; p += 16)
		{
			__m128i w0 = unpacker<F>(q, r, shift);
			__m128i w1 = unpacker<F>(q, r, shift);
			__m128i s0 = _mm_unpacklo_epi64(w0, w1);

			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 1));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 2));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 4));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 8));
			s0 = _mm_add_epi8(s0, prev);
			_mm_storeu_si128((__m128i *)p, _mm_add_epi8(s0, _mm_loadu_si128((const __m128i *)(p - cbStride))));
			prev = _mm_shuffle_epi8(s0, _mm_set1_epi8(15));
		}
	}
}

#ifdef GENERATE_SSE41
template void tuned_Unpack8SymAndRestorePlanarGradient8<CODEFEATURE_SSE41>(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pPacked, const uint8_t *pControl, size_t cbStride);
#endif

#ifdef GENERATE_AVX1
template void tuned_Unpack8SymAndRestorePlanarGradient8<CODEFEATURE_AVX1>(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pPacked, const uint8_t *pControl, size_t cbStride);
#endif


template<int F>
static inline FORCEINLINE void VECTORCALL PackForDelta(uint8_t*& q, uint8_t*& r, int& shift, __m128i wa, __m128i wb, __m128i ta, __m128i tb)
{
	int bits0, bits1, bits2, bits3;

	auto getvbits = [](__m128i xa, __m128i xb)
	{
		__m128i za, zb;
		__m128i viszeroa = _mm_cmpeq_epi64(xa, _mm_setzero_si128());
		__m128i viszerob = _mm_cmpeq_epi64(xb, _mm_setzero_si128());
		__m128i viszero = _mm_blend_epi16(viszeroa, viszerob, 0xcc);
		__m128i signsa = _mm_cmpgt_epi8(_mm_setzero_si128(), xa);
		__m128i signsb = _mm_cmpgt_epi8(_mm_setzero_si128(), xb);
		za = _mm_xor_si128(xa, signsa);
		zb = _mm_xor_si128(xb, signsb);
		za = _mm_or_si128(za, _mm_srli_epi64(za, 32));
		zb = _mm_or_si128(zb, _mm_slli_epi64(zb, 32));
		__m128i z = _mm_blend_epi16(za, zb, 0xcc);
		z = _mm_or_si128(_mm_or_si128(z, _mm_set1_epi32(1)), _mm_srli_epi32(z, 16));
		z = _mm_or_si128(z, _mm_srli_epi32(z, 8));
		z = _mm_and_si128(z, _mm_set1_epi32(0xff));

		z = _mm_castps_si128(_mm_cvtepi32_ps(z));
		z = _mm_srli_epi32(z, 23);
		z = _mm_sub_epi32(z, _mm_set1_epi32(0x7d));

		return _mm_andnot_si128(viszero, z);
	};

	__m128i vbitsw = getvbits(wa, wb);
	__m128i vbitst = getvbits(ta, tb);
	__m128i vspatial = _mm_cmpgt_epi32(vbitst, vbitsw);
	wa = _mm_blendv_epi8(ta, wa, _mm_shuffle_epi32(vspatial, _MM_SHUFFLE(2, 2, 0, 0)));
	wb = _mm_blendv_epi8(tb, wb, _mm_shuffle_epi32(vspatial, _MM_SHUFFLE(3, 3, 1, 1)));
	__m128i vbits = _mm_min_epi32(vbitsw, vbitst);
	__m128i vrembits = _mm_sub_epi32(_mm_set1_epi32(8), vbits);
	__m128i vmode = _mm_add_epi32(vbits, _mm_cmpgt_epi32(vbits, _mm_setzero_si128()));
	vmode = _mm_or_si128(vmode, _mm_andnot_si128(vspatial, _mm_set1_epi32(8)));

	bits0 = _mm_cvtsi128_si32(vbits);
	bits1 = _mm_extract_epi32(vbits, 2);
	bits2 = _mm_extract_epi32(vbits, 1);
	bits3 = _mm_extract_epi32(vbits, 3);

	__m128i vrembitsa = _mm_and_si128(vrembits, _mm_set1_epi64x(0xffffffff));
	__m128i vrembits0 = vrembitsa;
	__m128i vrembits1 = _mm_srli_si128(vrembitsa, 8);
	__m128i vrembitsb = _mm_srli_epi64(vrembits, 32);
	__m128i vrembits2 = vrembitsb;
	__m128i vrembits3 = _mm_srli_si128(vrembitsb, 8);
	__m128i w0 = _mm_add_epi8(wa, _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits0));
	__m128i w1 = _mm_add_epi8(_mm_srli_si128(wa, 8), _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits1));
	__m128i w2 = _mm_add_epi8(wb, _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits2));
	__m128i w3 = _mm_add_epi8(_mm_srli_si128(wb, 8), _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits3));
	w0 = _mm_or_si128(_mm_and_si128(w0, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w0), vrembits0));
	w1 = _mm_or_si128(_mm_and_si128(w1, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w1), vrembits1));
	w2 = _mm_or_si128(_mm_and_si128(w2, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w2), vrembits2));
	w3 = _mm_or_si128(_mm_and_si128(w3, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w3), vrembits3));
	vrembits0 = _mm_slli_epi64(vrembits0, 1);
	vrembits1 = _mm_slli_epi64(vrembits1, 1);
	vrembits2 = _mm_slli_epi64(vrembits2, 1);
	vrembits3 = _mm_slli_epi64(vrembits3, 1);
	w0 = _mm_or_si128(_mm_and_si128(w0, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w0), vrembits0));
	w1 = _mm_or_si128(_mm_and_si128(w1, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w1), vrembits1));
	w2 = _mm_or_si128(_mm_and_si128(w2, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w2), vrembits2));
	w3 = _mm_or_si128(_mm_and_si128(w3, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w3), vrembits3));
	vrembits0 = _mm_slli_epi64(vrembits0, 1);
	vrembits1 = _mm_slli_epi64(vrembits1, 1);
	vrembits2 = _mm_slli_epi64(vrembits2, 1);
	vrembits3 = _mm_slli_epi64(vrembits3, 1);
	w0 = _mm_or_si128(_mm_and_si128(w0, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w0), vrembits0));
	w1 = _mm_or_si128(_mm_and_si128(w1, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w1), vrembits1));
	w2 = _mm_or_si128(_mm_and_si128(w2, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w2), vrembits2));
	w3 = _mm_or_si128(_mm_and_si128(w3, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w3), vrembits3));
	_mm_storel_epi64((__m128i*)q, w0);
	q += bits0;
	_mm_storel_epi64((__m128i*)q, w1);
	q += bits1;
	_mm_storel_epi64((__m128i*)q, w2);
	q += bits2;
	_mm_storel_epi64((__m128i*)q, w3);
	q += bits3;

	vmode = _mm_shuffle_epi8(vmode, _mm_set_epi8(-1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0));
#ifdef __x86_64__
	uint64_t modes = _mm_cvtsi128_si64(vmode);
	modes |= modes >> (32 - 4);
#else
	vmode = _mm_or_si128(vmode, _mm_srli_epi64(vmode, 32 - 4));
	uint32_t modes = _mm_cvtsi128_si32(vmode);
#endif
	modes |= modes >> (16 - 8);

	*(uint32_t *)r |= (((uint32_t)(uint16_t)modes) << shift);
	shift += 16;
	if (shift == 32)
	{
		r += 4;
		shift = 0;
	}
}

template<int F>
void tuned_Pack8SymWithDiff8(uint8_t *pPacked, size_t *cbPacked, uint8_t *pControl, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, const uint8_t *pPrevBegin, size_t cbStride)
{
	int shift = 0;
	auto q = pPacked;
	auto r = pControl;
	memset(pControl, 0, (pSrcEnd - pSrcBegin) / 64 * 4);

	{
		__m128i prev = _mm_set1_epi8((char)0x80);

		for (auto p = pSrcBegin, t = pPrevBegin; p != pSrcBegin + cbStride; p += 32, t += 32)
		{
			__m128i value0 = _mm_loadu_si128((const __m128i *)p);
			__m128i value1 = _mm_loadu_si128((const __m128i *)(p + 16));
			__m128i left0 = _mm_alignr_epi8(value0, prev, 15);
			__m128i left1 = _mm_alignr_epi8(value1, value0, 15);

			__m128i error0 = _mm_sub_epi8(value0, left0);
			__m128i error1 = _mm_sub_epi8(value1, left1);
			__m128i t0 = _mm_sub_epi8(value0, _mm_loadu_si128((const __m128i *)t));
			__m128i t1 = _mm_sub_epi8(value1, _mm_loadu_si128((const __m128i *)(t + 16)));
			prev = value1;
			PackForDelta<F>(q, r, shift, error0, error1, t0, t1);
		}
	}

	for (auto pp = pSrcBegin + cbStride, tt = pPrevBegin + cbStride; pp != pSrcEnd; pp += cbStride, tt += cbStride)
	{
		__m128i prev = _mm_set1_epi8((char)0x80);
		__m128i topprev = _mm_set1_epi8((char)0x80);

		for (auto p = pp, t = tt; p != pp + cbStride; p += 32, t += 32)
		{
			__m128i value0 = _mm_loadu_si128((const __m128i *)p);
			__m128i value1 = _mm_loadu_si128((const __m128i *)(p + 16));
			__m128i top0 = _mm_loadu_si128((const __m128i *)(p - cbStride));
			__m128i top1 = _mm_loadu_si128((const __m128i *)(p + 16 - cbStride));
			__m128i left0 = _mm_alignr_epi8(value0, prev, 15);
			__m128i left1 = _mm_alignr_epi8(value1, value0, 15);
			__m128i topleft0 = _mm_alignr_epi8(top0, topprev, 15);
			__m128i topleft1 = _mm_alignr_epi8(top1, top0, 15);

			__m128i error0 = _mm_sub_epi8(_mm_add_epi8(value0, topleft0), _mm_add_epi8(left0, top0));
			__m128i error1 = _mm_sub_epi8(_mm_add_epi8(value1, topleft1), _mm_add_epi8(left1, top1));
			__m128i t0 = _mm_sub_epi8(value0, _mm_loadu_si128((const __m128i *)t));
			__m128i t1 = _mm_sub_epi8(value1, _mm_loadu_si128((const __m128i *)(t + 16)));
			prev = value1;
			topprev = top1;
			PackForDelta<F>(q, r, shift, error0, error1, t0, t1);
		}
	}

	*cbPacked = q - pPacked;
}

#ifdef GENERATE_SSE41
template void tuned_Pack8SymWithDiff8<CODEFEATURE_SSE41>(uint8_t *pPacked, size_t *cbPacked, uint8_t *pControl, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, const uint8_t *pPrevBegin, size_t cbStride);
#endif

#ifdef GENERATE_AVX1
template void tuned_Pack8SymWithDiff8<CODEFEATURE_AVX1>(uint8_t *pPacked, size_t *cbPacked, uint8_t *pControl, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, const uint8_t *pPrevBegin, size_t cbStride);
#endif


template<int F>
static inline FORCEINLINE std::pair<__m128i, __m128i> UnpackForDelta(const uint8_t*& q, const uint8_t *& r, int& shift)
{
	__m128i w;
	int mode = ((*(const uint32_t *)r) >> shift) & 7;
	__m128i rmask = _mm_cmpeq_epi64(_mm_and_si128(_mm_cvtsi32_si128(*(const uint32_t *)r >> shift), _mm_set1_epi64x(8)), _mm_set1_epi64x(8));
	int bits;
	if (mode == 0)
	{
		w = _mm_setzero_si128();
		bits = 0;
	}
	else
	{
		__m128i z, mask;

		w = _mm_loadl_epi64((const __m128i *)q);
		bits = mode + 1;
		int rembits = 8 - bits;
		__m128i vrembits = _mm_cvtsi32_si128(rembits);
		__m128i vbits = _mm_cvtsi32_si128(bits);

		__m128i vrembitsn = _mm_slli_epi64(vrembits, 2);
		z = _mm_sll_epi64(w, vrembitsn);
		w = _mm_blend_epi16(w, z, 0xcc);
		vrembitsn = _mm_srli_epi64(vrembitsn, 1);
		z = _mm_sll_epi64(w, vrembitsn);
		w = _mm_blend_epi16(w, z, 0xaa);
		z = _mm_sll_epi64(w, vrembits);
		w = _mm_blendv_epi8(w, z, _mm_set1_epi16((short)0xff00));
		mask = _mm_sub_epi64(_mm_sll_epi64(_mm_set1_epi8(1), vbits), _mm_set1_epi8(1));
		w = _mm_and_si128(w, mask);
		__m128i offset = _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits);
		w = _mm_sub_epi8(w, offset);
	}

	q += bits;

	shift += 4;
	if (shift == 32)
	{
		r += 4;
		shift = 0;
	}

	return { w, rmask };
}

template<int F>
void tuned_Unpack8SymWithDiff8(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pPacked, const uint8_t *pControl, const uint8_t *pPrevBegin, size_t cbStride)
{
	int shift = 0;
	auto q = pPacked;
	auto r = pControl;

	{
		__m128i prev = _mm_set1_epi8((char)0x80);

		auto t = pPrevBegin;
		for (auto p = pDstBegin; p != pDstBegin + cbStride; p += 16, t += 16)
		{
			auto w0 = UnpackForDelta<F>(q, r, shift);
			auto w1 = UnpackForDelta<F>(q, r, shift);
			__m128i s0 = _mm_unpacklo_epi64(w0.first, w1.first);
			__m128i m0 = _mm_unpacklo_epi64(w0.second, w1.second);

			auto t0 = _mm_add_epi8(s0, _mm_loadu_si128((const __m128i*)t));
			auto a = _mm_alignr_epi8(_mm_and_si128(t0, m0), prev, 15);
			s0 = _mm_andnot_si128(m0, _mm_add_epi8(s0, a));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 1));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 2));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 4));
			s0 = _mm_add_epi8(s0, _mm_andnot_si128(m0, _mm_slli_si128(s0, 8)));
			s0 = _mm_blendv_epi8(s0, t0, m0);
			_mm_storeu_si128((__m128i *)p, s0);
			prev = s0;
		}
	}

	auto tt = pPrevBegin + cbStride;
	for (auto pp = pDstBegin + cbStride; pp != pDstEnd; pp += cbStride, tt += cbStride)
	{
		__m128i prev = _mm_set1_epi8((char)0);

		auto t = tt;
		for (auto p = pp; p != pp + cbStride; p += 16, t += 16)
		{
			auto w0 = UnpackForDelta<F>(q, r, shift);
			auto w1 = UnpackForDelta<F>(q, r, shift);
			__m128i s0 = _mm_unpacklo_epi64(w0.first, w1.first);
			__m128i m0 = _mm_unpacklo_epi64(w0.second, w1.second);

			__m128i top = _mm_loadu_si128((const __m128i*)(p - cbStride));
			auto t0 = _mm_add_epi8(s0, _mm_loadu_si128((const __m128i*)t));
			auto a = _mm_alignr_epi8(_mm_and_si128(_mm_sub_epi8(t0, top), m0), prev, 15);
			s0 = _mm_andnot_si128(m0, _mm_add_epi8(s0, a));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 1));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 2));
			s0 = _mm_add_epi8(s0, _mm_slli_si128(s0, 4));
			s0 = _mm_add_epi8(s0, _mm_andnot_si128(m0, _mm_slli_si128(s0, 8)));
			s0 = _mm_add_epi8(s0, top);
			s0 = _mm_blendv_epi8(s0, t0, m0);
			_mm_storeu_si128((__m128i *)p, s0);
			prev = _mm_sub_epi8(s0, top);
		}
	}
}

#ifdef GENERATE_SSE41
template void tuned_Unpack8SymWithDiff8<CODEFEATURE_SSE41>(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pPacked, const uint8_t *pControl, const uint8_t *pPrevBegin, size_t cbStride);
#endif

#ifdef GENERATE_AVX1
template void tuned_Unpack8SymWithDiff8<CODEFEATURE_AVX1>(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pPacked, const uint8_t *pControl, const uint8_t *pPrevBegin, size_t cbStride);
#endif
