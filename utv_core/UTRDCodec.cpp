/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "utvideo.h"
#include "utv_core.h"
#include "UTRDCodec.h"
#include "Predict.h"
#include "TunedFunc.h"
#include "resource.h"
#include "ByteOrder.h"
#include "TunedFunc_x86x64.h"
#include "Predict_x86x64.h"
#include <lz4.h>

const utvf_t CUTRDCodec::m_utvfEncoderInput[] = {
	UTVF_NFCC_BGR_BU,
	UTVF_INVALID,
};

const utvf_t CUTRDCodec::m_utvfDecoderOutput[] = {
	UTVF_NFCC_BGR_BU,
	UTVF_INVALID,
};

const utvf_t CUTRDCodec::m_utvfCompressed[] = {
	UTVF_UTRD,
	UTVF_INVALID,
};

CUTRDCodec::CUTRDCodec(const char *pszInterfaceName) : CCodecBase("UTRD", pszInterfaceName)
{
}

void CUTRDCodec::GetLongFriendlyName(char *pszName, size_t cchName)
{
	char buf[128];

	sprintf(buf, "UtVideo R&D");
	strncpy(pszName, buf, cchName);
	pszName[cchName - 1] = '\0';
}

#ifdef _WIN32
INT_PTR CUTRDCodec::Configure(HWND hwnd)
{
	return 0;
}
#endif

size_t CUTRDCodec::GetStateSize(void)
{
	return 0;
}

int CUTRDCodec::GetState(void *pState, size_t cb)
{
	return 0;
}

int CUTRDCodec::InternalSetState(const void *pState, size_t cb)
{
	return 0;
}

size_t CUTRDCodec::EncodeFrame(void *pOutput, bool *pbKeyFrame, const void *pInput)
{
	uint8_t *p;

	m_pInput = pInput;
	m_pOutput = pOutput;

	ConvertBGRToULRG(
		m_pCurFrame->GetPlane(1),
		m_pCurFrame->GetPlane(0),
		m_pCurFrame->GetPlane(2),
		((const uint8_t *)m_pInput) + m_nWidth * (m_nHeight - 1) * 3,
		((const uint8_t *)m_pInput) - m_nWidth * 3,
		m_nWidth * 3,
		-(ssize_t)m_nWidth * 3);

	for (int nPlaneIndex = 0; nPlaneIndex < 3; nPlaneIndex++)
	{
//		uint32_t counts[256];
/*		PredictCylindricalLeftAndCount8(
			m_pMedianPredicted->GetPlane(nPlaneIndex),
			m_pCurFrame->GetPlane(nPlaneIndex),
			m_pCurFrame->GetPlane(nPlaneIndex) + m_nWidth * m_nHeight,
			 counts);*/
		//PredictPlanarGradientAndCount8
		//PredictCylindricalWrongMedianAndCount8
		tuned_PredictPlanarGradient8<CODEFEATURE_AVX1>
		(
			m_pMedianPredicted->GetPlane(nPlaneIndex),
			m_pCurFrame->GetPlane(nPlaneIndex),
			m_pCurFrame->GetPlane(nPlaneIndex) + m_nWidth * m_nHeight,
			m_nWidth);
	}

	p = (uint8_t *)pOutput;

//	volatile unsigned n[4] = { 0, 0, 0, 0 };
	uint8_t *index = (uint8_t*)malloc(m_nWidth * m_nHeight);
	for (int nPlaneIndex = 0; nPlaneIndex < 3; nPlaneIndex++)
	{
		int shift = 0;
		memset(index, 0, m_nWidth * m_nHeight);
		uint8_t *idxp = index;
		uint32_t *pCompressedLen = (uint32_t*)p;
		uint32_t *pModeLen = (uint32_t*)(p + 4);
		p += 8;
		auto pCompressedBegin = p;
		for (unsigned y = 0; y < m_nHeight; y += 1)
		{
			for (unsigned x = 0; x < m_nWidth; x += 8)
			{
				uint64_t wi = *(uint64_t*)&m_pMedianPredicted->GetPlane(nPlaneIndex)[y * m_nWidth + x];
				__m128i w = _mm_cvtsi64x_si128(wi);

				int mode;
				int bits;

				if (wi == 0)
				{
					mode = 0;
					bits = 0;
				}
				else
				{
					__m128i z;
					__m128i signs = _mm_cmpgt_epi8(_mm_setzero_si128(), w);
					z = _mm_xor_si128(w, signs);
					z = _mm_or_si128(z, _mm_srli_epi64(z, 32));
					z = _mm_or_si128(z, _mm_srli_epi64(z, 16));
					z = _mm_or_si128(z, _mm_srli_epi64(z, 8));
					z = _mm_and_si128(z, _mm_set1_epi64x(0xff));
					z = _mm_or_si128(z, _mm_set1_epi64x(1));
					_BitScanReverse((unsigned long *)&mode, _mm_cvtsi128_si32(z));
					bits = mode + 2;
					int rembits = 6 - mode;
					mode++;
					__m128i vrembits = _mm_cvtsi64x_si128(rembits);
					w = _mm_add_epi8(w, _mm_srl_epi64(_mm_set1_epi8((char)0x80), vrembits));
					w = _mm_or_si128(_mm_and_si128(w, _mm_set1_epi16(0x00ff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi16(0x00ff), w), vrembits));
					vrembits = _mm_slli_epi64(vrembits, 1);
					w = _mm_or_si128(_mm_and_si128(w, _mm_set1_epi32(0x0000ffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi32(0x0000ffff), w), vrembits));
					vrembits = _mm_slli_epi64(vrembits, 1);
					w = _mm_or_si128(_mm_and_si128(w, _mm_set1_epi64x(0x00000000ffffffff)), _mm_srl_epi64(_mm_andnot_si128(_mm_set1_epi64x(0x00000000ffffffff), w), vrembits));
					*(uint64_t*)p = _mm_cvtsi128_si64x(w);
				}

				p += bits;
				*(uint32_t *)idxp |= (mode << shift);
#define MODEBITS 3
				shift += MODEBITS;
				if (shift == MODEBITS*8)
				{
					idxp += MODEBITS;
					shift = 0;
				}
			}
		}
		*pCompressedLen = p - pCompressedBegin;
		size_t idxlen = idxp - index;
#define USE_LZ4 1
#if USE_LZ4
		*pModeLen = LZ4_compress_default((const char*)index, (char*)p, idxlen, m_nWidth * m_nHeight);
#else
		*pModeLen = idxlen;
		memcpy(p, index, idxlen);
#endif
		p += *pModeLen;
	}

	free(index);
	*pbKeyFrame = true;

	return p - ((uint8_t *)pOutput);
}

int CUTRDCodec::InternalEncodeBegin(utvf_t infmt, unsigned int width, unsigned int height, size_t cbGrossWidth)
{
	int ret;

	ret = EncodeQuery(infmt, width, height);
	if (ret != 0)
		return ret;

	m_utvfRaw = infmt;
	m_nWidth = width;
	m_nHeight = height;


	m_pCurFrame = new CFrameBuffer();
	for (int i = 0; i < 3; i++)
		m_pCurFrame->AddPlane(width*height, width*2);

	m_pMedianPredicted = new CFrameBuffer();
	for (int i = 0; i < 3; i++)
		m_pMedianPredicted->AddPlane(width*height, width * 2);

	return 0;
}

int CUTRDCodec::InternalEncodeEnd(void)
{
	delete m_pCurFrame;
	delete m_pMedianPredicted;

	return 0;
}

size_t CUTRDCodec::EncodeGetExtraDataSize(void)
{
	return 0;
}

int CUTRDCodec::EncodeGetExtraData(void *pExtraData, size_t cb, utvf_t infmt, unsigned int width, unsigned int height)
{
	return 0;
}

size_t CUTRDCodec::EncodeGetOutputSize(utvf_t infmt, unsigned int width, unsigned int height)
{
	return ROUNDUP(width, 4) * ROUNDUP(height, 2) * 24 / 8 + 4096; // +4096 �͂ǂ�Ԃ芨��B
}

int CUTRDCodec::InternalEncodeQuery(utvf_t infmt, unsigned int width, unsigned int height)
{
	for (const utvf_t *utvf = GetEncoderInputFormat(); *utvf; utvf++)
	{
		if (infmt == *utvf)
			return 0;
	}

	return -1;
}

size_t CUTRDCodec::DecodeFrame(void *pOutput, const void *volatile pInput)
{
	const uint8_t* pPlaneBegin[3];

	{
		const uint8_t *p = (const uint8_t*)pInput;
		for (int nPlaneIndex = 0; nPlaneIndex < 3; nPlaneIndex++)
		{
			pPlaneBegin[nPlaneIndex] = p;
			uint32_t cbCompressed = *(uint32_t*)p;
			uint32_t cbMode = *(uint32_t*)(p + 4);
			p += 8;
			p += cbCompressed + cbMode;
		}
	}

	const uint8_t *p = (const uint8_t*)pInput;

	uint8_t *index = (uint8_t*)malloc(m_nWidth * m_nHeight);
	for (volatile int nPlaneIndex = 0; nPlaneIndex < 3; nPlaneIndex++)
	{
		_ASSERT(p == pPlaneBegin[nPlaneIndex]);
		uint32_t cbCompressed = *(uint32_t*)p;
		uint32_t cbMode = *(uint32_t*)(p + 4);
		p += 8;
#if USE_LZ4
		LZ4_decompress_safe((const char *)(p+cbCompressed),(char*)index, cbMode, m_nWidth * m_nHeight);
#else
		memcpy(index, p + cbCompressed, cbMode);
#endif
		int shift = 0;
		uint8_t *idxp = index;

		for (unsigned y = 0; y < m_nHeight; y += 1)
		{
			for (unsigned x = 0; x < m_nWidth; x += 8)
			{
				uint64_t w;
				int bits = ((*(uint32_t *)idxp) >> shift) & 7;
				if (bits == 0)
					w = 0;
				else
				{
					uint64_t z, mask;

					w = *(uint64_t *)p;
					bits++;
					int rembits = 8 - bits;

					z = w << rembits * 4;
					w = (w & 0x00000000ffffffffULL) | (z & 0xffffffff00000000ULL);

					z = w << rembits * 2;
					w = (w & 0x0000ffff0000ffffULL) | (z & 0xffff0000ffff0000ULL);

					z = w << rembits;
					w = (w & 0x00ff00ff00ff00ffULL) | (z & 0xff00ff00ff00ff00ULL);

					mask = (0x101010101010101ULL << bits) - 0x101010101010101ULL;
					w &= mask;

					uint64_t offset = 0x8080808080808080ULL >> rembits;
					uint64_t offadj = (~w & offset) << (rembits + 1); // packed byte ���Z�ł͂Ȃ��̂ň�������������␳����
					w -= offset;
					w += offadj;
				}

				p += bits;
				*(uint64_t*)&m_pDecodedFrame->GetPlane(nPlaneIndex)[y * m_nWidth + x] = w;

				shift += MODEBITS;
				if (shift == MODEBITS*8)
				{
					idxp += MODEBITS;
					shift = 0;
				}
			}
		}
		p += cbMode;

		//RestoreCylindricalWrongMedian8
		RestorePlanarGradient8
		(
			m_pRestoredFrame->GetPlane(nPlaneIndex),
			m_pDecodedFrame->GetPlane(nPlaneIndex),
			m_pDecodedFrame->GetPlane(nPlaneIndex) + m_nWidth * m_nHeight,
			m_nWidth);
	}

	ConvertULRGToBGR(
		((uint8_t*)pOutput) + m_nWidth * 3 * (m_nHeight - 1),
		((uint8_t*)pOutput) - m_nWidth * 3,
		m_pRestoredFrame->GetPlane(1),
		m_pRestoredFrame->GetPlane(0),
		m_pRestoredFrame->GetPlane(2),
		m_nWidth * 3,
		-(ssize_t)m_nWidth * 3);
	free(index);
	return m_cbRawSize;
}

int CUTRDCodec::DecodeGetFrameType(bool *pbKeyFrame, const void *pInput)
{
	*pbKeyFrame = true;
	return 0;
}

int CUTRDCodec::InternalDecodeBegin(utvf_t outfmt, unsigned int width, unsigned int height, size_t cbGrossWidth, const void *pExtraData, size_t cbExtraData)
{
	int ret;

	ret = DecodeQuery(outfmt, width, height, pExtraData, cbExtraData);
	if (ret != 0)
		return ret;

	m_utvfRaw = outfmt;
	m_nWidth = width;
	m_nHeight = height;

	m_pRestoredFrame = new CFrameBuffer();
	for (int i = 0; i < 3; i++)
		m_pRestoredFrame->AddPlane(width*height, width * 2);

	m_pDecodedFrame = new CFrameBuffer();
	for (int i = 0; i < 3; i++)
		m_pDecodedFrame->AddPlane(width*height, width * 2);

	return 0;
}

int CUTRDCodec::InternalDecodeEnd(void)
{
	delete m_pRestoredFrame;
	delete m_pDecodedFrame;

	return 0;
}

size_t CUTRDCodec::DecodeGetOutputSize(utvf_t outfmt, unsigned int width, unsigned int height, size_t cbGrossWidth)
{
	int ret;

	ret = CalcRawFrameMetric(outfmt, width, height, cbGrossWidth);
	if (ret != 0)
		return 0;

	return m_cbRawSize;
}

int CUTRDCodec::InternalDecodeQuery(utvf_t outfmt, unsigned int width, unsigned int height, const void *pExtraData, size_t cbExtraData)
{
	if (!outfmt)
		return 0;

	for (const utvf_t *utvf = GetDecoderOutputFormat(); *utvf; utvf++)
	{
		if (outfmt == *utvf)
			return 0;
	}

	return -1;
}