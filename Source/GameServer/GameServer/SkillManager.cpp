// SkillManager.cpp: implementation of the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillManager.h"
#include "Math.h"
#include "Attack.h"
#include "CastleSiege.h"
#include "Duel.h"
#include "EffectManager.h"
#include "Fruit.h"
#include "ItemManager.h"
#include "Map.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Move.h"
#include "MoveSummon.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "SkillHitBox.h"
#include "Util.h"
#include "Viewport.h"

CSkillManager gSkillManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillManager::CSkillManager() // OK
{
	this->m_SkillInfo.clear();
}

CSkillManager::~CSkillManager() // OK
{

}

void CSkillManager::Load(char* path) // OK
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

	this->m_SkillInfo.clear();

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
			
			int index = lpMemScript->GetNumber();
			
			SKILL_INFO info;

			strcpy_s(info.Name, lpMemScript->GetAsString());

			info.Damage = lpMemScript->GetAsNumber();
			info.Mana = lpMemScript->GetAsNumber();
			info.BP = lpMemScript->GetAsNumber();
			info.Range = lpMemScript->GetAsNumber();
			info.Radio = lpMemScript->GetAsNumber();
			info.Delay = lpMemScript->GetAsNumber();
			info.Type = lpMemScript->GetAsNumber();
			info.Effect = lpMemScript->GetAsNumber();
			info.RequireLevel = lpMemScript->GetAsNumber();
			info.RequireEnergy = lpMemScript->GetAsNumber();
			info.RequireLeadership = lpMemScript->GetAsNumber();
			info.RequireKillCount = lpMemScript->GetAsNumber();
			info.RequireGuildStatus = lpMemScript->GetAsNumber();

			for (int n = 0; n < MAX_CLASS; n++)
			{
				info.RequireClass[n] = lpMemScript->GetAsNumber();
			}

			this->m_SkillInfo[index] = info;
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CSkillManager::GetInfo(BYTE index, SKILL_INFO* lpInfo) // OK
{
	if (this->m_SkillInfo.count(index) > 0)
	{
		(*lpInfo) = this->m_SkillInfo.at(index);
		return 1;
	}

	return 0;
}

int CSkillManager::GetSkillDamage(BYTE index) // OK
{
	auto it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}

	return it->second.Damage;
}

int CSkillManager::GetSkillMana(BYTE index) // OK
{
	auto it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	
	return it->second.Mana;
}

int CSkillManager::GetSkillBP(BYTE index) // OK
{
	auto it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	
	return it->second.BP;
}

int CSkillManager::GetSkillType(BYTE index) // OK
{
	auto it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	
	return it->second.Type;
}

int CSkillManager::GetSkillEffect(BYTE index) // OK
{
	auto it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	
	return it->second.Effect;
}

int CSkillManager::GetSkillAngle(int x, int y, int tx, int ty) // OK
{
	float dx = (float)(x - tx);
	float dy = (float)(y - ty);
	float rad = atan2(dy, dx) * 180.f;
	int angle = (int)(rad / 3.141592741012573) + 90;

	return ((angle < 0) ? (angle + 360) : angle);
}

int CSkillManager::GetSkillFrustrum(int* SkillFrustrumX, int* SkillFrustrumY, BYTE angle, int x, int y, float sx, float sy, float tx, float ty) // OK
{
	vec3_t p[4];

	Vector(-sx, sy, 0.0f, p[0]);
	Vector(sx, sy, 0.0f, p[1]);
	Vector(tx, ty, 0.f, p[2]);
	Vector(-tx, ty, 0.f, p[3]);

	vec3_t Angle;

	Vector(0.0f, 0.0f, (vec_t)((angle * 360) / 255), Angle);

	float Matrix[3][4];

	AngleMatrix(Angle, Matrix);

	vec3_t vFrustrum[4];

	for (int n = 0; n < 4; n++)
	{
		VectorRotate(p[n], Matrix, vFrustrum[n]);
		SkillFrustrumX[n] = (int)vFrustrum[n][0] + x;
		SkillFrustrumY[n] = (int)vFrustrum[n][1] + y;
	}

	return 1;
}

int CSkillManager::GetSkillNumber(int index, int level) // OK
{
	if (index >= GET_ITEM(15, 0) && index <= GET_ITEM(15, 15))
	{
		return ((index % MAX_ITEM_TYPE) + 1);
	}

	switch (index)
	{
		case GET_ITEM(12, 7):
			return SKILL_TWISTING_SLASH;
		case GET_ITEM(12, 8):
			return SKILL_HEAL;
		case GET_ITEM(12, 9):
			return SKILL_GREATER_DEFENSE;
		case GET_ITEM(12, 10):
			return SKILL_GREATER_DAMAGE;
		case GET_ITEM(12, 11):
			return (SKILL_SUMMON1 + level);
		case GET_ITEM(12, 12):
			return SKILL_RAGEFUL_BLOW;
		case GET_ITEM(12, 13):
			return SKILL_IMPALE;
		case GET_ITEM(12, 14):
			return SKILL_GREATER_LIFE;
		case GET_ITEM(12, 16):
			return SKILL_FIRE_SLASH;
		case GET_ITEM(12, 17):
			return SKILL_PENETRATION;
		case GET_ITEM(12, 18):
			return SKILL_ICE_ARROW;
		case GET_ITEM(12, 19):
			return SKILL_DEATH_STAB;
		case GET_ITEM(12, 21):
			return SKILL_FIRE_BURST;
		case GET_ITEM(12, 22):
			return SKILL_SUMMON_PARTY;
		case GET_ITEM(12, 23):
			return SKILL_GREATER_CRITICAL_DAMAGE;
		case GET_ITEM(12, 24):
			return SKILL_ELECTRIC_SPARK;
		case GET_ITEM(12, 35):
			return SKILL_FIRE_SCREAM;
		case GET_ITEM(15, 16):
			return SKILL_DECAY;
		case GET_ITEM(15, 17):
			return SKILL_ICE_STORM;
		case GET_ITEM(15, 18):
			return SKILL_NOVA;
	}

	return -1;
}

bool CSkillManager::CheckSkillMana(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if (lpObj->Mana < ((SkillInfo.Mana * lpObj->MPConsumptionRate) / 100))
	{
		return 0;
	}
	
	return 1;
}

bool CSkillManager::CheckSkillBP(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if (lpObj->BP < ((SkillInfo.BP * lpObj->BPConsumptionRate) / 100))
	{
		return 0;
	}
	
	return 1;
}

bool CSkillManager::CheckSkillRange(BYTE index, int x, int y, int tx, int ty) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if (SkillInfo.Range == 0)
	{
		return 0;
	}

	if (sqrt(pow(((float)x - (float)tx), 2) + pow(((float)y - (float)ty), 2)) <= SkillInfo.Range)
	{
		return 1;
	}
	
	return 0;
}

bool CSkillManager::CheckSkillRadio(BYTE index, int x, int y, int tx, int ty) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if (SkillInfo.Radio == 0)
	{
		return 0;
	}

	if (sqrt(pow(((float)x - (float)tx), 2) + pow(((float)y - (float)ty), 2)) <= SkillInfo.Radio)
	{
		return 1;
	}
	
	return 0;
}

bool CSkillManager::CheckSkillFrustrum(int* SkillFrustrumX, int* SkillFrustrumY, int x, int y) // OK
{
	for (int n = 0, i = 3; n < 4; i = n, n++)
	{
		if ((((SkillFrustrumX[n] - x) * (SkillFrustrumY[i] - y)) - ((SkillFrustrumX[i] - x) * (SkillFrustrumY[n] - y))) < 0)
		{
			return 0;
		}
	}

	return 1;
}

bool CSkillManager::CheckSkillDelay(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}


	if ((GetTickCount() - lpObj->SkillDelay[index]) < ((DWORD)SkillInfo.Delay))
	{
		return 0;
	}

	lpObj->SkillDelay[index] = GetTickCount();
	return 1;
}

bool CSkillManager::CheckSkillTarget(LPOBJ lpObj, int aIndex, int bIndex, int type) // OK
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if (type == OBJECT_NPC)
	{
		return 0;
	}

	if (gObj[aIndex].Live == 0 || gObj[aIndex].State != OBJECT_PLAYING || gObj[aIndex].Teleport != 0)
	{
		return 0;
	}

	if (lpObj->Type == OBJECT_MONSTER && type == OBJECT_USER)
	{
		return 1;
	}

	if (aIndex == bIndex)
	{
		return 1;
	}

	int SummonIndex = aIndex;

	if (type == OBJECT_MONSTER)
	{
		if ((gObj[aIndex].Class >= 100 && gObj[aIndex].Class <= 110) || gObj[aIndex].Class == 523) // Trap
		{
			return 0;
		}

		if (OBJECT_RANGE(gObj[aIndex].SummonIndex) == 0)
		{
			return 1;
		}
		else
		{
			SummonIndex = gObj[aIndex].SummonIndex;
		}
	}

	if (gDuel.CheckDuel(lpObj, &gObj[SummonIndex]) != 0)
	{
		return 1;
	}

	if (gObjTargetGuildWarCheck(lpObj, &gObj[SummonIndex]) != 0)
	{
		return 1;
	}
	
#if(GAMESERVER_TYPE==1)

	if (gObj[SummonIndex].Map == MAP_CASTLE_SIEGE && gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		if (lpObj->Type == OBJECT_USER && gObj[SummonIndex].Type == OBJECT_USER)
		{
			if (gServerInfo.m_CastleSiegeDamageRate2 != 0 || lpObj->CsJoinSide == 0 || gObj[SummonIndex].CsJoinSide == 0 || lpObj->CsJoinSide != gObj[SummonIndex].CsJoinSide)
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}

#endif

	return 0;
}

bool CSkillManager::CheckSkillRequireLevel(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if (lpObj->Level >= SkillInfo.RequireLevel)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CSkillManager::CheckSkillRequireEnergy(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if ((lpObj->Energy + lpObj->AddEnergy) >= SkillInfo.RequireEnergy)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CSkillManager::CheckSkillRequireLeadership(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if ((lpObj->Leadership + lpObj->AddLeadership) >= SkillInfo.RequireLeadership)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CSkillManager::CheckSkillRequireKillPoint(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if (lpObj->KillCount >= SkillInfo.RequireKillCount)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CSkillManager::CheckSkillRequireGuildStatus(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if (SkillInfo.RequireGuildStatus == 0 || (SkillInfo.RequireGuildStatus & 0x20) == lpObj->GuildStatus || (SkillInfo.RequireGuildStatus & 0x40) == lpObj->GuildStatus || (SkillInfo.RequireGuildStatus & 0x80) == lpObj->GuildStatus)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CSkillManager::CheckSkillRequireClass(LPOBJ lpObj, BYTE index) // OK
{
	SKILL_INFO SkillInfo;

	if (this->GetInfo(index, &SkillInfo) == 0)
	{
		return 0;
	}

	if (CHECK_RANGE(lpObj->Class, MAX_CLASS) == 0)
	{
		return 0;
	}

	if (SkillInfo.RequireClass[lpObj->Class] == 0)
	{
		return 0;
	}

	if ((lpObj->ChangeUp + 1) >= SkillInfo.RequireClass[lpObj->Class])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CSkillManager::CheckSkillRequireWeapon(LPOBJ lpObj, BYTE index) // OK
{
	if (index != SKILL_DEFENSE && index != SKILL_FALLING_SLASH && index != SKILL_LUNGE && index != SKILL_UPPERCUT && index != SKILL_CYCLONE && index != SKILL_SLASH && index != SKILL_TRIPLE_SHOT && index != SKILL_FIRE_BREATH && index != SKILL_POWER_SLASH && index != SKILL_EARTHQUAKE)
	{
		return 1;
	}

	for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Option1 != 0)
			{
				if (lpObj->Inventory[n].m_SpecialIndex[SPECIAL_OPTION1] == index)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

int CSkillManager::AddSkillWeapon(LPOBJ lpObj, BYTE index, int level) // OK
{
	int slot = 0;

	if ((slot = this->AddSkill(lpObj, index, level)) >= 0)
	{
		this->GCSkillAddSend(lpObj->Index, slot, index, (BYTE)level, 0); return 1;
	}
	else
	{
		return 0;
	}
}

int CSkillManager::DelSkillWeapon(LPOBJ lpObj, BYTE index, int level) // OK
{
	int slot = 0, count = 0;

	for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Option1 != 0)
			{
				if (lpObj->Inventory[n].m_SpecialIndex[SPECIAL_OPTION1] == index)
				{
					count++;
				}
			}
		}
	}

	if (count == 1 && (slot = this->DelSkill(lpObj, index)) >= 0)
	{
		this->GCSkillDelSend(lpObj->Index, slot, index, (BYTE)level, 0); return 1;
	}
	else
	{
		return 0;
	}
}

int CSkillManager::AddSkill(LPOBJ lpObj, BYTE index, int level) // OK
{
	if (lpObj->Type == OBJECT_USER && (this->CheckSkillRequireLevel(lpObj, index) == 0 || this->CheckSkillRequireEnergy(lpObj, index) == 0 || this->CheckSkillRequireLeadership(lpObj, index) == 0 || this->CheckSkillRequireClass(lpObj, index) == 0))
	{
		return -1;
	}

	if (this->GetSkill(lpObj, index) != 0)
	{
		return -1;
	}

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if (lpObj->Skill[n].IsSkill() == 0)
		{
			return ((lpObj->Skill[n].Set(index, level) == 0) ? -1 : n);
		}
	}

	return -1;
}

int CSkillManager::DelSkill(LPOBJ lpObj, BYTE index) // OK
{
	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if (lpObj->Skill[n].IsSkill() != 0)
		{
			if (lpObj->Skill[n].m_skill == index)
			{
				lpObj->Skill[n].Clear();
				return n;
			}
		}
	}

	return -1;
}

CSkill* CSkillManager::GetSkill(LPOBJ lpObj, BYTE index) // OK
{
	index = ((index == SKILL_NOVA_START) ? SKILL_NOVA : index);

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if (lpObj->Skill[n].IsSkill() != 0)
		{
			if (lpObj->Skill[n].m_skill == index)
			{
				return &lpObj->Skill[n];
			}
		}
	}

	return 0;
}

bool CSkillManager::ConvertSkillByte(CSkill* lpSkill, BYTE* lpMsg) // OK
{
	if (lpMsg[0] == SKILL_NONE)
	{
		return 0;
	}

	lpSkill->m_skill = lpMsg[0];
	lpSkill->m_level = lpMsg[1];

	return 1;
}

void CSkillManager::SkillByteConvert(BYTE* lpMsg, CSkill* lpSkill) // OK
{
	lpMsg[0] = lpSkill->m_skill;
	lpMsg[1] = lpSkill->m_level;
}

void CSkillManager::UseAttackSkill(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type == OBJECT_USER && this->CheckSkillRequireWeapon(lpObj, lpSkill->m_skill) == 0)
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TWISTING_SLASH && lpObj->Inventory[0].IsItem() == 0)
	{
		return;
	}

	bool combo = 0;

	if (gQuest.CheckQuestListState(lpObj, 3, QUEST_FINISH) != 0 && lpObj->ComboSkill.CheckCombo(lpSkill->m_skill) != 0)
	{
		if (gServerInfo.m_CheckAutoComboHack == 0 || (GetTickCount() - lpObj->ComboTime) > ((DWORD)gServerInfo.m_CheckAutoComboHackTolerance))
		{
			combo = 1;
			lpObj->ComboTime = GetTickCount();
		}
	}

	if (lpObj->SkillNovaState != 0 && lpSkill->m_skill != SKILL_NOVA && this->GetSkill(lpObj, SKILL_NOVA) != 0)
	{
		this->RunningSkill(aIndex, 0, this->GetSkill(lpObj, SKILL_NOVA), (BYTE)lpObj->X, (BYTE)lpObj->Y, 0, 0);
		return;
	}

	if (lpSkill->m_skill == SKILL_NOVA && bIndex != SKILL_NOVA_START)
	{
		this->RunningSkill(aIndex, bIndex, lpSkill, (BYTE)lpObj->X, (BYTE)lpObj->Y, 0, combo);
		return;
	}

	if (lpObj->Type != OBJECT_USER || (this->CheckSkillMana(lpObj, lpSkill->m_skill) != 0 && this->CheckSkillBP(lpObj, lpSkill->m_skill) != 0))
	{
		if (this->RunningSkill(aIndex, bIndex, lpSkill, (BYTE)lpObj->X, (BYTE)lpObj->Y, 0, combo) != 0 && lpObj->Type == OBJECT_USER)
		{
			lpObj->Mana -= ((this->GetSkillMana(lpSkill->m_skill) * lpObj->MPConsumptionRate) / 100);
			lpObj->BP -= ((this->GetSkillBP(lpSkill->m_skill) * lpObj->BPConsumptionRate) / 100);
			GCManaSend(aIndex, 0xFF, (int)lpObj->Mana, lpObj->BP);
		}
	}
}

void CSkillManager::UseDurationSkillAttack(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y, BYTE dir, BYTE angle) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type == OBJECT_USER && this->CheckSkillRequireWeapon(lpObj, lpSkill->m_skill) == 0)
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TWISTING_SLASH && lpObj->Inventory[0].IsItem() == 0)
	{
		return;
	}

	bool combo = 0;

	if (gQuest.CheckQuestListState(lpObj, 3, QUEST_FINISH) != 0 && lpObj->ComboSkill.CheckCombo(lpSkill->m_skill) != 0)
	{
		if (gServerInfo.m_CheckAutoComboHack == 0 || (GetTickCount() - lpObj->ComboTime) > ((DWORD)gServerInfo.m_CheckAutoComboHackTolerance))
		{
			combo = 1;
			lpObj->ComboTime = GetTickCount();
		}
	}

	if (lpObj->SkillNovaState != 0 && lpSkill->m_skill != SKILL_NOVA && this->GetSkill(lpObj, SKILL_NOVA) != 0)
	{
		this->RunningSkill(aIndex, 0, this->GetSkill(lpObj, SKILL_NOVA), x, y, 0, 0);
		return;
	}

	this->GCDurationSkillAttackSend(lpObj, lpSkill->m_skill, x, y, dir);

	if (lpObj->Type != OBJECT_USER || (this->CheckSkillMana(lpObj, lpSkill->m_skill) != 0 && this->CheckSkillBP(lpObj, lpSkill->m_skill) != 0))
	{
		if (this->RunningSkill(aIndex, bIndex, lpSkill, x, y, angle, combo) != 0 && lpObj->Type == OBJECT_USER)
		{
			lpObj->Mana -= ((this->GetSkillMana(lpSkill->m_skill) * lpObj->MPConsumptionRate) / 100);
			lpObj->BP -= ((this->GetSkillBP(lpSkill->m_skill) * lpObj->BPConsumptionRate) / 100);
			GCManaSend(aIndex, 0xFF, (int)lpObj->Mana, lpObj->BP);
		}
	}
}

bool CSkillManager::RunningSkill(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y, BYTE angle, bool combo) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	lpObj->ShieldDamageReductionTime = 0;

	gEffectManager.DelEffect(lpObj, EFFECT_INVISIBILITY);

	switch (lpSkill->m_skill)
	{
		case SKILL_FLAME:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_TWISTER:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_EVIL_SPIRIT:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_HELL_FIRE:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_AQUA_BEAM:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_BLAST:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_INFERNO:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_MANA_SHIELD:
			return this->SkillManaShield(aIndex, bIndex, lpSkill);
		case SKILL_DEFENSE:
			return this->SkillDefense(aIndex, bIndex, lpSkill);
		case SKILL_TRIPLE_SHOT:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_HEAL:
			return this->SkillHeal(aIndex, bIndex, lpSkill);
		case SKILL_GREATER_DEFENSE:
			return this->SkillGreaterDefense(aIndex, bIndex, lpSkill);
		case SKILL_GREATER_DAMAGE:
			return this->SkillGreaterDamage(aIndex, bIndex, lpSkill);
		case SKILL_SUMMON1:
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		case SKILL_SUMMON2:
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		case SKILL_SUMMON3:
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		case SKILL_SUMMON4:
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		case SKILL_SUMMON5:
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		case SKILL_SUMMON6:
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		case SKILL_SUMMON7:
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		case SKILL_DECAY:
			return this->SkillDecay(aIndex, bIndex, lpSkill, x, y);
		case SKILL_ICE_STORM:
			return this->SkillIceStorm(aIndex, bIndex, lpSkill, x, y);
		case SKILL_NOVA:
			return this->SkillNova(aIndex, bIndex, lpSkill);
		case SKILL_TWISTING_SLASH:
			return this->SkillTwistingSlash(aIndex, bIndex, lpSkill, combo);
		case SKILL_RAGEFUL_BLOW:
			return this->SkillRagefulBlow(aIndex, bIndex, lpSkill, combo);
		case SKILL_DEATH_STAB:
			return this->SkillDeathStab(aIndex, bIndex, lpSkill, combo);
		case SKILL_CRESCENT_MOON_SLASH:
			return this->SkillCastleSiege(aIndex, bIndex, lpSkill);
		case SKILL_MANA_GLAIVE:
			return this->SkillCastleSiege(aIndex, bIndex, lpSkill);
		case SKILL_STAR_FALL:
			return this->SkillCastleSiege(aIndex, bIndex, lpSkill);
		case SKILL_IMPALE:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_GREATER_LIFE:
			return this->SkillGreaterLife(aIndex, bIndex, lpSkill);
		case SKILL_MONSTER_AREA_ATTACK:
			return this->SkillMonsterAreaAttack(aIndex, bIndex, lpSkill);
		case SKILL_PENETRATION:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_FIRE_SLASH:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_POWER_SLASH:
			return this->SkillPowerSlash(aIndex, bIndex, lpSkill, angle);
		case SKILL_SPIRAL_SLASH:
			return this->SkillCastleSiege(aIndex, bIndex, lpSkill);
		case SKILL_FORCE:
			return this->SkillForce(aIndex, bIndex, lpSkill);
		case SKILL_FIRE_BURST:
			return this->SkillFireBurst(aIndex, bIndex, lpSkill);
		case SKILL_EARTHQUAKE:
			return this->SkillEarthquake(aIndex, bIndex, lpSkill);
		case SKILL_SUMMON_PARTY:
			return this->SkillSummonParty(aIndex, bIndex, lpSkill);
		case SKILL_GREATER_CRITICAL_DAMAGE:
			return this->SkillGreaterCriticalDamage(aIndex, bIndex, lpSkill);
		case SKILL_ELECTRIC_SPARK:
			return this->SkillElectricSpark(aIndex, bIndex, lpSkill, angle);
		case SKILL_STERN:
			return this->SkillStern(aIndex, bIndex, lpSkill, angle);
		case SKILL_REMOVE_STERN:
			return this->SkillRemoveStern(aIndex, bIndex, lpSkill);
		case SKILL_GREATER_MANA:
			return this->SkillGreaterMana(aIndex, bIndex, lpSkill);
		case SKILL_INVISIBILITY:
			return this->SkillInvisibility(aIndex, bIndex, lpSkill);
		case SKILL_REMOVE_INVISIBILITY:
			return this->SkillRemoveInvisibility(aIndex, bIndex, lpSkill);
		case SKILL_REMOVE_ALL_EFFECT:
			return this->SkillRemoveAllEffect(aIndex, bIndex, lpSkill);
		case SKILL_MANA_RAYS:
			return this->SkillCastleSiege(aIndex, bIndex, lpSkill);
		case SKILL_FIRE_BLAST:
			return this->SkillCastleSiege(aIndex, bIndex, lpSkill);
		case SKILL_BRAND:
			return this->SkillBrand(aIndex, bIndex, lpSkill);
		case SKILL_FIRE_SCREAM:
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		case SKILL_MONSTER_SUMMON:
			return this->SkillMonsterSummon(aIndex, bIndex, lpSkill);
		case SKILL_MAGIC_DAMAGE_IMMUNITY:
			return this->SkillMagicDamageImmunity(aIndex, bIndex, lpSkill);
		case SKILL_PHYSI_DAMAGE_IMMUNITY:
			return this->SkillPhysiDamageImmunity(aIndex, bIndex, lpSkill);
		default:
			return this->BasicSkillAttack(aIndex, bIndex, lpSkill);
	}

	return 0;
}

bool CSkillManager::BasicSkillAttack(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return 1;
	}

	if (lpObj->Type == OBJECT_USER && this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[bIndex].X, gObj[bIndex].Y) == 0)
	{
		return 0;
	}

	gAttack.Attack(lpObj, &gObj[bIndex], lpSkill, 1, 0, 0, 0, 0);

	return 1;
}

bool CSkillManager::MultiSkillAttack(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type == OBJECT_USER)
	{
		lpObj->MultiSkillIndex = lpSkill->m_skill;
		lpObj->MultiSkillCount = 0;

		switch (lpSkill->m_skill)
		{
			case SKILL_FLAME:
			case SKILL_BLAST:
			case SKILL_IMPALE:
			case SKILL_FIRE_SLASH:
			case SKILL_FIRE_SCREAM:
				this->BasicSkillAttack(aIndex, bIndex, lpSkill);
				break;
		}
	}
	else
	{
		switch (lpSkill->m_skill)
		{
			case SKILL_EVIL_SPIRIT:
				this->SkillEvilSpirit(aIndex, bIndex, lpSkill);
				break;
		}
	}

	return 1;
}

bool CSkillManager::SkillChangeUse(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	int change = -1;

	for (int n = 0; n < 2; n++)
	{
		CItem* Ring = &lpObj->Inventory[10 + n];

		if (Ring->IsItem() == 0 || Ring->m_Durability == 0 || change != -1)
		{
			continue;
		}

		if (Ring->m_Index == GET_ITEM(13, 10)) // Transformation Ring
		{
			change = Ring->m_Level;
		}
	}

	switch (change)
	{
		case 0:
			change = gServerInfo.m_TransformationRing1;
			break;
		case 1:
			change = gServerInfo.m_TransformationRing2;
			break;
		case 2:
			change = gServerInfo.m_TransformationRing3;
			break;
		case 3:
			change = gServerInfo.m_TransformationRing4;
			break;
		case 4:
			change = gServerInfo.m_TransformationRing5;
			break;
		case 5:
			change = gServerInfo.m_TransformationRing6;
			break;
	}

	if (lpObj->Change == change)
	{
		return 0;
	}
	else
	{
		lpObj->Change = change; return 1;
	}
}

bool CSkillManager::SkillEvilSpirit(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillManaShield(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->PartyNumber != lpTarget->PartyNumber)
	{
		return 0;
	}

	if (CC_MAP_RANGE(lpObj->Map) != 0 && aIndex != bIndex)
	{
		return 0;
	}

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
	{
		return 0;
	}

	int value1 = gServerInfo.m_ManaShieldConstA + ((lpObj->Dexterity + lpObj->AddDexterity) / gServerInfo.m_ManaShieldConstB) + ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_ManaShieldConstC);
	
	value1 = ((value1 > gServerInfo.m_ManaShieldMaxRate) ? gServerInfo.m_ManaShieldMaxRate : value1);
	value1 = (value1 * ((lpTarget->Type == OBJECT_USER) ? gServerInfo.m_ManaShieldRate[lpTarget->Class] : 100)) / 100;

	int value2 = 0;
	int count = gServerInfo.m_ManaShieldTimeConstA + ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_ManaShieldTimeConstB);
	
	gEffectManager.AddEffect(lpTarget, 0, this->GetSkillEffect(lpSkill->m_skill), count, value1, value2, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, bIndex, 1);
	return 1;
}

bool CSkillManager::SkillDefense(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Inventory[1].IsItem() != 0)
	{
		if (lpObj->Inventory[1].m_Index >= GET_ITEM(6, 0) && lpObj->Inventory[1].m_Index < GET_ITEM(7, 0))
		{
			if (lpObj->Inventory[1].m_Option1 != 0)
			{
				lpObj->ShieldDamageReductionTime = GetTickCount();
				GCActionSend(lpObj, SKILL_DEFENSE, lpObj->Index, bIndex);
				return 1;
			}
		}
	}

	return 0;
}

bool CSkillManager::SkillHeal(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (CC_MAP_RANGE(lpObj->Map) != 0 && aIndex != bIndex)
	{
		return 0;
	}

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
	{
		return 0;
	}

	int value = gServerInfo.m_HealConstA + ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_HealConstB);

#if(GAMESERVER_UPDATE>=602)

	value += (value * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_HEAL_IMPROVED)) / 100;

#elif(GAMESERVER_UPDATE>=401)

	value += (value * gMasterSkillTree.GetMasterSkillValue(lpObj, lpSkill->m_skill)) / 100;

#endif

	if ((lpTarget->Life + value) > (lpTarget->MaxLife + lpTarget->AddLife))
	{
		lpTarget->Life = lpTarget->MaxLife + lpTarget->AddLife;
	}
	else
	{
		lpTarget->Life += value;
	}

	if (lpTarget->Type == OBJECT_USER)
	{
		GCLifeSend(bIndex, 0xFF, (int)lpTarget->Life);
	}

	if (lpTarget->Type == OBJECT_MONSTER && OBJECT_RANGE(lpTarget->SummonIndex) != 0)
	{
		GCSummonLifeSend(lpTarget->SummonIndex, (int)lpTarget->Life, (int)lpTarget->MaxLife);
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != VIEWPORT_NONE && lpObj->VpPlayer2[n].type == OBJECT_MONSTER && gObj[lpObj->VpPlayer2[n].index].CurrentAI != 0)
		{
			gObj[lpObj->VpPlayer2[n].index].Agro.IncAgro(lpObj->Index, (value / 40));
		}
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, bIndex, 1);
	return 1;
}

bool CSkillManager::SkillGreaterDefense(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (CC_MAP_RANGE(lpObj->Map) != 0 && aIndex != bIndex)
	{
		return 0;
	}

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
	{
		return 0;
	}

	int value = gServerInfo.m_GreaterDefenseConstA + ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_GreaterDefenseConstB);

#if(GAMESERVER_UPDATE>=602)

	value += (value * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_DEFENSE_IMPROVED)) / 100;

	value += (value * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_DEFENSE_ENHANCED)) / 100;

#elif(GAMESERVER_UPDATE>=401)

	value += (value * gMasterSkillTree.GetMasterSkillValue(lpObj, lpSkill->m_skill)) / 100;

#endif

	value = (value * ((lpTarget->Type == OBJECT_USER) ? gServerInfo.m_GreaterDefenseRate[lpTarget->Class] : 100)) / 100;

	int count = gServerInfo.m_GreaterDefenseTimeConstA;

#if(GAMESERVER_UPDATE>=602)

	count += gMasterSkillTree.GetMasterSkillLevel(lpObj, MASTER_SKILL_ADD_GREATER_DEFENSE_ENHANCED);

#endif

	gEffectManager.AddEffect(lpTarget, 0, this->GetSkillEffect(lpSkill->m_skill), count, (value * 2), 0, 0, 0);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != VIEWPORT_NONE && lpObj->VpPlayer2[n].type == OBJECT_MONSTER && gObj[lpObj->VpPlayer2[n].index].CurrentAI != 0)
		{
			gObj[lpObj->VpPlayer2[n].index].Agro.IncAgro(lpObj->Index, (value / 10));
		}
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, bIndex, 1);
	return 1;
}

bool CSkillManager::SkillGreaterDamage(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (CC_MAP_RANGE(lpObj->Map) != 0 && aIndex != bIndex)
	{
		return 0;
	}

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
	{
		return 0;
	}

	int value = gServerInfo.m_GreaterDamageConstA + ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_GreaterDamageConstB);

#if(GAMESERVER_UPDATE>=602)

	value += (value * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_DAMAGE_IMPROVED)) / 100;

	value += (value * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_DAMAGE_ENHANCED)) / 100;

#elif(GAMESERVER_UPDATE>=401)

	value += (value * gMasterSkillTree.GetMasterSkillValue(lpObj, lpSkill->m_skill)) / 100;

#endif

	value = (value * ((lpTarget->Type == OBJECT_USER) ? gServerInfo.m_GreaterDamageRate[lpTarget->Class] : 100)) / 100;

	int count = gServerInfo.m_GreaterDamageTimeConstA;

#if(GAMESERVER_UPDATE>=602)

	count += gMasterSkillTree.GetMasterSkillLevel(lpObj, MASTER_SKILL_ADD_GREATER_DAMAGE_ENHANCED);

#endif

	gEffectManager.AddEffect(lpTarget, 0, this->GetSkillEffect(lpSkill->m_skill), count, value, 0, 0, 0);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != VIEWPORT_NONE && lpObj->VpPlayer2[n].type == OBJECT_MONSTER && gObj[lpObj->VpPlayer2[n].index].CurrentAI != 0)
		{
			gObj[lpObj->VpPlayer2[n].index].Agro.IncAgro(lpObj->Index, (value / 10));
		}
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, bIndex, 1);
	return 1;
}

bool CSkillManager::SkillSummon(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map == MAP_ICARUS || CC_MAP_RANGE(lpObj->Map) != 0)
	{
		return 0;
	}

	int monster = 0;

	switch (lpSkill->m_skill)
	{
		case SKILL_SUMMON1:
			monster = gServerInfo.m_SummonMonster1;
			break;
		case SKILL_SUMMON2:
			monster = gServerInfo.m_SummonMonster2;
			break;
		case SKILL_SUMMON3:
			monster = gServerInfo.m_SummonMonster3;
			break;
		case SKILL_SUMMON4:
			monster = gServerInfo.m_SummonMonster4;
			break;
		case SKILL_SUMMON5:
			monster = gServerInfo.m_SummonMonster5;
			break;
		case SKILL_SUMMON6:
			monster = gServerInfo.m_SummonMonster6;
			break;
		case SKILL_SUMMON7:
			monster = gServerInfo.m_SummonMonster7;
			break;
		default:
			return 0;
	}

	if (OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(aIndex);
		GCSummonLifeSend(aIndex, 0, 1);
		return 0;
	}

	int index = gObjAddSummon();

	if (OBJECT_RANGE(index) == 0)
	{
		return 0;
	}

	lpObj->SummonIndex = index;

	LPOBJ lpSummon = &gObj[index];

	lpSummon->X = lpObj->X - 1;
	lpSummon->Y = lpObj->Y + 1;
	lpSummon->MTX = lpSummon->X;
	lpSummon->MTY = lpSummon->Y;
	lpSummon->Dir = 2;
	lpSummon->Map = lpObj->Map;

	gObjSetMonster(index, monster);

#if(GAMESERVER_UPDATE>=602)
	lpSummon->Life += (float)((__int64)lpSummon->Life * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
	lpSummon->MaxLife += (float)((__int64)lpSummon->MaxLife * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
	lpSummon->ScriptMaxLife += (float)((__int64)lpSummon->ScriptMaxLife * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
	lpSummon->Defense += ((__int64)lpSummon->Defense * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DEFENSE)) / 100;
	lpSummon->PhysiDamageMin += ((__int64)lpSummon->PhysiDamageMin * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DAMAGE)) / 100;
	lpSummon->PhysiDamageMax += ((__int64)lpSummon->PhysiDamageMax * gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DAMAGE)) / 100;
#endif
	lpSummon->SummonIndex = aIndex;
	lpSummon->Attribute = 100;
	lpSummon->TargetNumber = -1;
	lpSummon->ActionState.Attack = 0;
	lpSummon->ActionState.Emotion = 0;
	lpSummon->ActionState.EmotionCount = 0;
	lpSummon->PathCount = 0;
	lpSummon->MoveRange = 15;

	GCSummonLifeSend(lpSummon->SummonIndex, (int)lpSummon->Life, (int)lpSummon->MaxLife);
	return 1;
}

bool CSkillManager::SkillDecay(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, x, y) == 0)
	{
		return 0;
	}

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, x, y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillIceStorm(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, x, y) == 0)
	{
		return 0;
	}

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, x, y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillNova(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

#if(GAMESERVER_UPDATE>=602)
	if (bIndex != SKILL_NOVA_START && bIndex != MASTER_SKILL_ADD_NOVA_START_IMPROVED)
	#else
	if (bIndex != SKILL_NOVA_START)
	#endif
	{
		if (lpObj->SkillNovaState == 0)
		{
			return 0;
		}
	}
	else
	{
		if (lpObj->SkillNovaState != 0)
		{
			return 0;
		}

		lpObj->SkillNovaState = 1;
		lpObj->SkillNovaCount = 0;
		lpObj->SkillNovaTime = GetTickCount();
		this->GCSkillAttackSend(lpObj, bIndex, aIndex, 1);
		return 1;
	}

	lpObj->SkillNovaState = 0;

	lpObj->SkillNovaTime = 0;

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gObjAddAttackProcMsgSendDelay(lpObj, 50, index, 600, lpSkill->m_skill, 0);

		if (CHECK_SKILL_ATTACK_EXTENDED_COUNT(count) == 0)
		{
			break;
		}
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);
	return 1;
}

bool CSkillManager::SkillTwistingSlash(int aIndex, int bIndex, CSkill* lpSkill, bool combo) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, combo);

		if (combo != 0)
		{
			this->GCSkillAttackSend(lpObj, SKILL_COMBO, index, 1);
		}

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillRagefulBlow(int aIndex, int bIndex, CSkill* lpSkill, bool combo) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gObjAddAttackProcMsgSendDelay(lpObj, 50, index, 500, lpSkill->m_skill, combo);

		if (combo != 0)
		{
			this->GCSkillAttackSend(lpObj, SKILL_COMBO, index, 1);
		}

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillDeathStab(int aIndex, int bIndex, CSkill* lpSkill, bool combo) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
	{
		return 0;
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, bIndex, 1);

	gAttack.Attack(lpObj, lpTarget, lpSkill, 0, 0, 0, 0, combo);

	if (combo != 0)
	{
		this->GCSkillAttackSend(lpObj, SKILL_COMBO, bIndex, 1);
	}

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		if (lpObj->VpPlayer2[n].index == bIndex)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpTarget->X, lpTarget->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, combo);

		if (combo != 0)
		{
			this->GCSkillAttackSend(lpObj, SKILL_COMBO, index, 1);
		}

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillGreaterLife(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	int value1 = gServerInfo.m_GreaterLifeConstA + ((lpObj->Vitality + lpObj->AddVitality) / gServerInfo.m_GreaterLifeConstB) + ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_GreaterLifeConstC);

#if(GAMESERVER_UPDATE>=602)

	value1 += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_LIFE_IMPROVED);

#elif(GAMESERVER_UPDATE>=401)

	value1 += gMasterSkillTree.GetMasterSkillValue(lpObj, lpSkill->m_skill);

#endif

	value1 = ((value1 > gServerInfo.m_GreaterLifeMaxRate) ? gServerInfo.m_GreaterLifeMaxRate : value1);

#if(GAMESERVER_UPDATE>=602)

	int value2 = gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_LIFE_ENHANCED);

	int value3 = gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_LIFE_MASTERED);

#else

	int value2 = 0;

	int value3 = 0;

#endif

	int count = gServerInfo.m_GreaterLifeTimeConstA + ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_GreaterLifeTimeConstB);

	if (OBJECT_RANGE(lpObj->PartyNumber) == 0)
	{
		value1 = (value1 * ((lpObj->Type == OBJECT_USER) ? gServerInfo.m_GreaterLifeRate[lpObj->Class] : 100)) / 100;

		gEffectManager.AddEffect(lpObj, 0, this->GetSkillEffect(lpSkill->m_skill), count, value1, value2, value3, 0);

		this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);
	}
	else
	{
		PARTY_INFO* lpParty = &gParty.m_PartyInfo[lpObj->PartyNumber];

		for (int n = 0; n < MAX_PARTY_USER; n++)
		{
			int index = lpParty->Index[n];

			if (OBJECT_RANGE(index) == 0)
			{
				continue;
			}

			if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
			{
				continue;
			}

			value1 = (value1 * ((gObj[index].Type == OBJECT_USER) ? gServerInfo.m_GreaterLifeRate[gObj[index].Class] : 100)) / 100;

			gEffectManager.AddEffect(&gObj[index], 0, this->GetSkillEffect(lpSkill->m_skill), count, value1, value2, value3, 0);

			this->GCSkillAttackSend(lpObj, lpSkill->m_skill, index, 1);
		}
	}

	return 1;
}

bool CSkillManager::SkillMonsterAreaAttack(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, 0);

		if (CHECK_SKILL_ATTACK_EXTENDED_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillPowerSlash(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int SkillFrustrumX[4], SkillFrustrumY[4];

	this->GetSkillFrustrum(SkillFrustrumX, SkillFrustrumY, angle, lpObj->X, lpObj->Y, 6.0f, 6.0f, 1.0f, 0.0f);

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		if (this->CheckSkillFrustrum(SkillFrustrumX, SkillFrustrumY, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillForce(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
	{
		return 0;
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, bIndex, 1);

	if (lpObj->SkillLongSpearChange == 0)
	{
		gAttack.Attack(lpObj, lpTarget, lpSkill, 0, 0, 0, 0, 0);
		return 1;
	}

	int angle = this->GetSkillAngle(lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y);

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (gSkillHitBox.HitCheck(angle, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillFireBurst(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
	{
		return 0;
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, bIndex, 1);

	gAttack.Attack(lpObj, lpTarget, lpSkill, 0, 0, 0, 0, 0);

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		if (lpObj->VpPlayer2[n].index == bIndex)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpTarget->X, lpTarget->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, 0, 0, 0, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillEarthquake(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gObjAddAttackProcMsgSendDelay(lpObj, 50, index, 500, lpSkill->m_skill, 0);

		if (CHECK_SKILL_ATTACK_EXTENDED_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillSummonParty(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	if (OBJECT_RANGE(lpObj->PartyNumber) != 0)
	{
		PARTY_INFO* lpParty = &gParty.m_PartyInfo[lpObj->PartyNumber];

		for (int n = 0; n < MAX_PARTY_USER; n++)
		{
			int index = lpParty->Index[n];

			if (OBJECT_RANGE(index) == 0 || aIndex == index)
			{
				continue;
			}

			if (gMoveSummon.CheckMoveSummon(&gObj[index], lpObj->Map, lpObj->X, lpObj->Y) == 0)
			{
				gNotice.GCNoticeSend(index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(272));
				continue;
			}

		#if(GAMESERVER_TYPE==1)

			if (lpObj->Map == MAP_CASTLE_SIEGE && gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE && lpObj->CsJoinSide != gObj[index].CsJoinSide)
			{
				gNotice.GCNoticeSend(index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(272));
				continue;
			}

		#endif

			int px = lpObj->X;

			int py = lpObj->Y;

			if (gObjGetRandomFreeLocation(lpObj->Map, &px, &py, 4, 4, 10) == 0)
			{
				gObj[index].SkillSummonPartyTime = 5;

				gObj[index].SkillSummonPartyMap = lpObj->Map;

				gObj[index].SkillSummonPartyX = (BYTE)lpObj->X;

				gObj[index].SkillSummonPartyY = (BYTE)lpObj->Y;

				gNotice.GCNoticeSend(index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(273), gObj[index].SkillSummonPartyTime);
			}
			else
			{
				gObj[index].SkillSummonPartyTime = 5;

				gObj[index].SkillSummonPartyMap = lpObj->Map;

				gObj[index].SkillSummonPartyX = (BYTE)px;

				gObj[index].SkillSummonPartyY = (BYTE)py;

				gNotice.GCNoticeSend(index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(273), gObj[index].SkillSummonPartyTime);
			}
		}
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);
	return 1;
}

bool CSkillManager::SkillGreaterCriticalDamage(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	int value1 = ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_GreaterCriticalDamageConstA) + ((lpObj->Leadership + lpObj->AddLeadership) / gServerInfo.m_GreaterCriticalDamageConstB);

#if(GAMESERVER_UPDATE>=602)

	value1 += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_CRITICAL_DAMAGE_IMPROVED);

	int value2 = gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_CRITICAL_DAMAGE_MASTERED);

	int value3 = gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_CRITICAL_DAMAGE_EXTENDED);

#else

	int value2 = 0;

	int value3 = 0;

#endif

	int count = gServerInfo.m_GreaterCriticalDamageTimeConstA + ((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_GreaterCriticalDamageTimeConstB);

#if(GAMESERVER_UPDATE>=602)

	count += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_GREATER_CRITICAL_DAMAGE_ENHANCED);

#endif

	if (OBJECT_RANGE(lpObj->PartyNumber) == 0)
	{
		gEffectManager.AddEffect(lpObj, 0, this->GetSkillEffect(lpSkill->m_skill), count, value1, value2, value3, 0);

		this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);
	}
	else
	{
		PARTY_INFO* lpParty = &gParty.m_PartyInfo[lpObj->PartyNumber];

		for (int n = 0; n < MAX_PARTY_USER; n++)
		{
			int index = lpParty->Index[n];

			if (OBJECT_RANGE(index) == 0)
			{
				continue;
			}

			if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
			{
				continue;
			}

			gEffectManager.AddEffect(&gObj[index], 0, this->GetSkillEffect(lpSkill->m_skill), count, value1, value2, value3, 0);

			this->GCSkillAttackSend(lpObj, lpSkill->m_skill, index, 1);
		}
	}

	return 1;
}

bool CSkillManager::SkillElectricSpark(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int x = lpObj->X - (8 - (angle & 15));
	int y = lpObj->Y - (8 - ((angle & 240) / 16));

	int nangle = this->GetSkillAngle(lpObj->X, lpObj->Y, x, y);

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (gSkillHitBox.HitCheck(nangle, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gObjAddAttackProcMsgSendDelay(lpObj, 50, index, 1000, lpSkill->m_skill, 0);

		if (CHECK_SKILL_ATTACK_EXTENDED_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillCastleSiege(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != MAP_CASTLE_SIEGE && lpObj->Map != MAP_LAND_OF_TRIALS)
	{
		return 0;
	}

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[bIndex].X, gObj[bIndex].Y) == 0)
	{
		return 0;
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, bIndex, 1);

	if (lpSkill->m_skill == SKILL_CRESCENT_MOON_SLASH || lpSkill->m_skill == SKILL_SPIRAL_SLASH || lpSkill->m_skill == SKILL_MANA_RAYS)
	{
		gAttack.Attack(lpObj, &gObj[bIndex], lpSkill, 0, 0, 0, 0, 0);
	}
	else if (lpSkill->m_skill == SKILL_MANA_GLAIVE || lpSkill->m_skill == SKILL_FIRE_BLAST)
	{
		gObjAddAttackProcMsgSendDelay(lpObj, 50, bIndex, 500, lpSkill->m_skill, 0);
	}
	else if (lpSkill->m_skill == SKILL_MANA_GLAIVE || lpSkill->m_skill == SKILL_STAR_FALL)
	{
		gObjAddAttackProcMsgSendDelay(lpObj, 50, bIndex, 500, lpSkill->m_skill, 0);
	}
	else if (lpSkill->m_skill == SKILL_SPIRAL_SLASH)
	{
		gObjAddAttackProcMsgSendDelay(lpObj, 50, bIndex, 500, lpSkill->m_skill, 0);
	}

	return 1;
}

bool CSkillManager::SkillStern(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != MAP_CASTLE_SIEGE && lpObj->Map != MAP_LAND_OF_TRIALS)
	{
		return 0;
	}

	int x = lpObj->X - (8 - (angle & 15));
	int y = lpObj->Y - (8 - ((angle & 240) / 16));

	int nangle = this->GetSkillAngle(lpObj->X, lpObj->Y, x, y);

	gObjSetKillCount(aIndex, 0);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		if (gSkillHitBox.HitCheck(nangle, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) != 0)
		{
			if (gObj[index].Teleport == 0 && gEffectManager.CheckEffect(&gObj[index], EFFECT_IRON_DEFENSE) == 0 && gEffectManager.CheckEffect(&gObj[index], EFFECT_IRON_DEFENSE_IMPROVED) == 0)
			{
				gEffectManager.AddEffect(&gObj[index], 0, EFFECT_STERN, gServerInfo.m_SternTimeConstA, 0, 0, 0, 0);

				gObjSetPosition(index, gObj[index].X, gObj[index].Y);

				this->GCSkillAttackSend(lpObj, lpSkill->m_skill, index, 1);
			}
		}
	}

	return 1;
}

bool CSkillManager::SkillRemoveStern(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != MAP_CASTLE_SIEGE && lpObj->Map != MAP_LAND_OF_TRIALS)
	{
		return 0;
	}

	gObjSetKillCount(aIndex, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) != 0)
		{
			gEffectManager.DelEffect(&gObj[index], EFFECT_STERN);

			this->GCSkillAttackSend(lpObj, lpSkill->m_skill, index, 1);
		}
	}

	return 1;
}

bool CSkillManager::SkillGreaterMana(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != MAP_CASTLE_SIEGE && lpObj->Map != MAP_LAND_OF_TRIALS)
	{
		return 0;
	}

	gObjSetKillCount(aIndex, 0);

	gEffectManager.AddEffect(lpObj, 0, this->GetSkillEffect(lpSkill->m_skill), gServerInfo.m_GreaterManaTimeConstA, gServerInfo.m_GreaterManaConstA, 0, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (lpObj->GuildNumber <= 0 || gObj[index].GuildNumber <= 0)
		{
			continue;
		}

		if (lpObj->GuildNumber != gObj[index].GuildNumber)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) != 0)
		{
			gEffectManager.AddEffect(&gObj[index], 0, this->GetSkillEffect(lpSkill->m_skill), gServerInfo.m_GreaterManaTimeConstA, gServerInfo.m_GreaterManaConstA, 0, 0, 0);

			this->GCSkillAttackSend(lpObj, lpSkill->m_skill, index, 1);
		}
	}

	return 1;
}

bool CSkillManager::SkillInvisibility(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != MAP_CASTLE_SIEGE && lpObj->Map != MAP_LAND_OF_TRIALS)
	{
		return 0;
	}

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER)
	{
		return 0;
	}

	if (this->CheckSkillRange(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
	{
		return 0;
	}

	gObjSetKillCount(aIndex, 0);

	gEffectManager.AddEffect(lpTarget, 0, EFFECT_INVISIBILITY, gServerInfo.m_InvisibilityTimeConstA, 0, 0, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);
	return 1;
}

bool CSkillManager::SkillRemoveInvisibility(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != MAP_CASTLE_SIEGE && lpObj->Map != MAP_LAND_OF_TRIALS)
	{
		return 0;
	}

	gObjSetKillCount(aIndex, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) != 0)
		{
			gEffectManager.DelEffect(&gObj[index], EFFECT_INVISIBILITY);

			this->GCSkillAttackSend(lpObj, lpSkill->m_skill, index, 1);
		}
	}

	return 1;
}

bool CSkillManager::SkillRemoveAllEffect(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != MAP_CASTLE_SIEGE && lpObj->Map != MAP_LAND_OF_TRIALS)
	{
		return 0;
	}

	gObjSetKillCount(aIndex, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) != 0)
		{
			gEffectManager.ClearAllEffect(&gObj[index]);

			this->GCSkillAttackSend(lpObj, lpSkill->m_skill, index, 1);
		}
	}

	return 1;
}

bool CSkillManager::SkillBrand(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != MAP_CASTLE_SIEGE && lpObj->Map != MAP_LAND_OF_TRIALS)
	{
		return 0;
	}

	gObjSetKillCount(aIndex, 0);

	gEffectManager.AddEffect(lpObj, 0, EFFECT_GUILD_STATE5, gServerInfo.m_BrandTimeConstA, 0, 0, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) != 0)
		{
			gEffectManager.AddEffect(&gObj[index], 0, EFFECT_GUILD_STATE5, gServerInfo.m_BrandTimeConstA, 0, 0, 0, 0);

			this->GCSkillAttackSend(lpObj, lpSkill->m_skill, index, 1);
		}
	}

	return 1;
}

bool CSkillManager::SkillPlasmaStorm(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if (this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y) == 0)
		{
			continue;
		}

		gObjAddAttackProcMsgSendDelay(lpObj, 53, index, 500, lpSkill->m_skill, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return 1;
}

bool CSkillManager::SkillInfinityArrow(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER || aIndex != bIndex)
	{
		return 0;
	}

#if(GAMESERVER_UPDATE>=602)

	int value = gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_INFINITY_ARROW_IMPROVED);

#else

	int value = 0;

#endif

	int count = gServerInfo.m_InfinityArrowTimeConstA;

	gEffectManager.AddEffect(lpObj, 0, this->GetSkillEffect(lpSkill->m_skill), count, value, 0, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, aIndex, 1);
	return 1;
}

bool CSkillManager::SkillMonsterSummon(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if ((GetLargeRand() % 10) == 0)
	{
		return 0;
	}

	int MonsterClass = 0;

	switch (lpObj->Class)
	{
		case 161:
			MonsterClass = 147 + GetLargeRand() % 2;
			break;
		case 181:
			MonsterClass = 177 + GetLargeRand() % 2;
			break;
		case 189:
			MonsterClass = 185 + GetLargeRand() % 2;
			break;
		case 197:
			MonsterClass = 193 + GetLargeRand() % 2;
			break;
		case 267:
			MonsterClass = 263 + GetLargeRand() % 2;
			break;
		case 275:
			MonsterClass = 271 + GetLargeRand() % 2;
			break;
		default:
			return 0;
	}

	bool success = 0;

	int x = lpObj->X;
	int y = lpObj->Y;

	if (gObjGetRandomFreeLocation(lpObj->Map, &x, &y, 2, 2, 10) != 0)
	{
		int index = gObjAddMonster(lpObj->Map);

		if (OBJECT_RANGE(index) != 0)
		{
			LPOBJ lpSummon = &gObj[index];

			lpSummon->PosNum = -1;
			lpSummon->X = x;
			lpSummon->Y = y;
			lpSummon->TX = x;
			lpSummon->TY = y;
			lpSummon->OldX = x;
			lpSummon->OldY = y;
			lpSummon->StartX = x;
			lpSummon->StartY = y;
			lpSummon->Dir = GetLargeRand() % 8;
			lpSummon->Map = lpObj->Map;

			gObjSetMonster(index, MonsterClass);

			lpSummon->Attribute = 60;
			lpSummon->MaxRegenTime = 0;

			success = 1;
		}
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, ((y * 256) + x), success);
	return success;
}

bool CSkillManager::SkillMagicDamageImmunity(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	gEffectManager.AddEffect(lpObj, 0, EFFECT_MAGIC_DAMAGE_IMMUNITY, gServerInfo.m_MagicDamageImmunityTimeConstA, 0, 0, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, lpObj->Index, 1);
	return 1;
}

bool CSkillManager::SkillPhysiDamageImmunity(int aIndex, int bIndex, CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	gEffectManager.AddEffect(lpObj, 0, EFFECT_PHYSI_DAMAGE_IMMUNITY, gServerInfo.m_PhysiDamageImmunityTimeConstA, 0, 0, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_skill, lpObj->Index, 1);
	return 1;
}

bool CSkillManager::SkillSwordPowerGetDefense(int aIndex, int* defense) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_IMPROVED) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_ENHANCED) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_MASTERED) == 0)
	{
		return 0;
	}

	int value = lpObj->EffectOption.AddSwordPowerDefenseRate;

	(*defense) -= (((lpObj->Dexterity + lpObj->AddDexterity) / gServerInfo.m_SUDefenseConstA) * value) / 100;

	return 1;
}

bool CSkillManager::SkillSwordPowerGetPhysiDamage(int aIndex, int* DamageMin, int* DamageMax) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_IMPROVED) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_ENHANCED) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_MASTERED) == 0)
	{
		return 0;
	}

	int value = lpObj->EffectOption.AddSwordPowerDamageRate;

	value = ((value > gServerInfo.m_SwordPowerMaxRate) ? gServerInfo.m_SwordPowerMaxRate : value);

	(*DamageMin) += ((((lpObj->Strength + lpObj->AddStrength) + (lpObj->Dexterity + lpObj->AddDexterity)) / gServerInfo.m_SUPhysiDamageMinConstA) * value) / 100;

	(*DamageMax) += ((((lpObj->Strength + lpObj->AddStrength) + (lpObj->Dexterity + lpObj->AddDexterity)) / gServerInfo.m_SUPhysiDamageMaxConstA) * value) / 100;

#if(GAMESERVER_UPDATE>=602)

	(*DamageMin) += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SWORD_POWER_MASTERED);

	(*DamageMax) += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SWORD_POWER_MASTERED);

#endif

	return 1;
}

bool CSkillManager::SkillSwordPowerGetMagicDamage(int aIndex, int* DamageMin, int* DamageMax) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_IMPROVED) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_ENHANCED) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_MASTERED) == 0)
	{
		return 0;
	}

	int value = lpObj->EffectOption.AddSwordPowerDamageRate;

#if(GAMESERVER_UPDATE>=602)

	value += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SWORD_POWER_ENHANCED);

#endif

	value = ((value > gServerInfo.m_SwordPowerMaxRate) ? gServerInfo.m_SwordPowerMaxRate : value);

	(*DamageMin) += (((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_SUMagicDamageMinConstA) * value) / 100;

	(*DamageMax) += (((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_SUMagicDamageMaxConstA) * value) / 100;

#if(GAMESERVER_UPDATE>=602)

	(*DamageMin) += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SWORD_POWER_MASTERED);

	(*DamageMax) += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SWORD_POWER_MASTERED);

#endif

	return 1;
}

bool CSkillManager::SkillSwordPowerGetCurseDamage(int aIndex, int* DamageMin, int* DamageMax) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_IMPROVED) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_ENHANCED) == 0 && gEffectManager.CheckEffect(lpObj, EFFECT_SWORD_POWER_MASTERED) == 0)
	{
		return 0;
	}

	int value = lpObj->EffectOption.AddSwordPowerDamageRate;

#if(GAMESERVER_UPDATE>=602)

	value += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SWORD_POWER_IMPROVED);

#endif

	value = ((value > gServerInfo.m_SwordPowerMaxRate) ? gServerInfo.m_SwordPowerMaxRate : value);

	(*DamageMin) += (((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_SUMagicDamageMinConstA) * value) / 100;

	(*DamageMax) += (((lpObj->Energy + lpObj->AddEnergy) / gServerInfo.m_SUMagicDamageMaxConstA) * value) / 100;

#if(GAMESERVER_UPDATE>=602)

	(*DamageMin) += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SWORD_POWER_MASTERED);

	(*DamageMax) += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SWORD_POWER_MASTERED);

#endif

	return 1;
}

void CSkillManager::ApplyMeteoriteEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
#if(GAMESERVER_UPDATE>=602)

	if ((GetLargeRand() % 100) < (gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_METEORITE_ENHANCED) - lpTarget->ResistStunRate))
	{
		gEffectManager.AddEffect(lpTarget, 0, EFFECT_STERN, 3, 0, 0, 0, 0);
	}

#endif
}

void CSkillManager::ApplyIceStormEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
#if(GAMESERVER_UPDATE>=602)

	if ((GetLargeRand() % 100) < gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_ICE_STORM_ENHANCED))
	{
		gEffectManager.AddEffect(lpTarget, 0, EFFECT_ICE_STORM_ENHANCED, 3, 0, 0, 0, 0);
	}
	else
	{
		gEffectManager.AddEffect(lpTarget, 0, gSkillManager.GetSkillEffect(lpSkill->m_index), 10, 0, 0, 0, 0);
	}

#else

	gEffectManager.AddEffect(lpTarget, 0, gSkillManager.GetSkillEffect(lpSkill->m_skill), 10, 0, 0, 0, 0);

#endif
}

void CSkillManager::ApplyTwistingSlashEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
#if(GAMESERVER_UPDATE>=602)

	if ((GetLargeRand() % 100) < gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_TWISTING_SLASH_ENHANCED))
	{
		gObjAddMsgSendDelay(lpTarget, 2, lpObj->Index, 150, 0);
	}

#endif
}

void CSkillManager::ApplyRagefulBlowEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
#if(GAMESERVER_UPDATE>=602)

	if ((GetLargeRand() % 100) < gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_RAGEFUL_BLOW_ENHANCED))
	{
		if (lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
		{
			CItem* lpItem = &lpTarget->Inventory[(damage = (2 + (GetLargeRand() % 5)))];

			if (lpItem->IsItem() != 0 && lpItem->IsLuckyItem() == 0 && lpItem->m_IsPeriodicItem == 0)
			{
				lpItem->m_Durability = (lpItem->m_Durability * 90) / 100;
				gItemManager.GCItemDurSend(lpTarget->Index, damage, (BYTE)lpItem->m_Durability, 0);
			}
		}
	}

#endif
}

void CSkillManager::ApplyDeathStabEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
#if(GAMESERVER_UPDATE>=602)

	if ((GetLargeRand() % 100) < (gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_DEATH_STAB_MASTERED) - lpTarget->ResistStunRate))
	{
		gEffectManager.AddEffect(lpTarget, 0, EFFECT_STERN, 2, 0, 0, 0, 0);
	}
	else if ((GetLargeRand() % 100) < gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_DEATH_STAB_ENHANCED))
	{
		damage = ((lpObj->Strength + lpObj->AddStrength) * 10) / 100;
		gEffectManager.AddEffect(lpTarget, 0, EFFECT_DEATH_STAB_ENHANCED, 10, lpObj->Index, 1, SET_NUMBERHW(damage), SET_NUMBERLW(damage));
	}

#endif
}

void CSkillManager::ApplyFireSlashEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
	int value = gServerInfo.m_FireSlashConstA + ((lpObj->Strength + lpObj->AddStrength) / gServerInfo.m_FireSlashConstB);

#if(GAMESERVER_UPDATE>=602)

	value += gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_FIRE_SLASH_ENHANCED);

#endif

	value = ((value > gServerInfo.m_FireSlashMaxRate) ? gServerInfo.m_FireSlashMaxRate : value);

	int count = gServerInfo.m_FireSlashTimeConstA;

	gEffectManager.AddEffect(lpTarget, 0, this->GetSkillEffect(lpSkill->m_skill), count, value, 0, 0, 0);
}

void CSkillManager::ApplyFireBurstEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
#if(GAMESERVER_UPDATE>=602)

	if ((GetLargeRand() % 100) < (gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_FIRE_BURST_ENHANCED) - lpTarget->ResistStunRate))
	{
		gEffectManager.AddEffect(lpTarget, 0, EFFECT_STERN, 2, 0, 0, 0, 0);
	}

#endif
}

void CSkillManager::ApplyFireScreamEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{

}

void CSkillManager::ApplyEarthquakeEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
#if(GAMESERVER_UPDATE>=602)

	if ((GetLargeRand() % 100) < (gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_EARTHQUAKE_ENHANCED) - lpTarget->ResistStunRate))
	{
		gEffectManager.AddEffect(lpTarget, 0, EFFECT_STERN, 2, 0, 0, 0, 0);
	}
	else
	{
		gObjBackSpring2(lpTarget, lpObj, 3);
	}

#else

	gObjBackSpring2(lpTarget, lpObj, 3);

#endif
}

void CSkillManager::ApplySwordSlashEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage) // OK
{
	gEffectManager.AddEffect(lpTarget, 0, gSkillManager.GetSkillEffect(lpSkill->m_skill), 1, 0, 0, 0, 0);
}

void CSkillManager::CGMultiSkillAttackRecv(PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg, int aIndex, bool type) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Live == 0 || lpObj->Teleport != 0)
	{
		return;
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0)
	{
		return;
	}

	int skill = lpMsg->skill;

	CSkill* lpSkill = 0;

	if (lpObj->Type == OBJECT_USER)
	{
		lpSkill = this->GetSkill(lpObj, skill);

		if (lpSkill == 0)
		{
			return;
		}

		if (this->CheckSkillDelay(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}

		if (this->CheckSkillRequireClass(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}
	}
	else
	{
		lpSkill = &lpObj->Skill[skill];

		if (lpSkill->IsSkill() == 0)
		{
			return;
		}
	}

	if (lpSkill->m_skill != SKILL_FLAME && lpSkill->m_skill != SKILL_TWISTER && lpSkill->m_skill != SKILL_EVIL_SPIRIT && lpSkill->m_skill != SKILL_HELL_FIRE && lpSkill->m_skill != SKILL_AQUA_BEAM && lpSkill->m_skill != SKILL_BLAST && lpSkill->m_skill != SKILL_INFERNO && lpSkill->m_skill != SKILL_TRIPLE_SHOT && lpSkill->m_skill != SKILL_IMPALE && lpSkill->m_skill != SKILL_MONSTER_AREA_ATTACK && lpSkill->m_skill != SKILL_PENETRATION && lpSkill->m_skill != SKILL_FIRE_SLASH && lpSkill->m_skill != SKILL_FIRE_SCREAM)
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && (lpObj->MultiSkillIndex == 0 || lpObj->MultiSkillIndex != lpSkill->m_skill || (lpObj->MultiSkillCount++) >= 5))
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TRIPLE_SHOT)
	{
		if (lpObj->Inventory[0].IsItem() == 0 || lpObj->Inventory[1].IsItem() == 0)
		{
			return;
		}

		if (lpObj->Inventory[0].m_Index < GET_ITEM(4, 0) || lpObj->Inventory[0].m_Index >= GET_ITEM(5, 0))
		{
			return;
		}

		if (lpObj->Inventory[1].m_Index < GET_ITEM(4, 0) || lpObj->Inventory[1].m_Index >= GET_ITEM(5, 0))
		{
			return;
		}
	}

	lpMsg->count = ((lpMsg->count > 5) ? 5 : lpMsg->count);

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_MULTI_SKILL_ATTACK* lpInfo = (PMSG_MULTI_SKILL_ATTACK*)(((BYTE*)lpMsg) + sizeof(PMSG_MULTI_SKILL_ATTACK_RECV) + (sizeof(PMSG_MULTI_SKILL_ATTACK) * n));

	#if(GAMESERVER_UPDATE>=701)

		int bIndex = MAKE_NUMBERW(lpInfo->indexH, lpInfo->indexL);

	#else

		int bIndex = MAKE_NUMBERW(lpInfo->index[0], lpInfo->index[1]);

	#endif

		if (OBJECT_RANGE(bIndex) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[bIndex];

		if (lpObj->Type == OBJECT_USER && this->CheckSkillRadio(lpSkill->m_skill, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y) == 0)
		{
			continue;
		}

		if (type != 0)
		{
			gAttack.Attack(lpObj, lpTarget, lpSkill, 1, 1, 0, 0, 0);
		}
		else if (lpSkill->m_skill == SKILL_FIRE_SLASH)
		{
			gAttack.Attack(lpObj, lpTarget, lpSkill, 1, 1, 0, 0, 0);
		}
		else
		{
			gAttack.Attack(lpObj, lpTarget, lpSkill, 0, 1, 0, 0, 0);
		}
	}
}

void CSkillManager::CGSkillAttackRecv(PMSG_SKILL_ATTACK_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Live == 0)
	{
		return;
	}

#if(GAMESERVER_UPDATE>=701)

	int bIndex = MAKE_NUMBERW(lpMsg->indexH, lpMsg->indexL);

#else

	int bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

#endif

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (gObjIsConnectedGS(bIndex) == 0)
	{
		return;
	}

	if (lpTarget->Live == 0)
	{
		return;
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X, lpTarget->Y, 1) != 0)
	{
		return;
	}
	
	CSkill* lpSkill = 0;

	if (lpObj->Type == OBJECT_USER)
	{
		lpSkill = this->GetSkill(lpObj, lpMsg->skill);

		if (lpSkill == 0)
		{
			return;
		}

		if (this->CheckSkillDelay(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}

		if (this->CheckSkillRequireKillPoint(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}

		if (this->CheckSkillRequireGuildStatus(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}

		if (this->CheckSkillRequireClass(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}
	}
	else
	{
		lpSkill = &lpObj->Skill[lpMsg->skill];

		if (lpSkill->IsSkill() == 0)
		{
			return;
		}
	}

	lpObj->MultiSkillIndex = 0;
	lpObj->MultiSkillCount = 0;

	this->UseAttackSkill(aIndex, ((lpMsg->skill == SKILL_NOVA_START) ? lpMsg->skill : bIndex), lpSkill);
}

void CSkillManager::CGDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Live == 0)
	{
		return;
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0)
	{
		return;
	}

	CSkill* lpSkill = 0;

	if (lpObj->Type == OBJECT_USER)
	{
		lpSkill = this->GetSkill(lpObj, lpMsg->skill);

		if (lpSkill == 0)
		{
			return;
		}

		if (this->CheckSkillDelay(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}

		if (this->CheckSkillRequireKillPoint(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}

		if (this->CheckSkillRequireGuildStatus(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}

		if (this->CheckSkillRequireClass(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}
	}
	else
	{
		lpSkill = &lpObj->Skill[lpMsg->skill];

		if (lpSkill->IsSkill() == 0)
		{
			return;
		}
	}

	lpObj->MultiSkillIndex = 0;
	lpObj->MultiSkillCount = 0;

	int bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	this->UseDurationSkillAttack(lpObj->Index, bIndex, lpSkill, lpMsg->x, lpMsg->y, lpMsg->dir, lpMsg->angle);
}

void CSkillManager::CGSkillCancelRecv(PMSG_SKILL_CANCEL_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	gEffectManager.DelEffect(lpObj, this->GetSkillEffect(lpMsg->skill));
}

void CSkillManager::CGSkillTeleportAllyRecv(PMSG_SKILL_TELEPORT_ALLY_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (OBJECT_RANGE(lpMsg->index) == 0)
	{
		return;
	}

	if (gObj[lpMsg->index].Connected != OBJECT_ONLINE || gObj[lpMsg->index].CloseCount > 0)
	{
		lpMsg->index = aIndex;
	}

	LPOBJ lpTarget = &gObj[lpMsg->index];

	if (OBJECT_RANGE(lpObj->PartyNumber) == 0 || OBJECT_RANGE(lpTarget->PartyNumber) == 0)
	{
		return;
	}

	if (lpObj->PartyNumber != lpTarget->PartyNumber)
	{
		return;
	}

	if (gEffectManager.CheckStunEffect(lpObj) != 0)
	{
		return;
	}

	if (lpObj->Map != lpTarget->Map)
	{
		return;
	}

	if (gObjCheckTeleportArea(lpMsg->index, lpMsg->x, lpMsg->y) == 0)
	{
		return;
	}

#if(GAMESERVER_TYPE==1)

	if (lpObj->Map == MAP_CASTLE_SIEGE && gCastleSiege.CheckTeleportMagicAxisY(lpObj->Y, lpMsg->x, lpMsg->y) == 0)
	{
		lpMsg->y = (BYTE)lpObj->Y;
	}

#endif

	CSkill* lpSkill = gSkillManager.GetSkill(lpObj, SKILL_TELEPORT_ALLY);

	if (lpSkill != 0)
	{
		if (gSkillManager.CheckSkillMana(lpObj, lpSkill->m_skill) == 0 || gSkillManager.CheckSkillBP(lpObj, lpSkill->m_skill) == 0)
		{
			return;
		}

		gSkillManager.GCSkillAttackSend(lpObj, lpSkill->m_skill, lpMsg->index, 1);

		gObjTeleportMagicUse(lpMsg->index, lpMsg->x, lpMsg->y);

		lpObj->Mana -= (gSkillManager.GetSkillMana(lpSkill->m_skill) * lpObj->MPConsumptionRate) / 100;

		lpObj->BP -= (gSkillManager.GetSkillBP(lpSkill->m_skill) * lpObj->BPConsumptionRate) / 100;

		GCManaSend(lpObj->Index, 0xFF, (int)lpObj->Mana, lpObj->BP);
	}
}

void CSkillManager::GCSkillAttackSend(LPOBJ lpObj, BYTE skill, int aIndex, BYTE type) // OK
{
	PMSG_SKILL_ATTACK_SEND pMsg;

	pMsg.header.setE(0x19, sizeof(pMsg));

	pMsg.skill = skill;
	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
	pMsg.target[0] = SET_NUMBERHB(aIndex) | (type * 0x80);
	pMsg.target[1] = SET_NUMBERLB(aIndex);

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void CSkillManager::GCSkillCancelSend(LPOBJ lpObj, BYTE skill) // OK
{
	PMSG_SKILL_CANCEL_SEND pMsg;

	pMsg.header.set(0x1B, sizeof(pMsg));

	pMsg.skill = skill;
	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void CSkillManager::GCDurationSkillAttackSend(LPOBJ lpObj, BYTE skill, BYTE x, BYTE y, BYTE dir) // OK
{
	PMSG_DURATION_SKILL_ATTACK_SEND pMsg;

	pMsg.header.setE(0x1E, sizeof(pMsg));

	pMsg.skill = skill;
	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
	pMsg.x = x;
	pMsg.y = y;
	pMsg.dir = dir;

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void CSkillManager::GCSkillAddSend(int aIndex, BYTE slot, BYTE skill, BYTE level, BYTE type) // OK
{
	BYTE send[256];

	PMSG_SKILL_LIST_SEND pMsg;

	pMsg.header.set(0xF3, 0x11, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0xFE;
	pMsg.type = type;

	PMSG_SKILL_LIST info;

	info.slot = slot;
	info.skill = skill;
	info.level = (level << 3) | ((skill / 255) & 7);

	memcpy(&send[size], &info, sizeof(info));
	size += sizeof(info);

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void CSkillManager::GCSkillDelSend(int aIndex, BYTE slot, BYTE skill, BYTE level, BYTE type) // OK
{
	BYTE send[256];

	PMSG_SKILL_LIST_SEND pMsg;

	pMsg.header.set(0xF3, 0x11, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0xFF;
	pMsg.type = type;

	PMSG_SKILL_LIST info;

	info.slot = slot;
	info.skill = skill;
	info.level = (level << 3) | ((skill / 255) & 7);

	memcpy(&send[size], &info, sizeof(info));
	size += sizeof(info);

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void CSkillManager::GCSkillListSend(LPOBJ lpObj, BYTE type) // OK
{
	//
	// Envia lista de skills
	//
	{
		BYTE send[1024];

		PMSG_SKILL_LIST_SEND pMsg;

		pMsg.header.set(0xF3, 0x11, 0);

		int size = sizeof(pMsg);

		pMsg.count = 0;
		pMsg.type = type;

		PMSG_SKILL_LIST info;

		for (int n = 0; n < MAX_SKILL_LIST; n++)
		{
			if (lpObj->Skill[n].IsSkill() == 0)
			{
				continue;
			}

			info.slot = n;
			info.skill = lpObj->Skill[n].m_skill;
			info.level = (lpObj->Skill[n].m_level << 3) | ((lpObj->Skill[n].m_skill / 255) & 7);

			memcpy(&send[size], &info, sizeof(info));
			size += sizeof(info);

			++pMsg.count;
		}

		pMsg.header.size = size;

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(lpObj->Index, send, size);
	}

	//
	// Envia registros das skills
	//
	{
		PMSG_OPTION_DATA_SEND pMsg;

		pMsg.header.set(0xF3, 0x30, sizeof(pMsg));

		memcpy(pMsg.SkillKey, lpObj->SkillOptionData.SkillKey, sizeof(pMsg.SkillKey));

		pMsg.GameOption = lpObj->SkillOptionData.GameOption;
		pMsg.QKey = lpObj->SkillOptionData.QKey;
		pMsg.WKey = lpObj->SkillOptionData.WKey;
		pMsg.EKey = lpObj->SkillOptionData.EKey;
		pMsg.ChatWindow = lpObj->SkillOptionData.ChatWindow;

		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}
}
