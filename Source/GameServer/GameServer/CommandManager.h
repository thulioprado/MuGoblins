// CommandManager.h: interface for the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

#define MAX_COMMAND 31

enum eCommandNumber
{
	COMMAND_MOVE = 1,
	COMMAND_POST = 2,
	COMMAND_ADD_POINT1 = 3,
	COMMAND_ADD_POINT2 = 4,
	COMMAND_ADD_POINT3 = 5,
	COMMAND_ADD_POINT4 = 6,
	COMMAND_ADD_POINT5 = 7,
	COMMAND_PK_CLEAR = 8,
	COMMAND_MONEY = 9,
	COMMAND_CHANGE = 10,
	COMMAND_WARE = 11,
	COMMAND_RESET = 12,
	COMMAND_GM_MOVE = 13,
	COMMAND_GM_POST = 14,
	COMMAND_TRACK = 15,
	COMMAND_TRACE = 16,
	COMMAND_DISCONNECT = 17,
	COMMAND_FIREWORKS = 18,
	COMMAND_MAKE = 19,
	COMMAND_SKIN = 20,
	COMMAND_SET_MONEY = 21,
	COMMAND_NOTICE = 22,
	COMMAND_MASTER_RESET = 23,
	COMMAND_GUILD_WAR = 24,
	COMMAND_BATTLE_SOCCER = 25,
	COMMAND_REQUEST = 26,
	COMMAND_HIDE = 27,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

struct SDHP_COMMAND_MASTER_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
	UINT MasterResetDay;
	UINT MasterResetWek;
	UINT MasterResetMon;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_COMMAND_MASTER_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct COMMAND_INFO
{
	int code;
	char label[32];
};

class CCommandManager
{
public:
	CCommandManager();
	virtual ~CCommandManager();
	void Init();
	void MainProc();
	void Add(char* label,int code);
	long GetNumber(char* arg,int pos);
	void GetString(char* arg,char* out,int size,int pos);
	long GetCommandCode(char* label);
	bool ManagementCore(LPOBJ lpObj,char* message);
	void CommandMove(LPOBJ lpObj,char* arg);
	void CommandPost(LPOBJ lpObj,char* arg);
	void CommandAddPoint(LPOBJ lpObj,char* arg,int type);
	void CommandAddPointAuto(LPOBJ lpObj,char* arg,int type);
	void CommandPKClear(LPOBJ lpObj,char* arg);
	void CommandMoney(LPOBJ lpObj,char* arg);
	void CommandChange(LPOBJ lpObj,char* arg);
	void CommandWare(LPOBJ lpObj,char* arg);
	void CommandReset(LPOBJ lpObj,char* arg);
	void CommandResetAuto(LPOBJ lpObj,char* arg);
	void CommandGMMove(LPOBJ lpObj,char* arg);
	void CommandGMPost(LPOBJ lpObj,char* arg);
	void CommandTrack(LPOBJ lpObj,char* arg);
	void CommandTrace(LPOBJ lpObj,char* arg);
	void CommandDisconnect(LPOBJ lpObj,char* arg);
	void CommandFireworks(LPOBJ lpObj,char* arg);
	void CommandMake(LPOBJ lpObj,char* arg);
	void CommandSkin(LPOBJ lpObj,char* arg);
	void CommandSetMoney(LPOBJ lpObj,char* arg);
	void CommandNotice(LPOBJ lpObj,char* arg);
	void CommandMasterReset(LPOBJ lpObj,char* arg);
	void CommandGuildWar(LPOBJ lpObj,char* arg);
	void CommandBattleSoccer(LPOBJ lpObj,char* arg);
	void CommandRequest(LPOBJ lpObj,char* arg);
	void CommandHide(LPOBJ lpObj,char* arg);
	void CommandAddPointAutoProc(LPOBJ lpObj);
	void CommandResetAutoProc(LPOBJ lpObj);
	void DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg);
	void DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg);
private:
	COMMAND_INFO m_CommandInfo[MAX_COMMAND];
};

extern CCommandManager gCommandManager;
