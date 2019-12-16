#pragma once

#include "JoinServerProtocol.h"
#include "ServerDisplayer.h"
#include "ServerManager.h"

struct SDHP_JOIN_SERVER_LIVE_SEND
{
	PBMSG_HEAD header; // C1:02
	DWORD QueueSize;
};

struct SDHP_JOIN_SERVER_TOTAL_SEND
{
	PBMSG_HEAD header; // C1:FF
	int total;
};

void ErrorMessageBox(char* message,...);
void LogAdd(eLogColor color,char* text,...);
void JoinServerLiveProc();
void JoinServerTotalProc();
bool CheckTextSyntax(char* text,int size);
char CheckAccountCaseSensitive(int value);
int GetFreeServerIndex();
int SearchFreeServerIndex(int* index,int MinIndex,int MaxIndex,DWORD MinTime);
CServerManager* FindServerByCode(WORD ServerCode);
DWORD MakeAccountKey(char* account);

extern int gServerCount;
