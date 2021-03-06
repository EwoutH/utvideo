/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "utvideo.h"
#include "TunedFunc.h"
#include "Predict.h"
#include "HuffmanCode.h"
#include "Convert.h"
#include "ColorOrder.h"
#include "Coefficient.h"
#include "SymPack.h"

const TUNEDFUNC_PREDICT tfnPredictCPP = {
	NULL,
	{ 0 },
	cpp_PredictCylindricalWrongMedianAndCount,
	cpp_PredictCylindricalLeftAndCount<8>,
	cpp_RestoreCylindricalWrongMedian,
	cpp_RestoreCylindricalLeft<8>,
	cpp_PredictCylindricalLeftAndCount<10>,
	cpp_RestoreCylindricalLeft<10>,
	cpp_PredictPlanarGradientAndCount<8>,
	cpp_PredictPlanarGradient<8>,
	cpp_RestorePlanarGradient<8>,
};

const TUNEDFUNC_HUFFMAN_ENCODE tfnHuffmanEncodeCPP = {
	NULL,
	{ 0 },
	cpp_HuffmanEncode<8>,
	cpp_HuffmanEncode<10>,
};

const TUNEDFUNC_HUFFMAN_DECODE tfnHuffmanDecodeCPP = {
	NULL,
	{ 0 },
	cpp_HuffmanDecode<8>,
	cpp_HuffmanDecode<10>,
};

const TUNEDFUNC_CONVERT_YUVRGB tfnConvertYUVRGBCPP = {
	NULL,
	{ 0 },
	{
		cpp_ConvertULY2ToRGB<CBT601Coefficient, CBGRColorOrder>,
		cpp_ConvertULY2ToRGB<CBT601Coefficient, CBGRAColorOrder>,
		cpp_ConvertULY2ToRGB<CBT601Coefficient, CRGBColorOrder>,
		cpp_ConvertULY2ToRGB<CBT601Coefficient, CARGBColorOrder>,
		cpp_ConvertRGBToULY2<CBT601Coefficient, CBGRColorOrder>,
		cpp_ConvertRGBToULY2<CBT601Coefficient, CBGRAColorOrder>,
		cpp_ConvertRGBToULY2<CBT601Coefficient, CRGBColorOrder>,
		cpp_ConvertRGBToULY2<CBT601Coefficient, CARGBColorOrder>,
		cpp_ConvertULY4ToRGB<CBT601Coefficient, CBGRColorOrder>,
		cpp_ConvertULY4ToRGB<CBT601Coefficient, CBGRAColorOrder>,
		cpp_ConvertULY4ToRGB<CBT601Coefficient, CRGBColorOrder>,
		cpp_ConvertULY4ToRGB<CBT601Coefficient, CARGBColorOrder>,
		cpp_ConvertRGBToULY4<CBT601Coefficient, CBGRColorOrder>,
		cpp_ConvertRGBToULY4<CBT601Coefficient, CBGRAColorOrder>,
		cpp_ConvertRGBToULY4<CBT601Coefficient, CRGBColorOrder>,
		cpp_ConvertRGBToULY4<CBT601Coefficient, CARGBColorOrder>,
		cpp_ConvertULY0ToRGB<CBT601Coefficient, CBGRColorOrder>,
		cpp_ConvertULY0ToRGB<CBT601Coefficient, CBGRAColorOrder>,
		cpp_ConvertULY0ToRGB<CBT601Coefficient, CRGBColorOrder>,
		cpp_ConvertULY0ToRGB<CBT601Coefficient, CARGBColorOrder>,
		cpp_ConvertRGBToULY0<CBT601Coefficient, CBGRColorOrder>,
		cpp_ConvertRGBToULY0<CBT601Coefficient, CBGRAColorOrder>,
		cpp_ConvertRGBToULY0<CBT601Coefficient, CRGBColorOrder>,
		cpp_ConvertRGBToULY0<CBT601Coefficient, CARGBColorOrder>,
	},
	{
		cpp_ConvertULY2ToRGB<CBT709Coefficient, CBGRColorOrder>,
		cpp_ConvertULY2ToRGB<CBT709Coefficient, CBGRAColorOrder>,
		cpp_ConvertULY2ToRGB<CBT709Coefficient, CRGBColorOrder>,
		cpp_ConvertULY2ToRGB<CBT709Coefficient, CARGBColorOrder>,
		cpp_ConvertRGBToULY2<CBT709Coefficient, CBGRColorOrder>,
		cpp_ConvertRGBToULY2<CBT709Coefficient, CBGRAColorOrder>,
		cpp_ConvertRGBToULY2<CBT709Coefficient, CRGBColorOrder>,
		cpp_ConvertRGBToULY2<CBT709Coefficient, CARGBColorOrder>,
		cpp_ConvertULY4ToRGB<CBT709Coefficient, CBGRColorOrder>,
		cpp_ConvertULY4ToRGB<CBT709Coefficient, CBGRAColorOrder>,
		cpp_ConvertULY4ToRGB<CBT709Coefficient, CRGBColorOrder>,
		cpp_ConvertULY4ToRGB<CBT709Coefficient, CARGBColorOrder>,
		cpp_ConvertRGBToULY4<CBT709Coefficient, CBGRColorOrder>,
		cpp_ConvertRGBToULY4<CBT709Coefficient, CBGRAColorOrder>,
		cpp_ConvertRGBToULY4<CBT709Coefficient, CRGBColorOrder>,
		cpp_ConvertRGBToULY4<CBT709Coefficient, CARGBColorOrder>,
		cpp_ConvertULY0ToRGB<CBT709Coefficient, CBGRColorOrder>,
		cpp_ConvertULY0ToRGB<CBT709Coefficient, CBGRAColorOrder>,
		cpp_ConvertULY0ToRGB<CBT709Coefficient, CRGBColorOrder>,
		cpp_ConvertULY0ToRGB<CBT709Coefficient, CARGBColorOrder>,
		cpp_ConvertRGBToULY0<CBT709Coefficient, CBGRColorOrder>,
		cpp_ConvertRGBToULY0<CBT709Coefficient, CBGRAColorOrder>,
		cpp_ConvertRGBToULY0<CBT709Coefficient, CRGBColorOrder>,
		cpp_ConvertRGBToULY0<CBT709Coefficient, CARGBColorOrder>,
	},
};

const TUNEDFUNC_CONVERT_SHUFFLE tfnConvertShuffleCPP = {
	NULL,
	{ 0 },
	cpp_ConvertRGBToULRG<CBGRColorOrder>,
	cpp_ConvertRGBToULRG<CBGRAColorOrder>,
	cpp_ConvertRGBToULRG<CARGBColorOrder>,
	cpp_ConvertARGBToULRA<CBGRAColorOrder>,
	cpp_ConvertARGBToULRA<CARGBColorOrder>,
	cpp_ConvertYUV422ToULY2<CYUYVColorOrder>,
	cpp_ConvertYUV422ToULY2<CUYVYColorOrder>,
	cpp_ConvertULRGToRGB<CBGRColorOrder>,
	cpp_ConvertULRGToRGB<CBGRAColorOrder>,
	cpp_ConvertULRGToRGB<CARGBColorOrder>,
	cpp_ConvertULRAToARGB<CBGRAColorOrder>,
	cpp_ConvertULRAToARGB<CARGBColorOrder>,
	cpp_ConvertULY2ToYUV422<CYUYVColorOrder>,
	cpp_ConvertULY2ToYUV422<CUYVYColorOrder>,
	cpp_ConvertRGBToUQRG<CB48rColorOrder>,
	cpp_ConvertRGBToUQRG<CB64aColorOrder>,
	cpp_ConvertB64aToUQRA,
	cpp_ConvertUQRGToRGB<CB48rColorOrder>,
	cpp_ConvertUQRGToRGB<CB64aColorOrder>,
	cpp_ConvertUQRAToB64a,
	cpp_ConvertV210ToUQY2,
	cpp_ConvertUQY2ToV210,
	cpp_ConvertR210ToUQRG,
	cpp_ConvertUQRGToR210,
};

extern const TUNEDFUNC_SYMPACK tfnSymPackCPP = {
	NULL,
	{ 0 },
	cpp_Pack8SymAfterPredictPlanarGradient8,
	cpp_Unpack8SymAndRestorePlanarGradient8,
	cpp_Pack8SymWithDiff8,
	cpp_Unpack8SymWithDiff8,
};

TUNEDFUNC tfn = {
	&tfnPredictCPP,
	&tfnHuffmanEncodeCPP,
	&tfnHuffmanDecodeCPP,
	&tfnConvertYUVRGBCPP,
	&tfnConvertShuffleCPP,
	&tfnSymPackCPP,
};


void ResolveTunedFunc(const TUNEDFUNC *ptfnRoot, const uint32_t *pdwSupportedFeatures)
{
	const TUNEDFUNC_FRAGMENT **pSrc, *pTest;
	const TUNEDFUNC_FRAGMENT **pDst;

	pSrc = (const TUNEDFUNC_FRAGMENT **)ptfnRoot;
	pDst = (const TUNEDFUNC_FRAGMENT **)&tfn;

	for (int i = 0; i < (sizeof(TUNEDFUNC) / sizeof(void *)); i++)
	{
		for (pTest = pSrc[i]; pTest != NULL; pTest = (const TUNEDFUNC_FRAGMENT *)pTest->pNext)
		{
			int j;
			for (j = 0; j < _countof(pTest->dwRequiredFeatures); j++)
			{
				if ((pTest->dwRequiredFeatures[j] & pdwSupportedFeatures[j]) != pTest->dwRequiredFeatures[j])
					break;
			}
			if (j == _countof(pTest->dwRequiredFeatures))
				break;
		}
		if (pTest != NULL)
			pDst[i] = pTest;
	}
}
