/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "test_win_fmt.h"
#include "test_dmo_fmt.h"

void dmo_DMOGetName(REFGUID clsidCodec, wstring wstrName)
{
	HRESULT hr;
	WCHAR name[128];

	hr = DMOGetName(clsidCodec, name);
	BOOST_REQUIRE(hr == S_OK);
	BOOST_CHECK_EQUAL(wstring(name), wstrName);
}

BOOST_DATA_TEST_CASE(dmo_DMOGetName_encoder, data::make(vecCodecFourcc) ^ data::make(vecEncoderName), fcc, wstrName)
{
	DMOEncoderCLSID guid(fcc);
	dmo_DMOGetName(guid, wstrName);
}

BOOST_DATA_TEST_CASE(dmo_DMOGetName_decoder, data::make(vecCodecFourcc) ^ data::make(vecDecoderName), fcc, wstrName)
{
	DMODecoderCLSID guid(fcc);
	dmo_DMOGetName(guid, wstrName);
}
