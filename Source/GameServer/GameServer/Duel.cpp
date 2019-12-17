// Duel.cpp: implementation of the CDuel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Duel.h"
#include "CastleSiege.h"
#include "DarkSpirit.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "Guild.h"
#include "Map.h"
#include "MapServerManager.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "User.h"
#include "Util.h"

CDuel gDuel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDuel::CDuel() // OK
{
}

CDuel::~CDuel() // OK
{

}

bool CDuel::CheckDuel(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return 0;
	}

	if(OBJECT_RANGE(lpObj->DuelUser) == 0 || OBJECT_RANGE(lpTarget->DuelUser) == 0)
	{
		return 0;
	}

	if(lpObj->DuelUser == lpTarget->Index && lpTarget->DuelUser == lpObj->Index)
	{
		return 1;
	}

	return 0;
}

void CDuel::CheckDuelScore(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(this->CheckDuel(lpObj,lpTarget) == 0)
	{
		return;
	}

	lpTarget->KillerType = 3;

	if(lpObj->DuelScore >= 10)
	{
		this->ResetDuel(lpObj);

		this->GCDuelEndSend(lpObj->Index,0);
		
		GDRankingDuelSaveSend(lpObj->Index,1,0);

		this->ResetDuel(lpTarget);

		this->GCDuelEndSend(lpTarget->Index,0);
		
		GDRankingDuelSaveSend(lpTarget->Index,0,1);

		GCFireworksSend(lpObj,lpObj->X,lpObj->Y);
	}
}

void CDuel::CheckDuelUser() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		if(OBJECT_RANGE(lpObj->DuelUserReserved) != 0)
		{
			if((GetTickCount()-lpObj->DuelTickCount) > 30000)
			{
				this->ResetDuel(&gObj[lpObj->DuelUserReserved]);
				this->GCDuelStartSend(lpObj->Index,0,lpObj->DuelUserReserved);
				this->ResetDuel(lpObj);
			}
		}

		if(OBJECT_RANGE(lpObj->DuelUserRequested) != 0)
		{
			if((GetTickCount()-lpObj->DuelTickCount) > 30000)
			{
				this->ResetDuel(&gObj[lpObj->DuelUserRequested]);
				this->GCDuelStartSend(lpObj->DuelUserRequested,0,lpObj->Index);
				this->ResetDuel(lpObj);
			}
		}

		if(OBJECT_RANGE(lpObj->DuelUser) != 0)
		{
			if((GetTickCount()-lpObj->DuelTickCount) > 60000)
			{
				this->ResetDuel(&gObj[lpObj->DuelUser]);
				this->GCDuelEndSend(lpObj->DuelUser,0);
				this->ResetDuel(lpObj);
				this->GCDuelEndSend(lpObj->Index,0);
			}
		}
	}
}

void CDuel::ResetDuel(LPOBJ lpObj) // OK
{
	gDarkSpirit[lpObj->Index].ResetTarget(lpObj->DuelUser);

	lpObj->DuelUserReserved = -1;
	lpObj->DuelUserRequested = -1;
	lpObj->DuelScore = 0;
	lpObj->DuelUser = -1;
	lpObj->DuelTickCount = 0;
}

void CDuel::RespawnDuelUser(LPOBJ lpObj) // OK
{
	gMap[lpObj->Map].GetMapRandomPos(&lpObj->X,&lpObj->Y,18);
}

void CDuel::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(this->CheckDuel(lpObj,lpTarget) == 0)
	{
		return;
	}

	lpTarget->DuelScore++;

	this->GCDuelScoreSend(lpObj->Index,lpTarget->Index);
}

void CDuel::CGDuelStartRecv(PMSG_DUEL_START_RECV* lpMsg,int aIndex) // OK
{
	if(gServerInfo.m_DuelSwitch == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0 || aIndex == bIndex)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(160));
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(161));
		return; 
	}

	if((lpTarget->Option & 2) == 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(162));
		return;
	}

	if(OBJECT_RANGE(lpObj->DuelUserReserved) != 0 || OBJECT_RANGE(lpObj->DuelUserRequested) != 0 || OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(163));
		return;
	}

	if(OBJECT_RANGE(lpTarget->DuelUserReserved) != 0 || OBJECT_RANGE(lpTarget->DuelUserRequested) != 0 || OBJECT_RANGE(lpTarget->DuelUser) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(164));
		return;
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState == 1)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(165));
		return;
	}

	if(lpTarget->Guild != 0 && lpTarget->Guild->WarState == 1)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(166));
		return;
	}

	if(DS_MAP_RANGE(lpObj->Map) != 0 || BC_MAP_RANGE(lpObj->Map) != 0 || CC_MAP_RANGE(lpObj->Map) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(167));
		return;
	}

	#if(GAMESERVER_TYPE==1)

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_NOTIFY || gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_READYSIEGE || gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(168));
		return;
	}

	#endif

	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	if(strcmp(lpTarget->Name,name) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(169));
		return;
	}

	for(int n=0;n < MAX_SELF_DEFENSE;n++)
	{
		if(lpObj->SelfDefense[n] >= 0)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(170));
			return;
		}

		if(lpTarget->SelfDefense[n] >= 0)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(171));
			return;
		}
	}

	lpObj->DuelUserReserved = bIndex;
	lpObj->DuelUserRequested = -1;
	lpObj->DuelScore = 0;
	lpObj->DuelUser = -1;
	lpObj->DuelTickCount = GetTickCount();
	
	lpTarget->DuelUserReserved = -1;
	lpTarget->DuelUserRequested = aIndex;
	lpTarget->DuelScore = 0;
	lpTarget->DuelUser = -1;
	lpTarget->DuelTickCount = GetTickCount();

	this->GCDuelOkSend(bIndex,aIndex);
}

void CDuel::CGDuelOkRecv(PMSG_DUEL_OK_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpMsg->flag == 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(172));
		this->ResetDuel(lpObj);
		this->ResetDuel(lpTarget);
		this->GCDuelStartSend(bIndex,0,aIndex);
		return;
	}

	if(OBJECT_RANGE(lpObj->DuelUserReserved) != 0 || OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(163));
		this->ResetDuel(lpObj);
		this->ResetDuel(lpTarget);
		this->GCDuelStartSend(bIndex,0,aIndex);
		return;
	}

	if(OBJECT_RANGE(lpTarget->DuelUserRequested) != 0 || OBJECT_RANGE(lpTarget->DuelUser) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(164));
		this->ResetDuel(lpObj);
		this->ResetDuel(lpTarget);
		this->GCDuelStartSend(bIndex,0,aIndex);
		return;
	}

	if(lpTarget->DuelUserReserved != aIndex)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(164));
		this->ResetDuel(lpObj);
		this->ResetDuel(lpTarget);
		this->GCDuelStartSend(bIndex,0,aIndex);
		return;
	}

	if(DS_MAP_RANGE(lpTarget->Map) != 0 || BC_MAP_RANGE(lpTarget->Map) != 0 || CC_MAP_RANGE(lpTarget->Map) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(167));
		this->ResetDuel(lpObj);
		this->ResetDuel(lpTarget);
		this->GCDuelStartSend(bIndex,0,aIndex);
		return;
	}

	#if(GAMESERVER_TYPE==1)

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_NOTIFY || gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_READYSIEGE || gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(168));
		this->ResetDuel(lpObj);
		this->ResetDuel(lpTarget);
		this->GCDuelStartSend(bIndex,0,aIndex);
		return;
	}

	#endif

	char name[11] = {0};

	memcpy(name,lpMsg->name,10);

	if(strcmp(lpTarget->Name,name) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(169));
		this->ResetDuel(lpObj);
		this->ResetDuel(lpTarget);
		this->GCDuelStartSend(bIndex,0,aIndex);
		return;
	}
	
	lpObj->DuelUserReserved = -1;
	lpObj->DuelUserRequested = -1;
	lpObj->DuelScore = 0;
	lpObj->DuelUser = bIndex;
	lpObj->DuelTickCount = GetTickCount();
	
	lpTarget->DuelUserReserved = -1;
	lpTarget->DuelUserRequested = -1;
	lpTarget->DuelScore = 0;
	lpTarget->DuelUser = aIndex;
	lpTarget->DuelTickCount = GetTickCount();

	this->GCDuelStartSend(aIndex,1,bIndex);
	this->GCDuelStartSend(bIndex,1,aIndex);
	this->GCDuelScoreSend(aIndex,bIndex);
}

void CDuel::CGDuelEndRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(OBJECT_RANGE(lpObj->DuelUserReserved) != 0)
	{
		this->ResetDuel(&gObj[lpObj->DuelUserReserved]);
		this->GCDuelStartSend(aIndex,0,lpObj->DuelUserReserved);
		this->ResetDuel(lpObj);
		return;
	}

	if(OBJECT_RANGE(lpObj->DuelUserRequested) != 0)
	{
		this->ResetDuel(&gObj[lpObj->DuelUserRequested]);
		this->GCDuelStartSend(lpObj->DuelUserRequested,0,aIndex);
		this->ResetDuel(lpObj);
		return;
	}

	if(OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		this->ResetDuel(&gObj[lpObj->DuelUser]);
		this->GCDuelEndSend(lpObj->DuelUser,0);
		this->ResetDuel(lpObj);
		this->GCDuelEndSend(aIndex,0);
		return;
	}
}

void CDuel::GCDuelStartSend(int aIndex,BYTE result,int bIndex) // OK
{
	PMSG_DUEL_START_SEND pMsg;

	pMsg.header.set(0xAA,sizeof(pMsg));

	pMsg.result = result;

	pMsg.index[0] = SET_NUMBERHB(bIndex);
	pMsg.index[1] = SET_NUMBERLB(bIndex);

	memcpy(pMsg.name,gObj[bIndex].Name,sizeof(pMsg.name));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CDuel::GCDuelOkSend(int aIndex,int bIndex) // OK
{
	PMSG_DUEL_OK_SEND pMsg;

	pMsg.header.set(0xAC,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(bIndex);
	pMsg.index[1] = SET_NUMBERLB(bIndex);

	memcpy(pMsg.name,gObj[bIndex].Name,sizeof(pMsg.name));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CDuel::GCDuelEndSend(int aIndex,BYTE result) // OK
{
	PMSG_DUEL_END_SEND pMsg;

	pMsg.header.set(0xAB,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CDuel::GCDuelScoreSend(int aIndex,int bIndex) // OK
{
	PMSG_DUEL_SCORE_SEND pMsg;

	pMsg.header.set(0xAD,sizeof(pMsg));

	pMsg.index1[0] = SET_NUMBERHB(aIndex);
	pMsg.index1[1] = SET_NUMBERLB(aIndex);

	pMsg.index2[0] = SET_NUMBERHB(bIndex);
	pMsg.index2[1] = SET_NUMBERLB(bIndex);

	pMsg.score[0] = gObj[aIndex].DuelScore;
	pMsg.score[1] = gObj[bIndex].DuelScore;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	DataSend(bIndex,(BYTE*)&pMsg,pMsg.header.size);
}