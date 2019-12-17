// NpcTalk.cpp: implementation of the CNpcTalk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NpcTalk.h"
#include "BloodCastle.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "CastleSiegeWeapon.h"
#include "ChaosBox.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "Duel.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "ItemBagManager.h"
#include "Map.h"
#include "Message.h"
#include "Notice.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "ShopManager.h"
#include "Trade.h"
#include "Util.h"
#include "Warehouse.h"

CNpcTalk gNpcTalk;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcTalk::CNpcTalk() // OK
{

}

CNpcTalk::~CNpcTalk() // OK
{

}

bool CNpcTalk::NpcTalk(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->Interface.use != 0)
	{
		return 1;
	}

	if(gQuest.NpcTalk(lpNpc,lpObj) != 0)
	{
		return 1;
	}

	switch(lpNpc->Class)
	{
		case 205:
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 206:
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 207: 
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 208:
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 209: 
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 216:
			this->NpcCastleSiegeCrown(lpNpc,lpObj);
			break;
		case 217:
			this->NpcCastleSiegeCrownSwitch(lpNpc,lpObj);
			break;
		case 218:
			this->NpcCastleSiegeCrownSwitch(lpNpc,lpObj);
			break;
		case 219:
			this->NpcCastleSiegeGateSwitch(lpNpc,lpObj);
			break;
		case 220:
			this->NpcCastleSiegeGuard(lpNpc,lpObj);
			break;
		case 221:
			this->NpcCastleSiegeWeaponOffense(lpNpc,lpObj);
			break;
		case 222:
			this->NpcCastleSiegeWeaponDefense(lpNpc,lpObj);
			break;
		case 223:
			this->NpcCastleSiegeSenior(lpNpc,lpObj);
			break;
		case 224:
			this->NpcCastleSiegeGuardsman(lpNpc,lpObj);
			break;
		case 226:
			this->NpcTrainer(lpNpc,lpObj);
			break;
		case 232:
			this->NpcAngelKing(lpNpc,lpObj);
			break;
		case 233:
			this->NpcAngelMessenger(lpNpc,lpObj);
			break;
		case 234:
			this->NpcServerTransfer(lpNpc,lpObj);
			break;
		case 237:
			this->NpcCharon(lpNpc,lpObj);
			break;
		case 238:
			this->NpcChaosGoblin(lpNpc,lpObj);
			break;
		case 240:
			this->NpcWarehouse(lpNpc,lpObj);
			break;
		case 241:
			this->NpcGuildMaster(lpNpc,lpObj);
			break;
		case 256:
			this->NpcLehap(lpNpc,lpObj);
			break;
		case 257:
			this->NpcElfBuffer(lpNpc,lpObj);
			break;
		default:
			return 0;
	}

	return 1;
}

void CNpcTalk::NpcCrywolfAltar(LPOBJ lpNpc,LPOBJ lpObj) // OK
{

}

void CNpcTalk::NpcCastleSiegeCrown(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	#if(GAMESERVER_TYPE==1)
	if ( gObjIsConnected(lpObj->Index) == FALSE )
	{
		return;
	}

	if(gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		return;
	}
	
	if(lpObj->CsJoinSide < 2 || lpObj->CsGuildInvolved == FALSE )
	{
		return;
	}

	if(gCastleSiege.CheckUnionGuildMaster(lpObj->Index) == FALSE )
	{
		return;
	}

	if( (abs(lpObj->Y - lpNpc->Y)) > 3 || (abs(lpObj->X - lpNpc->X)) > 3)
	{
		return;
	}

	if(gCastleSiege.GetRegCrownAvailable() == FALSE )
	{
		return;
	}

	int iUserIndex = gCastleSiege.GetCrownUserIndex();

	if(gObjIsConnected(iUserIndex) == FALSE )
	{
		int iSwitchIndex1 = gCastleSiege.GetCrownSwitchUserIndex(217);
		int iSwitchIndex2 = gCastleSiege.GetCrownSwitchUserIndex(218);

		if(gObjIsConnected(iSwitchIndex1) == FALSE || gObjIsConnected(iSwitchIndex2) == FALSE )
		{
			GCAnsCsAccessCrownState(lpObj->Index,4);
			LogAdd(LOG_BLACK,"[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->Account,lpObj->Name,lpObj->GuildName);
			return;
		}

		if(lpObj->CsJoinSide != gObj[iSwitchIndex1].CsJoinSide || lpObj->CsJoinSide != gObj[iSwitchIndex2].CsJoinSide)
		{
			GCAnsCsAccessCrownState(lpObj->Index,4);
			LogAdd(LOG_BLACK,"[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s) (S1:%s/%s)(S2:%s/%s)",lpObj->Account,lpObj->Name,lpObj->GuildName,gObj[iSwitchIndex1].Name,gObj[iSwitchIndex1].GuildName,gObj[iSwitchIndex2].Name,gObj[iSwitchIndex2].GuildName);
			return;
		}
		else
		{
			GCAnsCsAccessCrownState(lpObj->Index,0);
			gCastleSiege.SetCrownUserIndex(lpObj->Index);
			gCastleSiege.SetCrownAccessUserX((BYTE)lpObj->X);
			gCastleSiege.SetCrownAccessUserY((BYTE)lpObj->Y);
			gCastleSiege.SetCrownAccessTickCount();
			gCastleSiege.NotifyAllUserCsProgState(0,lpObj->GuildName);
			LogAdd(LOG_BLACK,"[CastleSiege] [%s][%s] Start to Register Castle Crown (GUILD:%s)",lpObj->Account,lpObj->Name,lpObj->GuildName);
		}
	}
	else if(lpObj->Index != iUserIndex)
	{
		GCAnsCsAccessCrownState(lpObj->Index,3);
	}
	#endif
}

void CNpcTalk::NpcCastleSiegeCrownSwitch(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	#if(GAMESERVER_TYPE==1)
	if(gObjIsConnected(lpObj->Index) == FALSE )
	{
		return;
	}

	if(gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		return;
	}
	
	if(lpObj->CsJoinSide < 2)
	{
		return;
	}

	if( (abs(lpObj->Y - lpNpc->Y)) > 3 || (abs(lpObj->X - lpNpc->X)) > 3)
	{
		return;
	}

	if(gCastleSiege.CheckGuardianStatueExist() == TRUE)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(427));
		return;
	}

	int iUserIndex = gCastleSiege.GetCrownSwitchUserIndex(lpNpc->Class);

	if(!gObjIsConnected(iUserIndex))
	{
		GCAnsCsAccessSwitchState(lpObj->Index,lpNpc->Index,-1,1);
		gCastleSiege.SetCrownSwitchUserIndex(lpNpc->Class,lpObj->Index);
		LogAdd(LOG_BLACK,"[CastleSiege] [%s][%s] Start to Push Castle Crown Switch (GUILD:%s) - CS X:%d/Y:%d",lpObj->Account,lpObj->Name,lpObj->GuildName,lpNpc->X,lpNpc->Y);
	}
	else if(lpObj->Index != iUserIndex)
	{
		GCAnsCsAccessSwitchState(lpObj->Index,lpNpc->Index,iUserIndex,2);
	}

	#endif
}

void CNpcTalk::NpcCastleSiegeGateSwitch(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	#if(GAMESERVER_TYPE==1)

	if ( (lpObj->Interface.use) > 0 )
	{
		return;
	}

	BOOL bControlEnable = FALSE;
	BYTE btResult = 0;
	int iGateIndex = -1;

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		if(lpObj->CsJoinSide != TRUE)
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}
	else
	{
		if(gCastleSiege.CheckCastleOwnerMember(lpObj->Index) == FALSE && gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index) == FALSE)
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}

	if(bControlEnable != FALSE)
	{
		if(gCastleSiege.CheckLeverAlive(lpNpc->Index) == TRUE)
		{
			if(gCastleSiege.CheckCsGateAlive(lpNpc->CsGateLeverLinkIndex) == TRUE)
			{
				btResult = 1;
				iGateIndex = lpNpc->CsGateLeverLinkIndex;
			}
			else
			{
				btResult = 2;
			}
		}
		else
		{
			btResult = 3;
		}

	}

	GCAnsCsGateState(lpObj->Index,btResult,iGateIndex);

	if(btResult != 1)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = 12;
	#endif
}

void CNpcTalk::NpcCastleSiegeGuard(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	#if(GAMESERVER_TYPE==1)

	if(gObjIsConnected(lpObj->Index) == FALSE )
	{
		return;
	}

	if ( (lpObj->Interface.use ) > 0 )
	{
		return;
	}

	PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE pResult = {0};

	pResult.h.set(0xB9,0x03,sizeof(PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE));
	pResult.btResult = 1;
	pResult.iMaxPrice = 300000;
	pResult.iUnitOfPrice = 10000;
	pResult.btUsable = gCastleSiege.GetHuntZoneEnter();
	pResult.iCurrentPrice = gCastleSiegeSync.GetTaxHuntZone(lpObj->Index,FALSE);

	if(lpObj->Guild != NULL)
	{
		if(gCastleSiege.CheckCastleOwnerMember(lpObj->Index))
		{
			if(lpObj->GuildStatus == 0x80)
			{
				pResult.btResult = 3;
			}
			else
			{
				pResult.btResult = 2;
				pResult.btUsable = 0;
			}
		}
		else if(gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index))
		{
			pResult.btResult = 2;
			pResult.btUsable = 0;
		}
	}

	DataSend(lpObj->Index,(LPBYTE)&pResult,pResult.h.size);

	#endif
}

void CNpcTalk::NpcCastleSiegeWeaponOffense(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gCastleSiegeWeapon.NpcCastleSiegeWeaponOffense(lpNpc,lpObj);
}

void CNpcTalk::NpcCastleSiegeWeaponDefense(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gCastleSiegeWeapon.NpcCastleSiegeWeaponDefense(lpNpc,lpObj);
}

void CNpcTalk::NpcCastleSiegeSenior(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	#if(GAMESERVER_TYPE==0)

	GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(240));

	#else

	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(241));
		return;
	}

	if(gCastleSiege.CheckGuildOwnCastle(lpObj->GuildName) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(242));
		return;
	}

	if(lpObj->GuildStatus != 0x40 && lpObj->GuildStatus != 0x80)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(243));
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 12;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	lpObj->CsGuildInvolved = 0;

	gObjInventoryTransaction(lpObj->Index);

	#endif
}

void CNpcTalk::NpcCastleSiegeGuardsman(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	#if(GAMESERVER_TYPE==1)

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(244));
		return;
	}

	#endif
	
	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_COMMON;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 13;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CNpcTalk::NpcTrainer(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_TRAINER;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 7;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcAngelKing(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gBloodCastle.NpcAngelKing(lpNpc,lpObj);
}

void CNpcTalk::NpcAngelMessenger(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gBloodCastle.NpcAngelMessenger(lpNpc,lpObj);
}

void CNpcTalk::NpcServerTransfer(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->Guild != 0)
	{
		GCServerCommandSend(lpObj->Index,6,0,0);
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_TRANSFER;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 5;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CNpcTalk::NpcCharon(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gDevilSquare.NpcCharon(lpNpc,lpObj);
}

void CNpcTalk::NpcChaosGoblin(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 3;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcWarehouse(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_WAREHOUSE;
	lpObj->Interface.state = 0;

	gWarehouse.GDWarehouseItemSend(lpObj->Index,lpObj->Account);
}

void CNpcTalk::NpcGuildMaster(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->GuildNumber > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(245));
		return;
	}

	if(lpObj->Level < gServerInfo.m_GuildCreateMinLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(246));
		return;
	}

	GCGuildMasterQuestionSend(lpObj->Index);
}

void CNpcTalk::NpcLehap(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_COMMON;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 9;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CNpcTalk::NpcElfBuffer(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->Level > gServerInfo.m_ElfBufferMaxLevel[lpObj->AccountLevel])
	{
		GCServerCommandSend(lpObj->Index,13,0,0);
		return;
	}

	if(lpObj->Reset > gServerInfo.m_ElfBufferMaxReset[lpObj->AccountLevel])
	{
		GCServerCommandSend(lpObj->Index,13,0,0);
		return;
	}

	gEffectManager.AddEffect(lpObj,0,EFFECT_ELF_BUFFER,1800,(gServerInfo.m_ElfBufferDamageConstA+(lpObj->Level/gServerInfo.m_ElfBufferDamageConstB)),(gServerInfo.m_ElfBufferDefenseConstA+(lpObj->Level/gServerInfo.m_ElfBufferDefenseConstB)),0,0);
}

void CNpcTalk::CGNpcTalkRecv(PMSG_NPC_TALK_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use != 0)
	{
		return;
	}

	if(lpObj->Live == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpNpc = &gObj[bIndex];

	if(lpNpc->Type != OBJECT_NPC || lpObj->Map != lpNpc->Map)
	{
		return;
	}

	if(lpObj->X < (lpNpc->X-5) || lpObj->X > (lpNpc->X+5) || lpObj->Y < (lpNpc->Y-5) || lpObj->Y > (lpNpc->Y+5))
	{
		return;
	}

	if(this->NpcTalk(lpNpc,lpObj) != 0)
	{
		return;
	}

	if(SHOP_RANGE(lpNpc->ShopNumber) == 0)
	{
		return;
	}

	if(gShopManager.GetItemCountByIndex(lpNpc->ShopNumber) == 0)
	{
		GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(247));
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_SHOP;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 0;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	lpObj->TargetShopNumber = lpNpc->ShopNumber;

	gShopManager.GCShopItemListSendByIndex(lpNpc->ShopNumber,aIndex);

	GCTaxInfoSend(aIndex,2,gCastleSiegeSync.GetTaxRateStore(aIndex));
}

void CNpcTalk::CGNpcTalkCloseRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type == INTERFACE_NONE || lpObj->Interface.type == INTERFACE_CHAOS_BOX)
	{
		return;
	}

	switch(lpObj->Interface.type)
	{
		case INTERFACE_TRADE:
			gTrade.CGTradeCancelButtonRecv(aIndex);
			break;
		case INTERFACE_SHOP:
			if(((lpObj->Interface.state==1)?(lpObj->Interface.state=0):1)==0){return;}
			lpObj->TargetShopNumber = -1;
			break;
		case INTERFACE_WAREHOUSE:
			gWarehouse.CGWarehouseClose(aIndex);
			break;
		case INTERFACE_TRAINER:
			gChaosBox.ChaosBoxInit(lpObj);
			gObjInventoryCommit(aIndex);
			break;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
}