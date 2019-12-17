// MonsterManager.cpp: implementation of the CMonsterManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterManager.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "InvasionManager.h"
#include "ItemManager.h"
#include "Map.h"
#include "MemScript.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "User.h"
#include "Util.h"

CMonsterManager gMonsterManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterManager::CMonsterManager() // OK
{
	this->Init();
}

CMonsterManager::~CMonsterManager() // OK
{

}

void CMonsterManager::Init() // OK
{
	for (int n = 0; n < MAX_MONSTER_INFO; n++)
	{
		this->m_MonsterInfo[n].Index = -1;
	}

	for (int n = 0; n < MAX_MONSTER_LEVEL; n++)
	{
		memset(this->m_MonsterItemInfo[n].IndexTable, -1, sizeof(this->m_MonsterItemInfo[n].IndexTable));

		this->m_MonsterItemInfo[n].IndexCount = 0;
	}
}

void CMonsterManager::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	char buffer[10000] = "";

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			MONSTER_INFO info;

			memset(&info, 0, sizeof(info));

			info.Index = lpMemScript->GetNumber();
			info.Rate = lpMemScript->GetAsNumber();

			strcpy_s(info.Name, lpMemScript->GetAsString());

			info.Level = lpMemScript->GetAsNumber();
			info.Life = lpMemScript->GetAsNumber();
			info.Mana = lpMemScript->GetAsNumber();
			info.DamageMin = lpMemScript->GetAsNumber();
			info.DamageMax = lpMemScript->GetAsNumber();
			info.Defense = lpMemScript->GetAsNumber();
			info.MagicDefense = lpMemScript->GetAsNumber();
			info.AttackRate = lpMemScript->GetAsNumber();
			info.DefenseRate = lpMemScript->GetAsNumber();
			info.MoveRange = lpMemScript->GetAsNumber();
			info.AttackType = lpMemScript->GetAsNumber();
			info.AttackRange = lpMemScript->GetAsNumber();
			info.ViewRange = lpMemScript->GetAsNumber();
			info.MoveSpeed = lpMemScript->GetAsNumber();
			info.AttackSpeed = lpMemScript->GetAsNumber();
			info.RegenTime = lpMemScript->GetAsNumber();
			info.Attribute = lpMemScript->GetAsNumber();
			info.ItemRate = lpMemScript->GetAsNumber();
			info.MoneyRate = lpMemScript->GetAsNumber();
			info.MaxItemLevel = lpMemScript->GetAsNumber();
			info.MonsterSkill = lpMemScript->GetAsNumber();
			info.Resistance[0] = lpMemScript->GetAsNumber();
			info.Resistance[1] = lpMemScript->GetAsNumber();
			info.Resistance[2] = lpMemScript->GetAsNumber();
			info.Resistance[3] = lpMemScript->GetAsNumber();

			this->SetInfo(info);

			char temp[50];
			sprintf_s(temp, "%d|", info.Index);
			strcat_s(buffer, temp);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	this->InitMonsterItem();

	delete lpMemScript;
}

void CMonsterManager::SetInfo(MONSTER_INFO info) // OK
{
	if (info.Index < 0 || info.Index >= MAX_MONSTER_INFO)
	{
		return;
	}

	info.Life = ((__int64)info.Life * gServerInfo.m_MonsterLifeRate) / 100;

	info.ScriptLife = info.Life;

	this->m_MonsterInfo[info.Index] = info;
}

MONSTER_INFO* CMonsterManager::GetInfo(int index) // OK
{
	if (index < 0 || index >= MAX_MONSTER_INFO)
	{
		return 0;
	}

	if (this->m_MonsterInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_MonsterInfo[index];
}

void CMonsterManager::InitMonsterItem() // OK
{
	for (int n = 0; n < MAX_MONSTER_LEVEL; n++)
	{
		this->InsertMonsterItem(n);
	}
}

void CMonsterManager::InsertMonsterItem(int level) // OK
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		ITEM_INFO ItemInfo;

		if (gItemManager.GetInfo(n, &ItemInfo) == 0)
		{
			continue;
		}

		if (ItemInfo.Level < 0 || ItemInfo.DropItem == 0)
		{
			continue;
		}

		if ((ItemInfo.Level + 4) >= level && (ItemInfo.Level - 2) <= level)
		{
			MONSTER_ITEM_INFO* lpItem = &this->m_MonsterItemInfo[level];

			if (lpItem->IndexCount >= 0 && lpItem->IndexCount < MAX_MONSTER_ITEM)
			{
				lpItem->IndexTable[lpItem->IndexCount++] = n;
			}
		}
	}
}

long CMonsterManager::GetMonsterItem(int level, int excellent, int socket) // OK
{
	if (level < 0 || level >= MAX_MONSTER_LEVEL)
	{
		return -1;
	}

	if (this->m_MonsterItemInfo[level].IndexCount == 0)
	{
		return -1;
	}

	int IndexTable[MAX_MONSTER_ITEM];
	int IndexCount = 0;

	for (int n = 0; n < this->m_MonsterItemInfo[level].IndexCount; n++)
	{
		if (excellent != 0 && this->m_MonsterItemInfo[level].IndexTable[n] >= GET_ITEM(12, 0))
		{
			continue;
		}

		IndexTable[IndexCount++] = this->m_MonsterItemInfo[level].IndexTable[n];
	}

	return ((IndexCount == 0) ? -1 : IndexTable[GetLargeRand() % IndexCount]);
}

void CMonsterManager::SetMonsterData() // OK
{
	for (int n = 0; n < gMonsterSetBase.m_count; n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if (lpInfo->Type == 3 || lpInfo->Type == 4)
		{
			continue;
		}

		int index = gObjAddMonster(lpInfo->Map);

		if (OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		if (gObjSetPosMonster(index, n) == 0)
		{
			gObjDel(index);
			continue;
		}

		if (gObjSetMonster(index, lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}

	#if(GAMESERVER_TYPE==1)

		LPOBJ lpObj = &gObj[index];

		if (lpObj->Map == MAP_CASTLE_SIEGE)
		{
			if (lpObj->Class == 216)
			{
				gCastleSiege.SetCrownIndex(index);
			}
		}
	#endif
	}

	gBloodCastle.Init();
	gBonusManager.Init();
	gCastleDeep.Init();
	gChaosCastle.Init();
	gDevilSquare.Init();
	gInvasionManager.Init();
}