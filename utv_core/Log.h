/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

class CLogInitializer
{
private:
	CLogInitializer();
	~CLogInitializer();
	static CLogInitializer __li__;

public:
	static void Initialize();
};
