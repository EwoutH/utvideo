/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "utvideo.h"
#include "TunedFunc.h"
#include "Predict.h"
#include "HuffmanCode.h"
#include "Convert.h"

const TUNEDFUNC tfnCPP = {
	cpp_PredictWrongMedianAndCount,
	cpp_PredictWrongMedianAndCount,
	cpp_PredictLeftAndCount,
	cpp_RestoreWrongMedian,
	cpp_HuffmanEncode,
	cpp_HuffmanDecode,
	cpp_HuffmanDecodeAndAccum,
	cpp_HuffmanDecodeAndAccumStep2,
	cpp_HuffmanDecodeAndAccumStep4,
	cpp_HuffmanDecodeAndAccumStep4ForBottomupBGRXGreen,
	cpp_HuffmanDecodeAndAccumStep4ForBottomupBGRXBlue,
	cpp_HuffmanDecodeAndAccumStep4ForBottomupBGRXRed,
	cpp_HuffmanDecodeAndAccumStep4ForBottomupBGRXRedAndDummyAlpha,
	cpp_HuffmanDecodeAndAccumStep3ForBottomupBGRGreen,
	cpp_HuffmanDecodeAndAccumStep3ForBottomupBGRBlue,
	cpp_HuffmanDecodeAndAccumStep3ForBottomupBGRRed,
	cpp_HuffmanDecodeAndAccumStep4ForTopdownXRGBGreen,
	cpp_HuffmanDecodeAndAccumStep4ForTopdownXRGBBlue,
	cpp_HuffmanDecodeAndAccumStep4ForTopdownXRGBRed,
	cpp_HuffmanDecodeAndAccumStep4ForTopdownXRGBRedAndDummyAlpha,
	cpp_HuffmanDecodeAndAccumStep3ForTopdownRGBGreen,
	cpp_HuffmanDecodeAndAccumStep3ForTopdownRGBBlue,
	cpp_HuffmanDecodeAndAccumStep3ForTopdownRGBRed,
	cpp_HuffmanDecodeAndAccumStep4ForTopdownBGRXRedAndDummyAlpha,
	cpp_ConvertULY2ToBGR,
	cpp_ConvertULY2ToBGRX,
	cpp_ConvertULY2ToRGB,
	cpp_ConvertULY2ToXRGB,
	cpp_ConvertBGRToULY2,
	cpp_ConvertBGRXToULY2,
	cpp_ConvertRGBToULY2,
	cpp_ConvertXRGBToULY2,
	cpp_ConvertBGRToULRG,
	cpp_ConvertBGRXToULRG,
	cpp_ConvertXRGBToULRG,
	cpp_ConvertBGRAToULRA,
	cpp_ConvertARGBToULRA,
	cpp_ConvertYUYVToULY2,
	cpp_ConvertUYVYToULY2,
	cpp_ConvertULRGToBGR,
	cpp_ConvertULRGToBGRX,
	cpp_ConvertULRGToXRGB,
	cpp_ConvertULRAToBGRA,
	cpp_ConvertULRAToARGB,
	cpp_ConvertULY2ToYUYV,
	cpp_ConvertULY2ToUYVY,
	DummyTunedFunc
};

TUNEDFUNC tfn = tfnCPP;
