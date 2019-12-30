// Notice.h: interface for the CNotice class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

#define MAX_NOTICE 100

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_NOTICE_SEND
{
	PBMSG_HEAD header; // C1:0D
	BYTE type;
	BYTE count;
	BYTE opacity;
	WORD delay;
	DWORD color;
	BYTE speed;
	char message[256];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CNotice
{
public:
	CNotice();
	virtual ~CNotice();
	void GCNoticeSend(int aIndex, BYTE type, BYTE count, BYTE opacity, WORD delay, DWORD color, BYTE speed, char* message, ...);
	void GCNoticeSendToAll(BYTE type, BYTE count, BYTE opacity, WORD delay, DWORD color, BYTE speed, char* message, ...);
};

extern CNotice gNotice;
