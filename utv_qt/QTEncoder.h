/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

#include "QTCodec.h"
#include "Codec.h"

struct CQTEncoder : public CQTCodec
{
	ICMCompressorSessionRef session;
	ICMCompressionSessionOptionsRef sessionOptions;
    bool encodeBegun;
};
