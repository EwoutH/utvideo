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
#include "Encoder.h"
#include "FrameBuffer.h"
#include "HuffmanCode.h"
#include "Thread.h"

class CPackedEncoder :
	public CEncoder
{
protected:
	ENCODERCONF m_ec;
	//BOOL m_bBottomUpFrame;
	DWORD m_dwNumStrides;
	DWORD m_dwStrideSize;
	DWORD m_dwDivideCount;
	//DWORD m_dwFrameSize;
	//DWORD m_dwFrameStride;
	//DWORD m_dwPlaneSize[4];
	//DWORD m_dwPlaneStride[4];

	CThreadManager *m_ptm;
	const ICCOMPRESS *m_icc;
	//CFrameBuffer *m_pCurFrame;
	//CFrameBuffer *m_pMedianPredicted;
	/*struct COUNTS
	{
		DWORD dwCount[4][256];
	} *m_counts;*/
	//BYTE *m_pCodeLengthTable[4];
	HUFFMAN_ENCODE_TABLE m_het[4];
	HANDLE *m_phEncodeCompletionEvent;
	CFrameBuffer *m_pEncodeBuffer;
	BYTE *m_pEncodedDataBase;
	DWORD *m_pdwTailOffsetTable;
	DWORD (*m_pfnHuffmanEncodeFirstRawWithDiff)(BYTE *pDstBegin, const BYTE *pSrcBegin, const BYTE *pSrcEnd, const HUFFMAN_ENCODE_TABLE *pEncodeTable);


public:
	struct INPUTFORMAT
	{
		DWORD fcc;
		WORD nBitCount;
		WORD nEffectiveBitCount;
		BOOL bPositiveHeightAllowed;
		BOOL bNegativeHeightAllowed;
	};

public:
	CPackedEncoder(void);
	virtual ~CPackedEncoder(void);

public:
	virtual DWORD Configure(HWND hwnd);
	static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual DWORD GetState(void *pState, DWORD dwSize);
	virtual DWORD SetState(const void *pState, DWORD dwSize);
	virtual DWORD Compress(const ICCOMPRESS *icc, DWORD dwSize);
	virtual DWORD CompressBegin(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);
	virtual DWORD CompressEnd(void);
	virtual DWORD CompressGetFormat(const BITMAPINFOHEADER *pbihIn, BITMAPINFOHEADER *pbihOut);
	virtual DWORD CompressGetSize(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);
	virtual DWORD CompressQuery(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut);

	const INPUTFORMAT *LookupInputFormat(const BITMAPINFOHEADER *pbihIn);

protected:
	virtual DWORD GetOutputFCC(void) = 0;
	virtual const char *GetColorFormatName(void) = 0;
	virtual WORD GetOutputBitCount(void) = 0;
	virtual const INPUTFORMAT *GetSupportedInputFormats(void) = 0;
	virtual int GetNumSupportedInputFormats(void) = 0;
	virtual int GetNumChannels(void) = 0;
	virtual void SetCompressionProperty(const BITMAPINFOHEADER *pbihIn, const BITMAPINFOHEADER *pbihOut) = 0;

private:
	void EncodeProc(DWORD nStripIndex);
	class CEncodeJob : public CThreadJob
	{
	private:
		DWORD m_nStripIndex;
		CPackedEncoder *m_pEncoder;
	public:
		CEncodeJob(CPackedEncoder *pEncoder, DWORD nStripIndex)
		{
			m_nStripIndex = nStripIndex;
			m_pEncoder = pEncoder;
		}
		void JobProc(CThreadManager *)
		{
			m_pEncoder->EncodeProc(m_nStripIndex);
		}
	};
};
