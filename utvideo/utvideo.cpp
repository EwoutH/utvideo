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

#include "stdafx.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

const DWORD fccVIDC = FCC('VIDC');
const DWORD fccUAY2 = FCC('UAY2');

HMODULE hModule;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		::hModule = hModule;

	return TRUE;
}

BOOL APIENTRY ICInstallSelf(void)
{
	char szFilename[MAX_PATH];

	GetModuleFileName(hModule, szFilename, sizeof(szFilename));
	return ICInstall(fccVIDC, fccUAY2, (LPARAM)szFilename, NULL, ICINSTALL_DRIVER);
}

BOOL APIENTRY ICRemoveSelf(void)
{
	return ICRemove(fccVIDC, fccUAY2, 0);
}

/*
 * ICInstall()/ICRemove() �̕Ԃ�l�͐M�p�ł��Ȃ��i��� nonzero ���Ԃ�j�̂ŁA
 * �G���[�`�F�b�N�͍s��Ȃ��B
 */

void CALLBACK ICInstallByRundll(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	ICInstallSelf();
}

void CALLBACK ICRemoveByRundll(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	ICRemoveSelf();
}

LRESULT CALLBACK DriverProc(DWORD dwDriverId, HDRVR hdrvr,UINT uMsg, LPARAM lParam1, LPARAM lParam2)
{
	return DefDriverProc(dwDriverId, hdrvr, uMsg, lParam1, lParam2);
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
