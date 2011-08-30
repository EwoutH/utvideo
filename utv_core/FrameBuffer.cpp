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

void CFrameBuffer::AddPlane(size_t cbBuffer, size_t cbMargin)
{
	SYSTEM_INFO si;
	size_t cbAllocateUnit;
	size_t cbAllocated;
	uint8_t *pAllocatedAddr;

	GetSystemInfo(&si);
	cbAllocateUnit = si.dwPageSize;

	cbMargin = ROUNDUP(cbMargin, cbAllocateUnit);
	cbAllocated = cbMargin + ROUNDUP(cbBuffer + cbMargin, cbAllocateUnit);

	pAllocatedAddr = (uint8_t *)VirtualAlloc(NULL, cbAllocated, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAllocatedAddr == NULL)
		return;

	m_pAllocatedAddr[m_nPlanes] = pAllocatedAddr;
	m_cbAllocated[m_nPlanes] = cbAllocated;
	/*
	 * �v���[�����ƂɊJ�n�A�h���X�����������炵�A�L���b�V���̃X���b�V���O���������B
	 * 256 �̓}�W�b�N�i���o�[�ł��邪�A
	 *   - �L���b�V�����C���T�C�Y�i�ŋ߂̑����̃v���Z�b�T�ł� 64 �o�C�g�j�̐����{
	 *   - �L���b�V���T�C�Y���\��������
	 * �𖞂����K�v������B
	 */
	m_pBufferAddr[m_nPlanes] = pAllocatedAddr + cbMargin + m_nPlanes * 256;

	m_nPlanes++;

	return;
}
