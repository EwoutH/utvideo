/* 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ */
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
 * Copyright (C) 2008  梅澤 威志
 * 
 * このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフ
 * トウェア財団によって発行された GNU 一般公衆利用許諾契約書(バージョ
 * ン2か、希望によってはそれ以降のバージョンのうちどれか)の定める条件
 * の下で再頒布または改変することができます。
 * 
 * このプログラムは有用であることを願って頒布されますが、*全くの無保
 * 証* です。商業可能性の保証や特定の目的への適合性は、言外に示された
 * ものも含め全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご
 * 覧ください。
 * 
 * あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を
 * 一部受け取ったはずです。もし受け取っていなければ、フリーソフトウェ
 * ア財団まで請求してください(宛先は the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA)。
 */

#pragma once

#include "TunedFunc.h"

// intra-frame prediction
void PredictMedian(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride);
void PredictMedianAndCount(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD *pCountTable);
//void RestoreMedian(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride);
#define RestoreMedian tfn.pfnRestoreMedian_align1

void cpp_PredictMedian(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride);
extern "C" void sse2_PredictMedian_align16(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride);

void cpp_PredictMedianAndCount(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD *pCountTable);
extern "C" void sse2_PredictMedianAndCount_align16(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD *pCountTable);
extern "C" void sse2_PredictMedianAndCount_align1(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride, DWORD *pCountTable);

void cpp_RestoreMedian(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride);
extern "C" void i686_RestoreMedian_align1(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride);
extern "C" void sse1mmx_RestoreMedian_align1(BYTE *pDst, const BYTE *pSrcBegin, const BYTE *pSrcEnd, DWORD dwStride);
