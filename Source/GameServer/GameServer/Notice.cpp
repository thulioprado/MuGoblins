// Notice.cpp: implementation of the CNotice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Notice.h"
#include "MemScript.h"
#include "Util.h"

CNotice gNotice;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNotice::CNotice() // OK
{
}

CNotice::~CNotice() // OK
{

}

void CNotice::GCNoticeSend(int aIndex, BYTE type, BYTE count, BYTE opacity, WORD delay, DWORD color, BYTE speed, char* message, ...) // OK
{
	char buff[256] = {0};

	va_list arg;
	va_start(arg, message);
	vsprintf_s(buff, message, arg);
	va_end(arg);

	int size = strlen(buff);

	size = ((size > MAX_MESSAGE_SIZE) ? MAX_MESSAGE_SIZE : size);

	PMSG_NOTICE_SEND pMsg;

	pMsg.header.set(0x0D, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

	pMsg.type = type;
	pMsg.count = count;
	pMsg.opacity = opacity;
	pMsg.delay = delay;
	pMsg.color = color;
	pMsg.speed = speed;

	memcpy(pMsg.message, buff, size);

	pMsg.message[size] = 0;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CNotice::GCNoticeSendToAll(BYTE type, BYTE count, BYTE opacity, WORD delay, DWORD color, BYTE speed, char* message, ...) // OK
{
	char buff[256] = {0};

	va_list arg;
	va_start(arg, message);
	vsprintf_s(buff, message, arg);
	va_end(arg);

	int size = strlen(buff);

	size = ((size > MAX_MESSAGE_SIZE) ? MAX_MESSAGE_SIZE : size);

	PMSG_NOTICE_SEND pMsg;

	pMsg.header.set(0x0D, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

	pMsg.type = type;
	pMsg.count = count;
	pMsg.opacity = opacity;
	pMsg.delay = delay;
	pMsg.color = color;
	pMsg.speed = speed;

	memcpy(pMsg.message, buff, size);

	pMsg.message[size] = 0;

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}
