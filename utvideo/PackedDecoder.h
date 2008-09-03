/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
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
 * Copyright (C) 2008  �~�V �Ўu
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
#include "Decoder.h"
#include "FrameBuffer.h"
#include "Thread.h"
#include "HuffmanCode.h"

class CPackedDecoder :
	public CDecoder
{
protected:
	BOOL m_bBottomUpFrame;
	DWORD m_dwNumStrides;
	DWORD m_dwDivideCount;
	DWORD m_dwFrameSize;
	DWORD m_dwStrideSize;
	//DWORD m_dwPlaneSize[4];
	//DWORD m_dwPlaneStride[4];

	CThreadManager *m_ptm;
	//CFrameBuffer *m_pCurFrame;
	//CFrameBuffer *m_pRestoredFrame;
	//CFrameBuffer *m_pDecodedFrame;
	FRAMEINFO m_fi;
	HUFFMAN_DECODE_TABLE m_hdt[4];
	//const BYTE *m_pCodeLengthTable[4];
	const ICDECOMPRESS *m_icd;
	const BYTE *m_pEncodedDataBase;
	const DWORD *m_pdwTailOffsetTable;
	void (*m_pfnHuffmanDecodeFirstRawWithAccum)(BYTE *pDstBegin, BYTE *pDstEnd, const BYTE *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable);

public:
	struct OUTPUTFORMAT
	{
		DWORD fcc;
		WORD nBitCount;
		WORD nEffectiveBitCount;
		BOOL bPositiveHeightAllowed;
		BOOL bNegativeHeightAllowed;
	};

public:
	CPackedDecoder(void);
	virtual ~CPackedDecoder(void);

public:
	virtual DWORD Decompress(const ICDECOMPRESS *icd, DWORD dwSize);
	virtual DWORD DecompressBegin(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);
	virtual DWORD DecompressEnd(void);
	virtual DWORD DecompressGetFormat(const BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut);
	virtual DWORD DecompressQuery(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);


	const OUTPUTFORMAT *LookupOutputFormat(const BITMAPINFOHEADER *pbihOut);

protected:
	virtual DWORD GetInputFCC(void) = 0;
	virtual WORD GetInputBitCount(void) = 0;
	virtual const OUTPUTFORMAT *GetSupportedOutputFormats(void) = 0;
	virtual int GetNumSupportedOutputFormats(void) = 0;
	virtual int GetNumChannels(void) = 0;
	virtual void SetDecompressionProperty(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut) = 0;

private:
	void DecodeProc(DWORD nStripIndex);
	class CDecodeJob : public CThreadJob
	{
	private:
		DWORD m_nStripIndex;
		CPackedDecoder *m_pDecoder;
	public:
		CDecodeJob(CPackedDecoder *pDecoder, DWORD nStripIndex)
		{
			m_nStripIndex = nStripIndex;
			m_pDecoder = pDecoder;
		}
		void JobProc(CThreadManager *)
		{
			m_pDecoder->DecodeProc(m_nStripIndex);
		}
	};
};
