/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"

#pragma warning(disable:4073)
#pragma init_seg(lib)

#include "test_win_fmt.h"

vector<DWORD> vecCodecFcc = {
	FCC('ULRG'),
	FCC('ULRA'),
	FCC('ULY4'),
	FCC('ULY2'),
	FCC('ULY0'),
	FCC('ULH4'),
	FCC('ULH2'),
	FCC('ULH0'),

	FCC('UQY2'),
	FCC('UQRG'),
	FCC('UQRA'),

	FCC('UMRG'),
	FCC('UMRA'),
	FCC('UMY4'),
	FCC('UMY2'),
	FCC('UMH4'),
	FCC('UMH2'),
};

vector<wstring> vecCodecShortName = {
	L"UtVideo (ULRG)",
	L"UtVideo (ULRA)",
	L"UtVideo (ULY4)",
	L"UtVideo (ULY2)",
	L"UtVideo (ULY0)",
	L"UtVideo (ULH4)",
	L"UtVideo (ULH2)",
	L"UtVideo (ULH0)",

	L"UtVideo (UQY2)",
	L"UtVideo (UQRG)",
	L"UtVideo (UQRA)",

	L"UtVideo (UMRG)",
	L"UtVideo (UMRA)",
	L"UtVideo (UMY4)",
	L"UtVideo (UMY2)",
	L"UtVideo (UMH4)",
	L"UtVideo (UMH2)",
};

vector<wstring> vecCodecLongName = {
	L"UtVideo RGB VCM",
	L"UtVideo RGBA VCM",
	L"UtVideo YUV444 BT.601 VCM",
	L"UtVideo YUV422 BT.601 VCM",
	L"UtVideo YUV420 BT.601 VCM",
	L"UtVideo YUV444 BT.709 VCM",
	L"UtVideo YUV422 BT.709 VCM",
	L"UtVideo YUV420 BT.709 VCM",

	L"UtVideo Pro YUV422 10bit VCM",
	L"UtVideo Pro RGB 10bit VCM",
	L"UtVideo Pro RGBA 10bit VCM",

	L"UtVideo T2 RGB VCM",
	L"UtVideo T2 RGBA VCM",
	L"UtVideo T2 YUV444 BT.601 VCM",
	L"UtVideo T2 YUV422 BT.601 VCM",
	L"UtVideo T2 YUV444 BT.709 VCM",
	L"UtVideo T2 YUV422 BT.709 VCM",
};

vector<vector<DWORD> > vecSupportedInputFccs = {
	{ 32, 24 },
	{ 32 },
	{ 32, 24, FCC('YV24') },
	{ 32, 24, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('YV16') },
	{ 32, 24, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('YV16'), FCC('YV12') },
	{ 32, 24, FCC('YV24') },
	{ 32, 24, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('HDYC'), FCC('YV16') },
	{ 32, 24, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('HDYC'), FCC('YV16'), FCC('YV12') },

	{ FCC('v210') },
	{ FCC('b48r'), FCC('b64a') },
	{ FCC('b64a') },

	{ 32, 24 },
	{ 32 },
	{ 32, 24, FCC('YV24') },
	{ 32, 24, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('YV16') },
	{ 32, 24, FCC('YV24') },
	{ 32, 24, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('HDYC'), FCC('YV16') },
};

vector<vector<DWORD> > vecSupportedOutputFccs = vecSupportedInputFccs;

vector<vector<DWORD> > vecUnsupportedInputFccs = {
	{ 15, 16 },
	{ 15, 16, 24 },
	{ 15, 16, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('HDYC'), FCC('YV16'), FCC('YV12') },
	{ 15, 16, FCC('YVYU'), FCC('VYUY'), FCC('YV12'), FCC('HDYC') },
	{ 15, 16, FCC('YVYU'), FCC('VYUY'), FCC('HDYC') },
	{ 15, 16, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('HDYC'), FCC('YV16'), FCC('YV12') },
	{ 15, 16, FCC('YVYU'), FCC('VYUY'), FCC('YV12') },
	{ 15, 16, FCC('YVYU'), FCC('VYUY') },

	{ 15, 16, 24, 32, FCC('YUY2'), FCC('YUYV'), FCC('UYVY'), FCC('YV12'), FCC('b48r'), FCC('b64a') },
	{ 15, 16, 24, 32, FCC('YUY2'), FCC('YUYV'), FCC('UYVY'), FCC('YV12'), FCC('v210'), },
	{ 15, 16, 24, 32, FCC('YUY2'), FCC('YUYV'), FCC('UYVY'), FCC('YV12'), FCC('b48r'), FCC('v210') },

	{ 15, 16 },
	{ 15, 16, 24 },
	{ 15, 16, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('HDYC'), FCC('YV16'), FCC('YV12') },
	{ 15, 16, FCC('YVYU'), FCC('VYUY'), FCC('YV12'), FCC('HDYC') },
	{ 15, 16, FCC('YUY2'), FCC('YUYV'), FCC('YUNV'), FCC('yuvs'), FCC('UYVY'), FCC('UYNV'), FCC('2vuy'), FCC('HDYC'), FCC('YV16'), FCC('YV12') },
	{ 15, 16, FCC('YVYU'), FCC('VYUY'), FCC('YV12') },
};

vector<vector<DWORD> > vecUnsupportedOutputFccs = vecUnsupportedInputFccs;

vector<vector<LONG>> vecSupportedWidth = {
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1922, 1924, 1926 },
	{ 1920, 1922, 1924, 1926 },
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1922, 1924, 1926 },
	{ 1920, 1922, 1924, 1926 },

	{ 1920, 1922, 1924, 1926 },
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },

	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1922, 1924, 1926 },
	{ 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927 },
	{ 1920, 1922, 1924, 1926 },
};

vector<vector<LONG>> vecSupportedHeight = {
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1082, 1084, 1086 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1082, 1084, 1086 },

	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },

	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
	{ 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087 },
};

vector<vector<LONG>> vecUnsupportedWidth = {
	{ -1920, 0 },
	{ -1920, 0 },
	{ -1920, 0 },
	{ -1920, 0, 1921, 1923, 1925, 1927 },
	{ -1920, 0, 1921, 1923, 1925, 1927 },
	{ -1920, 0 },
	{ -1920, 0, 1921, 1923, 1925, 1927 },
	{ -1920, 0, 1921, 1923, 1925, 1927 },

	{ -1920, 0, 1921, 1923, 1925, 1927 },
	{ -1920, 0 },
	{ -1920, 0 },

	{ -1920, 0 },
	{ -1920, 0 },
	{ -1920, 0 },
	{ -1920, 0, 1921, 1923, 1925, 1927 },
	{ -1920, 0 },
	{ -1920, 0, 1921, 1923, 1925, 1927 },
};

vector<vector<LONG>> vecUnsupportedHeight = {
	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0, 1081, 1083, 1085, 1087 },
	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0, 1081, 1083, 1085, 1087 },

	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0 },

	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0 },
	{ -1080, 0 },
};

vector<DWORD> vecTopPriorityRawFcc = {
	24,
	32,
	FCC('YV24'),
	FCC('YUY2'),
	FCC('YV12'),
	FCC('YV24'),
	FCC('HDYC'),
	FCC('YV12'),

	FCC('v210'),
	FCC('b48r'),
	FCC('b64a'),

	24,
	32,
	FCC('YV24'),
	FCC('YUY2'),
	FCC('YV24'),
	FCC('HDYC'),
};

vector<BOOL> vecTemporalCompressionSupported = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,

	false,
	false,
	false,

	true,
	true,
	true,
	true,
	true,
	true,
};
