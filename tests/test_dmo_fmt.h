/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

extern vector<wstring> vecEncoderName;
extern vector<wstring> vecDecoderName;

class DMOEncoderCLSID : public GUID
{
public:
	DMOEncoderCLSID(DWORD fcc) : GUID({ fcc, 0xE991, 0x460D, 0x84, 0x0B, 0xC1, 0xC6, 0x49, 0x74, 0x57, 0xEF }) {}
};

class DMODecoderCLSID : public GUID
{
public:
	DMODecoderCLSID(DWORD fcc) : GUID({ fcc, 0xD991, 0x460D, 0x84, 0x0B, 0xC1, 0xC6, 0x49, 0x74, 0x57, 0xEF }) {}
};
