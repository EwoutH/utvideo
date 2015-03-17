/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"

#include <LogReader.h>


void OpenProc(int id, int idx, const char *ident);
void ReadProc(int id, int idx, const char *ident, const char *buf);
void CloseProc(int id, int idx, const char *ident);

int main(int argc, char **argv)
{
	InitializeLogReader(OpenProc, ReadProc, CloseProc, true);
	printf("%d\n", GetLastError());
	return 0;
}

void OpenProc(int id, int idx, const char *ident)
{
	printf("{%d} %s: OPEN\n", id, ident);
}

void ReadProc(int id, int idx, const char *ident, const char *buf)
{
	printf("{%d} %s: %s\n", id, ident, buf);
}

void CloseProc(int id, int idx, const char *ident)
{
	printf("{%d} %s: CLOSE\n", id, ident);
}
