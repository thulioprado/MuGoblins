// EffectManager.cpp: implementation of the CEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectManager.h"
#include "Attack.h"
#include "Fruit.h"
#include "ItemManager.h"
#include "Map.h"
#include "MemScript.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Util.h"
#include "Viewport.h"

CEffectManager gEffectManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectManager::CEffectManager() // OK
{
	this->Init();
}

CEffectManager::~CEffectManager() // OK
{

}

void CEffectManager::Init() // OK
{
	for (int n = 0; n < MAX_EFFECT; n++)
	{
		this->m_EffectInfo[n].Index = -1;
	}
}

void CEffectManager::Load(char* path) // OK
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

			EFFECT_INFO info;

			info.Index = lpMemScript->GetNumber();
			info.Group = lpMemScript->GetAsNumber();
			info.ItemIndex = lpMemScript->GetAsNumber();

			strcpy_s(info.Name, lpMemScript->GetAsString());

			info.Save = lpMemScript->GetAsNumber();
			info.Type = lpMemScript->GetAsNumber();
			info.Flag = lpMemScript->GetAsNumber();
			info.Count = lpMemScript->GetAsNumber();
			info.Value[0] = lpMemScript->GetAsNumber();
			info.Value[1] = lpMemScript->GetAsNumber();
			info.Value[2] = lpMemScript->GetAsNumber();
			info.Value[3] = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CEffectManager::SetInfo(EFFECT_INFO info) // OK
{
	if (EFFECT_RANGE(info.Index) == 0)
	{
		return;
	}

	this->m_EffectInfo[info.Index] = info;
}

EFFECT_INFO* CEffectManager::GetInfo(int index) // OK
{
	if (EFFECT_RANGE(index) == 0)
	{
		return 0;
	}

	if (this->m_EffectInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_EffectInfo[index];
}

EFFECT_INFO* CEffectManager::GetInfoByItem(int ItemIndex) // OK
{
	for (int n = 0; n < MAX_EFFECT; n++)
	{
		EFFECT_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return 0;
}

void CEffectManager::MainProc() // OK
{
	for (int n = 0; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		for (int i = 0; i < MAX_EFFECT_LIST; i++)
		{
			CEffect* lpEffect = &lpObj->Effect[i];

			if (lpEffect->IsEffect() == 0)
			{
				continue;
			}

			if (((lpEffect->m_count == 0) ? 0 : (lpEffect->m_count--)) > 0)
			{
				this->PeriodicEffect(lpObj, lpEffect);

				if (lpEffect->m_count == 0)
				{
					this->RemoveEffect(lpObj, lpEffect);

					lpEffect->Clear();
				}
			}
		}
	}
}

bool CEffectManager::AddEffect(LPOBJ lpObj, bool type, int index, int count, WORD value1, WORD value2, WORD value3, WORD value4) // OK
{
	EFFECT_INFO* lpInfo = this->GetInfo(index);

	if (lpInfo == 0)
	{
		return 0;
	}

	if (type == 0 && lpInfo->Count != -1)
	{
		count = ((lpInfo->Type == 2) ? ((int)time(0) + lpInfo->Count) : lpInfo->Count);
	}

	if (lpInfo->Value[0] != -1)
	{
		value1 = lpInfo->Value[0];
	}

	if (lpInfo->Value[1] != -1)
	{
		value2 = lpInfo->Value[1];
	}

	if (lpInfo->Value[2] != -1)
	{
		value3 = lpInfo->Value[2];
	}

	if (lpInfo->Value[3] != -1)
	{
		value4 = lpInfo->Value[3];
	}

	CEffect* lpEffect = this->GetEffectByGroup(lpObj, lpInfo->Group);

	if (lpEffect == 0)
	{
		for (int n = 0; n < MAX_EFFECT_LIST; n++)
		{
			lpEffect = &lpObj->Effect[n];

			if (lpEffect->Set(index, lpInfo->Group, lpInfo->Save, lpInfo->Type, lpInfo->Flag, 0, count, value1, value2, value3, value4) != 0)
			{
				this->InsertEffect(lpObj, lpEffect);
				return 1;
			}
		}
	}
	else
	{
		if (index != lpEffect->m_index || (value1 >= lpEffect->m_value[0] && value2 >= lpEffect->m_value[1] && value3 >= lpEffect->m_value[2] && value4 >= lpEffect->m_value[3]))
		{
			this->RemoveEffect(lpObj, lpEffect);
			
			lpEffect->Clear();

			if (lpEffect->Set(index, lpInfo->Group, lpInfo->Save, lpInfo->Type, lpInfo->Flag, 0, count, value1, value2, value3, value4) != 0)
			{
				this->InsertEffect(lpObj, lpEffect);
				return 1;
			}
		}
	}

	return 0;
}

bool CEffectManager::DelEffect(LPOBJ lpObj, int index) // OK
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == 0)
		{
			continue;
		}

		if (lpEffect->m_index == index)
		{
			this->RemoveEffect(lpObj, lpEffect);
			lpEffect->Clear();
			return 1;
		}
	}

	return 0;
}

bool CEffectManager::DelEffectByGroup(LPOBJ lpObj, int group) // OK
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == 0)
		{
			continue;
		}

		if (lpEffect->m_group == group)
		{
			this->RemoveEffect(lpObj, lpEffect);
			lpEffect->Clear();
			return 1;
		}
	}

	return 0;
}

CEffect* CEffectManager::GetEffect(LPOBJ lpObj, int index) // OK
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == 0)
		{
			continue;
		}

		if (lpEffect->m_index == index)
		{
			return lpEffect;
		}
	}

	return 0;
}

CEffect* CEffectManager::GetEffectByGroup(LPOBJ lpObj, int group) // OK
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == 0)
		{
			continue;
		}

		if (lpEffect->m_group == group)
		{
			return lpEffect;
		}
	}

	return 0;
}

bool CEffectManager::CheckEffect(LPOBJ lpObj, int index) // OK
{
	if (this->GetEffect(lpObj, index) != 0)
	{
		return 1;
	}

	return 0;
}

bool CEffectManager::CheckEffectByGroup(LPOBJ lpObj, int group) // OK
{
	if (this->GetEffectByGroup(lpObj, group) != 0)
	{
		return 1;
	}

	return 0;
}

void CEffectManager::InsertEffect(LPOBJ lpObj, CEffect* lpEffect) // OK
{
	this->InsertViewSkillState(lpObj, lpEffect->m_index);

	switch (lpEffect->m_index)
	{
		case EFFECT_GREATER_DAMAGE:
			lpObj->EffectOption.AddPhysiDamage += lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicDamage += lpEffect->m_value[0];
			lpObj->EffectOption.AddCurseDamage += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_DEFENSE:
			lpObj->EffectOption.AddDefense += lpEffect->m_value[0];
			break;
		case EFFECT_ELF_BUFFER:
			lpObj->EffectOption.AddPhysiDamage += lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicDamage += lpEffect->m_value[0];
			lpObj->EffectOption.AddCurseDamage += lpEffect->m_value[0];
			lpObj->EffectOption.AddDefense += lpEffect->m_value[1];
			break;
		case EFFECT_MANA_SHIELD:
			lpObj->EffectOption.AddDamageReduction += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_CRITICAL_DAMAGE:
			lpObj->EffectOption.AddCriticalDamage += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_LIFE:
			lpObj->EffectOption.MulMaxHP += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_MANA:
			lpObj->EffectOption.MulMaxMP += lpEffect->m_value[0];
			break;
		case EFFECT_SOUL_POTION:
			lpObj->EffectOption.AddPhysiSpeed += lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicSpeed += lpEffect->m_value[0];
			lpObj->EffectOption.AddBPRecovery += lpEffect->m_value[1];
			GCItemUseSpecialTimeSend(lpObj->Index, 2, 60);
			break;
		case EFFECT_ICE:
			lpObj->DelayLevel = 1;
			lpObj->DelayActionTime = 800;
			break;
		case EFFECT_FIRE_SLASH:
			lpObj->EffectOption.DivDefense += lpEffect->m_value[0];
			break;
		case EFFECT_DAMAGE_REFLECT:
			lpObj->EffectOption.AddDamageReflect += lpEffect->m_value[0];
			break;
		case EFFECT_BLIND:
			lpObj->EffectOption.DivAttackSuccessRate += lpEffect->m_value[0];
			break;
		case EFFECT_SWORD_POWER:
			lpObj->EffectOption.AddSwordPowerDamageRate += lpEffect->m_value[0];
			lpObj->EffectOption.MulMaxMP += lpEffect->m_value[1];
			lpObj->EffectOption.AddSwordPowerDefenseRate += lpEffect->m_value[2];
			lpObj->EffectOption.DivMaxHP += lpEffect->m_value[3];
			break;
		case EFFECT_MAGIC_CIRCLE:
			lpObj->EffectOption.AddMinMagicDamage += lpEffect->m_value[0];
			break;
		case EFFECT_FROZEN_STAB:
			lpObj->DelayLevel = 1;
			lpObj->DelayActionTime = 1000;
			break;
		case EFFECT_GLADIATORS_GLORY:
			lpObj->EffectOption.AddVolcanoExperienceRate += lpEffect->m_value[0];
			break;
		case EFFECT_PARTY_EXPERIENCE_BONUS:
			lpObj->EffectOption.AddExperienceRate += lpEffect->m_value[0];
			lpObj->EffectOption.AddMasterExperienceRate += lpEffect->m_value[0];
			lpObj->EffectOption.AddPartyBonusExperienceRate += lpEffect->m_value[1];
			break;
		case EFFECT_HAWK_FIGURINE:
			lpObj->EffectOption.AddCriticalDamageRate += lpEffect->m_value[0];
			break;
		case EFFECT_GOAT_FIGURINE:
			lpObj->EffectOption.AddExcellentDamageRate += lpEffect->m_value[0];
			break;
		case EFFECT_OAK_CHARM:
			lpObj->EffectOption.AddMaxHP += lpEffect->m_value[0];
			break;
		case EFFECT_MAPLE_CHARM:
			lpObj->EffectOption.AddMaxMP += lpEffect->m_value[0];
			break;
		case EFFECT_GOLDEN_OAK_CHARM:
			lpObj->EffectOption.AddMaxHP += lpEffect->m_value[0];
			lpObj->EffectOption.AddMaxSD += lpEffect->m_value[1];
			break;
		case EFFECT_GOLDEN_MAPLE_CHARM:
			lpObj->EffectOption.AddMaxMP += lpEffect->m_value[0];
			lpObj->EffectOption.AddMaxBP += lpEffect->m_value[1];
			break;
		case EFFECT_WORN_HORSESHOE:
			lpObj->EffectOption.AddItemDropRate += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_IGNORE_DEFENSE_RATE:
			lpObj->EffectOption.AddIgnoreDefenseRate += lpEffect->m_value[0];
			break;
		case EFFECT_FITNESS:
			lpObj->EffectOption.AddVitality += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_DEFENSE_SUCCESS_RATE:
			lpObj->EffectOption.AddDefenseSuccessRate += lpEffect->m_value[0];
			break;
		case EFFECT_IRON_DEFENSE:
			lpObj->EffectOption.AddMaxHP += lpEffect->m_value[0];
			lpObj->EffectOption.AddDefense += lpEffect->m_value[1];
			break;
		case EFFECT_BLESS:
			lpObj->EffectOption.AddStrength += lpEffect->m_value[0];
			lpObj->EffectOption.AddDexterity += lpEffect->m_value[0];
			lpObj->EffectOption.AddVitality += lpEffect->m_value[0];
			lpObj->EffectOption.AddEnergy += lpEffect->m_value[0];
			break;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);
}

void CEffectManager::RemoveEffect(LPOBJ lpObj, CEffect* lpEffect) // OK
{
	this->RemoveViewSkillState(lpObj, lpEffect->m_index);

	switch (lpEffect->m_index)
	{
		case EFFECT_GREATER_DAMAGE:
			lpObj->EffectOption.AddPhysiDamage -= lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicDamage -= lpEffect->m_value[0];
			lpObj->EffectOption.AddCurseDamage -= lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_DEFENSE:
			lpObj->EffectOption.AddDefense -= lpEffect->m_value[0];
			break;
		case EFFECT_ELF_BUFFER:
			lpObj->EffectOption.AddPhysiDamage -= lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicDamage -= lpEffect->m_value[0];
			lpObj->EffectOption.AddCurseDamage -= lpEffect->m_value[0];
			lpObj->EffectOption.AddDefense -= lpEffect->m_value[1];
			break;
		case EFFECT_MANA_SHIELD:
			lpObj->EffectOption.AddDamageReduction -= lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_CRITICAL_DAMAGE:
			lpObj->EffectOption.AddCriticalDamage -= lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_LIFE:
			lpObj->EffectOption.MulMaxHP -= lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_MANA:
			lpObj->EffectOption.MulMaxMP -= lpEffect->m_value[0];
			break;
		case EFFECT_SOUL_POTION:
			lpObj->EffectOption.AddPhysiSpeed -= lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicSpeed -= lpEffect->m_value[0];
			lpObj->EffectOption.AddBPRecovery -= lpEffect->m_value[1];
			break;
		case EFFECT_SEAL_OF_ASCENSION1:
			lpObj->EffectOption.AddExperienceRate -= lpEffect->m_value[0];
			break;
		case EFFECT_SEAL_OF_WEALTH1:
			lpObj->EffectOption.AddExperienceRate -= lpEffect->m_value[0];
			lpObj->EffectOption.AddItemDropRate -= lpEffect->m_value[1];
			break;
		case EFFECT_ORDER_OF_PROTECTION:
			lpObj->EffectOption.AddDamageReduction -= lpEffect->m_value[0];
			break;
		case EFFECT_SEAL_OF_ASCENSION2:
			lpObj->EffectOption.AddExperienceRate -= lpEffect->m_value[0];
			break;
		case EFFECT_SEAL_OF_WEALTH2:
			lpObj->EffectOption.AddExperienceRate -= lpEffect->m_value[0];
			lpObj->EffectOption.AddItemDropRate -= lpEffect->m_value[1];
			break;
		case EFFECT_SCROLL_OF_QUICK:
			lpObj->EffectOption.AddPhysiSpeed -= lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicSpeed -= lpEffect->m_value[0];
			break;
		case EFFECT_SCROLL_OF_DEFENSE:
			lpObj->EffectOption.AddDefense -= lpEffect->m_value[0];
			break;
		case EFFECT_SCROLL_OF_DAMAGE:
			lpObj->EffectOption.AddPhysiDamage -= lpEffect->m_value[0];
			break;
		case EFFECT_SCROLL_OF_MAGIC_DAMAGE:
			lpObj->EffectOption.AddMagicDamage -= lpEffect->m_value[0];
			break;
		case EFFECT_SCROLL_OF_LIFE:
			lpObj->EffectOption.AddMaxHP -= lpEffect->m_value[0];
			break;
		case EFFECT_SCROLL_OF_MANA:
			lpObj->EffectOption.AddMaxMP -= lpEffect->m_value[0];
			break;
		case EFFECT_ELIXIR_OF_STRENGTH:
			lpObj->EffectOption.AddStrength -= lpEffect->m_value[0];
			break;
		case EFFECT_ELIXIR_OF_DEXTERITY:
			lpObj->EffectOption.AddDexterity -= lpEffect->m_value[0];
			break;
		case EFFECT_ELIXIR_OF_VITALITY:
			lpObj->EffectOption.AddVitality -= lpEffect->m_value[0];
			break;
		case EFFECT_ELIXIR_OF_ENERGY:
			lpObj->EffectOption.AddEnergy -= lpEffect->m_value[0];
			break;
		case EFFECT_ELIXIR_OF_LEADERSHIP:
			lpObj->EffectOption.AddLeadership -= lpEffect->m_value[0];
			break;
		case EFFECT_ICE:
			lpObj->DelayLevel = 0;
			lpObj->DelayActionTime = 0;
			break;
		case EFFECT_FIRE_SLASH:
			lpObj->EffectOption.DivDefense -= lpEffect->m_value[0];
			break;
		case EFFECT_DAMAGE_REFLECT:
			lpObj->EffectOption.AddDamageReflect -= lpEffect->m_value[0];
			break;
		case EFFECT_SWORD_POWER:
			lpObj->EffectOption.AddSwordPowerDamageRate -= lpEffect->m_value[0];
			lpObj->EffectOption.MulMaxMP -= lpEffect->m_value[1];
			lpObj->EffectOption.AddSwordPowerDefenseRate -= lpEffect->m_value[2];
			lpObj->EffectOption.DivMaxHP -= lpEffect->m_value[3];
			break;
		case EFFECT_MAGIC_CIRCLE:
			lpObj->EffectOption.AddMinMagicDamage -= lpEffect->m_value[0];
			break;
		case EFFECT_FROZEN_STAB:
			lpObj->DelayLevel = 0;
			lpObj->DelayActionTime = 0;
			break;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);
}

bool CEffectManager::ConvertEffectByte(CEffect* lpEffect, BYTE* lpMsg) // OK
{
	if (lpMsg[0] == 0xFF)
	{
		return 0;
	}

	lpEffect->m_index = lpMsg[0];

	lpEffect->m_count = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg[1], lpMsg[2]), MAKE_NUMBERW(lpMsg[3], lpMsg[4]));

	lpEffect->m_value[0] = MAKE_NUMBERW(lpMsg[5], lpMsg[6]);
	lpEffect->m_value[1] = MAKE_NUMBERW(lpMsg[7], lpMsg[8]);
	lpEffect->m_value[2] = MAKE_NUMBERW(lpMsg[9], lpMsg[10]);
	lpEffect->m_value[3] = MAKE_NUMBERW(lpMsg[11], lpMsg[12]);
	return 1;
}

void CEffectManager::EffectByteConvert(BYTE* lpMsg, CEffect* lpEffect) // OK
{
	if (lpEffect->IsEffect() == 0 || lpEffect->m_save == 0)
	{
		memset(lpMsg, 0xFF, 13);
		return;
	}

	lpMsg[0] = lpEffect->m_index;

	if (lpEffect->m_type == 2)
	{
		lpMsg[1] = SET_NUMBERHB(SET_NUMBERHW(lpEffect->m_time));
		lpMsg[2] = SET_NUMBERLB(SET_NUMBERHW(lpEffect->m_time));
		lpMsg[3] = SET_NUMBERHB(SET_NUMBERLW(lpEffect->m_time));
		lpMsg[4] = SET_NUMBERLB(SET_NUMBERLW(lpEffect->m_time));
	}
	else
	{
		lpMsg[1] = SET_NUMBERHB(SET_NUMBERHW(lpEffect->m_count));
		lpMsg[2] = SET_NUMBERLB(SET_NUMBERHW(lpEffect->m_count));
		lpMsg[3] = SET_NUMBERHB(SET_NUMBERLW(lpEffect->m_count));
		lpMsg[4] = SET_NUMBERLB(SET_NUMBERLW(lpEffect->m_count));
	}

	lpMsg[5] = SET_NUMBERHB(lpEffect->m_value[0]);
	lpMsg[6] = SET_NUMBERLB(lpEffect->m_value[0]);

	lpMsg[7] = SET_NUMBERHB(lpEffect->m_value[1]);
	lpMsg[8] = SET_NUMBERLB(lpEffect->m_value[1]);

	lpMsg[9] = SET_NUMBERHB(lpEffect->m_value[2]);
	lpMsg[10] = SET_NUMBERLB(lpEffect->m_value[2]);

	lpMsg[11] = SET_NUMBERHB(lpEffect->m_value[3]);
	lpMsg[12] = SET_NUMBERLB(lpEffect->m_value[3]);
}

int CEffectManager::GenerateEffectList(LPOBJ lpObj, BYTE* lpMsg, int* size) // OK
{
	int count = 0;

	PMSG_VIEWPORT_STATE info;

	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		if (lpObj->Effect[n].IsEffect() != 0)
		{
			info.effect = lpObj->Effect[n].m_index;

			memcpy(&lpMsg[(*size)], &info, sizeof(info));
			(*size) += sizeof(info);

			count++;
		}
	}

	return count;
}

int CEffectManager::GeneratePartyEffectList(LPOBJ lpObj, BYTE* lpMsg, int* size) // OK
{
	int count = 0;

	PMSG_PARTY_EFFECT_LIST info;

	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		if (lpObj->Effect[n].IsEffect() != 0)
		{
			info.effect = lpObj->Effect[n].m_index;

			info.count = lpObj->Effect[n].m_count;

			memcpy(&lpMsg[(*size)], &info, sizeof(info));
			(*size) += sizeof(info);

			count++;
		}
	}

	return count;
}

bool CEffectManager::CheckStunEffect(LPOBJ lpObj) // OK
{
	if (this->CheckEffect(lpObj, EFFECT_STERN) != 0 || this->CheckEffect(lpObj, EFFECT_SLEEP) != 0 || this->CheckEffect(lpObj, EFFECT_EARTH_PRISON) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CEffectManager::CheckImmobilizeEffect(LPOBJ lpObj) // OK
{
	if (this->CheckEffect(lpObj, EFFECT_ICE_ARROW) != 0 || this->CheckEffect(lpObj, EFFECT_ORDER_OF_RESTRAINT) != 0 || this->CheckEffect(lpObj, EFFECT_ICE_STORM_ENHANCED) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void CEffectManager::ClearAllEffect(LPOBJ lpObj) // OK
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == 0 || lpEffect->m_type != 0)
		{
			continue;
		}

		this->RemoveEffect(lpObj, lpEffect);
		lpEffect->Clear();
	}
}

void CEffectManager::ClearDebuffEffect(LPOBJ lpObj, int count) // OK
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == 0 || lpEffect->m_type != 0 || lpEffect->m_flag == 0)
		{
			continue;
		}

		if ((--count) >= 0)
		{
			this->RemoveEffect(lpObj, lpEffect);
			lpEffect->Clear();
		}
	}
}

void CEffectManager::PeriodicEffect(LPOBJ lpObj, CEffect* lpEffect) // OK
{
	if (lpEffect->m_index != EFFECT_POISON && lpEffect->m_index != EFFECT_NEIL && lpEffect->m_index != EFFECT_SAHAMUTT && lpEffect->m_index != EFFECT_DEATH_STAB_ENHANCED && lpEffect->m_index != EFFECT_DRAIN_LIFE_ENHANCED && lpEffect->m_index != EFFECT_DRAGON_ROAR_ENHANCED && lpEffect->m_index != EFFECT_CHAIN_DRIVER_ENHANCED && lpEffect->m_index != EFFECT_POISON_ARROW && lpEffect->m_index != EFFECT_POISON_ARROW_IMPROVED)
	{
		return;
	}

	if (lpEffect->m_value[1] == 0 || (lpEffect->m_count % lpEffect->m_value[1]) != 0)
	{
		return;
	}

	if (lpObj->Type == OBJECT_NPC || lpObj->Live == 0 || OBJECT_RANGE(lpEffect->m_value[0]) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[lpEffect->m_value[0]];

	int damage = 0;
	int absorb = 0;
	int effect = 0;

	switch (lpEffect->m_index)
	{
		case EFFECT_POISON:
			damage = (((damage = (int)((lpObj->Life * lpEffect->m_value[2]) / 100)) > lpEffect->m_value[3]) ? ((lpEffect->m_value[3] == 0) ? damage : lpEffect->m_value[3]) : damage);
			absorb = 0;
			effect = 2;
			break;
		case EFFECT_NEIL:
			damage = MAKE_NUMBERDW(lpEffect->m_value[2], lpEffect->m_value[3]);
			absorb = 0;
			effect = 3;
			break;
		case EFFECT_SAHAMUTT:
			damage = MAKE_NUMBERDW(lpEffect->m_value[2], lpEffect->m_value[3]);
			absorb = 0;
			effect = 3;
			break;
		case EFFECT_DEATH_STAB_ENHANCED:
			damage = MAKE_NUMBERDW(lpEffect->m_value[2], lpEffect->m_value[3]);
			absorb = 0;
			effect = 3;
			break;
		case EFFECT_DRAIN_LIFE_ENHANCED:
			damage = MAKE_NUMBERDW(lpEffect->m_value[2], lpEffect->m_value[3]);
			absorb = MAKE_NUMBERDW(lpEffect->m_value[2], lpEffect->m_value[3]);
			effect = 3;
			break;
		case EFFECT_DRAGON_ROAR_ENHANCED:
			damage = MAKE_NUMBERDW(lpEffect->m_value[2], lpEffect->m_value[3]);
			absorb = 0;
			effect = 3;
			break;
		case EFFECT_CHAIN_DRIVER_ENHANCED:
			damage = MAKE_NUMBERDW(lpEffect->m_value[2], lpEffect->m_value[3]);
			absorb = 0;
			effect = 3;
			break;
		case EFFECT_POISON_ARROW:
			damage = (((damage = (int)((lpObj->Life * lpEffect->m_value[2]) / 100)) > lpEffect->m_value[3]) ? ((lpEffect->m_value[3] == 0) ? damage : lpEffect->m_value[3]) : damage);
			absorb = 0;
			effect = 2;
			break;
		case EFFECT_POISON_ARROW_IMPROVED:
			damage = (((damage = (int)((lpObj->Life * lpEffect->m_value[2]) / 100)) > lpEffect->m_value[3]) ? ((lpEffect->m_value[3] == 0) ? damage : lpEffect->m_value[3]) : damage);
			absorb = 0;
			effect = 2;
			break;
	}

	if (lpObj->Life < damage)
	{
		lpObj->Life = 0;
	}
	else
	{
		lpObj->Life -= damage;
	}

	if ((lpTarget->Life + absorb) > (lpTarget->MaxLife + lpTarget->AddLife))
	{
		lpTarget->Life = lpTarget->MaxLife + lpTarget->AddLife;
	}
	else
	{
		lpTarget->Life += absorb;
	}

	if (damage > 0)
	{
		gObjectManager.CharacterLifeCheck(lpTarget, lpObj, damage, effect, 0, 0, 0);
	}

	if (absorb > 0)
	{
		GCLifeSend(lpTarget->Index, 0xFF, (int)lpTarget->Life);
	}
}

void CEffectManager::InsertViewSkillState(LPOBJ lpObj, BYTE index)
{
	switch (index)
	{
		case EFFECT_GREATER_DAMAGE:
			lpObj->ViewSkillState |= 0x4;
			break;
		case EFFECT_GREATER_DEFENSE:
			lpObj->ViewSkillState |= 0x8;
			break;
		case EFFECT_ELF_BUFFER:
			lpObj->ViewSkillState |= 0x2000000;
			break;
		case EFFECT_MANA_SHIELD:
			lpObj->ViewSkillState |= 0x100;
			break;
		case EFFECT_GREATER_LIFE:
			lpObj->ViewSkillState |= 0x10;
			break;
		case EFFECT_GREATER_CRITICAL_DAMAGE:
			lpObj->ViewSkillState |= 0x800;
			break;
		case EFFECT_PHYSI_DAMAGE_IMMUNITY:
			lpObj->ViewSkillState |= 0x8;
			break;
		case EFFECT_MAGIC_DAMAGE_IMMUNITY:
			lpObj->ViewSkillState |= 0x80;
			break;
		case EFFECT_GREATER_MANA:
			lpObj->ViewSkillState |= 0x2000;
			break;
		case EFFECT_BLESS_POTION:
			lpObj->ViewSkillState |= 0x8000;
			break;
		case EFFECT_SOUL_POTION:
			lpObj->ViewSkillState |= 0x10000;
			break;
		case EFFECT_INVISIBILITY:
			lpObj->ViewSkillState |= 0x4000;
			break;
		case EFFECT_GUILD_STATE5:
			lpObj->ViewSkillState |= 0x200;
			break;
		case EFFECT_POISON:
			lpObj->ViewSkillState |= 0x1;
			break;
		case EFFECT_ICE:
			lpObj->ViewSkillState |= 0x2;
			break;
		case EFFECT_ICE_ARROW:
			lpObj->ViewSkillState |= 0x20;
			break;
		case EFFECT_FIRE_SLASH:
			lpObj->ViewSkillState |= 0x40;
			break;
		case EFFECT_STERN:
			lpObj->ViewSkillState |= 0x1000;
			break;
		case EFFECT_GAME_MASTER:
			lpObj->ViewSkillState |= 0x1000000;
			break;
		case EFFECT_CRYWOLF_STATE1:
			lpObj->ViewSkillState |= 0x10000;
			break;
		case EFFECT_CRYWOLF_STATE2:
			lpObj->ViewSkillState |= 0x20000;
			break;
		case EFFECT_CRYWOLF_STATE3:
			lpObj->ViewSkillState |= 0x40000;
			break;
		case EFFECT_CRYWOLF_STATE4:
			lpObj->ViewSkillState |= 0x80000;
			break;
		case EFFECT_CRYWOLF_STATE7:
			lpObj->ViewSkillState |= 0x10000000;
			break;
		case EFFECT_CRYWOLF_STATE8:
			lpObj->ViewSkillState |= 0;
			break;
		case EFFECT_CRYWOLF_STATE9:
			lpObj->ViewSkillState |= 0x1;
			break;
		case EFFECT_CRYWOLF_STATE10:
			lpObj->ViewSkillState |= 0x2;
			break;
		case EFFECT_CRYWOLF_STATE11:
			lpObj->ViewSkillState |= 0x4;
			break;
		case EFFECT_CRYWOLF_STATE12:
			lpObj->ViewSkillState |= 0x100;
			break;
		case EFFECT_CASTLE_GATE_STATE:
		case EFFECT_CASTLE_CROWN_STATE:
			lpObj->ViewSkillState |= 0x20000;
			break;
		case EFFECT_GUILD_STATE1:
		case EFFECT_GUILD_STATE3:
		case EFFECT_ORDER_OF_PROTECTION:
			lpObj->ViewSkillState |= 0x40000;
			break;
		case EFFECT_GUILD_STATE2:
		case EFFECT_GUILD_STATE4:
		case EFFECT_ORDER_OF_RESTRAINT:
			lpObj->ViewSkillState |= 0x80000;
			break;
		case EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY:
		case EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY:
			lpObj->ViewSkillState |= 0x2000000;
			break;
	}

	this->GCEffectStateSend(lpObj, 1, lpObj->ViewSkillState);
}

void CEffectManager::RemoveViewSkillState(LPOBJ lpObj, BYTE index)
{
	switch (index)
	{
		case EFFECT_GREATER_DAMAGE:
			lpObj->ViewSkillState &= ~0x4;
			break;
		case EFFECT_GREATER_DEFENSE:
			lpObj->ViewSkillState &= ~0x8;
			break;
		case EFFECT_ELF_BUFFER:
			lpObj->ViewSkillState &= ~0x2000000;
			break;
		case EFFECT_MANA_SHIELD:
			lpObj->ViewSkillState &= ~0x100;
			break;
		case EFFECT_GREATER_LIFE:
			lpObj->ViewSkillState &= ~0x10;
			break;
		case EFFECT_GREATER_CRITICAL_DAMAGE:
			lpObj->ViewSkillState &= ~0x800;
			break;
		case EFFECT_PHYSI_DAMAGE_IMMUNITY:
			lpObj->ViewSkillState &= ~0x8;
			break;
		case EFFECT_MAGIC_DAMAGE_IMMUNITY:
			lpObj->ViewSkillState &= ~0x80;
			break;
		case EFFECT_GREATER_MANA:
			lpObj->ViewSkillState &= ~0x2000;
			break;
		case EFFECT_BLESS_POTION:
			lpObj->ViewSkillState &= ~0x8000;
			break;
		case EFFECT_SOUL_POTION:
			lpObj->ViewSkillState &= ~0x10000;
			break;
		case EFFECT_INVISIBILITY:
			lpObj->ViewSkillState &= ~0x4000;
			break;
		case EFFECT_GUILD_STATE5:
			lpObj->ViewSkillState &= ~0x200;
			break;
		case EFFECT_POISON:
			lpObj->ViewSkillState &= ~0x1;
			break;
		case EFFECT_ICE:
			lpObj->ViewSkillState &= ~0x2;
			break;
		case EFFECT_ICE_ARROW:
			lpObj->ViewSkillState &= ~0x20;
			break;
		case EFFECT_FIRE_SLASH:
			lpObj->ViewSkillState &= ~0x40;
			break;
		case EFFECT_STERN:
			lpObj->ViewSkillState &= ~0x1000;
			break;
		case EFFECT_GAME_MASTER:
			lpObj->ViewSkillState &= ~0x1000000;
			break;
		case EFFECT_CRYWOLF_STATE1:
			lpObj->ViewSkillState &= ~0x10000;
			break;
		case EFFECT_CRYWOLF_STATE2:
			lpObj->ViewSkillState &= ~0x20000;
			break;
		case EFFECT_CRYWOLF_STATE3:
			lpObj->ViewSkillState &= ~0x40000;
			break;
		case EFFECT_CRYWOLF_STATE4:
			lpObj->ViewSkillState &= ~0x80000;
			break;
		case EFFECT_CRYWOLF_STATE7:
			lpObj->ViewSkillState &= ~0x10000000;
			break;
		case EFFECT_CRYWOLF_STATE8:
			lpObj->ViewSkillState &= ~0;
			break;
		case EFFECT_CRYWOLF_STATE9:
			lpObj->ViewSkillState &= ~0x1;
			break;
		case EFFECT_CRYWOLF_STATE10:
			lpObj->ViewSkillState &= ~0x2;
			break;
		case EFFECT_CRYWOLF_STATE11:
			lpObj->ViewSkillState &= ~0x4;
			break;
		case EFFECT_CRYWOLF_STATE12:
			lpObj->ViewSkillState &= ~0x100;
			break;
		case EFFECT_CASTLE_GATE_STATE:
		case EFFECT_CASTLE_CROWN_STATE:
			lpObj->ViewSkillState &= ~0x20000;
			break;
		case EFFECT_GUILD_STATE1:
		case EFFECT_GUILD_STATE3:
		case EFFECT_ORDER_OF_PROTECTION:
			lpObj->ViewSkillState &= ~0x40000;
			break;
		case EFFECT_GUILD_STATE2:
		case EFFECT_GUILD_STATE4:
		case EFFECT_ORDER_OF_RESTRAINT:
			lpObj->ViewSkillState &= ~0x80000;
			break;
		case EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY:
		case EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY:
			lpObj->ViewSkillState &= ~0x2000000;
			break;
	}

	this->GCEffectStateSend(lpObj, 0, lpObj->ViewSkillState);
}

void CEffectManager::GCEffectStateSend(LPOBJ lpObj, BYTE state, DWORD ViewSkillSTate) // OK
{
	PMSG_EFFECT_STATE_SEND pMsg;

	pMsg.header.set(0x07, sizeof(pMsg));

	pMsg.state = state;
	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
	pMsg.ViewSkillSTate = ViewSkillSTate;

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	if (CC_MAP_RANGE(lpObj->Map) == 0)
	{
		MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
	}
}