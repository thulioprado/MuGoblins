#include "Library.h"

DWORD TickCount = 0;

void XorConvert(LPBYTE buff, int size)
{
	BYTE mask[3] = {0xFC, 0xCF, 0xAB};

	for (int n = 0; n < size; ++n)
	{
		buff[n] ^= mask[n % 3];
	}
}