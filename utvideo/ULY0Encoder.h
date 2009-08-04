/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */
/*
 * Ut Video Codec Suite
 * Copyright (C) 2008-2009  UMEZAWA Takeshi
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
 * Copyright (C) 2008-2009  �~�V �Ўu
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

class CULY0Encoder :
	public CEncoder
{
protected:
	ENCODERCONF m_ec;
	BOOL m_bBottomUpFrame;
	DWORD m_dwNumStripes;
	DWORD m_dwDivideCount;
	DWORD m_dwPlaneSize[4];
	DWORD m_dwPlaneWidth[4];
	DWORD m_dwPlaneStripeSize[4];

	CThreadManager *m_ptm;
	const ICCOMPRESS *m_icc;
	CFrameBuffer *m_pCurFrame;
	CFrameBuffer *m_pMedianPredicted;
	struct COUNTS
	{
		DWORD dwCount[4][256];
	} *m_counts;
	BYTE *m_pCodeLengthTable[4];
	HUFFMAN_ENCODE_TABLE m_het[4];

public:
	struct INPUTFORMAT
	{
		DWORD fcc;
		WORD nBitCount;
	};

private:
	static const INPUTFORMAT m_infmts[];

public:
	CULY0Encoder(void);
	virtual ~CULY0Encoder(void);
	static CEncoder *CreateInstance(void);

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

protected:
	virtual DWORD GetOutputFCC(void) { return FCC('ULY0'); }
	virtual const char *GetColorFormatName(void) { return "YUV420"; }
	virtual WORD GetOutputBitCount(void) { return 12; }
	virtual WORD GetMaxBitCount(void) { return 24; }
	virtual const INPUTFORMAT *GetSupportedInputFormats(void) { return m_infmts; }
	virtual int GetNumSupportedInputFormats(void);
	virtual int GetNumPlanes(void) { return 3; }
	virtual void CalcPlaneSizes(const BITMAPINFOHEADER *pbihIn);
	virtual void ConvertToPlanar(DWORD nBandIndex);
	virtual int GetMacroPixelWidth(void) { return 2; }
	virtual int GetMacroPixelHeight(void) { return 2; }

private:
	void PredictProc(DWORD nBandIndex);
	class CPredictJob : public CThreadJob
	{
	private:
		DWORD m_nBandIndex;
		CULY0Encoder *m_pEncoder;
	public:
		CPredictJob(CULY0Encoder *pEncoder, DWORD nBandIndex)
		{
			m_nBandIndex = nBandIndex;
			m_pEncoder = pEncoder;
		}
		void JobProc(CThreadManager *)
		{
			m_pEncoder->PredictProc(m_nBandIndex);
		}
	};

	void EncodeProc(DWORD nBandIndex);
	class CEncodeJob : public CThreadJob
	{
	private:
		DWORD m_nBandIndex;
		CULY0Encoder *m_pEncoder;
	public:
		CEncodeJob(CULY0Encoder *pEncoder, DWORD nBandIndex)
		{
			m_nBandIndex = nBandIndex;
			m_pEncoder = pEncoder;
		}
		void JobProc(CThreadManager *)
		{
			m_pEncoder->EncodeProc(m_nBandIndex);
		}
	};
};
