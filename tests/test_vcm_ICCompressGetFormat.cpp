/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "test_win_fmt.h"
#include "expand.h"

void vcm_ICCompressGetFormat_ok(DWORD fccCodec, DWORD fccRaw, LONG width, LONG height)
{
	HIC hic;
	LRESULT lr;
	DWORD gfret;

	BITMAPINFOHEADER bihIn;
	union
	{
		BITMAPINFOHEADER bihOut;
		char bihOutBuf[128];
	};

	hic = ICOpen(ICTYPE_VIDEO, fccCodec, ICMODE_COMPRESS);
	BOOST_REQUIRE(hic != NULL);

	memset(&bihIn, 0, sizeof(BITMAPINFOHEADER));
	bihIn.biSize = sizeof(BITMAPINFOHEADER);
	bihIn.biWidth = width;
	bihIn.biHeight = height;
	bihIn.biPlanes = 1;
	bihIn.biBitCount = FCC2BitCount(fccRaw);
	bihIn.biCompression = FCC2Compression(fccRaw);
	bihIn.biSizeImage = 10000000;

	gfret = ICCompressGetFormatSize(hic, &bihIn);
	BOOST_TEST_REQUIRE(gfret >= sizeof(BITMAPINFOHEADER));
	BOOST_TEST_REQUIRE(gfret <= sizeof(bihOutBuf));

	lr = ICCompressGetFormat(hic, &bihIn, &bihOut);
	BOOST_REQUIRE(lr == ICERR_OK);

	BOOST_TEST_CHECK(bihOut.biSize == gfret);
	BOOST_TEST_CHECK(bihOut.biWidth == width);
	BOOST_TEST_CHECK(bihOut.biHeight == height);
	BOOST_TEST_CHECK(bihOut.biPlanes == 1);
//	BOOST_TEST_CHECK(bihOut.biBitCount == xxx); // XXX
	BOOST_TEST_CHECK(bihOut.biCompression == fccCodec);

	lr = ICClose(hic);
	BOOST_CHECK(lr == ICERR_OK);
}

void vcm_ICCompressGetFormat_ng(DWORD fccCodec, DWORD fccRaw, LONG width, LONG height)
{
	HIC hic;
	LRESULT lr;
	DWORD gfret;

	BITMAPINFOHEADER bihIn;
	union
	{
		BITMAPINFOHEADER bihOut;
		char bihOutBuf[128];
	};

	hic = ICOpen(ICTYPE_VIDEO, fccCodec, ICMODE_COMPRESS);
	BOOST_REQUIRE(hic != NULL);

	memset(&bihIn, 0, sizeof(BITMAPINFOHEADER));
	bihIn.biSize = sizeof(BITMAPINFOHEADER);
	bihIn.biWidth = width;
	bihIn.biHeight = height;
	bihIn.biPlanes = 1;
	bihIn.biBitCount = FCC2BitCount(fccRaw);
	bihIn.biCompression = FCC2Compression(fccRaw);
	bihIn.biSizeImage = 10000000;

	/*
	 * �T�|�[�g���Ȃ����̓t�H�[�}�b�g�ɂ��� ICCompressGetFormatSize ���Ă񂾎���
	 * �����Ԃ��Ă���ׂ����� MSDN Library ��ǂ�ł��C�}�C�`���R�Ƃ��Ȃ����A
	 * utv_vcm �̎d�l�Ƃ��Ă̓t�H�[�}�b�g�u���b�N�̃T�C�Y��Ԃ����̂Ƃ���B
	 */
	gfret = ICCompressGetFormatSize(hic, &bihIn);
	BOOST_TEST_REQUIRE(gfret >= sizeof(BITMAPINFOHEADER));
	BOOST_TEST_REQUIRE(gfret <= sizeof(bihOutBuf));

	lr = ICCompressGetFormat(hic, &bihIn, &bihOut);
	BOOST_CHECK(lr != ICERR_OK);

	lr = ICClose(hic);
	BOOST_CHECK(lr == ICERR_OK);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressGetFormat_biCompression_ok, expand(data::make(vecCodecFourcc) ^ data::make(vecSupportedInputFormat)), fccCodec, fccRaw)
{
	vcm_ICCompressGetFormat_ok(fccCodec, fccRaw, TEST_WIDTH, TEST_HEIGHT);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressGetFormat_biCompression_ng, expand(data::make(vecCodecFourcc) ^ data::make(vecUnsupportedInputFormat)), fccCodec, fccRaw)
{
	vcm_ICCompressGetFormat_ng(fccCodec, fccRaw, TEST_WIDTH, TEST_HEIGHT);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressGetFormat_biWidth_ok, expand(data::make(vecCodecFourcc) ^ data::make(vecTopPriorityRawFormat) ^ data::make(vecSupportedWidth)), fccCodec, fccRaw, width)
{
	vcm_ICCompressGetFormat_ok(fccCodec, fccRaw, width, TEST_HEIGHT);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressGetFormat_biWidth_ng, expand(data::make(vecCodecFourcc) ^ data::make(vecTopPriorityRawFormat) ^ data::make(vecUnsupportedWidth)), fccCodec, fccRaw, width)
{
	vcm_ICCompressGetFormat_ng(fccCodec, fccRaw, width, TEST_HEIGHT);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressGetFormat_biHeight_ok, expand(data::make(vecCodecFourcc) ^ data::make(vecTopPriorityRawFormat) ^ data::make(vecSupportedHeight)), fccCodec, fccRaw, height)
{
	vcm_ICCompressGetFormat_ok(fccCodec, fccRaw, TEST_WIDTH, height);
}

BOOST_TEST_DECORATOR(*depends_on("vcm_ICOpen_encoder"))
BOOST_DATA_TEST_CASE(vcm_ICCompressGetFormat_biHeight_ng, expand(data::make(vecCodecFourcc) ^ data::make(vecTopPriorityRawFormat) ^ data::make(vecUnsupportedHeight)), fccCodec, fccRaw, height)
{
	vcm_ICCompressGetFormat_ng(fccCodec, fccRaw, TEST_WIDTH, height);
}
