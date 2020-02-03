// CommandManager.h: interface for the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

enum eCommandNumber
{
	COMMAND_MOVE,
	COMMAND_POST,
	COMMAND_ADD_POINT1,
	COMMAND_ADD_POINT2,
	COMMAND_ADD_POINT3,
	COMMAND_ADD_POINT4,
	COMMAND_ADD_POINT5,
	COMMAND_READD_POINTS,
	COMMAND_WARE,
	COMMAND_GM_MOVE,
	COMMAND_GM_POST,
	COMMAND_TRACK,
	COMMAND_TRACE,
	COMMAND_DISCONNECT,
	COMMAND_FIREWORKS,
	COMMAND_MAKE,
	COMMAND_SKIN,
	COMMAND_SET_MONEY,
	COMMAND_NOTICE,
	COMMAND_GUILD_WAR,
	COMMAND_BATTLE_SOCCER,
	COMMAND_REQUEST,
	COMMAND_HIDE,
	MAX_COMMAND
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

class CCommandManager
{
public:
	CCommandManager();
	virtual ~CCommandManager();
	void Init();
	void Add(BYTE code, char* label);
	long GetNumber(char* arg, int pos);
	void GetString(char* arg, char* out, int size, int pos);
	long GetCommandCode(char* label);
	bool ManagementCore(LPOBJ lpObj, char* message);
	void CommandMove(LPOBJ lpObj, char* arg);
	void CommandPost(LPOBJ lpObj, char* arg);
	void CommandAddPoint(LPOBJ lpObj, char* arg, int type);
	void CommandReAddPoint(LPOBJ lpObj);
	void CommandWare(LPOBJ lpObj, char* arg);
	void CommandGMMove(LPOBJ lpObj, char* arg);
	void CommandGMPost(LPOBJ lpObj, char* arg);
	void CommandTrack(LPOBJ lpObj, char* arg);
	void CommandTrace(LPOBJ lpObj, char* arg);
	void CommandDisconnect(LPOBJ lpObj, char* arg);
	void CommandFireworks(LPOBJ lpObj, char* arg);
	void CommandMake(LPOBJ lpObj, char* arg);
	void CommandSkin(LPOBJ lpObj, char* arg);
	void CommandSetMoney(LPOBJ lpObj, char* arg);
	void CommandNotice(LPOBJ lpObj, char* arg);
	void CommandGuildWar(LPOBJ lpObj, char* arg);
	void CommandBattleSoccer(LPOBJ lpObj, char* arg);
	void CommandRequest(LPOBJ lpObj, char* arg);
	void CommandHide(LPOBJ lpObj, char* arg);

private:
	std::unordered_map<BYTE, std::vector<std::string>> m_CommandLabels;
};

extern CCommandManager gCommandManager;
