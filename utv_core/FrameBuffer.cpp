/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "utvideo.h"
#include "FrameBuffer.h"

CFrameBuffer::CFrameBuffer(void)
{
	for (int i = 0; i < MAX_PLANE; i++)
	{
		m_pAllocatedAddr[i] = NULL;
		m_pBufferAddr[i] = NULL;
	}
	m_nPlanes = 0;
}

CFrameBuffer::~CFrameBuffer(void)
{
	for (int i = 0; i < MAX_PLANE; i++)
	{
		if (m_pAllocatedAddr[i] != NULL)
			VirtualFree(m_pAllocatedAddr[i], 0, MEM_RELEASE);
	}
}

void CFrameBuffer::AddPlane(DWORD dwSize, DWORD dwMarginSize)
{
	SYSTEM_INFO si;
	DWORD dwPageSize;
	DWORD dwPrecedingMarginSize;
	DWORD dwAllocateSize;
	uint8_t *pAllocatedAddr;

	GetSystemInfo(&si);
	dwPageSize = si.dwPageSize;

	dwPrecedingMarginSize = ROUNDUP(dwMarginSize, dwPageSize);
	dwAllocateSize = dwPrecedingMarginSize + ROUNDUP(dwSize + max(dwMarginSize, dwPageSize), dwPageSize);

	pAllocatedAddr = (uint8_t *)VirtualAlloc(NULL, dwAllocateSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAllocatedAddr == NULL)
		return;

	m_pAllocatedAddr[m_nPlanes] = pAllocatedAddr;
	/*
	 * �v���[�����ƂɊJ�n�A�h���X�����������炵�A�L���b�V���̃X���b�V���O���������B
	 * 256 �̓}�W�b�N�i���o�[�ł��邪�A
	 *   - �L���b�V�����C���T�C�Y�i�ŋ߂̑����̃v���Z�b�T�ł� 64 �o�C�g�j�̐����{
	 *   - �L���b�V���T�C�Y���\��������
	 * �𖞂����K�v������B
	 */
	m_pBufferAddr[m_nPlanes] = pAllocatedAddr + dwPrecedingMarginSize + m_nPlanes * 256;

	m_nPlanes++;

	return;
}
