// ObjectManager.cpp: implementation of the CObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectManager.h"
#include "Attack.h"
#include "BattleSoccerManager.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "ChaosBox.h"
#include "ChaosCastle.h"
#include "CommandManager.h"
#include "DarkSpirit.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "Duel.h"
#include "EffectManager.h"
#include "ESProtocol.h"
#include "ExperienceTable.h"
#include "GameMaster.h"
#include "Gate.h"
#include "Guild.h"
#include "InvasionManager.h"
#include "ItemOption.h"
#include "JSProtocol.h"
#include "Kalima.h"
#include "LifeStone.h"
#include "Map.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "Mercenary.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterSkillManager.h"
#include "Move.h"
#include "MoveSummon.h"
#include "Notice.h"
#include "Party.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "Reconnect.h"
#include "SerialCheck.h"
#include "ServerInfo.h"
#include "SetItemOption.h"
#include "SkillManager.h"
#include "Trade.h"
#include "Util.h"

CObjectManager gObjectManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectManager::CObjectManager() // OK
{

}

CObjectManager::~CObjectManager() // OK
{

}

void CObjectManager::ObjectMsgProc(LPOBJ lpObj) // OK
{
	for (int n = 0; n < MAX_MONSTER_SEND_MSG; n++)
	{
		if (gSMMsg[lpObj->Index][n].MsgCode != -1 && GetTickCount() > ((DWORD)gSMMsg[lpObj->Index][n].MsgTime))
		{
			if (lpObj->Type == OBJECT_MONSTER || lpObj->Type == OBJECT_NPC)
			{
				gObjMonsterStateProc(lpObj, gSMMsg[lpObj->Index][n].MsgCode, gSMMsg[lpObj->Index][n].SendUser, gSMMsg[lpObj->Index][n].SubCode);
				gSMMsg[lpObj->Index][n].Clear();
			}
			else
			{
				this->ObjectStateProc(lpObj, gSMMsg[lpObj->Index][n].MsgCode, gSMMsg[lpObj->Index][n].SendUser, gSMMsg[lpObj->Index][n].SubCode);
				gSMMsg[lpObj->Index][n].Clear();
			}
		}
	}
}

void CObjectManager::ObjectSetStateCreate(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if (lpObj->DieRegen == 1 && (GetTickCount() - lpObj->RegenTime) > (lpObj->MaxRegenTime + 1000))
	{
		lpObj->DieRegen = 2;
		lpObj->State = OBJECT_DIECMD;
		this->CharacterCalcAttribute(aIndex);
	}

	if (lpObj->Type == OBJECT_USER && lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;

		gMove.GCTeleportSend(aIndex, 0, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);

		gObjViewportListProtocolCreate(lpObj);

		this->CharacterUpdateMapEffect(lpObj);

		lpObj->Teleport = 0;
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
	}

	if (lpObj->Type == OBJECT_MONSTER && lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;

		gObjViewportListProtocolCreate(lpObj);

		lpObj->Teleport = 0;
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
	}

	if (lpObj->Type == OBJECT_NPC && lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		if (OBJECT_RANGE(lpObj->SummonIndex) == 0)
		{
			lpObj->X = lpObj->X + ((GetLargeRand() % 6) - 3);
			lpObj->Y = lpObj->Y + ((GetLargeRand() % 6) - 3);
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
		}
		else
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
		}

		gObjViewportListProtocolCreate(lpObj);

		lpObj->Teleport = 0;
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
	}

	lpObj->Teleport = ((lpObj->Teleport == 1) ? ((lpObj->DieRegen == 0) ? (((GetTickCount() - lpObj->TeleportTime) > 100) ? 2 : lpObj->Teleport) : 0) : lpObj->Teleport);

	if (lpObj->Type == OBJECT_USER && lpObj->RegenOk == 2)
	{
		lpObj->RegenOk = 3;
		lpObj->State = OBJECT_CREATE;
		lpObj->X = lpObj->RegenMapX;
		lpObj->Y = lpObj->RegenMapY;
		lpObj->TX = lpObj->RegenMapX;
		lpObj->TY = lpObj->RegenMapY;
		lpObj->Map = lpObj->RegenMapNumber;

		this->CharacterUpdateMapEffect(lpObj);
	}
}

void CObjectManager::ObjectSetStateProc() // OK
{
	for (int n = 0; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		lpObj->Teleport = ((lpObj->Teleport == 3) ? 0 : lpObj->Teleport);

		if (lpObj->State == OBJECT_CREATE)
		{
			lpObj->State = OBJECT_PLAYING;
			lpObj->RegenOk = ((lpObj->RegenOk == 3) ? 0 : lpObj->RegenOk);
		}

		if (lpObj->Type == OBJECT_MONSTER)
		{
			if (lpObj->DieRegen != 2)
			{
				continue;
			}

			lpObj->AttackerKilled = 0;

			if (lpObj->CurrentAI != 0 && lpObj->RegenType != 0)
			{
				continue;
			}

			if (BC_MAP_RANGE(lpObj->Map) != 0 && lpObj->Class >= 131 && lpObj->Class <= 134)
			{
				gObjDel(lpObj->Index);
				continue;
			}

			if (CC_MAP_RANGE(lpObj->Map) != 0)
			{
				gObjDel(lpObj->Index);
				continue;
			}

		#if(GAMESERVER_TYPE==1)

			if (lpObj->Map == MAP_CRYWOLF && gCrywolf.GetCrywolfState() == CRYWOLF_STATE_READY)
			{
				continue;
			}

		#endif

			if (lpObj->Attribute == 60 || lpObj->Attribute == 61)
			{
				gObjDel(lpObj->Index);
				continue;
			}

			if (lpObj->Attribute == 62 && lpObj->Class == 295)
			{
				gObjDel(lpObj->Index);
				continue;
			}

			if (lpObj->Attribute == 62 && lpObj->Class >= 300 && lpObj->Class <= 303)
			{
				gObjDel(lpObj->Index);
				continue;
			}

			if (lpObj->Attribute == 62 && (GetTickCount() - lpObj->LastCheckTick) > 600000)
			{
				gObjDel(lpObj->Index);
				continue;
			}

		#if(GAMESERVER_TYPE==1)

			if (lpObj->Class == 278)
			{
				gLifeStone.DeleteLifeStone(lpObj->Index);
				gObjDel(lpObj->Index);
				continue;
			}

			if (lpObj->Class == 286 || lpObj->Class == 287)
			{
				gMercenary.DeleteMercenary(lpObj->Index);
				gObjDel(lpObj->Index);
				continue;
			}

		#endif

			lpObj->Live = 1;
			lpObj->ViewState = 0;
			lpObj->Teleport = 0;
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;

			gEffectManager.ClearAllEffect(lpObj);

			if (gObjMonsterRegen(lpObj) == 0)
			{
				continue;
			}

			lpObj->DieRegen = 0;
			lpObj->State = OBJECT_CREATE;

			gObjViewportListProtocolCreate(lpObj);
		}

		if (lpObj->Type == OBJECT_USER)
		{
			if (lpObj->DieRegen != 2)
			{
				continue;
			}

			lpObj->Live = 1;
			lpObj->ViewState = 0;
			lpObj->Teleport = 0;
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			lpObj->HPAutoRecuperationTime = GetTickCount();
			lpObj->MPAutoRecuperationTime = GetTickCount();
			lpObj->BPAutoRecuperationTime = GetTickCount();
			lpObj->ResurrectionTalismanActive = 1;
			lpObj->ResurrectionTalismanMap = lpObj->Map;
			lpObj->ResurrectionTalismanX = lpObj->X;
			lpObj->ResurrectionTalismanY = lpObj->Y;

			memset(lpObj->SelfDefenseTime, 0, sizeof(lpObj->SelfDefenseTime));

			gObjTimeCheckSelfDefense(lpObj);

			gEffectManager.ClearAllEffect(lpObj);

			gObjClearViewport(lpObj);

			gDarkSpirit[lpObj->Index].SetMode(DARK_SPIRIT_MODE_NORMAL, -1);

			switch (lpObj->KillerType)
			{
				case 0:
					if (this->CharacterGetRespawnLocation(lpObj) == 0)
					{
						gMap[lpObj->Map].GetMapPos(lpObj->Map, &lpObj->X, &lpObj->Y);
					}
					break;
				case 1:
					if (this->CharacterGetRespawnLocation(lpObj) == 0)
					{
						gMap[lpObj->Map].GetMapPos(lpObj->Map, &lpObj->X, &lpObj->Y);
					}
					break;
				case 2:
					if (lpObj->Guild == 0 || lpObj->Guild->WarType != 1)
					{
						gMap[lpObj->Map].GetMapRandomPos(&lpObj->X, &lpObj->Y, 18);
					}
					else
					{
						gBattleGetTeamPosition(lpObj->Guild->BattleGroundIndex, lpObj->Guild->BattleTeamCode, (short&)lpObj->X, (short&)lpObj->Y);
					}
					break;
				case 3:
					gDuel.RespawnDuelUser(lpObj);
					break;
			}

			if (this->CharacterMapServerMove(lpObj->Index, lpObj->Map, lpObj->X, lpObj->Y) != 0)
			{
				continue;
			}

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			lpObj->PathStartEnd = 0;
			lpObj->DieRegen = 0;
			lpObj->State = OBJECT_CREATE;

			GCCharacterRegenSend(lpObj);

			gObjViewportListProtocolCreate(lpObj);

			this->CharacterUpdateMapEffect(lpObj);
		}
	}

	for (int n = 0; n < MAX_MAP; n++)
	{
		gMap[n].StateSetDestroy();
	}
}

void CObjectManager::ObjectStateProc(LPOBJ lpObj, int MessageCode, int aIndex, int SubCode) // OK
{
	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	switch (MessageCode)
	{
		case 2:
			if (lpObj->Live != 0)
			{
				gObjBackSpring(lpObj, &gObj[aIndex]);
			}
			break;
		case 3:
			if (lpObj->Live != 0)
			{
				this->CharacterMonsterDieHunt(lpObj, &gObj[aIndex]);
			}
			break;
		case 4:
			if (lpObj->Guild != 0 && lpObj->Guild->TargetGuildNode != 0)
			{
				gObjGuildWarEnd(lpObj->Guild, lpObj->Guild->TargetGuildNode);
			}
			break;
		case 5:
			if (lpObj->Guild != 0 && lpObj->Guild->WarState != 0 && lpObj->Guild->WarType == 1)
			{
				BattleSoccerGoalStart(0);
			}
			break;
		case 10:
			if (lpObj->Live != 0 && SubCode > 0)
			{
				gAttack.Attack(lpObj, &gObj[aIndex], 0, 0, 0, SubCode, 0, 0);
			}
			break;
		case 16:
			if ((lpObj->Life + SubCode) > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}
			else
			{
				lpObj->Life += SubCode;
			}

			GCLifeSend(lpObj->Index, 0xFF, (int)lpObj->Life);
			break;
	}
}

void CObjectManager::ObjectStateAttackProc(LPOBJ lpObj, int MessageCode, int aIndex, int SubCode1, int SubCode2) // OK
{
	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	switch (MessageCode)
	{
		case 50:
			if (lpObj->Live != 0 && (SubCode1 == 0 || gSkillManager.GetSkill(lpObj, SubCode1) != 0))
			{
				gAttack.Attack(lpObj, &gObj[aIndex], gSkillManager.GetSkill(lpObj, SubCode1), 0, 0, 0, 0, (bool)(SubCode2 & 1));
			}
			break;
		case 51:
			if (lpObj->Live != 0)
			{
				gDarkSpirit[lpObj->Index].Attack(lpObj, &gObj[aIndex], 0, (bool)(SubCode1 & 1), 0, SubCode2);
			}
			break;
		case 52:
			if (lpObj->Live != 0 && (SubCode1 == 0 || gSkillManager.GetSkill(lpObj, SubCode1) != 0))
			{
				gAttack.Attack(lpObj, &gObj[aIndex], gSkillManager.GetSkill(lpObj, SubCode1), 0, 0, 0, SubCode2, 0);
			}
			break;
		case 53:
			if (lpObj->Live != 0 && (SubCode1 == 0 || gSkillManager.GetSkill(lpObj, SubCode1) != 0))
			{
				gAttack.Attack(lpObj, &gObj[aIndex], gSkillManager.GetSkill(lpObj, SubCode1), 1, 0, 0, 0, (bool)(SubCode2 & 1));
			}
			break;
		case 54:
			if (lpObj->Live != 0 && (SubCode1 == 0 || gSkillManager.GetSkill(lpObj, SubCode1) != 0))
			{
				gAttack.Attack(lpObj, &gObj[aIndex], gSkillManager.GetSkill(lpObj, SubCode1), 1, 0, 0, SubCode2, 0);
			}
			break;
	}
}

void CObjectManager::ObjectMoveProc() // OK
{
	for (int n = 0; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		if (lpObj->State != OBJECT_PLAYING)
		{
			continue;
		}

		if (lpObj->CurrentAI != 0 && (lpObj->Type == OBJECT_MONSTER || lpObj->Type == OBJECT_NPC))
		{
			continue;
		}

		if (lpObj->PathCount == 0 || gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
		{
			continue;
		}

		if (lpObj->Type == OBJECT_MONSTER && (KALIMA_ATTRIBUTE_RANGE(lpObj->Attribute) != 0 || lpObj->Class == 131 || lpObj->Class == 132 || lpObj->Class == 133 || lpObj->Class == 134 || lpObj->Class == 204 || lpObj->Class == 205 || lpObj->Class == 206 || lpObj->Class == 207 || lpObj->Class == 208 || lpObj->Class == 209 || lpObj->Class == 216 || lpObj->Class == 217 || lpObj->Class == 218 || lpObj->Class == 219 || lpObj->Class == 277 || lpObj->Class == 278 || lpObj->Class == 283 || lpObj->Class == 288 || lpObj->Class == 524 || lpObj->Class == 525 || lpObj->Class == 527 || lpObj->Class == 528))
		{
			continue;
		}

		DWORD MoveTime = 0;

		if ((lpObj->PathDir[lpObj->PathCur] % 2) == 0)
		{
			MoveTime = (DWORD)((lpObj->MoveSpeed + ((lpObj->DelayLevel == 0) ? 0 : 300)) * (double)1.3);
		}
		else
		{
			MoveTime = (DWORD)((lpObj->MoveSpeed + ((lpObj->DelayLevel == 0) ? 0 : 300)) * (double)1.0);
		}

		if ((GetTickCount() - lpObj->PathTime) > MoveTime&& lpObj->PathCur < (MAX_ROAD_PATH_TABLE - 1))
		{
			if (gMap[lpObj->Map].CheckAttr(lpObj->PathX[lpObj->PathCur], lpObj->PathY[lpObj->PathCur], 4) != 0 || gMap[lpObj->Map].CheckAttr(lpObj->PathX[lpObj->PathCur], lpObj->PathY[lpObj->PathCur], 8) != 0)
			{
				lpObj->PathCur = 0;
				lpObj->PathCount = 0;
				lpObj->PathTime = GetTickCount();
				lpObj->PathStartEnd = ((lpObj->Type == OBJECT_USER) ? lpObj->PathStartEnd : 0);

				memset(lpObj->PathX, 0, sizeof(lpObj->PathX));

				memset(lpObj->PathY, 0, sizeof(lpObj->PathY));

				memset(lpObj->PathOri, 0, sizeof(lpObj->PathOri));

				gObjSetPosition(lpObj->Index, lpObj->X, lpObj->Y);
			}
			else
			{
				lpObj->X = lpObj->PathX[lpObj->PathCur];
				lpObj->Y = lpObj->PathY[lpObj->PathCur];
				lpObj->Dir = lpObj->PathDir[lpObj->PathCur];
				lpObj->PathTime = GetTickCount();

				if ((++lpObj->PathCur) >= lpObj->PathCount)
				{
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = ((lpObj->Type == OBJECT_USER) ? lpObj->PathStartEnd : 0);
				}
			}
		}
	}
}

void CObjectManager::ObjectMonsterAndMsgProc() // OK
{
	for (int n = 0; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnected(n) != 0)
		{
			if (gObj[n].Type == OBJECT_MONSTER || gObj[n].Type == OBJECT_NPC)
			{
				if (gObj[n].CurrentAI == 0)
				{
					gObjMonsterProcess(&gObj[n]);
				}
			}
			else
			{
				gDarkSpirit[n].MainProc();
				gObjSkillNovaCheckTime(&gObj[n]);
				gObjectManager.ObjectMsgProc(&gObj[n]);
			}
		}
	}

	for (int n = 0; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnected(n) != 0)
		{
			for (int i = 0; i < MAX_MONSTER_SEND_ATTACK_MSG; i++)
			{
				if (gSMAttackProcMsg[n][i].MsgCode != -1 && GetTickCount() > ((DWORD)gSMAttackProcMsg[n][i].MsgTime))
				{
					gObjectManager.ObjectStateAttackProc(&gObj[n], gSMAttackProcMsg[n][i].MsgCode, gSMAttackProcMsg[n][i].SendUser, gSMAttackProcMsg[n][i].SubCode, gSMAttackProcMsg[n][i].SubCode2);
					gSMAttackProcMsg[n][i].Clear();
				}
			}
		}
	}
}

bool CObjectManager::CharacterGameClose(int aIndex) // OK
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_ONLINE)
	{
		return 0;
	}

	if (lpObj->Type != OBJECT_USER || lpObj->MapServerMoveQuit != 0)
	{
		return 0;
	}

	if (OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(lpObj->Index);
	}

	if (lpObj->Interface.use != 0 && (lpObj->Interface.type == INTERFACE_CHAOS_BOX || lpObj->Interface.type == INTERFACE_TRAINER))
	{
		if (lpObj->IsChaosMixCompleted != 0 || lpObj->IsCastleNPCUpgradeCompleted != 0)
		{
			gChaosBox.ChaosBoxItemSave(lpObj);
		}
		else
		{
			gObjInventoryRollback(aIndex);
		}
	}

	if (OBJECT_RANGE(lpObj->TargetNumber) != 0)
	{
		gTrade.ResetTrade(lpObj->TargetNumber);
		gTrade.GCTradeResultSend(lpObj->TargetNumber, 0);
		gTrade.ResetTrade(aIndex);
	}

	if (OBJECT_RANGE(lpObj->DuelUserReserved) != 0)
	{
		gDuel.ResetDuel(&gObj[lpObj->DuelUserReserved]);
		gDuel.GCDuelStartSend(aIndex, 0, lpObj->DuelUserReserved);
		gDuel.ResetDuel(lpObj);
	}

	if (OBJECT_RANGE(lpObj->DuelUserRequested) != 0)
	{
		gDuel.ResetDuel(&gObj[lpObj->DuelUserRequested]);
		gDuel.GCDuelStartSend(lpObj->DuelUserRequested, 0, aIndex);
		gDuel.ResetDuel(lpObj);
	}

	if (OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		gDuel.ResetDuel(&gObj[lpObj->DuelUser]);
		gDuel.GCDuelEndSend(lpObj->DuelUser, 0);
		gDuel.ResetDuel(lpObj);
		gDuel.GCDuelEndSend(aIndex, 0);
	}

	gReconnect.SetReconnectInfo(lpObj);

	if (OBJECT_RANGE(lpObj->PartyNumber) != 0)
	{
		if (gParty.GetMemberCount(lpObj->PartyNumber) <= 1)
		{
			gParty.Destroy(lpObj->PartyNumber);
		}
		else
		{
			gParty.DelMember(lpObj->PartyNumber, lpObj->Index);
		}
	}

	if (lpObj->Guild != 0 && lpObj->Guild->WarState == 1)
	{
		gObjGuildWarMasterClose(lpObj);
	}

	if (BC_MAP_RANGE(lpObj->Map) != 0)
	{
		if (gBloodCastle.GetState(GET_BC_LEVEL(lpObj->Map)) == BC_STATE_START)
		{
			gBloodCastle.SearchUserDropEventItem(aIndex);
		}
		else
		{
			gBloodCastle.SearchUserDeleteEventItem(aIndex);
		}
	}

	gObjFixInventoryPointer(aIndex);
	gObjFixEventInventoryPointer(aIndex);
	GDCharacterInfoSaveSend(aIndex);
	GDDisconnectCharacterSend(aIndex);

	if (lpObj->Guild == 0)
	{
		GDCharClose(1, 0, lpObj->Name);
	}
	else
	{
		GDCharClose(0, lpObj->Guild->Name, lpObj->Name);
	}

	gDarkSpirit[aIndex].SetMode(DARK_SPIRIT_MODE_NORMAL, -1);

	gObjClearViewport(lpObj);

	LogAdd(LOG_BLACK, "[ObjectManager][%d] DelCharacterInfo (%s)", lpObj->Index, lpObj->Name);

	memset(lpObj->Name, 0, sizeof(lpObj->Name));

	lpObj->Connected = OBJECT_LOGGED;
	return 1;
}

void CObjectManager::CharacterGameCloseSet(int aIndex, int type) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->CloseCount > 0)
	{
		return;
	}

	if (lpObj->Connected == OBJECT_ONLINE)
	{
		if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_TRADE)
		{
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(261));
			return;
		}

		if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_WAREHOUSE)
		{
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(262));
			return;
		}

		if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_CHAOS_BOX)
		{
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(263));
			return;
		}

		if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_TRAINER)
		{
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(264));
			return;
		}

		if ((GetTickCount() - lpObj->MySelfDefenseTime) < 30000)
		{
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(265));
			return;
		}
	}

	GCLockSend(aIndex, TRUE);

	lpObj->CloseCount = 6;
	lpObj->CloseType = type;
	lpObj->EnableDelCharacter = 1;
}

bool CObjectManager::CharacterMapServerMove(int aIndex, int map, int x, int y) // OK
{
	if ((gObj[aIndex].NextServerCode = gMapServerManager.CheckMapServerMove(aIndex, map, gObj[aIndex].LastServerCode)) != -1)
	{
		if (gObj[aIndex].NextServerCode != gServerInfo.m_ServerCode)
		{
			GJMapServerMoveSend(aIndex, gObj[aIndex].NextServerCode, map, x, y);
			return 1;
		}
	}

	return 0;
}

void CObjectManager::CharacterUpdateMapEffect(LPOBJ lpObj) // OK
{
#if(GAMESERVER_TYPE==1)

	if (lpObj->Map == MAP_CASTLE_SIEGE)
	{
		GCAnsCsNotifyStart(lpObj->Index, ((gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) ? 1 : 0));

		if (gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			gCastleSiege.NotifySelfCsJoinSide(lpObj->Index);
			gCastleSiege.NotifyCsSelfLeftTime(lpObj->Index);
		}
	}

#endif
}

bool CObjectManager::CharacterGetRespawnLocation(LPOBJ lpObj) // OK
{
	bool result = 0;

	int gate, map, x, y, dir, level;

	if (lpObj->Map == MAP_LORENCIA)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_DUNGEON)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_DEVIAS)
	{
		result = gGate.GetGate(22, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_NORIA)
	{
		result = gGate.GetGate(27, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_LOST_TOWER)
	{
		result = gGate.GetGate(42, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_ARENA)
	{
		result = gGate.GetGate(115, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_ATLANS)
	{
		result = gGate.GetGate(49, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_TARKAN)
	{
		result = gGate.GetGate(57, &gate, &map, &x, &y, &dir, &level);
	}
	else if (DS_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(27, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_ICARUS)
	{
		result = gGate.GetGate(22, &gate, &map, &x, &y, &dir, &level);
	}
	else if (BC_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(22, &gate, &map, &x, &y, &dir, &level);
	}
	else if (CC_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (KALIMA_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_CASTLE_SIEGE)
	{
	#if(GAMESERVER_TYPE==0)

		result = gGate.GetGate(100, &gate, &map, &x, &y, &dir, &level);

	#else

		if (gLifeStone.SetReSpawnUserXY(lpObj->Index) == 0)
		{
			if (lpObj->CsJoinSide == 1)
			{
				result = gGate.GetGate(101, &gate, &map, &x, &y, &dir, &level);
			}
			else
			{
				result = gGate.GetGate(100, &gate, &map, &x, &y, &dir, &level);
			}
		}

	#endif
	}
	else if (lpObj->Map == MAP_LAND_OF_TRIALS)
	{
		result = gGate.GetGate(106, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_AIDA)
	{
		result = gGate.GetGate(119, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_CRYWOLF)
	{
	#if(GAMESERVER_TYPE==1)

		if (gCrywolf.GetCrywolfState() == CRYWOLF_STATE_START)
		{
			result = gGate.GetGate(118, &gate, &map, &x, &y, &dir, &level);
		}

	#endif
	}
	else if (lpObj->Map == MAP_RESERVED2)
	{
		result = gGate.GetGate(401, &gate, &map, &x, &y, &dir, &level);
	}

	if (result != 0)
	{
		lpObj->Map = map;
		lpObj->X = x;
		lpObj->Y = y;
		lpObj->Dir = dir;
	}

	return result;
}

void CObjectManager::CharacterCalcExperienceSplit(LPOBJ lpObj, LPOBJ lpMonster, int damage, int flag) // OK
{
	if (lpMonster->Type != OBJECT_MONSTER)
	{
		return;
	}

	gObjMonsterDelHitDamageUser(lpMonster);

	lpMonster->Money = 0;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (OBJECT_RANGE(lpMonster->HitDamage[n].index) == 0)
		{
			continue;
		}

		if (lpMonster->HitDamage[n].damage == 0)
		{
			continue;
		}

		if (lpMonster->HitDamage[n].index == lpObj->Index)
		{
			this->CharacterCalcExperienceAlone(&gObj[lpMonster->HitDamage[n].index], lpMonster, lpMonster->HitDamage[n].damage, flag, 0, damage);
		}
		else
		{
			this->CharacterCalcExperienceAlone(&gObj[lpMonster->HitDamage[n].index], lpMonster, lpMonster->HitDamage[n].damage, flag, 1, damage);
		}
	}
}

void CObjectManager::CharacterCalcExperienceAlone(LPOBJ lpObj, LPOBJ lpMonster, int damage, int flag, bool type, int AttackDamage) // OK
{
	if (lpMonster->Type != OBJECT_MONSTER)
	{
		return;
	}

	int level = ((lpMonster->Level + 25) * lpMonster->Level) / 3;

	level = (((lpMonster->Level + 10) < lpObj->Level) ? ((level * (lpMonster->Level + 10)) / lpObj->Level) : level);

	if (lpMonster->Level >= 65)
	{
		level += (lpMonster->Level - 64) * (lpMonster->Level / 4);
	}

	level = ((level < 0) ? 0 : level);

	damage = ((damage > lpMonster->MaxLife) ? (int)lpMonster->MaxLife : damage);

	QWORD experience = level + (level / 4);

	experience = ((damage * experience) / (int)lpMonster->MaxLife) * gServerInfo.m_ExperienceRate;
	experience += (experience * gServerInfo.m_AddExperienceRate[lpObj->AccountLevel]) / 100;
	experience = (experience * (lpObj->ExperienceRate + lpObj->EffectOption.AddExperienceRate)) / 100;
	experience = (experience * gMapManager.GetMapExperienceRate(lpObj->Map)) / 100;
	experience = (experience * gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_EXPERIENCE_RATE, 100, -1, -1, lpMonster->Class, lpMonster->Level)) / 100;
	experience = (experience * gExperienceTable.GetExperienceRate(lpObj)) / 100;
	
	lpMonster->Money = (DWORD)(experience);

	this->CharacterPetLevelUp(lpObj, (DWORD)experience);
	
	if (this->CharacterLevelUp(lpObj, experience, gServerInfo.m_MaxLevelUp, EXPERIENCE_COMMON))
	{
		GCMonsterDieSend(lpObj->Index, lpMonster->Index, experience, ((type == 0) ? AttackDamage : 0), flag);
	}
}

void CObjectManager::CharacterCalcExperienceParty(LPOBJ lpObj, LPOBJ lpMonster, int damage, int flag) // OK
{
	if (OBJECT_RANGE(lpObj->PartyNumber) == 0)
	{
		return;
	}

	if (lpMonster->Type != OBJECT_MONSTER)
	{
		return;
	}

	PARTY_INFO* lpParty = &gParty.m_PartyInfo[lpObj->PartyNumber];

	int PartyTopLevel = 0;

	for (int n = 0; n < MAX_PARTY_USER; n++)
	{
		if (OBJECT_RANGE(lpParty->Index[n]) != 0)
		{
			if (lpMonster->Map == gObj[lpParty->Index[n]].Map && gObjCalcDistance(lpMonster, &gObj[lpParty->Index[n]]) < MAX_PARTY_DISTANCE)
			{
				PartyTopLevel = ((gObj[lpParty->Index[n]].Level > PartyTopLevel) ? gObj[lpParty->Index[n]].Level : PartyTopLevel);
			}
		}
	}

	int TotalLevel = 0;
	int PartyLevel = 0;
	int PartyTable = 0;
	int PartyClass = 0;
	int PartyCount = 0;

	for (int n = 0; n < MAX_PARTY_USER; n++)
	{
		if (OBJECT_RANGE(lpParty->Index[n]) != 0)
		{
			if (gObj[lpParty->Index[n]].Map == lpMonster->Map && gObjCalcDistance(lpMonster, &gObj[lpParty->Index[n]]) < MAX_PARTY_DISTANCE)
			{
				TotalLevel += (((gObj[lpParty->Index[n]].Level + 200) < PartyTopLevel) ? (gObj[lpParty->Index[n]].Level + 200) : gObj[lpParty->Index[n]].Level);
				PartyTable += (((PartyTable & (1 << gObj[lpParty->Index[n]].Class)) == 0) ? (1 << gObj[lpParty->Index[n]].Class) : ((++PartyClass) * 0));
				PartyCount++;
			}
		}
	}

	if (PartyCount == 0)
	{
		return;
	}

	int ExperienceRate = (((PartyClass >= 3) ? gServerInfo.m_PartySpecialExperience[(PartyCount - 1)] : gServerInfo.m_PartyGeneralExperience[(PartyCount - 1)]) * PartyCount);

	PartyLevel = TotalLevel / PartyCount;

	int level = ((lpMonster->Level + 25) * lpMonster->Level) / 3;

	if ((lpMonster->Level + 10) < PartyLevel)
	{
		level = (level * (lpMonster->Level + 10)) / PartyLevel;
	}

	if (lpMonster->Level >= 65)
	{
		if (PartyCount == 1)
		{
			level += (lpMonster->Level - 64) * (lpMonster->Level / 4);
		}
		else
		{
			level += (200 - (lpObj->Level / 5));
		}
	}

	level = ((level < 0) ? 0 : level);

	DWORD TotalExperience = level + (level / 4);

	lpMonster->Money = 0;

	for (int n = 0; n < MAX_PARTY_USER; n++)
	{
		if (OBJECT_RANGE(lpParty->Index[n]) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpParty->Index[n]];

		if (lpTarget->Map != lpMonster->Map || gObjCalcDistance(lpMonster, lpTarget) >= MAX_PARTY_DISTANCE)
		{
			continue;
		}

		QWORD experience = 0;

		experience = ((((TotalExperience * ExperienceRate) * lpTarget->Level) / TotalLevel) / 100) * gServerInfo.m_ExperienceRate;
		experience += (experience * gServerInfo.m_AddExperienceRate[lpTarget->AccountLevel]) / 100;
		experience = (experience * (lpTarget->ExperienceRate + lpTarget->EffectOption.AddExperienceRate + (lpTarget->EffectOption.AddPartyBonusExperienceRate * (PartyCount - 1)))) / 100;
		experience = (experience * gMapManager.GetMapExperienceRate(lpTarget->Map)) / 100;
		experience = (experience * gBonusManager.GetBonusValue(lpTarget, BONUS_INDEX_EXPERIENCE_RATE, 100, -1, -1, lpMonster->Class, lpMonster->Level)) / 100;
		experience = (experience * gExperienceTable.GetExperienceRate(lpTarget)) / 100;
		
		lpMonster->Money += (DWORD)(experience) / PartyCount;

		this->CharacterPetLevelUp(lpTarget, (DWORD)experience);
		
		if (this->CharacterLevelUp(lpTarget, experience, gServerInfo.m_MaxLevelUp, EXPERIENCE_PARTY))
		{
			GCMonsterDieSend(lpTarget->Index, lpMonster->Index, experience, damage, flag);
		}
	}
}

bool CObjectManager::CharacterLevelUp(LPOBJ lpObj, QWORD& AddExperience, int MaxLevelUp, int ExperienceType) // OK
{
	if (lpObj->Level >= MAX_CHARACTER_LEVEL)
	{
		if (lpObj->Experience < lpObj->NextExperience)
		{
			if ((lpObj->Experience + AddExperience) > lpObj->NextExperience)
			{
				AddExperience = lpObj->NextExperience - lpObj->Experience;
			}

			lpObj->Experience += AddExperience;
			return true;
		}

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(266));
		return false;
	}

	if ((lpObj->Experience + AddExperience) < lpObj->NextExperience)
	{
		lpObj->Experience += AddExperience;
		return true;
	}
	
	QWORD Experience = lpObj->Experience + AddExperience;

	while (Experience >= lpObj->NextExperience)
	{
		++lpObj->Level;

		lpObj->LevelUpPoint += gServerInfo.m_LevelUpPoint[lpObj->Class];

		gObjCalcExperience(lpObj);

		if (lpObj->Level >= MAX_CHARACTER_LEVEL)
		{
			break;
		}
	}

	if (lpObj->Level >= MAX_CHARACTER_LEVEL && Experience > lpObj->NextExperience)
	{
		AddExperience = lpObj->NextExperience - lpObj->Experience;
	}
	
	lpObj->Experience += AddExperience;

	this->CharacterCalcAttribute(lpObj->Index);

	lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	lpObj->BP = lpObj->MaxBP + lpObj->AddBP;

	GCLevelUpSend(lpObj);

	if ((GetTickCount() - lpObj->CharSaveTime) > 60000)
	{
		lpObj->CharSaveTime = GetTickCount();
		GDCharacterInfoSaveSend(lpObj->Index);
	}

	return true;
}

bool CObjectManager::CharacterLevelUpPointAdd(LPOBJ lpObj, int type, int amount) // OK
{
	if (lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	if (lpObj->LevelUpPoint < amount)
	{
		return 0;
	}

	int* stat;

	switch (type)
	{
		case 0:
			stat = &lpObj->Strength;
			break;
		case 1:
			stat = &lpObj->Dexterity;
			break;
		case 2:
			stat = &lpObj->Vitality;
			break;
		case 3:
			stat = &lpObj->Energy;
			break;
		case 4:
			stat = &lpObj->Leadership;
			break;
		default:
			return 0;
	}

	if (((*stat) + amount) > gServerInfo.m_MaxStatPoint[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(268), gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]);
		return 0;
	}

	(*stat) += amount;

	lpObj->LevelUpPoint -= amount;

	this->CharacterCalcAttribute(lpObj->Index);
	return 1;
}

void CObjectManager::CharacterPetLevelUp(LPOBJ lpObj, DWORD AddExperience) // OK
{
	if (lpObj->Inventory[8].m_Index == GET_ITEM(13, 4)) // Dark Horse
	{
		if (lpObj->Inventory[8].AddPetItemExp((AddExperience / 10)) == 0)
		{
			GCPetItemInfoSend(lpObj->Index, 1, 0, 8, lpObj->Inventory[8].m_PetItemLevel, lpObj->Inventory[8].m_PetItemExp, (BYTE)lpObj->Inventory[8].m_Durability);
		}
		else
		{
			if (gItemManager.IsValidItem(lpObj, &lpObj->Inventory[8]) == 0)
			{
				if (lpObj->Inventory[8].DecPetItemExp((AddExperience / 10)) != 0)
				{
					GCPetItemInfoSend(lpObj->Index, 1, 0, 8, lpObj->Inventory[8].m_PetItemLevel, lpObj->Inventory[8].m_PetItemExp, (BYTE)lpObj->Inventory[8].m_Durability);
				}
			}
			else
			{
				GCPetItemInfoSend(lpObj->Index, 1, 0xFE, 8, lpObj->Inventory[8].m_PetItemLevel, lpObj->Inventory[8].m_PetItemExp, (BYTE)lpObj->Inventory[8].m_Durability);
			}
		}
	}

	if (lpObj->Inventory[1].m_Index == GET_ITEM(13, 5)) // Dark Reaven
	{
		if (lpObj->Inventory[1].AddPetItemExp((AddExperience / 10)) == 0)
		{
			GCPetItemInfoSend(lpObj->Index, 0, 0, 1, lpObj->Inventory[1].m_PetItemLevel, lpObj->Inventory[1].m_PetItemExp, (BYTE)lpObj->Inventory[1].m_Durability);
		}
		else
		{
			if (gItemManager.IsValidItem(lpObj, &lpObj->Inventory[1]) == 0)
			{
				if (lpObj->Inventory[1].DecPetItemExp((AddExperience / 10)) != 0)
				{
					GCPetItemInfoSend(lpObj->Index, 0, 0, 1, lpObj->Inventory[1].m_PetItemLevel, lpObj->Inventory[1].m_PetItemExp, (BYTE)lpObj->Inventory[1].m_Durability);
				}
			}
			else
			{
				GCPetItemInfoSend(lpObj->Index, 0, 0xFE, 1, lpObj->Inventory[1].m_PetItemLevel, lpObj->Inventory[1].m_PetItemExp, (BYTE)lpObj->Inventory[1].m_Durability);
			}
		}
	}
}

void CObjectManager::CharacterMakePreviewCharSet(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	lpObj->CharSet[0] = (lpObj->ChangeUp * 16);
	lpObj->CharSet[0] -= (lpObj->CharSet[0] / 32);
	lpObj->CharSet[0] += (lpObj->Class * 32);

	if (lpObj->ActionNumber == ACTION_SIT1)
	{
		lpObj->CharSet[0] |= 2;
	}

	if (lpObj->ActionNumber == ACTION_POSE1)
	{
		lpObj->CharSet[0] |= 3;
	}

	WORD TempInventory[INVENTORY_WEAR_SIZE];

	for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
	{
		if (n == 0 || n == 1)
		{
			if (lpObj->Inventory[n].IsItem() == 0)
			{
				TempInventory[n] = 0xFFFF;
			}
			else
			{
				TempInventory[n] = lpObj->Inventory[n].m_Index;
			}
		}
		else
		{
			if (lpObj->Inventory[n].IsItem() == 0)
			{
				TempInventory[n] = 0x1FF;
			}
			else
			{
				TempInventory[n] = lpObj->Inventory[n].m_Index % MAX_ITEM_TYPE;
			}
		}
	}

	lpObj->CharSet[1] = TempInventory[0] % 256;
	lpObj->CharSet[12] |= (TempInventory[0] / 16) & 0xF0;

	lpObj->CharSet[2] = TempInventory[1] % 256;
	lpObj->CharSet[13] |= (TempInventory[1] / 16) & 0xF0;

	lpObj->CharSet[3] |= (TempInventory[2] & 0x0F) << 4;
	lpObj->CharSet[9] |= (TempInventory[2] & 0x10) << 3;
	lpObj->CharSet[13] |= (TempInventory[2] & 0x1E0) >> 5;

	lpObj->CharSet[3] |= (TempInventory[3] & 0x0F);
	lpObj->CharSet[9] |= (TempInventory[3] & 0x10) << 2;
	lpObj->CharSet[14] |= (TempInventory[3] & 0x1E0) >> 1;

	lpObj->CharSet[4] |= (TempInventory[4] & 0x0F) << 4;
	lpObj->CharSet[9] |= (TempInventory[4] & 0x10) << 1;
	lpObj->CharSet[14] |= (TempInventory[4] & 0x1E0) >> 5;

	lpObj->CharSet[4] |= (TempInventory[5] & 0x0F);
	lpObj->CharSet[9] |= (TempInventory[5] & 0x10);
	lpObj->CharSet[15] |= (TempInventory[5] & 0x1E0) >> 1;

	lpObj->CharSet[5] |= (TempInventory[6] & 0x0F) << 4;
	lpObj->CharSet[9] |= (TempInventory[6] & 0x10) >> 1;
	lpObj->CharSet[15] |= (TempInventory[6] & 0x1E0) >> 5;

	int level = 0;

	BYTE table[7] = {1,0,6,5,4,3,2};

	for (int n = 0; n < 7; n++)
	{
		if (TempInventory[n] != 0x1FF && TempInventory[n] != 0xFFFF)
		{
			level |= ((lpObj->Inventory[n].m_Level - 1) / 2) << (n * 3);

			lpObj->CharSet[10] |= ((lpObj->Inventory[n].m_NewOption & 0x3F) ? 2 : 0) << table[n];
			lpObj->CharSet[11] |= ((lpObj->Inventory[n].m_SetOption & 0x03) ? 2 : 0) << table[n];
		}
	}

	this->CharacterCalcAttribute(aIndex);

	if (lpObj->IsFullSetItem != 0)
	{
		lpObj->CharSet[11] |= 1;
	}

	lpObj->CharSet[6] = level >> 16;
	lpObj->CharSet[7] = level >> 8;
	lpObj->CharSet[8] = level;

	if (TempInventory[7] == 0x1FF)
	{
		lpObj->CharSet[5] |= 12;
	}
	else if (TempInventory[7] >= 0 && TempInventory[7] <= 2)
	{
		lpObj->CharSet[5] |= TempInventory[7] << 2;
	}
	else if (TempInventory[7] >= 3 && TempInventory[7] <= 6)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[9] |= TempInventory[7] - 2;
	}
	else if (TempInventory[7] == 30)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[9] |= 5;
	}
	else if (TempInventory[7] >= 36 && TempInventory[7] <= 40)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[16] |= (TempInventory[7] - 35) << 2;
	}
	else if (TempInventory[7] == 41)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[9] |= 6;
	}
	else if (TempInventory[7] == 42)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[16] |= 28;
	}
	else if (TempInventory[7] == 43)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[16] |= 24;
	}
	else if (TempInventory[7] >= 130 && TempInventory[7] <= 135)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[17] |= (TempInventory[7] - 129) << 5;
	}

	if (TempInventory[8] == 0x1FF)
	{
		lpObj->CharSet[5] |= 3;
	}
	else if (TempInventory[8] >= 0 && TempInventory[8] <= 2)
	{
		lpObj->CharSet[5] |= TempInventory[8];
	}
	else if (TempInventory[8] == 3)
	{
		lpObj->CharSet[5] |= 3;
		lpObj->CharSet[10] |= 1;
	}
	else if (TempInventory[8] == 4)
	{
		lpObj->CharSet[5] |= 3;
		lpObj->CharSet[12] |= 1;
	}
	else if (TempInventory[8] == 37)
	{
		lpObj->CharSet[5] |= 3;
		lpObj->CharSet[10] &= 0xFE;
		lpObj->CharSet[12] &= 0xFE;
		lpObj->CharSet[12] |= 4;

		if ((lpObj->Inventory[8].m_NewOption & 1) != 0)
		{
			lpObj->CharSet[16] |= 1;
		}
		else if ((lpObj->Inventory[8].m_NewOption & 2) != 0)
		{
			lpObj->CharSet[16] |= 2;
		}
		else if ((lpObj->Inventory[8].m_NewOption & 4) != 0)
		{
			lpObj->CharSet[17] |= 1;
		}
	}
	else if (TempInventory[8] == 64 || TempInventory[8] == 65 || TempInventory[8] == 67)
	{
		lpObj->CharSet[16] |= (TempInventory[8] - 63) << 5;
	}
	else if (TempInventory[8] == 80)
	{
		lpObj->CharSet[16] |= 0xE0;
	}
	else if (TempInventory[8] == 106)
	{
		lpObj->CharSet[16] |= 0xA0;
	}
	else if (TempInventory[8] == 123)
	{
		lpObj->CharSet[16] |= 0x60;
	}

	bool PrismArmor = false;
	bool PrismWeapon = false;

	if (lpObj->Inventory[10].IsItem())
	{
		if (lpObj->Inventory[10].m_Index == GET_ITEM(13, 39))
		{
			PrismArmor = true;
			memcpy(&lpObj->CharSet[18], lpObj->Inventory[10].m_Prism, sizeof(lpObj->Inventory[10].m_Prism));
		}
		else if (lpObj->Inventory[10].m_Index == GET_ITEM(13, 40))
		{
			PrismWeapon = true;
			memcpy(&lpObj->CharSet[21], lpObj->Inventory[10].m_Prism, sizeof(lpObj->Inventory[10].m_Prism));
		}
	}

	if (lpObj->Inventory[11].IsItem())
	{
		if (!PrismArmor && lpObj->Inventory[11].m_Index == GET_ITEM(13, 39))
		{
			PrismArmor = true;
			memcpy(&lpObj->CharSet[18], lpObj->Inventory[11].m_Prism, sizeof(lpObj->Inventory[10].m_Prism));
		}
		else if (!PrismWeapon && lpObj->Inventory[11].m_Index == GET_ITEM(13, 40))
		{
			PrismWeapon = true;
			memcpy(&lpObj->CharSet[21], lpObj->Inventory[11].m_Prism, sizeof(lpObj->Inventory[10].m_Prism));
		}
	}
}

bool CObjectManager::CharacterUseScroll(LPOBJ lpObj, CItem* lpItem) // OK
{
	int skill, slot;

	if ((lpObj->Strength + lpObj->AddStrength) < lpItem->m_RequireStrength)
	{
		return 0;
	}

	if ((lpObj->Dexterity + lpObj->AddDexterity) < lpItem->m_RequireDexterity)
	{
		return 0;
	}

	if (lpItem->m_Index >= GET_ITEM(12, 8) && lpItem->m_Index <= GET_ITEM(12, 24))
	{
		if (lpObj->Level < lpItem->m_RequireLevel)
		{
			return 0;
		}
	}

	if (lpItem->m_Index == GET_ITEM(15, 18))
	{
		if (gQuest.CheckQuestListState(lpObj, 2, QUEST_FINISH) == 0)
		{
			return 0;
		}
	}

	if (gItemManager.CheckItemRequireClass(lpObj, lpItem->m_Index) == 0)
	{
		gItemManager.GCItemDeleteSend(lpObj->Index, -1, 1);
		return 0;
	}

	if ((skill = gSkillManager.GetSkillNumber(lpItem->m_Index, lpItem->m_Level)) == -1)
	{
		gItemManager.GCItemDeleteSend(lpObj->Index, -1, 1);
		return 0;
	}

	if ((slot = gSkillManager.AddSkill(lpObj, skill, lpItem->m_Level)) == -1)
	{
		gItemManager.GCItemDeleteSend(lpObj->Index, -1, 1);
		return 0;
	}

	gSkillManager.GCSkillAddSend(lpObj->Index, slot, skill, (BYTE)lpItem->m_Level, 0);
	return 1;
}

bool CObjectManager::CharacterUsePotion(LPOBJ lpObj, CItem* lpItem) // OK
{
	if (gServerInfo.m_CheckAutoPotionHack != 0 && (GetTickCount() - lpObj->PotionTime) < ((DWORD)gServerInfo.m_CheckAutoPotionHackTolerance))
	{
		return 0;
	}

	lpObj->PotionTime = GetTickCount();

	int HPValue = 0;
	int SDValue = 0;
	int MPValue = 0;
	int BPValue = 0;

	switch (lpItem->m_Index)
	{
		case GET_ITEM(14, 0):
			HPValue = (int)(((lpObj->MaxLife + lpObj->AddLife) * gServerInfo.m_ApplePotionRate) / 100);
			break;
		case GET_ITEM(14, 1):
			HPValue = (int)(((lpObj->MaxLife + lpObj->AddLife) * gServerInfo.m_SmallLifePotionRate) / 100);
			break;
		case GET_ITEM(14, 2):
			HPValue = (int)(((lpObj->MaxLife + lpObj->AddLife) * gServerInfo.m_MidleLifePotionRate) / 100);
			break;
		case GET_ITEM(14, 3):
			HPValue = (int)(((lpObj->MaxLife + lpObj->AddLife) * gServerInfo.m_LargeLifePotionRate) / 100);
			break;
		case GET_ITEM(14, 4):
			MPValue = (int)(((lpObj->MaxMana + lpObj->AddMana) * gServerInfo.m_SmallManaPotionRate) / 100);
			break;
		case GET_ITEM(14, 5):
			MPValue = (int)(((lpObj->MaxMana + lpObj->AddMana) * gServerInfo.m_MidleManaPotionRate) / 100);
			break;
		case GET_ITEM(14, 6):
			MPValue = (int)(((lpObj->MaxMana + lpObj->AddMana) * gServerInfo.m_LargeManaPotionRate) / 100);
			break;
	}

	if ((lpObj->Life + HPValue) > (lpObj->MaxLife + lpObj->AddLife))
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	}
	else
	{
		lpObj->Life += HPValue;
	}

	GCLifeSend(lpObj->Index, 0xFF, (int)lpObj->Life);

	if ((lpObj->Mana + MPValue) > (lpObj->MaxMana + lpObj->AddMana))
	{
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	}
	else
	{
		lpObj->Mana += MPValue;
	}

	if ((lpObj->BP + BPValue) > (lpObj->MaxBP + lpObj->AddBP))
	{
		lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
	}
	else
	{
		lpObj->BP += BPValue;
	}

	GCManaSend(lpObj->Index, 0xFF, (int)lpObj->Mana, lpObj->BP);
	return 1;
}

bool CObjectManager::CharacterUsePortal(LPOBJ lpObj, CItem* lpItem) // OK
{
	if (lpObj->Interface.use != 0)
	{
		return 0;
	}

	if (lpObj->Map == MAP_LORENCIA)
	{
		gObjMoveGate(lpObj->Index, 17);
	}
	else if (lpObj->Map == MAP_DUNGEON)
	{
		gObjMoveGate(lpObj->Index, 17);
	}
	else if (lpObj->Map == MAP_DEVIAS)
	{
		gObjMoveGate(lpObj->Index, 22);
	}
	else if (lpObj->Map == MAP_NORIA)
	{
		gObjMoveGate(lpObj->Index, 27);
	}
	else if (lpObj->Map == MAP_LOST_TOWER)
	{
		gObjMoveGate(lpObj->Index, 42);
	}
	else if (lpObj->Map == MAP_ARENA)
	{
		gObjMoveGate(lpObj->Index, 115);
	}
	else if (lpObj->Map == MAP_ATLANS)
	{
		gObjMoveGate(lpObj->Index, 49);
	}
	else if (lpObj->Map == MAP_TARKAN)
	{
		gObjMoveGate(lpObj->Index, 57);
	}
	else if (DS_MAP_RANGE(lpObj->Map) != 0)
	{
		gObjMoveGate(lpObj->Index, 27);
	}
	else if (lpObj->Map == MAP_ICARUS)
	{
		gObjMoveGate(lpObj->Index, 42);
	}
	else if (BC_MAP_RANGE(lpObj->Map) != 0)
	{
		gObjMoveGate(lpObj->Index, 22);
	}
	else if (CC_MAP_RANGE(lpObj->Map) != 0)
	{
		gObjMoveGate(lpObj->Index, 17);
	}
	else if (KALIMA_MAP_RANGE(lpObj->Map) != 0)
	{
		gObjMoveGate(lpObj->Index, 17);
	}
	else if (lpObj->Map == MAP_CASTLE_SIEGE)
	{
	#if(GAMESERVER_TYPE==0)

		gObjMoveGate(lpObj->Index, 100);

	#else

		if (gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			if (lpObj->CsJoinSide == 1)
			{
				gObjMoveGate(lpObj->Index, 101);
			}
			else
			{
				gObjMoveGate(lpObj->Index, 100);
			}
		}
		else
		{
			if (gCastleSiege.CheckCastleOwnerMember(lpObj->Index) != 0 || gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index) != 0)
			{
				gObjMoveGate(lpObj->Index, 101);
			}
			else
			{
				gObjMoveGate(lpObj->Index, 100);
			}
		}

	#endif
	}
	else if (lpObj->Map == MAP_LAND_OF_TRIALS)
	{
		gObjMoveGate(lpObj->Index, 106);
	}
	else if (lpObj->Map == MAP_AIDA)
	{
		gObjMoveGate(lpObj->Index, 119);
	}
	else if (lpObj->Map == MAP_CRYWOLF)
	{
		gObjMoveGate(lpObj->Index, 118);
	}
	else
	{
		gObjMoveGate(lpObj->Index, 17);
	}

	return 1;
}

bool CObjectManager::CharacterUseJewelOfBless(LPOBJ lpObj, int SourceSlot, int TargetSlot) // OK
{
	if (INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[SourceSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].m_IsPeriodicItem != 0)
	{
		return 0;
	}

	CItem* lpItem = &lpObj->Inventory[TargetSlot];

	if (lpItem->m_Index > GET_ITEM(12, 6) && lpItem->m_Index != GET_ITEM(13, 30))
	{
		return 0;
	}

	if (lpItem->m_Level >= 6)
	{
		return 0;
	}

	lpItem->m_Level++;

	float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index, lpItem->m_Level, lpItem->IsExcItem(), lpItem->IsSetItem());

	lpItem->m_Durability = (dur * lpItem->m_Durability) / lpItem->m_BaseDurability;

	lpItem->Convert(lpItem->m_Index, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption);

	this->CharacterMakePreviewCharSet(lpObj->Index);
	return 1;
}

bool CObjectManager::CharacterUseJewelOfSoul(LPOBJ lpObj, int SourceSlot, int TargetSlot) // OK
{
	if (INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[SourceSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].m_IsPeriodicItem != 0)
	{
		return 0;
	}

	CItem* lpItem = &lpObj->Inventory[TargetSlot];

	if (lpItem->m_Index > GET_ITEM(12, 6) && lpItem->m_Index != GET_ITEM(13, 30))
	{
		return 0;
	}

	if (lpItem->m_Level >= 9)
	{
		return 0;
	}

	int rate = gServerInfo.m_SoulSuccessRate[lpObj->AccountLevel];

	if (lpItem->m_Option2 != 0)
	{
		rate += gServerInfo.m_AddLuckSuccessRate1[lpObj->AccountLevel];
	}

	if ((GetLargeRand() % 100) < rate)
	{
		lpItem->m_Level++;
	}
	else
	{
		if (lpItem->m_Level >= 7)
		{
			lpItem->m_Level = 0;
		}
		else if (lpItem->m_Level >= 1)
		{
			lpItem->m_Level--;
		}
	}

	float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index, lpItem->m_Level, lpItem->IsExcItem(), lpItem->IsSetItem());

	lpItem->m_Durability = (dur * lpItem->m_Durability) / lpItem->m_BaseDurability;

	lpItem->Convert(lpItem->m_Index, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption);

	this->CharacterMakePreviewCharSet(lpObj->Index);
	return 1;
}

bool CObjectManager::CharacterUseJewelOfLife(LPOBJ lpObj, int SourceSlot, int TargetSlot) // OK
{
	if (INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[SourceSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].m_IsPeriodicItem != 0)
	{
		return 0;
	}

	CItem* lpItem = &lpObj->Inventory[TargetSlot];

	if (lpItem->m_Index > GET_ITEM(12, 6) && lpItem->m_Index != GET_ITEM(13, 30))
	{
		return 0;
	}

	if (lpItem->m_Option3 >= gServerInfo.m_MaxItemOption)
	{
		return 0;
	}

	if ((GetLargeRand() % 100) < gServerInfo.m_LifeSuccessRate[lpObj->AccountLevel])
	{
		if (lpItem->m_Option3 == 0 && (lpItem->m_Index >= GET_ITEM(12, 3) && lpItem->m_Index <= GET_ITEM(12, 6)))
		{
			lpItem->m_NewOption &= 0xDF;
			lpItem->m_NewOption |= (32 * (GetLargeRand() % 2));
		}

		lpItem->m_Option3++;
	}
	else
	{
		lpItem->m_Option3 = 0;
	}

	float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index, lpItem->m_Level, lpItem->IsExcItem(), lpItem->IsSetItem());

	lpItem->m_Durability = (dur * lpItem->m_Durability) / lpItem->m_BaseDurability;

	lpItem->Convert(lpItem->m_Index, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption);

	this->CharacterMakePreviewCharSet(lpObj->Index);
	return 1;
}

bool CObjectManager::CharacterUseInkOrNeutralizer(LPOBJ lpObj, int SourceSlot, int TargetSlot)
{
	if (INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[SourceSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	CItem* lpSource = &lpObj->Inventory[SourceSlot];
	CItem* lpTarget = &lpObj->Inventory[TargetSlot];

	if (lpTarget->m_Index != GET_ITEM(13, 39) && lpTarget->m_Index != GET_ITEM(13, 40))
	{
		return 0;
	}

	BYTE Red[] = {GET_NIBBLE_X(lpTarget->m_Prism[0]), GET_NIBBLE_Y(lpTarget->m_Prism[0])};
	BYTE Green[] = {GET_NIBBLE_X(lpTarget->m_Prism[1]), GET_NIBBLE_Y(lpTarget->m_Prism[1])};
	BYTE Blue[] = {GET_NIBBLE_X(lpTarget->m_Prism[2]), GET_NIBBLE_Y(lpTarget->m_Prism[2])};

	if (Red[1] != 0 || Green[1] != 0 || Blue[1] != 0)
	{
		return 0;
	}

	switch (lpSource->m_Index)
	{
		case GET_ITEM(14, 32):
		{
			if (Red[0] >= 15)
			{
				return 0;
			}

			lpTarget->m_Prism[0] = SET_BYTE(++Red[0], Red[1]);

			break;
		}
		case GET_ITEM(14, 33):
		{
			if (Green[0] >= 15)
			{
				return 0;
			}

			lpTarget->m_Prism[1] = SET_BYTE(++Green[0], Green[1]);

			break;
		}
		case GET_ITEM(14, 34):
		{
			if (Blue[0] >= 15)
			{
				return 0;
			}

			lpTarget->m_Prism[2] = SET_BYTE(++Blue[0], Blue[1]);

			break;
		}
		case GET_ITEM(14, 35):
		{
			if (Red[0] <= 0)
			{
				return 0;
			}

			lpTarget->m_Prism[0] = SET_BYTE(--Red[0], Red[1]);

			break;
		}
		case GET_ITEM(14, 36):
		{
			if (Green[0] <= 0)
			{
				return 0;
			}

			lpTarget->m_Prism[1] = SET_BYTE(--Green[0], Green[1]);

			break;
		}
		case GET_ITEM(14, 37):
		{
			if (Blue[0] <= 0)
			{
				return 0;
			}

			lpTarget->m_Prism[2] = SET_BYTE(--Blue[0], Blue[1]);

			break;
		}
	}

	return 1;
}

void CObjectManager::CharacterMonsterDieHunt(LPOBJ lpObj, LPOBJ lpTarget) // OK
{
	int HPValue = (int)(((lpObj->MaxLife + lpObj->AddLife) * lpObj->HuntHP) / 100) + lpTarget->Level;
	int MPValue = (int)(((lpObj->MaxMana + lpObj->AddMana) * lpObj->HuntMP) / 100);
	int BPValue = (int)(((lpObj->MaxBP + lpObj->AddBP) * lpObj->HuntBP) / 100);

	if (HPValue != 0)
	{
		if ((lpObj->Life + HPValue) > (lpObj->MaxLife + lpObj->AddLife))
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
		}
		else
		{
			lpObj->Life += HPValue;
		}

		GCLifeSend(lpObj->Index, 0xFF, (int)lpObj->Life);
	}

	if (MPValue != 0 || BPValue != 0)
	{
		if ((lpObj->Mana + MPValue) > (lpObj->MaxMana + lpObj->AddMana))
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		}
		else
		{
			lpObj->Mana += MPValue;
		}

		if ((lpObj->BP + BPValue) > (lpObj->MaxBP + lpObj->AddBP))
		{
			lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
		}
		else
		{
			lpObj->BP += BPValue;
		}

		GCManaSend(lpObj->Index, 0xFF, (int)lpObj->Mana, lpObj->BP);
	}
}

void CObjectManager::CharacterAutoRecuperation(LPOBJ lpObj) // OK
{
	int HPChange = 0;
	int SDChange = 0;
	int MPChange = 0;
	int BPChange = 0;

	if (((lpObj->HPRecoveryCount++) % 5) == 0)
	{
		if (lpObj->Life != (lpObj->MaxLife + lpObj->AddLife))
		{
			int rate = gServerInfo.m_HPRecoveryRate[lpObj->Class];

			rate += (((GetTickCount() - lpObj->HPAutoRecuperationTime) > 5000) ? 5 : 0);

			rate += lpObj->HPRecoveryRate;

			rate += lpObj->EffectOption.AddHPRecoveryRate;

			int value = (int)(((lpObj->MaxLife + lpObj->AddLife) * rate) / 100);

			value += lpObj->HPRecovery;

			value += lpObj->EffectOption.AddHPRecovery;

			if ((lpObj->Life + value) > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}
			else
			{
				lpObj->Life += value;
			}

			HPChange = 1;
		}
	}

	if (((lpObj->MPRecoveryCount++) % 3) == 0)
	{
		if (lpObj->Mana != (lpObj->MaxMana + lpObj->AddMana))
		{
			int rate = gServerInfo.m_MPRecoveryRate[lpObj->Class];

			rate += (((GetTickCount() - lpObj->MPAutoRecuperationTime) > 5000) ? 3 : 0);

			rate += lpObj->MPRecoveryRate;

			rate += lpObj->EffectOption.AddMPRecoveryRate;

			int value = (int)(((lpObj->MaxMana + lpObj->AddMana) * rate) / 100);

			value += lpObj->MPRecovery;

			value += lpObj->EffectOption.AddMPRecovery;

			if ((lpObj->Mana + value) > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}
			else
			{
				lpObj->Mana += value;
			}

			MPChange = 1;
		}
	}

	if (((lpObj->BPRecoveryCount++) % 3) == 0)
	{
		if (lpObj->BP != (lpObj->MaxBP + lpObj->AddBP))
		{
			int rate = gServerInfo.m_BPRecoveryRate[lpObj->Class];

			rate += (((GetTickCount() - lpObj->BPAutoRecuperationTime) > 5000) ? 3 : 0);

			rate += lpObj->BPRecoveryRate;

			rate += lpObj->EffectOption.AddBPRecoveryRate;

			int value = (int)(((lpObj->MaxBP + lpObj->AddBP) * rate) / 100);

			value += lpObj->BPRecovery;

			value += lpObj->EffectOption.AddBPRecovery;

			if ((lpObj->BP + value) > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}
			else
			{
				lpObj->BP += value;
			}

			BPChange = 1;
		}
	}

	if (HPChange != 0 || SDChange != 0)
	{
		GCLifeSend(lpObj->Index, 0xFF, (int)lpObj->Life);
	}

	if (MPChange != 0 || BPChange != 0)
	{
		GCManaSend(lpObj->Index, 0xFF, (int)lpObj->Mana, lpObj->BP);
	}
}

void CObjectManager::CharacterItemDurationDown(LPOBJ lpObj) // OK
{
	if (((lpObj->TimeCount++) % 10) != 0)
	{
		return;
	}

	for (int n = 2; n < 10; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			bool result = 0;

			switch (n)
			{
				case 7:
					result = lpObj->Inventory[n].WingDurabilityDown(lpObj->Index, ((gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0) ? 1 : 0));
					break;
				case 9:
					result = lpObj->Inventory[n].PendantDurabilityDown(lpObj->Index, ((gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0) ? 1 : 0));
					break;
			}

			if (result != 0)
			{
				gItemManager.GCItemDurSend(lpObj->Index, n, (BYTE)lpObj->Inventory[n].m_Durability, 0);
			}
		}
	}

	for (int n = 10; n < 12; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			bool result = 0;

			switch (lpObj->Inventory[n].m_Index)
			{
				case GET_ITEM(13, 10):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, 113);
					break;
				case GET_ITEM(13, 20):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, ((gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0) ? 70 : 0));
					break;
				case GET_ITEM(13, 32):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, 113);
					break;
				case GET_ITEM(13, 33):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, 113);
					break;
				case GET_ITEM(13, 34):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, 113);
					break;
				case GET_ITEM(13, 35):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, 113);
					break;
				case GET_ITEM(13, 36):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, 113);
					break;
				case GET_ITEM(13, 37):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, 113);
					break;
				case GET_ITEM(13, 38):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, 113);
					break;
				default:
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index, ((gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0) ? 1 : 0));
					break;
			}

			if (result != 0)
			{
				gItemManager.GCItemDurSend(lpObj->Index, n, (BYTE)lpObj->Inventory[n].m_Durability, 0);

				if (gSkillManager.SkillChangeUse(lpObj->Index) != 0)
				{
					gObjViewportListProtocolCreate(lpObj);
					this->CharacterUpdateMapEffect(lpObj);
				}
			}
		}
	}
}

void CObjectManager::CharacterCalcBP(LPOBJ lpObj) // OK
{
	switch (lpObj->Class)
	{
		case CLASS_DW:
			lpObj->MaxBP = (int)(((lpObj->Strength + lpObj->AddStrength) * 0.20) + ((lpObj->Dexterity + lpObj->AddDexterity) * 0.40) + ((lpObj->Vitality + lpObj->AddVitality) * 0.30) + ((lpObj->Energy + lpObj->AddEnergy) * 0.20));
			break;
		case CLASS_DK:
			lpObj->MaxBP = (int)(((lpObj->Strength + lpObj->AddStrength) * 0.15) + ((lpObj->Dexterity + lpObj->AddDexterity) * 0.20) + ((lpObj->Vitality + lpObj->AddVitality) * 0.30) + ((lpObj->Energy + lpObj->AddEnergy) * 1.00));
			break;
		case CLASS_FE:
			lpObj->MaxBP = (int)(((lpObj->Strength + lpObj->AddStrength) * 0.30) + ((lpObj->Dexterity + lpObj->AddDexterity) * 0.20) + ((lpObj->Vitality + lpObj->AddVitality) * 0.30) + ((lpObj->Energy + lpObj->AddEnergy) * 0.20));
			break;
		case CLASS_MG:
			lpObj->MaxBP = (int)(((lpObj->Strength + lpObj->AddStrength) * 0.20) + ((lpObj->Dexterity + lpObj->AddDexterity) * 0.25) + ((lpObj->Vitality + lpObj->AddVitality) * 0.30) + ((lpObj->Energy + lpObj->AddEnergy) * 0.15));
			break;
		case CLASS_DL:
			lpObj->MaxBP = (int)(((lpObj->Strength + lpObj->AddStrength) * 0.30) + ((lpObj->Dexterity + lpObj->AddDexterity) * 0.20) + ((lpObj->Vitality + lpObj->AddVitality) * 0.10) + ((lpObj->Energy + lpObj->AddEnergy) * 0.15) + ((lpObj->Leadership + lpObj->AddLeadership) * 0.30));
			break;
	}
}

void CObjectManager::CharacterCalcAttribute(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return;
	}

	CItem* Right = &lpObj->Inventory[0];
	CItem* Left = &lpObj->Inventory[1];
	CItem* Gloves = &lpObj->Inventory[5];
	CItem* Wings = &lpObj->Inventory[7];
	CItem* Helper = &lpObj->Inventory[8];
	CItem* Amulet = &lpObj->Inventory[9];
	CItem* Ring1 = &lpObj->Inventory[10];
	CItem* Ring2 = &lpObj->Inventory[11];

	float TotalHP = 100;
	float TotalMP = 100;
	float TotalBP = 100;

	if ((lpObj->MaxLife + lpObj->AddLife) != 0)
	{
		TotalHP = (float)(lpObj->Life * 100) / (float)(lpObj->MaxLife + lpObj->AddLife);
	}

	if ((lpObj->MaxMana + lpObj->AddMana) != 0)
	{
		TotalMP = (float)(lpObj->Mana * 100) / (lpObj->MaxMana + lpObj->AddMana);
	}

	if ((lpObj->MaxBP + lpObj->AddBP) != 0)
	{
		TotalBP = (float)(lpObj->BP * 100) / (float)(lpObj->MaxBP + lpObj->AddBP);
	}

	lpObj->HaveWeaponInHand = 1;

	if (Right->IsItem() == 0 && Left->IsItem() == 0)
	{
		lpObj->HaveWeaponInHand = 0;
	}
	else if (Left->IsItem() == 0 && Right->m_Index == GET_ITEM(4, 15)) // Arrow
	{
		lpObj->HaveWeaponInHand = 0;
	}
	else if (Right->IsItem() == 0)
	{
		if (Left->m_Index == GET_ITEM(4, 7)) // Bolt
		{
			lpObj->HaveWeaponInHand = 0;
		}
		else if (Left->m_Index >= GET_ITEM(6, 0) && Left->m_Index < GET_ITEM(7, 0)) // Shields
		{
			lpObj->HaveWeaponInHand = 0;
		}
	}

	for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			lpObj->Inventory[n].m_IsValidItem = 1;
		}
	}

	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->AddBP = 0;
	lpObj->SkillLongSpearChange = 0;

	memset(lpObj->AddResistance, 0, sizeof(lpObj->AddResistance));

	gObjClearSpecialOption(lpObj);

	bool change = 1;

	while (change != 0)
	{
		change = 0;
		lpObj->AddStrength = lpObj->EffectOption.AddStrength;
		lpObj->AddDexterity = lpObj->EffectOption.AddDexterity;
		lpObj->AddVitality = lpObj->EffectOption.AddVitality;
		lpObj->AddEnergy = lpObj->EffectOption.AddEnergy;
		lpObj->AddLeadership = lpObj->EffectOption.AddLeadership;

		gSetItemOption.CalcSetItemStat(lpObj);
		gItemOption.CalcItemCommonOption(lpObj, 1);
		gSetItemOption.CalcSetItemOption(lpObj, 1);

		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].m_IsValidItem != 0)
			{
				if (gItemManager.IsValidItem(lpObj, &lpObj->Inventory[n]) == 0)
				{
					change = 1;
					lpObj->Inventory[n].m_IsValidItem = 0;
				}
			}
		}
	}

	int Strength = lpObj->Strength + lpObj->AddStrength;
	int Dexterity = lpObj->Dexterity + lpObj->AddDexterity;
	int Vitality = lpObj->Vitality + lpObj->AddVitality;
	int Energy = lpObj->Energy + lpObj->AddEnergy;
	int Leadership = lpObj->Leadership + lpObj->AddLeadership;

	if (lpObj->Class == CLASS_DW)
	{
		lpObj->PhysiDamageMinRight = Strength / gServerInfo.m_DWPhysiDamageMinConstA;
		lpObj->PhysiDamageMaxRight = Strength / gServerInfo.m_DWPhysiDamageMaxConstA;
		lpObj->PhysiDamageMinLeft = Strength / gServerInfo.m_DWPhysiDamageMinConstA;
		lpObj->PhysiDamageMaxLeft = Strength / gServerInfo.m_DWPhysiDamageMaxConstA;
		lpObj->MagicDamageMin = Energy / gServerInfo.m_DWMagicDamageMinConstA;
		lpObj->MagicDamageMax = Energy / gServerInfo.m_DWMagicDamageMaxConstA;
		lpObj->CurseDamageMin = Energy / gServerInfo.m_DWMagicDamageMinConstA;
		lpObj->CurseDamageMax = Energy / gServerInfo.m_DWMagicDamageMaxConstA;
	}
	else if (lpObj->Class == CLASS_DK)
	{
		lpObj->PhysiDamageMinRight = Strength / gServerInfo.m_DKPhysiDamageMinConstA;
		lpObj->PhysiDamageMaxRight = Strength / gServerInfo.m_DKPhysiDamageMaxConstA;
		lpObj->PhysiDamageMinLeft = Strength / gServerInfo.m_DKPhysiDamageMinConstA;
		lpObj->PhysiDamageMaxLeft = Strength / gServerInfo.m_DKPhysiDamageMaxConstA;
		lpObj->MagicDamageMin = Energy / gServerInfo.m_DKMagicDamageMinConstA;
		lpObj->MagicDamageMax = Energy / gServerInfo.m_DKMagicDamageMaxConstA;
		lpObj->CurseDamageMin = Energy / gServerInfo.m_DKMagicDamageMinConstA;
		lpObj->CurseDamageMax = Energy / gServerInfo.m_DKMagicDamageMaxConstA;
		lpObj->DKDamageMultiplierRate = 200 + (Energy / gServerInfo.m_DKDamageMultiplierConstA);
		lpObj->DKDamageMultiplierRate = ((lpObj->DKDamageMultiplierRate > gServerInfo.m_DKDamageMultiplierMaxRate) ? gServerInfo.m_DKDamageMultiplierMaxRate : lpObj->DKDamageMultiplierRate);
	}
	else if (lpObj->Class == CLASS_FE)
	{
		if ((Right->m_Index >= GET_ITEM(4, 0) && Right->m_Index < GET_ITEM(5, 0) && Right->m_Index != GET_ITEM(4, 15)) || (Left->m_Index >= GET_ITEM(4, 0) && Left->m_Index < GET_ITEM(5, 0) && Left->m_Index != GET_ITEM(4, 7)))
		{
			if ((Right->IsItem() != 0 && Right->m_IsValidItem == 0) || (Left->IsItem() != 0 && Left->m_IsValidItem == 0))
			{
				lpObj->PhysiDamageMinRight = (Strength + Dexterity) / gServerInfo.m_FEPhysiDamageMinConstA;
				lpObj->PhysiDamageMaxRight = (Strength + Dexterity) / gServerInfo.m_FEPhysiDamageMaxConstA;
				lpObj->PhysiDamageMinLeft = (Strength + Dexterity) / gServerInfo.m_FEPhysiDamageMinConstA;
				lpObj->PhysiDamageMaxLeft = (Strength + Dexterity) / gServerInfo.m_FEPhysiDamageMaxConstA;
			}
			else
			{
				lpObj->PhysiDamageMinRight = (Strength / gServerInfo.m_FEPhysiDamageMinBowConstA) + (Dexterity / gServerInfo.m_FEPhysiDamageMinBowConstB);
				lpObj->PhysiDamageMaxRight = (Strength / gServerInfo.m_FEPhysiDamageMaxBowConstA) + (Dexterity / gServerInfo.m_FEPhysiDamageMaxBowConstB);
				lpObj->PhysiDamageMinLeft = (Strength / gServerInfo.m_FEPhysiDamageMinBowConstA) + (Dexterity / gServerInfo.m_FEPhysiDamageMinBowConstB);
				lpObj->PhysiDamageMaxLeft = (Strength / gServerInfo.m_FEPhysiDamageMaxBowConstA) + (Dexterity / gServerInfo.m_FEPhysiDamageMaxBowConstB);
			}
		}
		else
		{
			lpObj->PhysiDamageMinRight = (Strength + Dexterity) / gServerInfo.m_FEPhysiDamageMinConstA;
			lpObj->PhysiDamageMaxRight = (Strength + Dexterity) / gServerInfo.m_FEPhysiDamageMaxConstA;
			lpObj->PhysiDamageMinLeft = (Strength + Dexterity) / gServerInfo.m_FEPhysiDamageMinConstA;
			lpObj->PhysiDamageMaxLeft = (Strength + Dexterity) / gServerInfo.m_FEPhysiDamageMaxConstA;
		}

		lpObj->MagicDamageMin = Energy / gServerInfo.m_FEMagicDamageMinConstA;
		lpObj->MagicDamageMax = Energy / gServerInfo.m_FEMagicDamageMaxConstA;
		lpObj->CurseDamageMin = Energy / gServerInfo.m_FEMagicDamageMinConstA;
		lpObj->CurseDamageMax = Energy / gServerInfo.m_FEMagicDamageMaxConstA;
	}
	else if (lpObj->Class == CLASS_MG)
	{
		lpObj->PhysiDamageMinRight = (Strength / gServerInfo.m_MGPhysiDamageMinConstA) + (Energy / gServerInfo.m_MGPhysiDamageMinConstB);
		lpObj->PhysiDamageMaxRight = (Strength / gServerInfo.m_MGPhysiDamageMaxConstA) + (Energy / gServerInfo.m_MGPhysiDamageMaxConstB);
		lpObj->PhysiDamageMinLeft = (Strength / gServerInfo.m_MGPhysiDamageMinConstA) + (Energy / gServerInfo.m_MGPhysiDamageMinConstB);
		lpObj->PhysiDamageMaxLeft = (Strength / gServerInfo.m_MGPhysiDamageMaxConstA) + (Energy / gServerInfo.m_MGPhysiDamageMaxConstB);
		lpObj->MagicDamageMin = Energy / gServerInfo.m_MGMagicDamageMinConstA;
		lpObj->MagicDamageMax = Energy / gServerInfo.m_MGMagicDamageMaxConstA;
		lpObj->CurseDamageMin = Energy / gServerInfo.m_MGMagicDamageMinConstA;
		lpObj->CurseDamageMax = Energy / gServerInfo.m_MGMagicDamageMaxConstA;
	}
	else if (lpObj->Class == CLASS_DL)
	{
		lpObj->PhysiDamageMinRight = (Strength / gServerInfo.m_DLPhysiDamageMinConstA) + (Energy / gServerInfo.m_DLPhysiDamageMinConstB);
		lpObj->PhysiDamageMaxRight = (Strength / gServerInfo.m_DLPhysiDamageMaxConstA) + (Energy / gServerInfo.m_DLPhysiDamageMaxConstB);
		lpObj->PhysiDamageMinLeft = (Strength / gServerInfo.m_DLPhysiDamageMinConstA) + (Energy / gServerInfo.m_DLPhysiDamageMinConstB);
		lpObj->PhysiDamageMaxLeft = (Strength / gServerInfo.m_DLPhysiDamageMaxConstA) + (Energy / gServerInfo.m_DLPhysiDamageMaxConstB);
		lpObj->MagicDamageMin = Energy / gServerInfo.m_DLMagicDamageMinConstA;
		lpObj->MagicDamageMax = Energy / gServerInfo.m_DLMagicDamageMaxConstA;
		lpObj->CurseDamageMin = Energy / gServerInfo.m_DLMagicDamageMinConstA;
		lpObj->CurseDamageMax = Energy / gServerInfo.m_DLMagicDamageMaxConstA;
		lpObj->DLDamageMultiplierRate = 200 + (Energy / gServerInfo.m_DLDamageMultiplierConstA);
		lpObj->DLDamageMultiplierRate = ((lpObj->DLDamageMultiplierRate > gServerInfo.m_DLDamageMultiplierMaxRate) ? gServerInfo.m_DLDamageMultiplierMaxRate : lpObj->DLDamageMultiplierRate);
	}

	if (Right->IsItem() != 0)
	{
		if (Right->m_Index >= GET_ITEM(5, 0) && Right->m_Index < GET_ITEM(6, 0))
		{
			lpObj->PhysiDamageMinRight += Right->GetDamageMin() / 2;
			lpObj->PhysiDamageMaxRight += Right->GetDamageMax() / 2;
			lpObj->SkillLongSpearChange = ((Right->m_SkillChange == 0) ? lpObj->SkillLongSpearChange : 1);
		}
		else
		{
			lpObj->PhysiDamageMinRight += Right->GetDamageMin() / 1;
			lpObj->PhysiDamageMaxRight += Right->GetDamageMax() / 1;
			lpObj->SkillLongSpearChange = ((Right->m_SkillChange == 0) ? lpObj->SkillLongSpearChange : 1);
		}
	}

	if (Left->IsItem() != 0)
	{
		if (Left->m_Index >= GET_ITEM(5, 0) && Left->m_Index < GET_ITEM(6, 0))
		{
			lpObj->PhysiDamageMinLeft += Left->GetDamageMin() / 2;
			lpObj->PhysiDamageMaxLeft += Left->GetDamageMax() / 2;
			lpObj->SkillLongSpearChange = ((Left->m_SkillChange == 0) ? lpObj->SkillLongSpearChange : 1);
		}
		else
		{
			lpObj->PhysiDamageMinLeft += Left->GetDamageMin() / 1;
			lpObj->PhysiDamageMaxLeft += Left->GetDamageMax() / 1;
			lpObj->SkillLongSpearChange = ((Left->m_SkillChange == 0) ? lpObj->SkillLongSpearChange : 1);
		}
	}

	if (lpObj->Class == CLASS_DW)
	{
		lpObj->AttackSuccessRate = (lpObj->Level * gServerInfo.m_DWAttackSuccessRateConstA) + ((Dexterity * gServerInfo.m_DWAttackSuccessRateConstB) / gServerInfo.m_DWAttackSuccessRateConstC) + (Strength / gServerInfo.m_DWAttackSuccessRateConstD);
	}
	else if (lpObj->Class == CLASS_DK)
	{
		lpObj->AttackSuccessRate = (lpObj->Level * gServerInfo.m_DKAttackSuccessRateConstA) + ((Dexterity * gServerInfo.m_DKAttackSuccessRateConstB) / gServerInfo.m_DKAttackSuccessRateConstC) + (Strength / gServerInfo.m_DKAttackSuccessRateConstD);
	}
	else if (lpObj->Class == CLASS_FE)
	{
		lpObj->AttackSuccessRate = (lpObj->Level * gServerInfo.m_FEAttackSuccessRateConstA) + ((Dexterity * gServerInfo.m_FEAttackSuccessRateConstB) / gServerInfo.m_FEAttackSuccessRateConstC) + (Strength / gServerInfo.m_FEAttackSuccessRateConstD);
	}
	else if (lpObj->Class == CLASS_MG)
	{
		lpObj->AttackSuccessRate = (lpObj->Level * gServerInfo.m_MGAttackSuccessRateConstA) + ((Dexterity * gServerInfo.m_MGAttackSuccessRateConstB) / gServerInfo.m_MGAttackSuccessRateConstC) + (Strength / gServerInfo.m_MGAttackSuccessRateConstD);
	}
	else if (lpObj->Class == CLASS_DL)
	{
		lpObj->AttackSuccessRate = (lpObj->Level * gServerInfo.m_DLAttackSuccessRateConstA) + ((Dexterity * gServerInfo.m_DLAttackSuccessRateConstB) / gServerInfo.m_DLAttackSuccessRateConstC) + (Strength / gServerInfo.m_DLAttackSuccessRateConstD) + (Leadership / gServerInfo.m_DLAttackSuccessRateConstE);
	}

	if (lpObj->Class == CLASS_DW)
	{
		lpObj->AttackSuccessRatePvP = ((lpObj->Level * gServerInfo.m_DWAttackSuccessRatePvPConstA) / gServerInfo.m_DWAttackSuccessRatePvPConstB) + ((Dexterity * gServerInfo.m_DWAttackSuccessRatePvPConstC) / gServerInfo.m_DWAttackSuccessRatePvPConstD);
	}
	else if (lpObj->Class == CLASS_DK)
	{
		lpObj->AttackSuccessRatePvP = ((lpObj->Level * gServerInfo.m_DKAttackSuccessRatePvPConstA) / gServerInfo.m_DKAttackSuccessRatePvPConstB) + ((Dexterity * gServerInfo.m_DKAttackSuccessRatePvPConstC) / gServerInfo.m_DKAttackSuccessRatePvPConstD);
	}
	else if (lpObj->Class == CLASS_FE)
	{
		lpObj->AttackSuccessRatePvP = ((lpObj->Level * gServerInfo.m_FEAttackSuccessRatePvPConstA) / gServerInfo.m_FEAttackSuccessRatePvPConstB) + ((Dexterity * gServerInfo.m_FEAttackSuccessRatePvPConstC) / gServerInfo.m_FEAttackSuccessRatePvPConstD);
	}
	else if (lpObj->Class == CLASS_MG)
	{
		lpObj->AttackSuccessRatePvP = ((lpObj->Level * gServerInfo.m_MGAttackSuccessRatePvPConstA) / gServerInfo.m_MGAttackSuccessRatePvPConstB) + ((Dexterity * gServerInfo.m_MGAttackSuccessRatePvPConstC) / gServerInfo.m_MGAttackSuccessRatePvPConstD);
	}
	else if (lpObj->Class == CLASS_DL)
	{
		lpObj->AttackSuccessRatePvP = ((lpObj->Level * gServerInfo.m_DLAttackSuccessRatePvPConstA) / gServerInfo.m_DLAttackSuccessRatePvPConstB) + ((Dexterity * gServerInfo.m_DLAttackSuccessRatePvPConstC) / gServerInfo.m_DLAttackSuccessRatePvPConstD);
	}

	if (lpObj->Class == CLASS_DW)
	{
		lpObj->PhysiSpeed = Dexterity / gServerInfo.m_DWPhysiSpeedConstA;
		lpObj->MagicSpeed = Dexterity / gServerInfo.m_DWMagicSpeedConstA;
	}
	if (lpObj->Class == CLASS_DK)
	{
		lpObj->PhysiSpeed = Dexterity / gServerInfo.m_DKPhysiSpeedConstA;
		lpObj->MagicSpeed = Dexterity / gServerInfo.m_DKMagicSpeedConstA;
	}
	else if (lpObj->Class == CLASS_FE)
	{
		lpObj->PhysiSpeed = Dexterity / gServerInfo.m_FEPhysiSpeedConstA;
		lpObj->MagicSpeed = Dexterity / gServerInfo.m_FEMagicSpeedConstA;
	}
	else if (lpObj->Class == CLASS_MG)
	{
		lpObj->PhysiSpeed = Dexterity / gServerInfo.m_MGPhysiSpeedConstA;
		lpObj->MagicSpeed = Dexterity / gServerInfo.m_MGMagicSpeedConstA;
	}
	else if (lpObj->Class == CLASS_DL)
	{
		lpObj->PhysiSpeed = Dexterity / gServerInfo.m_DLPhysiSpeedConstA;
		lpObj->MagicSpeed = Dexterity / gServerInfo.m_DLMagicSpeedConstA;
	}

	lpObj->PhysiSpeed += lpObj->DrinkSpeed;
	lpObj->MagicSpeed += lpObj->DrinkSpeed;

	lpObj->PhysiSpeed += lpObj->EffectOption.AddPhysiSpeed;
	lpObj->MagicSpeed += lpObj->EffectOption.AddMagicSpeed;

	lpObj->PhysiSpeed += (lpObj->PhysiSpeed * lpObj->EffectOption.MulPhysiSpeed) / 100;
	lpObj->MagicSpeed += (lpObj->MagicSpeed * lpObj->EffectOption.MulMagicSpeed) / 100;

	lpObj->PhysiSpeed += (lpObj->PhysiSpeed * lpObj->EffectOption.DivPhysiSpeed) / 100;
	lpObj->MagicSpeed += (lpObj->MagicSpeed * lpObj->EffectOption.DivMagicSpeed) / 100;

	bool RightItem = 0;

	if (Right->m_Index >= GET_ITEM(0, 0) && Right->m_Index < GET_ITEM(6, 0) && Right->m_Index != GET_ITEM(4, 7) && Right->m_Index != GET_ITEM(4, 15))
	{
		if (Right->m_IsValidItem != 0)
		{
			RightItem = 1;
		}
	}

	bool LeftItem = 0;

	if (Left->m_Index >= GET_ITEM(0, 0) && Left->m_Index < GET_ITEM(6, 0) && Left->m_Index != GET_ITEM(4, 7) && Left->m_Index != GET_ITEM(4, 15))
	{
		if (Left->m_IsValidItem != 0)
		{
			LeftItem = 1;
		}
	}

	if (RightItem != 0 && LeftItem != 0)
	{
		lpObj->PhysiSpeed += (Right->m_AttackSpeed + Left->m_AttackSpeed) / 2;
		lpObj->MagicSpeed += (Right->m_AttackSpeed + Left->m_AttackSpeed) / 2;
	}
	else if (RightItem != 0)
	{
		lpObj->PhysiSpeed += Right->m_AttackSpeed;
		lpObj->MagicSpeed += Right->m_AttackSpeed;
	}
	else if (LeftItem != 0)
	{
		lpObj->PhysiSpeed += Left->m_AttackSpeed;
		lpObj->MagicSpeed += Left->m_AttackSpeed;
	}

	if (Gloves->IsItem() != 0 && Gloves->m_IsValidItem != 0)
	{
		lpObj->PhysiSpeed += Gloves->m_AttackSpeed;
		lpObj->MagicSpeed += Gloves->m_AttackSpeed;
	}

	if (Helper->IsItem() != 0 && Helper->m_IsValidItem != 0)
	{
		lpObj->PhysiSpeed += Helper->m_AttackSpeed;
		lpObj->MagicSpeed += Helper->m_AttackSpeed;
	}

	if (Amulet->IsItem() != 0 && Amulet->m_IsValidItem != 0)
	{
		lpObj->PhysiSpeed += Amulet->m_AttackSpeed;
		lpObj->MagicSpeed += Amulet->m_AttackSpeed;
	}

	if (lpObj->Class == CLASS_DW)
	{
		lpObj->DefenseSuccessRate = Dexterity / gServerInfo.m_DWDefenseSuccessRateConstA;
	}
	else if (lpObj->Class == CLASS_DK)
	{
		lpObj->DefenseSuccessRate = Dexterity / gServerInfo.m_DKDefenseSuccessRateConstA;
	}
	else if (lpObj->Class == CLASS_FE)
	{
		lpObj->DefenseSuccessRate = Dexterity / gServerInfo.m_FEDefenseSuccessRateConstA;
	}
	else if (lpObj->Class == CLASS_MG)
	{
		lpObj->DefenseSuccessRate = Dexterity / gServerInfo.m_MGDefenseSuccessRateConstA;
	}
	else if (lpObj->Class == CLASS_DL)
	{
		lpObj->DefenseSuccessRate = Dexterity / gServerInfo.m_DLDefenseSuccessRateConstA;
	}

	lpObj->DefenseSuccessRate += lpObj->Inventory[1].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[2].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[3].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[4].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[5].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[6].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[7].GetDefenseSuccessRate();

	if (lpObj->Class == CLASS_DW)
	{
		lpObj->DefenseSuccessRatePvP = ((lpObj->Level * gServerInfo.m_DWDefenseSuccessRatePvPConstA) / gServerInfo.m_DWDefenseSuccessRatePvPConstB) + (Dexterity / gServerInfo.m_DWDefenseSuccessRatePvPConstC);
	}
	else if (lpObj->Class == CLASS_DK)
	{
		lpObj->DefenseSuccessRatePvP = ((lpObj->Level * gServerInfo.m_DKDefenseSuccessRatePvPConstA) / gServerInfo.m_DKDefenseSuccessRatePvPConstB) + (Dexterity / gServerInfo.m_DKDefenseSuccessRatePvPConstC);
	}
	else if (lpObj->Class == CLASS_FE)
	{
		lpObj->DefenseSuccessRatePvP = ((lpObj->Level * gServerInfo.m_FEDefenseSuccessRatePvPConstA) / gServerInfo.m_FEDefenseSuccessRatePvPConstB) + (Dexterity / gServerInfo.m_FEDefenseSuccessRatePvPConstC);
	}
	else if (lpObj->Class == CLASS_MG)
	{
		lpObj->DefenseSuccessRatePvP = ((lpObj->Level * gServerInfo.m_MGDefenseSuccessRatePvPConstA) / gServerInfo.m_MGDefenseSuccessRatePvPConstB) + (Dexterity / gServerInfo.m_MGDefenseSuccessRatePvPConstC);
	}
	else if (lpObj->Class == CLASS_DL)
	{
		lpObj->DefenseSuccessRatePvP = ((lpObj->Level * gServerInfo.m_DLDefenseSuccessRatePvPConstA) / gServerInfo.m_DLDefenseSuccessRatePvPConstB) + (Dexterity / gServerInfo.m_DLDefenseSuccessRatePvPConstC);
	}

	int LastItemIndex = -1;

	for (int n = 2; n <= 6; n++)
	{
		if (n == 2 && lpObj->Class == CLASS_MG)
		{
			continue;
		}

		if (lpObj->Inventory[n].IsItem() == 0 || lpObj->Inventory[n].m_IsValidItem == 0 || (LastItemIndex != -1 && (lpObj->Inventory[n].m_Index % MAX_ITEM_TYPE) != LastItemIndex))
		{
			lpObj->ArmorSetBonus = 0;
			break;
		}
		else
		{
			lpObj->ArmorSetBonus = 1;
			LastItemIndex = lpObj->Inventory[n].m_Index % MAX_ITEM_TYPE;
		}
	}

	int Level11Count = 0;
	int Level10Count = 0;
	int Level12Count = 0;
	int Level13Count = 0;
	int Level14Count = 0;
	int Level15Count = 0;

	if (lpObj->ArmorSetBonus != 0)
	{
		for (int n = 2; n <= 6; n++)
		{
			if (n == 2 && lpObj->Class == CLASS_MG)
			{
				Level15Count++;
				continue;
			}
			
			if (lpObj->Inventory[n].m_Level == 10)
			{
				Level10Count++;
			}
			else if (lpObj->Inventory[n].m_Level == 11)
			{
				Level11Count++;
			}
			else if (lpObj->Inventory[n].m_Level == 12)
			{
				Level12Count++;
			}
			else if (lpObj->Inventory[n].m_Level == 13)
			{
				Level13Count++;
			}
			else if (lpObj->Inventory[n].m_Level == 14)
			{
				Level14Count++;
			}
			else if (lpObj->Inventory[n].m_Level == 15)
			{
				Level15Count++;
			}
		}

		lpObj->DefenseSuccessRate += (lpObj->DefenseSuccessRate * 10) / 100;
	}

	if (lpObj->Class == CLASS_DW)
	{
		lpObj->Defense = Dexterity / gServerInfo.m_DWDefenseConstA;
	}
	else if (lpObj->Class == CLASS_DK)
	{
		lpObj->Defense = Dexterity / gServerInfo.m_DKDefenseConstA;
	}
	else if (lpObj->Class == CLASS_FE)
	{
		lpObj->Defense = Dexterity / gServerInfo.m_FEDefenseConstA;
	}
	else if (lpObj->Class == CLASS_MG)
	{
		lpObj->Defense = Dexterity / gServerInfo.m_MGDefenseConstA;
	}
	else if (lpObj->Class == CLASS_DL)
	{
		lpObj->Defense = Dexterity / gServerInfo.m_DLDefenseConstA;
	}

	lpObj->Defense += lpObj->Inventory[1].GetDefense();

	lpObj->Defense += lpObj->Inventory[2].GetDefense();

	lpObj->Defense += lpObj->Inventory[3].GetDefense();

	lpObj->Defense += lpObj->Inventory[4].GetDefense();

	lpObj->Defense += lpObj->Inventory[5].GetDefense();

	lpObj->Defense += lpObj->Inventory[6].GetDefense();

	lpObj->Defense += lpObj->Inventory[7].GetDefense();

	if (Helper->IsItem() != 0 && Helper->m_IsValidItem != 0 && Helper->m_Durability != 0)
	{
		if (Helper->m_Index == GET_ITEM(13, 4))
		{
			lpObj->Defense += (Dexterity / 20) + (Helper->m_PetItemLevel * 2) + 5;
		}
	}

	if (lpObj->ArmorSetBonus != 0 && (Level15Count + Level14Count + Level13Count + Level12Count + Level11Count + Level10Count) >= 5)
	{
		if (Level15Count == 5)
		{
			lpObj->Defense += (lpObj->Defense * 30) / 100;
		}
		else if (Level14Count == 5 || (Level14Count + Level15Count) == 5)
		{
			lpObj->Defense += (lpObj->Defense * 25) / 100;
		}
		else if (Level13Count == 5 || (Level13Count + Level14Count + Level15Count) == 5)
		{
			lpObj->Defense += (lpObj->Defense * 20) / 100;
		}
		else if (Level12Count == 5 || (Level12Count + Level13Count + Level14Count + Level15Count) == 5)
		{
			lpObj->Defense += (lpObj->Defense * 15) / 100;
		}
		else if (Level11Count == 5 || (Level11Count + Level12Count + Level13Count + Level14Count + Level15Count) == 5)
		{
			lpObj->Defense += (lpObj->Defense * 10) / 100;
		}
		else if (Level10Count == 5 || (Level10Count + Level11Count + Level12Count + Level13Count + Level14Count + Level15Count) == 5)
		{
			lpObj->Defense += (lpObj->Defense * 5) / 100;
		}
	}

	gDarkSpirit[lpObj->Index].Set(lpObj->Index, &lpObj->Inventory[1]);

	lpObj->Resistance[0] = GET_MAX_RESISTANCE(Amulet->m_Resistance[0], Ring1->m_Resistance[0], Ring2->m_Resistance[0]);
	lpObj->Resistance[1] = GET_MAX_RESISTANCE(Amulet->m_Resistance[1], Ring1->m_Resistance[1], Ring2->m_Resistance[1]);
	lpObj->Resistance[2] = GET_MAX_RESISTANCE(Amulet->m_Resistance[2], Ring1->m_Resistance[2], Ring2->m_Resistance[2]);
	lpObj->Resistance[3] = GET_MAX_RESISTANCE(Amulet->m_Resistance[3], Ring1->m_Resistance[3], Ring2->m_Resistance[3]);
	lpObj->Resistance[4] = GET_MAX_RESISTANCE(Amulet->m_Resistance[4], Ring1->m_Resistance[4], Ring2->m_Resistance[4]);
	lpObj->Resistance[5] = GET_MAX_RESISTANCE(Amulet->m_Resistance[5], Ring1->m_Resistance[5], Ring2->m_Resistance[5]);
	lpObj->Resistance[6] = GET_MAX_RESISTANCE(Amulet->m_Resistance[6], Ring1->m_Resistance[6], Ring2->m_Resistance[6]);

	gObjCalcExperience(lpObj);
	gItemOption.CalcItemCommonOption(lpObj, 0);
	gSetItemOption.CalcSetItemOption(lpObj, 0);

	if (Right->m_Index >= GET_ITEM(4, 0) && Right->m_Index < GET_ITEM(5, 0) && Right->m_Index != GET_ITEM(4, 15) && Right->m_Slot == 0)
	{
		if (Left->m_Index == GET_ITEM(4, 7) && Left->m_Level > 0)
		{
			lpObj->PhysiDamageMinRight += ((lpObj->PhysiDamageMinRight * ((Left->m_Level * 2) + 1)) / 100) + 1;
			lpObj->PhysiDamageMaxRight += ((lpObj->PhysiDamageMinRight * ((Left->m_Level * 2) + 1)) / 100) + 1;
		}
	}
	else if (Left->m_Index >= GET_ITEM(4, 0) && Left->m_Index < GET_ITEM(5, 0) && Left->m_Index != GET_ITEM(4, 7) && Left->m_Slot == 1)
	{
		if (Right->m_Index == GET_ITEM(4, 15) && Right->m_Level > 0)
		{
			lpObj->PhysiDamageMinLeft += ((lpObj->PhysiDamageMinLeft * ((Right->m_Level * 2) + 1)) / 100) + 1;
			lpObj->PhysiDamageMaxLeft += ((lpObj->PhysiDamageMinLeft * ((Right->m_Level * 2) + 1)) / 100) + 1;
		}
	}

	if (lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL)
	{
		if (Right->IsItem() != 0 && Left->IsItem() != 0)
		{
			if (Right->m_Index >= GET_ITEM(0, 0) && Right->m_Index < GET_ITEM(4, 0) && Left->m_Index >= GET_ITEM(0, 0) && Left->m_Index < GET_ITEM(4, 0))
			{
				lpObj->PhysiDamageMinRight = (lpObj->PhysiDamageMinRight * 55) / 100;
				lpObj->PhysiDamageMaxRight = (lpObj->PhysiDamageMaxRight * 55) / 100;
				lpObj->PhysiDamageMinLeft = (lpObj->PhysiDamageMinLeft * 55) / 100;
				lpObj->PhysiDamageMaxLeft = (lpObj->PhysiDamageMaxLeft * 55) / 100;
			}
		}
	}

	lpObj->MaxLife = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].MaxLife;
	lpObj->MaxMana = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].MaxMana;

	lpObj->MaxLife += gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].LevelLife * (lpObj->Level - 1);
	lpObj->MaxMana += gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].LevelMana * (lpObj->Level - 1);

	lpObj->MaxLife += ((lpObj->Vitality - gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality) + lpObj->AddVitality) * lpObj->VitalityToLife;
	lpObj->MaxMana += ((lpObj->Energy - gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy) + lpObj->AddEnergy) * lpObj->EnergyToMana;

	this->CharacterCalcBP(lpObj);

	lpObj->AddLife += lpObj->EffectOption.AddMaxHP;
	lpObj->AddLife += (int)(lpObj->MaxLife * lpObj->EffectOption.MulMaxHP) / 100;
	lpObj->AddLife -= (int)(lpObj->MaxLife * lpObj->EffectOption.DivMaxHP) / 100;

	lpObj->AddMana += lpObj->EffectOption.AddMaxMP;
	lpObj->AddMana += (int)(lpObj->MaxMana * lpObj->EffectOption.MulMaxMP) / 100;
	lpObj->AddMana -= (int)(lpObj->MaxMana * lpObj->EffectOption.DivMaxMP) / 100;

	lpObj->AddBP += lpObj->EffectOption.AddMaxBP;
	lpObj->AddBP += (int)(lpObj->MaxBP * lpObj->EffectOption.MulMaxBP) / 100;
	lpObj->AddBP -= (int)(lpObj->MaxBP * lpObj->EffectOption.DivMaxBP) / 100;

	lpObj->Life = ((lpObj->MaxLife + lpObj->AddLife) * TotalHP) / 100;
	lpObj->Life = ((lpObj->Life > (lpObj->MaxLife + lpObj->AddLife)) ? (lpObj->MaxLife + lpObj->AddLife) : lpObj->Life);

	lpObj->Mana = ((lpObj->MaxMana + lpObj->AddMana) * TotalMP) / 100;
	lpObj->Mana = ((lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana)) ? (lpObj->MaxMana + lpObj->AddMana) : lpObj->Mana);

	lpObj->BP = (int)(((lpObj->MaxBP + lpObj->AddBP) * TotalBP) / 100);
	lpObj->BP = (int)(((lpObj->BP > (lpObj->MaxBP + lpObj->AddBP)) ? (lpObj->MaxBP + lpObj->AddBP) : lpObj->BP));

	GCLifeSend(aIndex, 0xFE, (int)(lpObj->MaxLife + lpObj->AddLife));
	GCLifeSend(aIndex, 0xFF, (int)lpObj->Life);

	GCManaSend(aIndex, 0xFE, (int)(lpObj->MaxMana + lpObj->AddMana), (lpObj->MaxBP + lpObj->AddBP));
	GCManaSend(aIndex, 0xFF, (int)lpObj->Mana, lpObj->BP);
}

bool CObjectManager::CharacterInfoSet(BYTE* aRecv, int aIndex) // OK
{
	SDHP_CHARACTER_INFO_RECV* lpMsg = (SDHP_CHARACTER_INFO_RECV*)aRecv;

	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected == OBJECT_OFFLINE || lpObj->Connected == OBJECT_CONNECTED)
	{
		return 0;
	}

	if (lpMsg->Class != DB_CLASS_DW && lpMsg->Class != DB_CLASS_SM && lpMsg->Class != DB_CLASS_DK && lpMsg->Class != DB_CLASS_BK && lpMsg->Class != DB_CLASS_FE && lpMsg->Class != DB_CLASS_ME && lpMsg->Class != DB_CLASS_MG && lpMsg->Class != DB_CLASS_DL)
	{
		return 0;
	}

	gObjCharZeroSet(lpObj->Index);

	gChaosBox.ChaosBoxInit(lpObj);

	lpObj->PosNum = -1;
	lpObj->Level = lpMsg->Level;
	lpObj->LevelUpPoint = lpMsg->LevelUpPoint;
	lpObj->DBClass = lpMsg->Class;
	lpObj->Class = lpMsg->Class / 16;
	lpObj->ChangeUp = lpMsg->Class % 16;
	lpObj->Reset = lpMsg->Reset;
	lpObj->MasterReset = lpMsg->MasterReset;

	memcpy(lpObj->Name, lpMsg->name, sizeof(lpObj->Name));

	lpObj->Map = lpMsg->Map;
	lpObj->X = lpMsg->X;
	lpObj->Y = lpMsg->Y;
	lpObj->StartX = lpMsg->X;
	lpObj->StartY = lpMsg->Y;

	bool result = 0;

	int gate, map, x, y, dir, level;

	if (MAP_RANGE(lpObj->Map) == 0)
	{
		lpObj->Map = MAP_LORENCIA;
		gMap[lpObj->Map].GetMapPos(lpObj->Map, &lpObj->X, &lpObj->Y);
	}

	if (lpObj->Level < 6 && lpObj->Class == CLASS_DW && gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Level < 6 && lpObj->Class == CLASS_DK && gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Level < 6 && lpObj->Class == CLASS_FE && gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0)
	{
		result = gGate.GetGate(27, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Level < 6 && lpObj->Class == CLASS_MG && gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Level < 6 && lpObj->Class == CLASS_DL && gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (DS_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(27, &gate, &map, &x, &y, &dir, &level);
	}
	else if (BC_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(22, &gate, &map, &x, &y, &dir, &level);
	}
	else if (CC_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(17, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_CASTLE_SIEGE && gCastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		result = gGate.GetGate(100, &gate, &map, &x, &y, &dir, &level);
	}
	else if (lpObj->Map == MAP_LAND_OF_TRIALS)
	{
		result = gGate.GetGate(106, &gate, &map, &x, &y, &dir, &level);
	}

	if (result != 0)
	{
		lpObj->Map = map;
		lpObj->X = x;
		lpObj->Y = y;
		lpObj->Dir = dir;
	}

	if (lpObj->MapServerMoveRequest != 0)
	{
		if (gMapServerManager.CheckMapServerMove(aIndex, lpObj->DestMap, lpObj->LastServerCode) == gServerInfo.m_ServerCode)
		{
			lpObj->Map = (BYTE)lpObj->DestMap;
			lpObj->X = lpObj->DestX;
			lpObj->Y = lpObj->DestY;
			lpObj->StartX = lpObj->DestX;
			lpObj->StartY = lpObj->DestY;
		}
		else
		{
			gObjDel(aIndex);
			return 0;
		}
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 4) != 0 || gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 8) != 0)
	{
		gMap[lpObj->Map].GetMapPos(lpObj->Map, &lpObj->X, &lpObj->Y);
	}

	gMap[lpObj->Map].SetStandAttr(lpObj->X, lpObj->Y);

	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;
	lpObj->Experience = lpMsg->Experience;
	lpObj->Strength = lpMsg->Strength;
	lpObj->Dexterity = lpMsg->Dexterity;
	lpObj->Vitality = lpMsg->Vitality;
	lpObj->Energy = lpMsg->Energy;
	lpObj->Leadership = lpMsg->Leadership;
	lpObj->Life = (float)lpMsg->Life;
	lpObj->MaxLife = (float)lpMsg->MaxLife;
	lpObj->Mana = (float)lpMsg->Mana;
	lpObj->MaxMana = (float)lpMsg->MaxMana;
	lpObj->BP = lpMsg->BP;
	lpObj->MaxBP = lpMsg->MaxBP;
	lpObj->Money = lpMsg->Money;
	lpObj->FruitAddPoint = lpMsg->FruitAddPoint;
	lpObj->FruitSubPoint = lpMsg->FruitSubPoint;
	lpObj->VitalityToLife = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].VitalityToLife;
	lpObj->EnergyToMana = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].EnergyToMana;
	lpObj->PKCount = (char)lpMsg->PKCount;
	lpObj->PKLevel = (char)lpMsg->PKLevel;
	lpObj->PKTime = lpMsg->PKTime;
	lpObj->MaxRegenTime = 4000;
	lpObj->MoveSpeed = 400;

	if (lpObj->PKLevel == 3)
	{
		lpObj->PKCount = 0;
		lpObj->PKTime = 0;
	}

	memcpy(lpObj->Quest, lpMsg->Quest, sizeof(lpObj->Quest));

	gObjSetInventory1Pointer(lpObj);

	lpObj->GuildNumber = 0;
	lpObj->Guild = 0;

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		CSkill skill;

		lpObj->Skill[n].Clear();

		if (gSkillManager.ConvertSkillByte(&skill, lpMsg->Skill[n]) == 0)
		{
			continue;
		}

		gSkillManager.AddSkill(lpObj, skill.m_skill, skill.m_level);
	}

	if (lpObj->Class == CLASS_DW)
	{
		gSkillManager.AddSkill(lpObj, SKILL_ENERGY_BALL, 0);
		gSkillManager.AddSkill(lpObj, SKILL_MANA_GLAIVE, 0);
	}
	else if (lpObj->Class == CLASS_DK)
	{
		gSkillManager.AddSkill(lpObj, SKILL_CRESCENT_MOON_SLASH, 0);
	}
	else if (lpObj->Class == CLASS_FE)
	{
		gSkillManager.AddSkill(lpObj, SKILL_STAR_FALL, 0);
	}
	else if (lpObj->Class == CLASS_MG)
	{
		gSkillManager.AddSkill(lpObj, SKILL_SPIRAL_SLASH, 0);
		gSkillManager.AddSkill(lpObj, SKILL_MANA_RAYS, 0);
	}
	else if (lpObj->Class == CLASS_DL)
	{
		gSkillManager.AddSkill(lpObj, SKILL_FORCE, 0);
		gSkillManager.AddSkill(lpObj, SKILL_FIRE_BLAST, 0);
	}

	gSkillManager.AddSkill(lpObj, SKILL_STERN, 0);
	gSkillManager.AddSkill(lpObj, SKILL_REMOVE_STERN, 0);
	gSkillManager.AddSkill(lpObj, SKILL_GREATER_MANA, 0);
	gSkillManager.AddSkill(lpObj, SKILL_INVISIBILITY, 0);
	gSkillManager.AddSkill(lpObj, SKILL_REMOVE_INVISIBILITY, 0);
	gSkillManager.AddSkill(lpObj, SKILL_REMOVE_ALL_EFFECT, 0);

	memcpy(&lpObj->SkillOptionData, &lpMsg->SkillOptionData, sizeof(lpObj->SkillOptionData));

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		CItem item;

		lpObj->Inventory[n].Clear();

		if (gItemManager.ConvertItemByte(&item, lpMsg->Inventory[n]) == 0)
		{
			continue;
		}

		gItemManager.InventoryAddItem(aIndex, item, n);
	}

	this->CharacterMakePreviewCharSet(aIndex);

	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect effect;

		lpObj->Effect[n].Clear();

		if (gEffectManager.ConvertEffectByte(&effect, lpMsg->Effect[n]) == 0)
		{
			continue;
		}

		gEffectManager.AddEffect(lpObj, 1, effect.m_index, effect.m_count, effect.m_value[0], effect.m_value[1], effect.m_value[2], effect.m_value[3]);
	}

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if (lpObj->Skill[n].IsSkill() != 0)
		{
			if (gSkillManager.CheckSkillRequireWeapon(lpObj, lpObj->Skill[n].m_skill) == 0)
			{
				gSkillManager.DelSkill(lpObj, lpObj->Skill[n].m_skill);
			}
		}
	}

	lpObj->Live = 1;
	lpObj->Type = OBJECT_USER;
	lpObj->State = OBJECT_CREATE;
	lpObj->Connected = OBJECT_ONLINE;
	lpObj->TargetNumber = -1;

	if (lpObj->Life == 0)
	{
		lpObj->Live = 1;
		lpObj->State = OBJECT_DYING;
		lpObj->DieRegen = 1;
		lpObj->RegenTime = GetTickCount();
	}

	lpObj->Authority = 1;

	if ((lpMsg->CtlCode & 8) != 0)
	{
		lpObj->Authority = 2;
	}

	if ((lpMsg->CtlCode & 32) != 0)
	{
		lpObj->Authority = 32;
		gGameMaster.SetGameMasterLevel(lpObj, 0);
		gEffectManager.AddEffect(lpObj, 0, EFFECT_GAME_MASTER, 0, 0, 0, 0, 0);
	}

	lpObj->Penalty = 0;

	if ((lpMsg->CtlCode & 2) != 0)
	{
		lpObj->Penalty = 4;
	}

	gObjAuthorityCodeSet(lpObj);

	GCWeatherSend(lpObj->Index, (gMap[lpObj->Map].GetWeather() / 16));

	GCMainCheckSend(aIndex);

	gDarkSpirit[aIndex].SetMode(DARK_SPIRIT_MODE_NORMAL, -1);

	lpObj->HPAutoRecuperationTime = GetTickCount();
	lpObj->MPAutoRecuperationTime = GetTickCount();
	lpObj->BPAutoRecuperationTime = GetTickCount();
	lpObj->CharSaveTime = GetTickCount();
	return 1;
	}

void CObjectManager::CharacterLifeCheck(LPOBJ lpObj, LPOBJ lpTarget, int damage, int DamageType, int flag, int type, int skill) // OK
{
	if (lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	if (KALIMA_ATTRIBUTE_RANGE(lpObj->Attribute) != 0 || KALIMA_ATTRIBUTE_RANGE(lpTarget->Attribute) != 0)
	{
		return;
	}

	int SummonIndex = lpObj->Index;

	if (lpObj->Type == OBJECT_MONSTER && OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		SummonIndex = lpObj->SummonIndex;
	}

	if (gObj[SummonIndex].Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER && damage > 0)
	{
		gObjMonsterSetHitDamage(lpTarget, SummonIndex, damage);
	}

	if (lpTarget->Type == OBJECT_MONSTER && OBJECT_RANGE(lpTarget->SummonIndex) != 0 && damage > 0)
	{
		GCSummonLifeSend(lpTarget->SummonIndex, (int)lpTarget->Life, (int)lpTarget->MaxLife);
	}

	if (lpTarget->Life <= 0 && lpTarget->Live != 0)
	{
		if (gObj[SummonIndex].Type == OBJECT_MONSTER)
		{
			lpTarget->KillerType = 1;
			gObjAddMsgSend(&gObj[SummonIndex], 3, lpTarget->Index, 0);
		}

		if (gObj[SummonIndex].Type == OBJECT_USER)
		{
			gDarkSpirit[SummonIndex].ResetTarget(lpTarget->Index);
		}

		if (gObj[SummonIndex].Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
		{
			gObjSetKillCount(SummonIndex, 1);
		}

		if (lpTarget->Type == OBJECT_MONSTER)
		{
			if (lpTarget->Class == 275 || lpTarget->Class == 295 || lpTarget->Map == MAP_CRYWOLF)
			{
				gObjAddMsgSendDelay(lpTarget, 1, SummonIndex, 500, 0);
			}
			else
			{
				gObjAddMsgSendDelay(lpTarget, 1, SummonIndex, 500, 0);
			}

			gQuestObjective.MonsterKill(lpTarget);
			gInvasionManager.MonsterDieProc(lpTarget, &gObj[SummonIndex]);

			if (BC_MAP_RANGE(lpTarget->Map) != 0)
			{
				gBloodCastle.MonsterDieProc(lpTarget, &gObj[SummonIndex]);
			}

			if (CC_MAP_RANGE(lpTarget->Map) != 0)
			{
				gChaosCastle.MonsterDieProc(lpTarget, &gObj[SummonIndex]);
			}

			if (DS_MAP_RANGE(lpTarget->Map) != 0)
			{
				gDevilSquare.MonsterDieProc(lpTarget, &gObj[SummonIndex]);
			}

			if (lpTarget->Map == MAP_CASTLE_SIEGE)
			{
				gCastleDeep.MonsterDieProc(lpTarget, &gObj[SummonIndex]);
			}

		#if(GAMESERVER_TYPE==1)

			if (lpTarget->Map == MAP_CRYWOLF)
			{
				gCrywolf.CrywolfMonsterDieProc(lpTarget, &gObj[SummonIndex]);
			}

		#endif

			if (gObj[SummonIndex].Type == OBJECT_USER)
			{
				gObjPKDownCheckTime(&gObj[SummonIndex], lpTarget->Level);
				gObjAddMsgSendDelay(&gObj[SummonIndex], 3, lpTarget->Index, 2000, 0);
			}
		}

		if (lpTarget->Type == OBJECT_USER)
		{
			gObjUserDie(lpTarget, &gObj[SummonIndex]);

			if (lpTarget->AttackerKilled == 0)
			{
				gObjPlayerKiller(&gObj[SummonIndex], lpTarget);
			}

			lpTarget->AttackerKilled = 0;

			if (gObjGuildWarCheck(&gObj[SummonIndex], lpTarget) == 0)
			{
				lpTarget->KillerType = 0;
			}
			else
			{
				lpTarget->KillerType = 2;
			}

			gDuel.CheckDuelScore(&gObj[SummonIndex], lpTarget);

			if (OBJECT_RANGE(lpTarget->SummonIndex) != 0)
			{
				gObjSummonKill(lpTarget->Index);
			}
		}

		lpTarget->Live = 0;
		lpTarget->State = OBJECT_DYING;
		lpTarget->RegenTime = GetTickCount();
		lpTarget->DieRegen = 1;
		lpTarget->PathCount = 0;
		lpTarget->Teleport = ((lpTarget->Teleport == 1) ? 0 : lpTarget->Teleport);

		GCUserDieSend(lpTarget, lpTarget->Index, skill, lpObj->Index);

		if (lpTarget->Type == OBJECT_MONSTER)
		{
			if (OBJECT_RANGE(lpTarget->SummonIndex) != 0)
			{
				return;
			}

			if (lpTarget->Class == 275 || lpTarget->Class == 277 || lpTarget->Class == 283 || lpTarget->Class == 286 || lpTarget->Class == 287 || lpTarget->Class == 288 || lpTarget->Class == 295)
			{
				return;
			}
		}

		if (gObj[SummonIndex].Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
		{
			if (gParty.IsParty(gObj[SummonIndex].PartyNumber) == 0)
			{
				this->CharacterCalcExperienceSplit(&gObj[SummonIndex], lpTarget, damage, flag);
			}
			else
			{
				this->CharacterCalcExperienceParty(&gObj[SummonIndex], lpTarget, damage, flag);
			}
		}
}
	else
	{
		if (DamageType == 0)
		{
			GCDamageSend(SummonIndex, lpTarget->Index, flag, damage, type);
		}
		else if (DamageType == 1)
		{
			GCMonsterDamageSend(lpTarget->Index, damage);
		}
		else if (DamageType == 2)
		{
			GCDamageSend(SummonIndex, lpTarget->Index, flag, damage, 5);
		}
		else if (DamageType == 3)
		{
			GCDamageSend(SummonIndex, lpTarget->Index, flag, damage, 6);
		}
	}
}
