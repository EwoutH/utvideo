/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

#include <LogWriter.h>

DLLEXPORT bool IsInterfaceDisabled(const char* pszInterface, const char* pszRole);

static inline bool CheckInterfaceDisabledAndLog(const char* pszInterface, const char* pszRole)
{
	if (IsInterfaceDisabled(pszInterface, pszRole))
	{
		LOGPRINTF("%s %s is disabled.", pszInterface, pszRole);
		return true;
	}
	else
	{
		return false;
	}
}
