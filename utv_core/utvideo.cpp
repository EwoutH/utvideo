/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "utvideo.h"
#include "TunedFunc.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HMODULE hModule;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		::hModule = hModule;
		InitializeTunedFunc();
	}

	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
