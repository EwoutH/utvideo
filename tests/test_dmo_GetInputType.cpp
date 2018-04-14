/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "test_win_fmt.h"
#include "test_dmo_fmt.h"

void dmo_GetInputType(REFGUID clsidCodec, const vector<GUID> &guidIn, BOOL bFixedSizeSamples, BOOL bTemporalCompression)
{
	HRESULT hr;
	IMediaObject *pObj;
	vector<GUID> inTypes;
	DMO_MEDIA_TYPE mt;

	hr = CoCreateInstance(clsidCodec, NULL, CLSCTX_INPROC_SERVER, IID_IMediaObject, (LPVOID*)&pObj);
	BOOST_REQUIRE(hr == S_OK);
	BOOST_REQUIRE(pObj != NULL);

	for (DWORD idx = 0; (hr = pObj->GetInputType(0, idx, &mt)) == S_OK; ++idx)
	{
		BOOST_CHECK(mt.majortype == MEDIATYPE_Video);
		BOOST_CHECK(mt.bFixedSizeSamples == bFixedSizeSamples);
		BOOST_CHECK(mt.bTemporalCompression == bTemporalCompression);
		BOOST_CHECK(mt.formattype == GUID_NULL);
		inTypes.push_back(mt.subtype);
		MoFreeMediaType(&mt);
	}
	BOOST_CHECK(hr == DMO_E_NO_MORE_ITEMS);

	BOOST_CHECK_EQUAL(inTypes, guidIn);

	pObj->Release();
}

BOOST_TEST_DECORATOR(*depends_on("dmo_CoCreateInstance_encoder")*depends_on("dmo_QueryInterface_encoder"))
BOOST_DATA_TEST_CASE(dmo_GetInputType_encoder, data::make(vecCodecFcc) ^ data::make(vecSupportedEncoderInputSubtypes), fcc, guids)
{
	DMOEncoderCLSID clsid(fcc);
	dmo_GetInputType(clsid, guids, TRUE, FALSE);
}

BOOST_TEST_DECORATOR(*depends_on("dmo_CoCreateInstance_decoder")*depends_on("dmo_QueryInterface_decoder"))
BOOST_DATA_TEST_CASE(dmo_GetInputType_decoder, data::make(vecCodecFcc) ^ data::make(vecTemporalCompressionSupported), fcc, temporal)
{
	DMODecoderCLSID clsid(fcc);
	vector<GUID> codecType;

	codecType.push_back(FourCCGUID(fcc));
	dmo_GetInputType(clsid, codecType, FALSE, temporal);
}
