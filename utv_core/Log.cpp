/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "utvideo.h"
#include "Log.h"

CLogInitializer::CLogInitializer()
{
	Initialize();
}

CLogInitializer::~CLogInitializer()
{
	UninitializeLogWriter();
}

void CLogInitializer::Initialize()
{
	if (IsLogWriterInitialized())
		return;

	InitializeLogWriter();

	LOGPRINTF("Ut Video Codec Suite %s (%s)", UTVIDEO_VERSION_STR, UTVIDEO_IMPLEMENTATION_STR);
}

CLogInitializer CLogInitializer::__li__;
