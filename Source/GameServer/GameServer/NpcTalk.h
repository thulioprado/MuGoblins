// NpcTalk.h: interface for the CNpcTalk class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_NPC_TALK_RECV
{
	PBMSG_HEAD header; // C1:30
	BYTE index[2];
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_NPC_TALK_SEND
{
	PBMSG_HEAD header; // C3:30
	BYTE result;
	BYTE level[7];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_NPC_LEO_THE_HELPER_RECV
{
	PSBMSG_HEAD header; // C1:0E:00
	WORD index;
	char account[11];
	char name[11];
	BYTE status;
};

struct SDHP_NPC_SANTA_CLAUS_RECV
{
	PSBMSG_HEAD header; // C1:0E:01
	WORD index;
	char account[11];
	char name[11];
	BYTE status;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_NPC_LEO_THE_HELPER_SEND
{
	PSBMSG_HEAD header; // C1:0E:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_NPC_SANTA_CLAUS_SEND
{
	PSBMSG_HEAD header; // C1:0E:01
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_NPC_LEO_THE_HELPER_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:0E:30
	WORD index;
	char account[11];
	char name[11];
	BYTE status;
};

struct SDHP_NPC_SANTA_CLAUS_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:0E:31
	WORD index;
	char account[11];
	char name[11];
	BYTE status;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CNpcTalk
{
public:
	CNpcTalk();
	virtual ~CNpcTalk();
	bool NpcTalk(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCrywolfAltar(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCastleSiegeCrown(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCastleSiegeCrownSwitch(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCastleSiegeGateSwitch(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCastleSiegeGuard(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCastleSiegeWeaponOffense(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCastleSiegeWeaponDefense(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCastleSiegeSenior(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCastleSiegeGuardsman(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcTrainer(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcAngelKing(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcAngelMessenger(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcServerTransfer(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCharon(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcChaosGoblin(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcWarehouse(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcGuildMaster(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcLehap(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcElfBuffer(LPOBJ lpNpc,LPOBJ lpObj);
	void CGNpcTalkRecv(PMSG_NPC_TALK_RECV* lpMsg,int aIndex);
	void CGNpcTalkCloseRecv(int aIndex);
};

extern CNpcTalk gNpcTalk;
