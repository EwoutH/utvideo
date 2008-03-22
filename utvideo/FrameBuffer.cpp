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

#include "StdAfx.h"
#include "utvideo.h"
#include "FrameBuffer.h"

CFrameBuffer::CFrameBuffer(void)
{
	m_pAllocatedAddr = NULL;
	m_pBufferAddr = NULL;
}

CFrameBuffer::~CFrameBuffer(void)
{
	if (m_pAllocatedAddr != NULL)
		VirtualFree(m_pAllocatedAddr, 0, MEM_RELEASE);
}

CFrameBuffer *CFrameBuffer::InternalNewBuffer(DWORD dwSize, DWORD dwMarginSize, DWORD flProtect)
{
	CFrameBuffer *pBuffer;
	SYSTEM_INFO si;
	DWORD dwPageSize;
	DWORD dwPrecedingMarginSize;
	DWORD dwAllocateSize;

	GetSystemInfo(&si);
	dwPageSize = si.dwPageSize;

	dwPrecedingMarginSize = ROUNDUP(dwMarginSize, dwPageSize);
	dwAllocateSize = dwPrecedingMarginSize + ROUNDUP(dwSize + dwMarginSize, dwPageSize);

	pBuffer = new CFrameBuffer();
	pBuffer->m_pAllocatedAddr = (BYTE *)VirtualAlloc(NULL, dwAllocateSize, MEM_COMMIT | MEM_RESERVE, flProtect);
	if (pBuffer->m_pAllocatedAddr == NULL)
	{
		delete pBuffer;
		return NULL;
	}
	pBuffer->m_pBufferAddr = pBuffer->m_pAllocatedAddr + dwPrecedingMarginSize;

	return pBuffer;
}
