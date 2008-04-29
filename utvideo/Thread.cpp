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
#include "Thread.h"

CThreadManager::CThreadManager(void)
{
	DWORD dwProcessAffinityMask;
	DWORD dwSystemAffinityMask;

	m_nNumThreads = 0;
	for (int i = 0; i < MAX_THREAD; i++)
		m_hThread[i] = NULL;

	GetProcessAffinityMask(GetCurrentProcess(), &dwProcessAffinityMask, &dwSystemAffinityMask);
	for (DWORD dwThreadAffinityMask = 1; dwThreadAffinityMask != 0; dwThreadAffinityMask <<= 1)
	{
		if (dwProcessAffinityMask & dwThreadAffinityMask)
		{
			m_hThread[m_nNumThreads] = CreateThread(NULL, 0, StaticThreadProc, this, CREATE_SUSPENDED, &m_dwThreadId[m_nNumThreads]);
			SetThreadAffinityMask(m_hThread[m_nNumThreads], dwThreadAffinityMask);
			m_nNumThreads++;
		}
	}

	for (int i = 0; i < m_nNumThreads; i++)
		ResumeThread(m_hThread[i]);
}

CThreadManager::~CThreadManager(void)
{
	WaitForMultipleObjects(m_nNumThreads, m_hThread, TRUE, INFINITE);
	for (int i = 0; i < m_nNumThreads; i++)
		CloseHandle(m_hThread[i]);
}

DWORD WINAPI CThreadManager::StaticThreadProc(LPVOID lpParameter)
{
	CThreadManager *pThis = (CThreadManager *)lpParameter;
	return pThis->ThreadProc();
}

DWORD CThreadManager::ThreadProc()
{
	_RPT0(_CRT_WARN, "CThreadManager::ThreadProc()\n");
	return 0;
}
