#include "stdafx.h"
#include "Monster.h"
#include "Attack.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "CriticalSection.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "ItemBagManager.h"
#include "ItemDrop.h"
#include "ItemOptionRate.h"
#include "Kalima.h"
#include "LifeStone.h"
#include "Map.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "MemoryAllocator.h"
#include "Mercenary.h"
#include "MonsterAIUtil.h"
#include "MonsterManager.h"
#include "MonsterSetBase.h"
#include "MonsterSkillManager.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "ServerInfo.h"
#include "SetItemType.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "Util.h"
#include "Viewport.h"

void gObjMonsterDieGiveItem(LPOBJ lpObj, LPOBJ lpTarget) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if (OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if (OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		return;
	}

	if (lpObj->Class == 247 || lpObj->Class == 249 || lpObj->Class == 277 || lpObj->Class == 283 || lpObj->Class == 288)
	{
		return;
	}

	if (gItemBagManager.DropItemByMonsterClass(lpObj->Class, lpTarget, lpObj->Map, lpObj->X, lpObj->Y) != 0)
	{
		return;
	}

	if (gQuestObjective.MonsterItemDrop(lpObj) != 0)
	{
		return;
	}

	if (gItemDrop.DropItem(lpObj, lpTarget) != 0)
	{
		return;
	}

	if (CC_MAP_RANGE(lpObj->Map) != 0)
	{
		return;
	}

	int DropIndex = -1;

	int DropLevel = lpObj->Level;

	int ExcItemDrop = 0;

	int ItemDropRate = gServerInfo.m_ItemDropRate[lpTarget->AccountLevel];

	ItemDropRate = (ItemDropRate * (lpTarget->ItemDropRate + lpTarget->EffectOption.AddItemDropRate)) / 100;

	ItemDropRate = (ItemDropRate * gMapManager.GetMapItemDropRate(lpObj->Map)) / 100;

	ItemDropRate = (ItemDropRate * gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_ITEM_DROP_RATE, 100, -1, -1, lpObj->Class, lpObj->Level)) / 100;

	int ExcItemDropRate = gMapManager.GetMapExcItemDropRate(lpObj->Map);

	ExcItemDropRate = gBonusManager.GetBonusValue(lpTarget, BONUS_INDEX_EXC_ITEM_DROP_RATE, ExcItemDropRate, -1, -1, lpObj->Class, lpObj->Level);

	int SetItemDropRate = gMapManager.GetMapSetItemDropRate(lpObj->Map);

	SetItemDropRate = gBonusManager.GetBonusValue(lpTarget, BONUS_INDEX_SET_ITEM_DROP_RATE, SetItemDropRate, -1, -1, lpObj->Class, lpObj->Level);

	if (lpObj->Level >= 25 && (GetLargeRand() % 1000000) < SetItemDropRate)
	{
		gSetItemType.MakeRandomSetItem(lpTarget->Index, lpObj->Map, lpObj->X, lpObj->Y);
		return;
	}

	if (lpObj->Level >= 25 && (GetLargeRand() % 1000000) < ExcItemDropRate)
	{
		DropLevel -= 25;
		ExcItemDrop = 1;
	}

	if ((DropIndex = gMonsterManager.GetMonsterItem(DropLevel, ExcItemDrop, gMapManager.GetMapSocketItemDrop(lpObj->Map))) >= GET_ITEM(0, 0) && (GetLargeRand() % ((lpObj->ItemRate == 0) ? 1 : lpObj->ItemRate)) < ItemDropRate)
	{
		WORD ItemIndex = DropIndex;
		BYTE ItemLevel = 0;
		BYTE ItemOption1 = 0;
		BYTE ItemOption2 = 0;
		BYTE ItemOption3 = 0;
		BYTE ItemNewOption = 0;
		BYTE ItemSetOption = 0;

		gItemOptionRate.GetItemOption0(((ExcItemDrop == 0) ? 0 : 1), &ItemLevel);
		gItemOptionRate.GetItemOption1(((ExcItemDrop == 0) ? 0 : 1), &ItemOption1);
		gItemOptionRate.GetItemOption2(((ExcItemDrop == 0) ? 0 : 1), &ItemOption2);
		gItemOptionRate.GetItemOption3(((ExcItemDrop == 0) ? 0 : 1), &ItemOption3);
		gItemOptionRate.GetItemOption4(((ExcItemDrop == 0) ? 0 : 1), &ItemNewOption);
		gItemOptionRate.GetItemOption5(((ExcItemDrop == 0) ? 0 : 1), &ItemSetOption);

		ItemLevel = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemLevel);
		ItemOption1 = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemOption1);
		ItemOption2 = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemOption2);
		ItemNewOption = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemNewOption);

		gItemOptionRate.MakeNewOption(ItemIndex, ItemNewOption, &ItemNewOption);
		gItemOptionRate.MakeSetOption(ItemIndex, ItemSetOption, &ItemSetOption);

		GDCreateItemSend(lpTarget->Index, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, ItemIndex, ItemLevel, 0, ItemOption1, ItemOption2, ItemOption3, lpTarget->Index, ItemNewOption, ItemSetOption, 0, 0);
	}
	else if (lpObj->Money > 0 && (GetLargeRand() % ((lpObj->MoneyRate == 0) ? 1 : lpObj->MoneyRate)) < 10)
	{
		__int64 money = ((__int64)lpObj->Money * gServerInfo.m_MoneyRate) / 100;
		money += (money * gServerInfo.m_MoneyAmountDropRate[lpTarget->AccountLevel]) / 100;
		money = (money * lpTarget->MoneyAmountDropRate) / 100;

		if (money <= 0)
		{
			money = 1;
		}

		gMap[lpObj->Map].MoneyItemDrop((int)money, lpObj->X, lpObj->Y);
	}
}

bool gObjSetPosMonster(int aIndex, int number) // OK
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (number < 0 || number > MAX_MSB_MONSTER)
	{
		return 0;
	}

	MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[number];

	short x, y;

	if (gMonsterSetBase.GetPosition(number, lpInfo->Map, &x, &y) == 0)
	{
		return 0;
	}

	lpObj->PosNum = number;
	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->Dir = lpInfo->Dir;
	lpObj->Map = lpInfo->Map;
	return 1;
}

bool gObjSetMonster(int aIndex, int MonsterClass) // OK
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->ConnectTickCount = GetTickCount();
	lpObj->ShopNumber = -1;
	lpObj->TargetNumber = -1;
	lpObj->SummonIndex = -1;
	lpObj->LastAttackerID = -1;
	lpObj->Connected = OBJECT_ONLINE;
	lpObj->Live = 1;
	lpObj->State = OBJECT_CREATE;
	lpObj->DieRegen = 0;
	lpObj->Type = OBJECT_MONSTER;
	lpObj->Class = MonsterClass;

	switch (MonsterClass)
	{
		case 200:
			gBSGround[0]->m_BallIndex = aIndex;
			break;
	}

	if ((MonsterClass >= 204 && MonsterClass <= 259))
	{
		lpObj->Type = OBJECT_NPC;
		lpObj->ShopNumber = (short)gShopManager.GetShopNumber(MonsterClass);
	}

	MONSTER_INFO* lpInfo = gMonsterManager.GetInfo(MonsterClass);

	if (lpInfo == 0)
	{
		return 0;
	}

	memcpy(lpObj->Name, lpInfo->Name, sizeof(lpObj->Name));

	lpObj->Level = lpInfo->Level;
	lpObj->PhysiSpeed = lpInfo->AttackSpeed;
	lpObj->PhysiDamageMin = lpInfo->DamageMin;
	lpObj->PhysiDamageMax = lpInfo->DamageMax;
	lpObj->Defense = lpInfo->Defense;
	lpObj->MagicDefense = lpInfo->MagicDefense;
	lpObj->AttackSuccessRate = lpInfo->AttackRate;
	lpObj->DefenseSuccessRate = lpInfo->DefenseRate;
	lpObj->Life = (float)lpInfo->Life;
	lpObj->MaxLife = (float)lpInfo->Life;
	lpObj->Mana = (float)lpInfo->Mana;
	lpObj->MaxMana = (float)lpInfo->Mana;
	lpObj->MoveRange = lpInfo->MoveRange;
	lpObj->MoveSpeed = lpInfo->MoveSpeed;
	lpObj->MaxRegenTime = lpInfo->RegenTime * 1000;
	lpObj->AttackRange = lpInfo->AttackRange;
	lpObj->ViewRange = lpInfo->ViewRange;
	lpObj->Attribute = lpInfo->Attribute;
	lpObj->AttackType = lpInfo->AttackType;
	lpObj->ItemRate = lpInfo->ItemRate;
	lpObj->MoneyRate = lpInfo->MoneyRate;
	lpObj->Resistance[0] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[0]);
	lpObj->Resistance[1] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[1]);
	lpObj->Resistance[2] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[2]);
	lpObj->Resistance[3] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[3]);
	lpObj->Resistance[4] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[4]);
	lpObj->Resistance[5] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[5]);
	lpObj->Resistance[6] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[6]);
	lpObj->ScriptMaxLife = (float)lpInfo->ScriptLife;
	lpObj->BasicAI = lpInfo->AINumber;
	lpObj->CurrentAI = lpInfo->AINumber;
	lpObj->CurrentAIState = 0;
	lpObj->LastAIRunTime = 0;
	lpObj->GroupNumber = 0;
	lpObj->SubGroupNumber = 0;
	lpObj->GroupMemberGuid = -1;
	lpObj->RegenType = 0;

	lpObj->Agro.ResetAll();

	gObjSetInventory1Pointer(lpObj);

	if (lpObj->AttackType != 0)
	{
		if (lpObj->AttackType == 150)
		{
			gSkillManager.AddSkill(lpObj, SKILL_MONSTER_AREA_ATTACK, 0);
		}
		else
		{
			gSkillManager.AddSkill(lpObj, lpObj->AttackType, 0);
		}

		switch (MonsterClass)
		{
			case 66: // Cursed King
			case 73: // Drakan
			case 144:
			case 145:
			case 146:
			case 147:
			case 148:
			case 160:
			case 163:
			case 165:
			case 167:
			case 169:
			case 171:
			case 173:
			case 174: 
			case 175:
			case 176:
			case 177:
			case 178:
			case 180:
			case 182:
			case 183:
			case 184:
			case 185:
			case 186:
			case 188:
			case 190:
			case 191:
			case 192:
			case 193:
			case 194:
			case 196:
			case 260: 
			case 261:
			case 262:
			case 263:
			case 264:
			case 266:
			case 268:
			case 269:
			case 270:
			case 271:
			case 272:
			case 274:
			case 331:
			case 332:
			case 333:
			case 334:
			case 335:
			case 337:
			{
				gSkillManager.AddSkill(lpObj, SKILL_ENERGY_BALL, 0);
				break;
			}
			case 77: // Phoenix of Darkness
			case 89:
			case 95:
			case 112:
			case 118:
			case 124:
			case 130:
			case 143:
			{
				gSkillManager.AddSkill(lpObj, SKILL_LIGHTNING, 0);
				break;
			}
			case 149:
			case 179:
			case 187:
			case 195:
			case 265:
			case 273:
			case 336:
			{
				gSkillManager.AddSkill(lpObj, SKILL_POISON, 0);
				gSkillManager.AddSkill(lpObj, SKILL_ENERGY_BALL, 0);
				break;
			}
			case 161:
			case 181:
			case 189:
			case 197:
			case 267:
			case 275:
			case 338:
			{
				gSkillManager.AddSkill(lpObj, SKILL_POISON, 0);
				gSkillManager.AddSkill(lpObj, SKILL_ENERGY_BALL, 0);
				gSkillManager.AddSkill(lpObj, SKILL_FIRE_SLASH, 0);
				gSkillManager.AddSkill(lpObj, SKILL_MONSTER_SUMMON, 0);
				gSkillManager.AddSkill(lpObj, SKILL_MAGIC_DAMAGE_IMMUNITY, 0);
				gSkillManager.AddSkill(lpObj, SKILL_PHYSI_DAMAGE_IMMUNITY, 0);
				break;
			}
			case 50:
			{
				gSkillManager.AddSkill(lpObj, SKILL_EXPLOSION, 0);

				CItem item;

				item.m_Level = 0;
				item.m_Serial = 0;
				item.m_Durability = (float)gItemManager.GetItemDurability(GET_ITEM(1, 9), 0, 0, 0);
				
				gItemManager.InventoryAddItem(lpObj->Index, item, 0);
				break;
			}
		}
	}

	gMap[lpObj->Map].SetStandAttr(lpObj->X, lpObj->Y);

	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;

	return 1;
}

bool gObjMonsterRegen(LPOBJ lpObj) // OK
{
	gObjClearViewport(lpObj);

	lpObj->TargetNumber = -1;
	lpObj->LastAttackerID = -1;
	lpObj->NextActionTime = 5000;
	lpObj->ActionState.Rest = 0;
	lpObj->ActionState.Attack = 0;
	lpObj->ActionState.Move = 0;
	lpObj->ActionState.Escape = 0;
	lpObj->ActionState.Emotion = 0;
	lpObj->ActionState.EmotionCount = 0;

	if (KALIMA_ATTRIBUTE_RANGE(lpObj->Attribute) != 0)
	{
		lpObj->Live = 1;
		lpObj->DieRegen = 0;
		gObjClearViewport(lpObj);
		gObjViewportListCreate(lpObj->Index);
		gObjViewportListProtocolCreate(lpObj);
		return 0;
	}

	if (lpObj->Attribute == 62)
	{
		int px = lpObj->X;
		int py = lpObj->Y;

		if (gObjGetRandomFreeLocation(lpObj->Map, &px, &py, 5, 5, 30) == 0)
		{
			lpObj->Live = 0;
			lpObj->State = OBJECT_DYING;
			lpObj->DieRegen = 1;
			lpObj->RegenTime = GetTickCount();
			return 0;
		}

		lpObj->X = px;
		lpObj->Y = py;
	}
	else
	{
		if (gMonsterSetBase.GetPosition(lpObj->PosNum, lpObj->Map, &lpObj->X, &lpObj->Y) == 0)
		{
			lpObj->Live = 0;
			lpObj->State = OBJECT_DYING;
			lpObj->DieRegen = 1;
			lpObj->RegenTime = GetTickCount();
			return 0;
		}
	}

	lpObj->MTX = lpObj->X;
	lpObj->MTY = lpObj->Y;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->PathCur = 0;
	lpObj->PathCount = 0;
	lpObj->PathStartEnd = 0;

	gObjMonsterInitHitDamage(lpObj);

	return 1;
}

bool gObjMonsterMoveCheck(LPOBJ lpObj, int tx, int ty) // OK
{
	if (lpObj->ActionState.Emotion == 1)
	{
		return 1;
	}

	if (lpObj->PosNum == -1)
	{
		return 0;
	}

	if (gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis == 0)
	{
		return 0;
	}

	if (sqrt((pow((float)(tx - lpObj->StartX), (float)2) + pow((float)(ty - lpObj->StartY), (float)2))) > gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis)
	{
		return 0;
	}

	return 1;
}

void gObjMonsterInitHitDamage(LPOBJ lpObj) // OK
{
	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		lpObj->HitDamage[n].index = -1;
		lpObj->HitDamage[n].damage = 0;
		lpObj->HitDamage[n].time = 0;
	}

	lpObj->HitDamageCount = 0;
}

void gObjMonsterSetHitDamage(LPOBJ lpObj, int aIndex, int damage) // OK
{
	int HitDamageIndex = -1;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (OBJECT_RANGE(lpObj->HitDamage[n].index) == 0)
		{
			HitDamageIndex = n;
			continue;
		}

		if (lpObj->HitDamage[n].index == aIndex)
		{
			lpObj->HitDamage[n].damage = (((lpObj->HitDamage[n].damage + damage) > lpObj->MaxLife) ? (int)lpObj->MaxLife : (lpObj->HitDamage[n].damage + damage));
			lpObj->HitDamage[n].time = GetTickCount();
			return;
		}
	}

	if (CHECK_RANGE(HitDamageIndex, MAX_HIT_DAMAGE) != 0 || OBJECT_RANGE(HitDamageIndex = gObjMonsterDelHitDamageUser(lpObj)) != 0)
	{
		lpObj->HitDamage[HitDamageIndex].index = aIndex;

		lpObj->HitDamage[HitDamageIndex].damage = ((damage > lpObj->MaxLife) ? (int)lpObj->MaxLife : damage);

		lpObj->HitDamage[HitDamageIndex].time = GetTickCount();

		lpObj->HitDamageCount++;
	}
}

int gObjMonsterDelHitDamageUser(LPOBJ lpObj) // OK
{
	int HitDamageIndex = -1;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (OBJECT_RANGE(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		int result = 0;

		if (gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			result = 1;
		}
		else if ((GetTickCount() - lpObj->HitDamage[n].time) > 30000)
		{
			result = 1;
		}
		else if (lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			result = 1;
		}
		else if (gObjCalcDistance(lpObj, &gObj[lpObj->HitDamage[n].index]) > 20)
		{
			result = 1;
		}

		if (result != 0)
		{
			lpObj->HitDamage[n].index = -1;
			lpObj->HitDamageCount--;
			HitDamageIndex = n;
		}
	}

	return HitDamageIndex;
}

int gObjMonsterGetTopHitDamageUser(LPOBJ lpObj) // OK
{
	int TopHitDamageUser = -1;
	int TopHitDamage = 0;
	int PartyTopHitDamageUser = -1;
	int PartyTopHitDamage = 0;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		if ((GetTickCount() - lpObj->HitDamage[n].time) > 30000)
		{
			continue;
		}

		if (lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			continue;
		}

		if (gObjCalcDistance(lpObj, &gObj[lpObj->HitDamage[n].index]) > 20)
		{
			continue;
		}

		if (OBJECT_RANGE(gObj[lpObj->HitDamage[n].index].PartyNumber) == 0)
		{
			if (lpObj->HitDamage[n].damage > TopHitDamage)
			{
				TopHitDamageUser = lpObj->HitDamage[n].index;
				TopHitDamage = lpObj->HitDamage[n].damage;
			}
		}
		else
		{
			if ((PartyTopHitDamage = gObjMonsterGetTopHitDamageParty(lpObj, gObj[lpObj->HitDamage[n].index].PartyNumber, &PartyTopHitDamageUser)) > TopHitDamage)
			{
				TopHitDamageUser = PartyTopHitDamageUser;
				TopHitDamage = PartyTopHitDamage;
			}
		}
	}

	return TopHitDamageUser;
}

int gObjMonsterGetTopHitDamageParty(LPOBJ lpObj, int PartyNumber, int* TopHitDamageUser) // OK
{
	int TopHitDamage = 0;
	int TotalHitDamage = 0;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		if ((GetTickCount() - lpObj->HitDamage[n].time) > 30000)
		{
			continue;
		}

		if (lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			continue;
		}

		if (gObjCalcDistance(lpObj, &gObj[lpObj->HitDamage[n].index]) > 20)
		{
			continue;
		}

		for (int i = 0; i < MAX_PARTY_USER; i++)
		{
			if (gParty.m_PartyInfo[PartyNumber].Index[i] == lpObj->HitDamage[n].index)
			{
				if (lpObj->HitDamage[n].damage > TopHitDamage)
				{
					(*TopHitDamageUser) = lpObj->HitDamage[n].index;
					TopHitDamage = lpObj->HitDamage[n].damage;
					TotalHitDamage += lpObj->HitDamage[n].damage;
				}
				else
				{
					(*TopHitDamageUser) = (*TopHitDamageUser);
					TopHitDamage = TopHitDamage;
					TotalHitDamage += lpObj->HitDamage[n].damage;
				}
			}
		}
	}

	return TotalHitDamage;
}

int gObjAddMonster(int map) // OK
{
	static CCriticalSection critical;

	critical.lock();

	int index = -1;
	int count = gObjMonCount;

	if (gMapServerManager.CheckMapServer(map) == 0)
	{
		critical.unlock();
		return -1;
	}

	if (gMemoryAllocator.GetMemoryAllocatorFree(&index, OBJECT_START_MONSTER, MAX_OBJECT_MONSTER, 10000) != 0)
	{
		gObjAllocData(index);
		gObjCharZeroSet(index);
		gObj[index].Index = index;
		gObj[index].Connected = OBJECT_CONNECTED;
		critical.unlock();
		return index;
	}

	for (int n = OBJECT_START_MONSTER; n < MAX_OBJECT_MONSTER; n++)
	{
		if (gObj[count].Connected == OBJECT_OFFLINE)
		{
			if (gObjAllocData(count) == 0)
			{
				break;
			}

			gObjCharZeroSet(count);
			gObj[count].Index = count;
			gObj[count].Connected = OBJECT_CONNECTED;
			critical.unlock();
			return count;
		}
		else
		{
			count = (((++count) >= MAX_OBJECT_MONSTER) ? OBJECT_START_MONSTER : count);
		}
	}

	critical.unlock();
	return -1;
}

int gObjAddSummon() // OK
{
	static CCriticalSection critical;

	critical.lock();

	int index = -1;
	int count = gObjCallMonCount;

	if (gMemoryAllocator.GetMemoryAllocatorFree(&index, MAX_OBJECT_MONSTER, OBJECT_START_USER, 10000) != 0)
	{
		gObjAllocData(index);
		gObjCharZeroSet(index);
		gObj[index].Index = index;
		gObj[index].Connected = OBJECT_CONNECTED;
		critical.unlock();
		return index;
	}

	for (int n = MAX_OBJECT_MONSTER; n < OBJECT_START_USER; n++)
	{
		if (gObj[count].Connected == OBJECT_OFFLINE)
		{
			if (gObjAllocData(count) == 0)
			{
				break;
			}

			gObjCharZeroSet(count);
			gObj[count].Index = count;
			gObj[count].Connected = OBJECT_CONNECTED;
			critical.unlock();
			return count;
		}
		else
		{
			count = (((++count) >= OBJECT_START_USER) ? MAX_OBJECT_MONSTER : count);
		}
	}

	critical.unlock();
	return -1;
}

int gObjSummonSetEnemy(LPOBJ lpObj, int aIndex) // OK
{
	if (lpObj->Type != OBJECT_USER)
	{
		return -1;
	}

	LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

	if (OBJECT_RANGE(lpSummon->TargetNumber) != 0)
	{
		return -1;
	}

	if (gObjIsConnected(aIndex) == 0 || aIndex == lpObj->Index || aIndex == lpSummon->Index)
	{
		return -1;
	}

	lpSummon->TargetNumber = aIndex;
	lpSummon->ActionState.Emotion = 1;
	lpSummon->ActionState.EmotionCount = 30;
	return aIndex;
}

void gObjSummonKill(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(lpObj->SummonIndex) == 0)
	{
		return;
	}

	LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

	if (lpSummon->Type != OBJECT_MONSTER || lpSummon->SummonIndex != aIndex)
	{
		return;
	}

	if (gObjIsConnected(lpSummon->Index) != 0)
	{
		lpSummon->SummonIndex = -1;
	}

	lpObj->SummonIndex = -1;

	gObjDel(lpSummon->Index);
}

void gObjMonsterMagicAttack(LPOBJ lpObj, int skill) // OK
{
	CSkill* lpSkill = &lpObj->Skill[skill];

	if (lpSkill->IsSkill() == 0)
	{
		return;
	}

	gSkillManager.GCDurationSkillAttackSend(lpObj, lpSkill->m_skill, (BYTE)lpObj->X, (BYTE)lpObj->Y, 0);

	BYTE send[256];

	PMSG_MULTI_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE4, sizeof(pMsg));

	int size = sizeof(pMsg);

	pMsg.skill = 0;
	pMsg.x = (BYTE)lpObj->X;
	pMsg.y = (BYTE)lpObj->Y;
	pMsg.serial = 0;
	pMsg.count = 0;

	PMSG_MULTI_SKILL_ATTACK info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (gObjCalcDistance(lpObj, &gObj[lpObj->VpPlayer2[n].index]) >= 6)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpObj->VpPlayer2[n].index);
		info.index[1] = SET_NUMBERLB(lpObj->VpPlayer2[n].index);
		info.MagicKey = 0;

		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);

		if (CHECK_SKILL_ATTACK_EXTENDED_COUNT(pMsg.count) == 0)
		{
			break;
		}
	}

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	if (lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275 || lpObj->Class == 338)
	{
		gObjMonsterBeattackRecv(send, lpObj->Index);
	}
	else
	{
		gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)send, lpObj->Index, 1);
	}
}

void gObjMonsterBeattackRecv(BYTE* aRecv, int aIndex) // OK
{
	PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg = (PMSG_MULTI_SKILL_ATTACK_RECV*)aRecv;

	CSkill* lpSkill = &gObj[aIndex].Skill[lpMsg->skill];

	if (lpSkill->IsSkill() == 0)
	{
		return;
	}

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_MULTI_SKILL_ATTACK* lpInfo = (PMSG_MULTI_SKILL_ATTACK*)(((BYTE*)lpMsg) + sizeof(PMSG_MULTI_SKILL_ATTACK_RECV) + (sizeof(PMSG_MULTI_SKILL_ATTACK) * n));

		int bIndex = MAKE_NUMBERW(lpInfo->index[0], lpInfo->index[1]);

		if (OBJECT_RANGE(bIndex) == 0)
		{
			continue;
		}

		gAttack.Attack(&gObj[aIndex], &gObj[bIndex], lpSkill, 1, 1, 0, 0, 0);
	}
}

void gObjMonsterAttack(LPOBJ lpObj, LPOBJ lpTarget) // OK
{
	if (lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19, sizeof(pMsg));

		pMsg.skill = 1;
		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
	}
	else if (lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19, sizeof(pMsg));

		pMsg.skill = 1;
		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
	}
	else if (lpObj->Class == 144 || lpObj->Class == 174 || lpObj->Class == 182 || lpObj->Class == 190 || lpObj->Class == 260 || lpObj->Class == 268)
	{
		if ((GetLargeRand() % 2) == 0)
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19, sizeof(pMsg));

			pMsg.skill = 0;
			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
		}
		else
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2, sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
			pMsg.action = ACTION_ATTACK1;
			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg, lpObj->Index);
		}
	}
	else if (lpObj->Class == 145 || lpObj->Class == 146 || lpObj->Class == 147 || lpObj->Class == 148 || lpObj->Class == 160 || lpObj->Class == 175 || lpObj->Class == 176 || lpObj->Class == 177 || lpObj->Class == 178 || lpObj->Class == 180 || lpObj->Class == 183 || lpObj->Class == 184 || lpObj->Class == 185 || lpObj->Class == 186 || lpObj->Class == 188 || lpObj->Class == 191 || lpObj->Class == 192 || lpObj->Class == 193 || lpObj->Class == 194 || lpObj->Class == 196 || lpObj->Class == 261 || lpObj->Class == 262 || lpObj->Class == 263 || lpObj->Class == 264 || lpObj->Class == 266 || lpObj->Class == 269 || lpObj->Class == 270 || lpObj->Class == 271 || lpObj->Class == 272 || lpObj->Class == 274)
	{
		if ((GetLargeRand() % 2) == 0)
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19, sizeof(pMsg));

			pMsg.skill = 0;
			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
		}
		else
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2, sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
			pMsg.action = ACTION_ATTACK1;
			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg, lpObj->Index);
		}
	}
	else if (lpObj->Class == 149 || lpObj->Class == 179 || lpObj->Class == 187 || lpObj->Class == 195 || lpObj->Class == 265 || lpObj->Class == 273)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19, sizeof(pMsg));

		pMsg.skill = 1 + (GetLargeRand() % 2);
		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
	}
	else if (lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19, sizeof(pMsg));

		pMsg.skill = 1 + (GetLargeRand() % 6);
		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);

		gObjMonsterMagicAttack(lpObj, 0);
	}
	else if (lpObj->Class == 163 || lpObj->Class == 165 || lpObj->Class == 167 || lpObj->Class == 169 || lpObj->Class == 171 || lpObj->Class == 173)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19, sizeof(pMsg));

		pMsg.skill = 0;
		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
	}
	else if (lpObj->Class == 50)
	{
		if ((GetLargeRand() % 5) == 0)
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2, sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
			pMsg.action = ACTION_ATTACK1;
			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg, lpObj->Index);
		} 
		else
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19, sizeof(pMsg));

			pMsg.skill = 1;
			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
		}
	}
	else
	{
		if (lpObj->AttackType == 0 || lpObj->AttackType >= 100)
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2, sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
			pMsg.action = ACTION_ATTACK1;
			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg, lpObj->Index);
		}
		else
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19, sizeof(pMsg));

			pMsg.skill = 0;
			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);
			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);
			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
		}
	}
}

void gObjMonsterDie(LPOBJ lpObj, LPOBJ lpTarget) // OK
{
#if(GAMESERVER_TYPE==1)

	if (lpObj->CsNpcType != 0)
	{
		switch (lpObj->CsNpcType)
		{
			case 1:
				gCastleSiege.DelNPC(lpObj->Index, lpObj->Class, lpObj->CsNpcExistVal, 1);
				break;
			case 2:
				gCastleSiege.DelNPC(lpObj->Index, lpObj->Class, lpObj->CsNpcExistVal, 0);
				break;
			case 3:
				gCastleSiege.DelNPC(lpObj->Index, lpObj->Class, lpObj->CsNpcExistVal, 0);
				break;
		}

		if (lpObj->Class == 278)
		{
			gLifeStone.DeleteLifeStone(lpObj->Index);
		}

		if (lpObj->Class == 286 || lpObj->Class == 287)
		{
			gMercenary.DeleteMercenary(lpObj->Index);
		}

		gObjDel(lpObj->Index);
	}

#endif

	if (KALIMA_MAP_RANGE(lpObj->Map) != 0)
	{
		if (lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267)
		{
			gKalima.CreateNextKalimaGate(lpTarget->Index, lpObj->Map, lpObj->X, lpObj->Y);
		}
	}

	gObjMonsterDieGiveItem(lpObj, lpTarget);

	lpObj->NextActionTime = 500;

	if (OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(lpObj->SummonIndex);
	}
}

void gObjMonsterStateProc(LPOBJ lpObj, int code, int aIndex, int SubCode) // OK
{
	switch (code)
	{
		case 0:
			gObjMonsterStateProcCase0(lpObj, aIndex);
			break;
		case 1:
			gObjMonsterDie(lpObj, &gObj[aIndex]);
			break;
		case 2:
			if (gObj[aIndex].Live != 0)
			{
				gObjBackSpring(lpObj, &gObj[aIndex]);
			}
			break;
		case 3:
			lpObj->TargetNumber = -1;
			lpObj->LastAttackerID = -1;
			lpObj->NextActionTime = 1000;
			lpObj->ActionState.Attack = 0;
			lpObj->ActionState.Move = 0;
			lpObj->ActionState.Emotion = 0;
			break;
		case 4:
			lpObj->ActionState.Emotion = 3;
			lpObj->ActionState.EmotionCount = 1;
			break;
		case 6:
			if (gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(lpObj, &gObj[aIndex], 2);
			}
			break;
		case 7:
			if (gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(lpObj, &gObj[aIndex], 3);
			}
			break;
		case 55:
			if (gObj[aIndex].Live != 0)
			{
				gAttack.Attack(lpObj, &gObj[aIndex], 0, 0, 0, 0, 0, 0);
			}
			break;
		case 56:
			if (gObjCheckResistance(&gObj[aIndex], 1) == 0)
			{
				gEffectManager.AddEffect(&gObj[aIndex], 0, EFFECT_POISON, SubCode, lpObj->Index, 2, 3, 0);
			}
			break;
		case 57:
			if (gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(&gObj[aIndex], lpObj, SubCode);
			}
			break;
	}
}

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

int gObjMonsterViewportIsCharacter(LPOBJ lpObj)
{
	int tObjNum;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[n].index;

			if (tObjNum == lpObj->TargetNumber)
			{
				return n;
			}
		}
	}

	return -1;
}

BOOL gObjMonsterGetTargetPos(LPOBJ lpObj)
{
	int tpx;	// Target Player X
	int tpy;
	int mtx;	// Monster Target X
	int mty;
	int searchp = 0;
	int sn = 0;
	int searchcount = MAX_ROAD_PATH_TABLE / 2 - 1;
	BYTE attr;
	BOOL result;
	LPOBJ lpTargetObj;

	if (lpObj->MoveRange == 0 && (gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0))
	{
		return FALSE;
	}

	if (gEffectManager.CheckEffect(lpObj, EFFECT_SLEEP) != 0)
	{
		return FALSE;
	}

	if (OBJECT_RANGE(lpObj->TargetNumber) == FALSE)
	{
		return FALSE;
	}

	lpTargetObj = &gObj[lpObj->TargetNumber];

	if (lpTargetObj->Teleport != 0)
	{
		return FALSE;
	}

	int vpn = gObjMonsterViewportIsCharacter(lpObj);

	if (vpn < 0)
	{
		return FALSE;
	}

	tpx = lpTargetObj->X;
	mtx = tpx;
	tpy = lpTargetObj->Y;
	mty = tpy;
	int dis;

	if (lpObj->AttackType >= 100)
	{
		dis = lpObj->AttackRange + 2;
	}
	else
	{
		dis = lpObj->AttackRange;
	}

	if (lpObj->X < mtx)
	{
		tpx -= dis;
	}

	if (lpObj->X > mtx)
	{
		tpx += dis;
	}

	if (lpObj->Y < mty)
	{
		tpy -= dis;
	}

	if (lpObj->Y > mty)
	{
		tpy += dis;
	}

	searchp = GetPathPacketDirPos((lpTargetObj->X - tpx), (lpTargetObj->Y - tpy)) * 2;

	if (gMap[lpObj->Map].CheckStandAttr(tpx, tpy) == 0)
	{
		while (searchcount--)
		{
			mtx = lpTargetObj->X + RoadPathTable[searchp];
			mty = lpTargetObj->Y + RoadPathTable[1 + searchp];
			attr = gMap[lpObj->Map].GetAttr(mtx, mty);
			result = gObjMonsterMoveCheck(lpObj, mtx, mty);

			if (lpObj->Class == 249)
			{
				if ((attr & 2) != 2 && result == TRUE)
				{
					lpObj->MTX = mtx;
					lpObj->MTY = mty;
					return TRUE;
				}
			}
			else if ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8 && result == TRUE)
			{
				lpObj->MTX = mtx;
				lpObj->MTY = mty;
				return TRUE;
			}

			searchp += 2;

			if (searchp > MAX_ROAD_PATH_TABLE - 1)
			{
				searchp = 0;
			}
		}

		return FALSE;
	}

	attr = gMap[lpObj->Map].GetAttr(tpx, tpy);
	result = gObjMonsterMoveCheck(lpObj, mtx, mty);

	if (lpObj->Class == 249)
	{
		if ((attr & 2) != 2 && result == TRUE)
		{
			lpObj->MTX = tpx;
			lpObj->MTY = tpy;
			return TRUE;
		}
	}
	else if ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8 && result == TRUE)
	{
		lpObj->MTX = tpx;
		lpObj->MTY = tpy;
		return TRUE;
	}

	return FALSE;
}

BOOL gObjGetTargetPos(LPOBJ lpObj, int sx, int sy, int& tx, int& ty)
{
	int tpx;	// Target Player X
	int tpy;
	int mtx;	// Monster Target X
	int mty;
	int searchp = 0;
	int sn = 0;
	int searchcount = MAX_ROAD_PATH_TABLE / 2 - 1;
	BYTE attr;
	int dis;

	tpx = sx;
	mtx = tpx;
	tpy = sy;
	mty = tpy;

	if (lpObj->AttackType >= 100)
	{
		dis = lpObj->AttackRange + 2;
	}
	else
	{
		dis = lpObj->AttackRange;
	}

	if (lpObj->X < mtx)
	{
		tpx -= dis;
	}

	if (lpObj->X > mtx)
	{
		tpx += dis;
	}

	if (lpObj->Y < mty)
	{
		tpy -= dis;
	}

	if (lpObj->Y > mty)
	{
		tpy += dis;
	}

	searchp = GetPathPacketDirPos(sx - tpx, sy - tpy) * 2;

	if (gMap[lpObj->Map].CheckStandAttr(tpx, tpy) == 0)
	{
		while (searchcount--)
		{
			mtx = sx + RoadPathTable[searchp];
			mty = sy + RoadPathTable[1 + searchp];
			attr = gMap[lpObj->Map].GetAttr(mtx, mty);

			if ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8)
			{
				tx = mtx;
				ty = mty;
				return TRUE;
			}

			searchp += 2;

			if (searchp > MAX_ROAD_PATH_TABLE - 1)
			{
				searchp = 0;
			}
		}

		return FALSE;
	}

	attr = gMap[lpObj->Map].GetAttr(tpx, tpy);

	if (lpObj->Attribute >= 100)
	{
		if ((attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8)
		{
			tx = tpx;
			ty = tpy;
			return TRUE;
		}
	}
	else if ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8)
	{
		tx = tpx;
		ty = tpy;
		return TRUE;
	}

	return FALSE;
}

int gObjMonsterSearchEnemy(LPOBJ lpObj, BYTE objtype)
{
	int n;
	int tx;
	int ty;
	int dis;
	int mindis = lpObj->ViewRange;
	int searchtarget = -1;
	int tObjNum;
	int count = 3;
	int t1 = objtype;
	int t2 = objtype;

	for (n = 0; n < MAX_VIEWPORT; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].index;

		if (tObjNum >= 0)
		{
			if ((gObj[tObjNum].Type == t1 || gObj[tObjNum].Type == t2) && (gObj[tObjNum].Live != FALSE))
			{
				if (((gObj[tObjNum].Class >= 100 && gObj[tObjNum].Class < 110) || gObj[tObjNum].Class == 523) || (gObj[tObjNum].Type == OBJECT_MONSTER && gObj[tObjNum].SummonIndex >= 0)) //TRAPS
				{

				}
				else if ((gObj[tObjNum].Authority & 2) != 2 && (gObj[tObjNum].Authority & 32) != 32 && gObj[tObjNum].Teleport == 0)
				{
					tx = lpObj->X - gObj[tObjNum].X;
					ty = lpObj->Y - gObj[tObjNum].Y;
					dis = (int)sqrt((float)tx * (float)tx + (float)ty * (float)ty);

					if (dis < mindis)
					{
						searchtarget = tObjNum;
						mindis = dis;
					}
				}
			}
		}
	}

	return searchtarget;
}

int gObjGuardSearchEnemy(LPOBJ lpObj)
{
	int n;
	int tx;
	int ty;
	int dis;
	int mindis = lpObj->ViewRange;
	int searchtarget = -1;
	int tObjNum;
	BYTE attr;

	for (n = 0; n < MAX_VIEWPORT; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].index;

		if (tObjNum >= 0)
		{
			BOOL bEnableAttack = FALSE; //Season 2.5 add-on

			if (gObj[tObjNum].PKLevel > 4)
			{
				bEnableAttack = TRUE;
			}

			if (gObj[tObjNum].Type == OBJECT_USER)
			{
				if (bEnableAttack == TRUE) //Season 2.5 add-on
				{
					attr = gMap[gObj[tObjNum].Map].GetAttr(gObj[tObjNum].X, gObj[tObjNum].Y);

					if ((attr & 1) != 1)
					{
						tx = lpObj->X - gObj[tObjNum].X;
						ty = lpObj->Y - gObj[tObjNum].Y;
						dis = (int)sqrt((float)tx * (float)tx + (float)ty * (float)ty);
						//lpObj->VpPlayer2[n].dis = dis;

						if (dis < mindis)
						{
							searchtarget = tObjNum;
							mindis = dis;
						}
					}
				}
			}
		}
	}

	return searchtarget;
}

void gObjMonsterProcess(LPOBJ lpObj)
{
	gObjectManager.ObjectMsgProc(lpObj);

	if (lpObj->Live == FALSE)
	{
		return;
	}

	if (lpObj->State != OBJECT_PLAYING)
	{
		return;
	}

	if ((GetTickCount() - lpObj->CurActionTime) < (lpObj->NextActionTime + lpObj->DelayActionTime))
	{
		return;
	}

	lpObj->CurActionTime = GetTickCount();

	if (BC_MAP_RANGE(lpObj->Map) != FALSE)
	{
		if (lpObj->Class == 131 || ((lpObj->Class - 132 < 0) ? FALSE : (lpObj->Class - 132 > 2) ? FALSE : TRUE) != FALSE)
		{
			return;
		}
	}

	if (KALIMA_ATTRIBUTE_RANGE(lpObj->Attribute) != FALSE)
	{
		if (lpObj->Attribute == 58)
		{
			gKalima.KalimaNextGateAct(lpObj->Index);
			return;
		}

		gKalima.KalimaGateAct(lpObj->Index);
		return;
	}

#if(GAMESERVER_TYPE==1)
	if (lpObj->Class == 283)
	{
		return;
	}
	else if (lpObj->Class == 288)
	{
		return;
	}
	else if (lpObj->Class == 278)
	{
		return;
	}
#endif

	if ((lpObj->Class >= 100 && lpObj->Class <= 110))
	{
		gObjMonsterTrapAct(lpObj);
	}
	else if (lpObj->Class == 200)
	{
		int ground;
		int team = gCheckGoal(lpObj->X, lpObj->Y, ground);

		if (team >= 0)
		{
			gObjMonsterRegen(lpObj);
			gBattleSoccerScoreUpdate(ground, team);
			return;
		}
	}
	else
	{
		if (lpObj->Class == 287 || lpObj->Class == 286)
		{
		#if(GAMESERVER_TYPE==1)
			gMercenary.MercenaryAct(lpObj->Index);
		#endif
		}
		else
		{
			gObjMonsterBaseAct(lpObj);
		}
	}

	if (lpObj->Attribute == 100)
	{
		if (lpObj->SummonIndex >= 0 && lpObj->SummonIndex < MAX_OBJECT)
		{
			LPOBJ lpCallMonObj;
			BOOL Success = FALSE;

			lpCallMonObj = &gObj[lpObj->SummonIndex];

			if (lpObj->Map != lpCallMonObj->Map)
			{
				Success = TRUE;
			}

			if (gObjCalcDistance(lpCallMonObj, lpObj) > 14)
			{
				Success = TRUE;
			}

			if (Success == TRUE)
			{
				gObjTeleportMagicUse(lpObj->Index, (BYTE)lpCallMonObj->X + 1, lpCallMonObj->Y);
				lpObj->Map = lpCallMonObj->Map;
				return;
			}
		}
	}

	if (lpObj->ActionState.Move != 0)
	{
		if (PathFindMoveMsgSend(lpObj) == TRUE)
		{
			lpObj->ActionState.Move = (DWORD)0;
			return;
		}
		else
		{
			lpObj->ActionState.Move = (DWORD)1;
			return;
		}
	}

	if (lpObj->ActionState.Attack == 1)
	{
		if (CMonsterSkillManager::CheckMonsterSkill(lpObj->Class, 0))
		{
			BOOL bEnableAttack = TRUE;

			if (lpObj->TargetNumber < 0)
			{
				lpObj->TargetNumber = -1;
				lpObj->ActionState.Emotion = 0;
				lpObj->ActionState.Attack = 0;
				lpObj->ActionState.Move = 0;
				lpObj->NextActionTime = 1000;
				return;
			}

			if (gObj[lpObj->TargetNumber].Live == FALSE || gObj[lpObj->TargetNumber].Teleport != 0)
			{
				bEnableAttack = FALSE;
			}

			if (gObj[lpObj->TargetNumber].Connected <= OBJECT_LOGGED || gObj[lpObj->TargetNumber].CloseCount != -1)
			{
				bEnableAttack = FALSE;
			}

			if (bEnableAttack == FALSE)
			{
				lpObj->TargetNumber = -1;
				lpObj->ActionState.Emotion = 0;
				lpObj->ActionState.Attack = 0;
				lpObj->ActionState.Move = 0;
				lpObj->NextActionTime = 1000;
				return;
			}

			if (GetLargeRand() % 4 == 0)
			{
				PMSG_ATTACK pAttackMsg;

				pAttackMsg.AttackAction = 120;
				pAttackMsg.DirDis = lpObj->Dir;
				pAttackMsg.NumberH = SET_NUMBERHB(lpObj->TargetNumber);
				pAttackMsg.NumberL = SET_NUMBERLB(lpObj->TargetNumber);

				GCActionSend(lpObj, 120, lpObj->Index, lpObj->TargetNumber);
				gAttack.Attack(lpObj, &gObj[lpObj->TargetNumber], NULL, FALSE, 0, 0, 0, FALSE);
			}
			else
			{
				CMonsterSkillManager::UseMonsterSkill(lpObj->Index, lpObj->TargetNumber, 0, -1, NULL);
			}

			lpObj->ActionState.Attack = 0;
		}
		else
		{
			int AttackType = lpObj->AttackType;
			int AttackFlag = 0;

			if (AttackType >= 100)
			{
				if ((GetLargeRand() % 5) == 0)
				{
					AttackType -= 100;
					AttackFlag = TRUE;
				}
				else
				{
					AttackType = 0;
				}
			}

			if (AttackFlag != FALSE || lpObj->AttackType == 50)
			{
				if (lpObj->TargetNumber >= 0)
				{
					if (gObj[lpObj->TargetNumber].Connected > OBJECT_LOGGED&& gObj[lpObj->TargetNumber].CloseCount == -1)
					{
						if (gObj[lpObj->TargetNumber].Live == FALSE)
						{
							lpObj->TargetNumber = -1;
							lpObj->ActionState.Emotion = 0;
							lpObj->ActionState.Attack = 0;
							lpObj->ActionState.Move = 0;
							lpObj->NextActionTime = 1000;
						}
						else if (gObj[lpObj->TargetNumber].Teleport == 0)
						{
							gObjMonsterMagicAttack(lpObj, 0);
						}
					}
					else
					{
						lpObj->TargetNumber = -1;
						lpObj->ActionState.Emotion = 0;
						lpObj->ActionState.Attack = 0;
						lpObj->ActionState.Move = 0;
						lpObj->NextActionTime = 1000;
					}
				}
			}
			else
			{
				if (lpObj->TargetNumber >= 0)
				{
					if (gObj[lpObj->TargetNumber].Connected > OBJECT_LOGGED&& gObj[lpObj->TargetNumber].CloseCount == -1)
					{
						if (gObj[lpObj->TargetNumber].Live == FALSE)
						{
							lpObj->TargetNumber = -1;
							lpObj->ActionState.Emotion = 0;
							lpObj->ActionState.Attack = 0;
							lpObj->ActionState.Move = 0;
							lpObj->NextActionTime = 1000;
						}
						else if (gObj[lpObj->TargetNumber].Teleport == 0)
						{
							gObjMonsterAttack(lpObj, &gObj[lpObj->TargetNumber]);
						}

					}
					else
					{
						lpObj->TargetNumber = -1;
						lpObj->ActionState.Emotion = 0;
						lpObj->ActionState.Attack = 0;
						lpObj->ActionState.Move = 0;
						lpObj->NextActionTime = 1000;
					}
				}
			}

			lpObj->ActionState.Attack = 0;
		}
	}
}

BOOL PathFindMoveMsgSend(LPOBJ lpObj)
{
	if (gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return 0;
	}

	PATH_INFO path;
	PMSG_MOVE pMove;
	BYTE bPath[8];

	pMove.h.type = 0xC1;
	pMove.h.head = PROTOCOL_CODE1;
	pMove.h.size = sizeof(pMove);
	pMove.X = (BYTE)lpObj->X;
	pMove.Y = (BYTE)lpObj->Y;

	BOOL bPathFound = gMap[lpObj->Map].PathFinding2(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY, &path);

	if (bPathFound)
	{
		int tx;
		int ty;
		int sx;
		int sy;
		BYTE pos = 0;

		memset(bPath, 0, sizeof(bPath));

		sx = lpObj->X;
		sy = lpObj->Y;

		for (int n = 1; n < path.PathNum; n++)
		{
			tx = path.PathX[n];
			ty = path.PathY[n];
			pos = GetPathPacketDirPos(tx - sx, ty - sy);
			sx = tx;
			sy = ty;

			if ((n % 2) == 1)
			{
				bPath[(n + 1) / 2] = (pos << 4);
			}
			else
			{
				bPath[(n + 1) / 2] |= pos;
			}
		}

		bPath[0] = (pos << 4) + ((path.PathNum - 1) & 0x0F);

		memcpy(pMove.Path, bPath, sizeof(pMove.Path));

		CGMoveRecv((PMSG_MOVE_RECV*)&pMove, lpObj->Index);
		return TRUE;
	}

	return FALSE;
}

void gObjMonsterMoveAction(LPOBJ lpObj)
{
	if (gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	int maxmoverange = lpObj->MoveRange * 2 + 1;
	int searchc = 10;
	lpObj->NextActionTime = 1000;
	BYTE tpx;
	BYTE tpy;

	while (searchc-- != 0)
	{
		__try
		{
			tpx = (lpObj->X - lpObj->MoveRange) + (BYTE)(GetLargeRand() % maxmoverange);
			tpy = (lpObj->Y - lpObj->MoveRange) + (BYTE)(GetLargeRand() % maxmoverange);
		}
		__except (maxmoverange = 1, 1)
		{

		}

		int mchk = gObjMonsterMoveCheck(lpObj, tpx, tpy);
		BYTE attr = gMap[lpObj->Map].GetAttr(tpx, tpy);

		if ((lpObj->Class == 249 || lpObj->Class == 247) && mchk)
		{
			if ((attr & 2) != 2)
			{
				lpObj->MTX = tpx;
				lpObj->MTY = tpy;
				lpObj->ActionState.Move = 1;
				lpObj->NextActionTime = 3000;
				return;
			}
		}
		else
		{
			if ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8 && mchk)
			{
				lpObj->TargetNumber = -1;
				lpObj->ActionState.Attack = 0;
				lpObj->NextActionTime = 500;
				lpObj->ActionState.Emotion = 0;
				lpObj->MTX = tpx;
				lpObj->MTY = tpy;
				lpObj->ActionState.Move = 1;
				return;
			}
		}
	}
}

void gObjMonsterBaseAct(LPOBJ lpObj)
{
	LPOBJ lpTargetObj = NULL;

	if (lpObj->TargetNumber >= 0)
	{
		lpTargetObj = &gObj[lpObj->TargetNumber];
	}
	else
	{
		lpObj->ActionState.Emotion = 0;
	}

	if (gEffectManager.CheckEffect(lpObj, EFFECT_SLEEP) != 0)
	{
		return;
	}

	if (lpObj->ActionState.Emotion == 0)
	{
		if (lpObj->Attribute)
		{
			if (lpObj->ActionState.Attack)
			{
				lpObj->ActionState.Attack = 0;
				lpObj->TargetNumber = -1;
				lpObj->NextActionTime = 500;
			}

			int actcode1 = GetLargeRand() % 2;

			if (lpObj->Attribute == 100)
			{
				actcode1 = 1;
			}

			if (actcode1 == 0)
			{
				lpObj->ActionState.Rest = 1;
				lpObj->NextActionTime = 500;
			}
			else if (lpObj->MoveRange > 0 && gEffectManager.CheckStunEffect(lpObj) == 0 && gEffectManager.CheckImmobilizeEffect(lpObj) == 0)
			{
				if (lpObj->Attribute != 100)
				{
					gObjMonsterMoveAction(lpObj);
				}
				else
				{
					int tx = 0;
					int ty = 0;

					if (lpObj->SummonIndex >= 0)
					{
						if (gObj[lpObj->SummonIndex].Connected > OBJECT_LOGGED)
						{
							LPOBJ lpRecallObj = &gObj[lpObj->SummonIndex];

							if (lpRecallObj->Rest == FALSE)
							{
								if (gObjGetTargetPos(lpObj, lpRecallObj->X, lpRecallObj->Y, tx, ty) == TRUE)
								{
									lpObj->MTX = tx;
									lpObj->MTY = ty;
									lpObj->ActionState.Move = 1;
									lpObj->NextActionTime = 1000;
								}
							}
						}
					}
				}
			}

			if (lpObj->Class == 249 || lpObj->Class == 247)
			{
				lpObj->TargetNumber = gObjGuardSearchEnemy(lpObj);

				if (lpObj->TargetNumber >= 0)
				{
					if (gObj[lpObj->TargetNumber].Class >= 248)
					{
						lpObj->TargetNumber = -1;
					}
				}
			}
			else
			{
				if (lpObj->Attribute == 100)
				{
					lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJECT_MONSTER);
				}
				else
				{
					lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJECT_USER);
				}
			}

			if (lpObj->TargetNumber >= 0)
			{
				lpObj->ActionState.EmotionCount = 30;
				lpObj->ActionState.Emotion = 1;
			}
		}
	}
	else if (lpObj->ActionState.Emotion == 1)
	{
		if (lpObj->ActionState.EmotionCount > 0)
		{
			lpObj->ActionState.EmotionCount--;
		}
		else
		{
			lpObj->ActionState.Emotion = 0;
		}

		if (lpObj->TargetNumber >= 0 && lpObj->PathStartEnd == 0)
		{
			if (BC_MAP_RANGE(lpObj->Map))
			{
				int iRAND_CHANGE_TARGET = GetLargeRand() % 10;

				if (iRAND_CHANGE_TARGET == 3)
				{
					lpObj->LastAttackerID = -1;
				}

				if (iRAND_CHANGE_TARGET == 1)
				{
					if (lpObj->LastAttackerID != -1 && lpObj->LastAttackerID != lpObj->TargetNumber)
					{
						if (gObj[lpObj->LastAttackerID].Connected > OBJECT_LOGGED&& lpObj->Map == gObj[lpObj->LastAttackerID].Map)
						{
							lpObj->TargetNumber = lpObj->LastAttackerID;
							lpTargetObj = &gObj[lpObj->LastAttackerID];
						}
					}
				}
			}

			int dis = gObjCalcDistance(lpObj, lpTargetObj);
			int attackrange;

			if (lpObj->AttackType >= 100)
			{
				attackrange = lpObj->AttackRange + 2;
			}
			else
			{
				attackrange = lpObj->AttackRange;
			}

			if (dis <= attackrange)
			{
				int tuser = lpObj->TargetNumber;
				int map = gObj[tuser].Map;
				BYTE attr;

				if (gMap[map].CheckWall(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE)
				{
					attr = gMap[map].GetAttr(gObj[tuser].X, gObj[tuser].Y);

					if ((attr & 1) != 1)
					{
						lpObj->ActionState.Attack = 1;
					}
					else
					{
						lpObj->TargetNumber = -1;
						lpObj->ActionState.EmotionCount = 30;
						lpObj->ActionState.Emotion = 1;
					}

					lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
					lpObj->NextActionTime = lpObj->PhysiSpeed;
				}
				else
				{
					lpObj->TargetNumber = -1;
					lpObj->ActionState.Attack = 0;
					lpObj->NextActionTime = 500;
					lpObj->ActionState.Emotion = 0;
					lpObj->ActionState.Move = 1;
				}
			}
			else
			{
				if (gObjMonsterGetTargetPos(lpObj) == TRUE)
				{
					if (gMap[lpObj->Map].CheckWall(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY) == TRUE)
					{
						lpObj->ActionState.Move = 1;
						lpObj->NextActionTime = 400;
						lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
					}
					else
					{
						gObjMonsterMoveAction(lpObj);
						lpObj->ActionState.Emotion = 3;
						lpObj->ActionState.EmotionCount = 10;

					}
				}
				else
				{
					gObjMonsterMoveAction(lpObj);
				}
			}
		}
		else
		{
		}
	}
	else if (lpObj->ActionState.Emotion == 2)
	{
		if (lpObj->ActionState.EmotionCount > 0)
		{
			lpObj->ActionState.EmotionCount--;
		}
		else
		{
			lpObj->ActionState.Emotion = 0;
		}

		lpObj->ActionState.Move = 1;
		lpObj->NextActionTime = 800;

		if (lpTargetObj != NULL)
		{
			int tdir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y) * 2;
			lpObj->MTX += RoadPathTable[tdir] * (-3);
			lpObj->MTY += RoadPathTable[tdir + 1] * (-3);
		}
	}
	else if (lpObj->ActionState.Emotion == 3)
	{
		if (lpObj->ActionState.EmotionCount > 0)
		{
			lpObj->ActionState.EmotionCount--;
		}
		else
		{
			lpObj->ActionState.Emotion = 0;
		}

		lpObj->ActionState.Move = 0;
		lpObj->ActionState.Attack = 0;
		lpObj->NextActionTime = 400;
	}
}

void gObjTrapAttackEnemySearchX(LPOBJ lpObj, int count)
{
	lpObj->TargetNumber = -1;
	int tObjNum;
	int pos = lpObj->X;
	int y = lpObj->Y;

	for (int n = 0; n < count; n++)
	{
		pos++;
		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			tObjNum = lpObj->VpPlayer2[i].index;

			if (tObjNum >= 0)
			{
				if (gObj[tObjNum].Type == OBJECT_USER)
				{
					if (gObj[tObjNum].Live)
					{
						if (y == gObj[tObjNum].Y)
						{
							if (pos == gObj[tObjNum].X)
							{
								if ((gObj[tObjNum].Authority & 2) != 2)
								{
									lpObj->TargetNumber = tObjNum;
									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void gObjTrapAttackEnemySearchY(LPOBJ lpObj, int count)
{
	lpObj->TargetNumber = -1;
	int tObjNum;
	int pos = lpObj->Y;
	int x = lpObj->X;

	for (int n = 0; n < count; n++)
	{
		pos--;
		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			tObjNum = lpObj->VpPlayer2[i].index;

			if (tObjNum >= 0)
			{
				if (gObj[tObjNum].Type == 1)
				{
					if (gObj[tObjNum].Live)
					{
						if (pos == gObj[tObjNum].Y)
						{
							if (x == gObj[tObjNum].X)
							{
								if ((gObj[tObjNum].Authority & 2) != 2)
								{
									lpObj->TargetNumber = tObjNum;
									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void gObjTrapAttackEnemySearch(LPOBJ lpObj)
{
	int tObjNum;
	lpObj->TargetNumber = -1;

	for (int i = 0; i < MAX_VIEWPORT; i++)
	{
		tObjNum = lpObj->VpPlayer2[i].index;

		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == 1)
			{
				if (gObj[tObjNum].Live)
				{
					if (lpObj->Y == gObj[tObjNum].Y)
					{
						if (lpObj->X == gObj[tObjNum].X)
						{
							if ((gObj[tObjNum].Authority & 2) != 2)
							{
								lpObj->TargetNumber = tObjNum;
								return;
							}
						}
					}
				}
			}
		}
	}
}

void gObjTrapAttackEnemySearchRange(LPOBJ lpObj, int iRange)
{
	int tObjNum = -1;
	int iTargetingRate = 0;
	int iSuccessRate = 0;

	lpObj->TargetNumber = -1;

	if (lpObj->VPCount <= 0)
	{
		return;
	}

	iTargetingRate = 100 / lpObj->VPCount;

	for (int i = 0; i < MAX_VIEWPORT; i++)
	{
		tObjNum = lpObj->VpPlayer2[i].index;

		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == 1)
			{
				if (gObj[tObjNum].Live)
				{
					if ((lpObj->Y - iRange) <= gObj[tObjNum].Y && (lpObj->Y + iRange) >= gObj[tObjNum].Y)
					{
						if ((lpObj->X - iRange) <= gObj[tObjNum].X && (lpObj->X + iRange) >= gObj[tObjNum].X)
						{
							if ((gObj[tObjNum].Authority & 2) != 2)
							{
								lpObj->TargetNumber = tObjNum;
								iSuccessRate = GetLargeRand() % 100;

								if (iSuccessRate < iTargetingRate)
								{
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void gObjMonsterTrapAct(LPOBJ lpObj)
{
	if (lpObj->VPCount2 < 1)
	{
		return;
	}

	if (lpObj->AttackRange > 0)
	{
		if (lpObj->Dir == 3)
		{
			gObjTrapAttackEnemySearchX(lpObj, lpObj->AttackRange + 1);
		}
		else if (lpObj->Dir == 1)
		{
			gObjTrapAttackEnemySearchY(lpObj, lpObj->AttackRange + 1);
		}
		else if (lpObj->Dir == 8)
		{
			gObjTrapAttackEnemySearchRange(lpObj, lpObj->AttackRange);
		}
	}
	else
	{
		gObjTrapAttackEnemySearch(lpObj);
	}

	if (lpObj->TargetNumber >= 0)
	{
		lpObj->ActionState.Attack = 1;
		lpObj->NextActionTime = lpObj->PhysiSpeed;
	}
	else
	{
		lpObj->NextActionTime = lpObj->MoveSpeed;
	}
}

void gObjMonsterStateProcCase0(LPOBJ lpObj, int aIndex)
{
	if (lpObj->Attribute == 0)
	{
		return;
	}

	if (gObj[aIndex].Live == FALSE || gObj[aIndex].State != 2)
	{
		return;
	}

	if (lpObj->ActionState.Emotion == 0)
	{
		lpObj->ActionState.Emotion = 1;
		lpObj->ActionState.EmotionCount = 10;
	}
	else if (lpObj->ActionState.Emotion == 1)
	{
		lpObj->ActionState.EmotionCount = 10;
	}

	if (lpObj->ActionState.Attack == 0 && lpObj->PathStartEnd == 0)
	{
		if (OBJECT_RANGE(aIndex))
		{
			int map = gObj[aIndex].Map;
			BYTE attr;
			int dis = gObjCalcDistance(lpObj, &gObj[aIndex]);
			int range;

			if (lpObj->AttackType >= 100)
			{
				range = lpObj->AttackRange + 2;
			}
			else
			{
				range = lpObj->AttackRange;
			}

			if (dis <= range)
			{
				if (gObj[aIndex].SummonIndex >= 0)
				{
					if (lpObj->SummonIndex >= 0)
					{
						if (gObj[aIndex].Type == OBJECT_MONSTER)
						{
							lpObj->TargetNumber = aIndex;
						}
					}
					else
					{
						lpObj->TargetNumber = aIndex;
					}
				}
				else if ((GetLargeRand() % 100) < 90)
				{
					if (lpObj->SummonIndex >= 0)
					{
						if (gObj[aIndex].Type == OBJECT_MONSTER)
						{
							lpObj->TargetNumber = aIndex;
						}
					}
					else
					{
						lpObj->TargetNumber = aIndex;
					}
				}
			}
			else
			{
				BYTE wall = 0;

				wall = gMap[map].CheckWall2(lpObj->X, lpObj->Y, gObj[aIndex].X, gObj[aIndex].Y);

				if (wall == 1)
				{
					attr = gMap[map].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);

					if ((attr & 1) != 1)
					{
						if (lpObj->TargetNumber < 0)
						{
							lpObj->TargetNumber = aIndex;
						}
					}
				}
			}
		}
	}
	else
	{
		if ((GetLargeRand() % 2) == 1 && lpObj->PathStartEnd == 0)
		{
			int IndexEnemy = lpObj->TargetNumber;

			if (!OBJECT_RANGE(IndexEnemy))
			{
				return;
			}

			int EnemyMap = gObj[IndexEnemy].Map;

			int enemydis = gObjCalcDistance(lpObj, &gObj[aIndex]);
			int range;

			if (lpObj->AttackType >= 100)
			{
				range = lpObj->AttackRange + 2;
			}
			else
			{
				range = lpObj->AttackRange;
			}

			if (enemydis <= range)
			{
				lpObj->ActionState.Attack = 1;
				lpObj->TargetNumber = aIndex;
			}
			else
			{
				if (gMap[EnemyMap].CheckWall2(lpObj->X, lpObj->Y, gObj[IndexEnemy].X, gObj[IndexEnemy].Y) == 1)
				{
					lpObj->ActionState.Attack = 1;
					lpObj->TargetNumber = aIndex;
				}
			}
		}
		else
		{
			int MaxLife = (int)lpObj->MaxLife;
			MaxLife >>= 1;

			if (MaxLife > lpObj->Life)
			{
				if (lpObj->Attribute != 2)
				{
					lpObj->ActionState.Emotion = 2;
					lpObj->ActionState.EmotionCount = 2;
				}
			}
		}
	}
}
