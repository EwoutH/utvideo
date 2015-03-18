/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

int InitializeLogWriter(void);
int WriteLog(const char *p);
int UninitializeLogWriter(void);

#ifdef _WIN32

extern HANDLE hLogPipe;

#define IsLogWriterInitialized() (::hLogPipe != INVALID_HANDLE_VALUE)

#define LOGPRINTF(__fmt__, ...) \
	do \
	{ \
		if (IsLogWriterInitialized()) \
		{ \
			char __LOGPRINTF_local_buf__[256]; \
			sprintf(__LOGPRINTF_local_buf__, __fmt__, __VA_ARGS__); \
			WriteLog(__LOGPRINTF_local_buf__); \
		} \
	} while (false)

#endif
