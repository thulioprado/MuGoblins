#include "Library.h"

void ErrorMessageBox(const char* message, ...)
{
	char buff[256];
	memset(buff, 0, sizeof(buff));

	va_list arg;
	va_start(arg, message);
	vsprintf_s(buff, message, arg);
	va_end(arg);

	MessageBox(0, buff, "Error", MB_OK | MB_ICONERROR);
	ExitProcess(0);
}

void XorConvert(LPBYTE buff, int size)
{
	BYTE mask[3] = {0xFC, 0xCF, 0xAB};

	for (int n = 0; n < size; ++n)
	{
		buff[n] ^= mask[n % 3];
	}
}