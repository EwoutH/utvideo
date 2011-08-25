/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

#include "TunedFunc.h"

// intra-frame prediction
void PredictMedianAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, DWORD *pCountTable);
#define PredictLeftAndCount tfn.pfnPredictLeftAndCount_align1
//void RestoreMedian(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);
#define RestoreMedian tfn.pfnRestoreMedian_align1

void cpp_PredictMedianAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, DWORD *pCountTable);
void cpp_PredictLeftAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, DWORD *pCountTable);
void cpp_RestoreMedian(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);

#ifdef _WIN32_X86
extern "C" void x86_sse2_PredictMedianAndCount_align16(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, DWORD *pCountTable);
extern "C" void x86_sse2_PredictMedianAndCount_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, DWORD *pCountTable);
extern "C" void x86_sse2_PredictLeftAndCount_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, DWORD *pCountTable);
extern "C" void x86_i686_RestoreMedian_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);
extern "C" void x86_sse1mmx_RestoreMedian_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);
#endif

#ifdef _WIN64_X64
extern "C" void x64_sse2_PredictMedianAndCount_align16(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, DWORD *pCountTable);
extern "C" void x64_sse2_PredictMedianAndCount_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, DWORD *pCountTable);
extern "C" void x64_sse2_PredictLeftAndCount_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, DWORD *pCountTable);
extern "C" void x64_sse1mmx_RestoreMedian_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);
#endif
