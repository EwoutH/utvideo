/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

#include "QTCodec.h"
#include "Codec.h"

struct CQTDecoder : public CQTCodec
{
	OSType **wantedDestinationPixelTypes;
	int decodeBegun;
	utvf_t outfmt;
};
