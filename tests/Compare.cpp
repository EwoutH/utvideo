/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#include "stdafx.h"
#include "Compare.h"

int CompareFrame(const void *frame1, const void *frame2, unsigned int width, size_t size, DWORD format, int tolerance)
{
	size_t net = 0;
	size_t stride = 0;

	switch (format)
	{
	case 24:
		net = width * (size_t)3;
		stride = (net + 3) & ~(size_t)3;
		break;

	case FCC('r210'):
		net = width * (size_t)4;
		stride = (width + 63) / 64 * (size_t)256;
		break;

	// v210 �ɂ���p�̏������K�v�ȋC�����邪���̂Ƃ�����͔������Ă��Ȃ��悤�Ɍ�����c�H
	default:
		net = size;
		stride = size;
	}

	const uint8_t *begin1 = (const uint8_t *)frame1;
	const uint8_t *begin2 = (const uint8_t *)frame2;
	const uint8_t *end1 = begin1 + size;
	for (auto p = begin1, q = begin2; p < end1; p += stride, q += stride)
	{
		for (auto pp = p, qq = q; pp != p + net; ++pp, ++qq)
		{
			int r = (int)*pp - (int)*qq;
			if (abs(r) > tolerance)
				return r;
		}
	}
	return 0;
}
