/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

class ICCloser
{
private:
	HIC m_hic;

public:
	ICCloser(HIC hic) : m_hic(hic)
	{
	}

	~ICCloser()
	{
		ICClose(m_hic);
	}
};