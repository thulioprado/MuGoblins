// Attack.cpp: implementation of the CAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Attack.h"
#include "BattleSoccerManager.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "DarkSpirit.h"
#include "Duel.h"
#include "EffectManager.h"
#include "Guild.h"
#include "Kalima.h"
#include "Map.h"
#include "MapManager.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "Party.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CAttack gAttack;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttack::CAttack() // OK
{

}

CAttack::~CAttack() // OK
{

}

bool CAttack::Attack(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,bool send,BYTE flag,int damage,int count,bool combo) // OK
{
	#pragma region ATTACK_CHECK

	if(lpObj->Index == lpTarget->Index)
	{
		return 0;
	}

	if(lpObj->Type == OBJECT_USER && gObjIsConnectedGP(lpObj->Index) == 0)
	{
		return 0;
	}

	if(lpTarget->Type == OBJECT_USER && gObjIsConnectedGP(lpTarget->Index) == 0)
	{
		return 0;
	}

	if(lpObj->Map != lpTarget->Map || lpObj->Teleport == 2)
	{
		return 0;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X,lpTarget->Y,1) != 0)
	{
		return 0;
	}

	#if(GAMESERVER_TYPE==1)

	if(gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		if(lpTarget->Type == OBJECT_MONSTER && lpTarget->Map == MAP_CASTLE_SIEGE && (lpTarget->Class == 277 || lpTarget->Class == 283 || lpTarget->Class == 288))
		{
			return 0;
		}
	}

	if(gCrywolf.GetCrywolfState() == CRYWOLF_STATE_READY || gCrywolf.GetCrywolfState() == CRYWOLF_STATE_END)
	{
		if(lpTarget->Type == OBJECT_MONSTER && lpTarget->Map == MAP_CRYWOLF)
		{
			return 0;
		}
	}

	#endif

	if(lpObj->GuildNumber > 0 && lpObj->Guild != 0 && lpObj->Guild->WarState != 0)
	{
		if(lpObj->Guild->WarType == 1 && GetBattleSoccerGoalMove(0) == 0)
		{
			return 1;
		}

		if(lpObj->Guild->WarType == 0 && lpTarget->Type == OBJECT_MONSTER)
		{
			return 0;
		}
	}

	if(lpTarget->Type == OBJECT_NPC || lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || lpTarget->Teleport != 0)
	{
		return 0;
	}

	if(lpTarget->Type == OBJECT_MONSTER)
	{
		if(KALIMA_ATTRIBUTE_RANGE(lpTarget->Attribute) != 0)
		{
			return 0;
		}

		if((lpTarget->Class >= 100 && lpTarget->Class <= 110)) // Trap
		{
			return 0;
		}

		if(lpTarget->Class == 221 || lpTarget->Class == 222) // Siege
		{
			return 0;
		}
	}

	if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
	{
		if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
		{
			if(lpObj->SummonIndex == lpTarget->Index)
			{
				return 0;
			}
		}
	}

	int SummonIndex = lpObj->Index;

	if(lpObj->Type == OBJECT_MONSTER && OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		SummonIndex = lpObj->SummonIndex;
	}

	int SummonTargetIndex = lpTarget->Index;

	if(lpTarget->Type == OBJECT_MONSTER && OBJECT_RANGE(lpTarget->SummonIndex) != 0)
	{
		SummonTargetIndex = lpTarget->SummonIndex;
	}

	if(this->CheckPlayerTarget(&gObj[SummonIndex],&gObj[SummonTargetIndex]) == 0)
	{
		return 0;
	}

	#pragma endregion

	#pragma region ATTACK_RETURN

	int skill = ((lpSkill==0)?SKILL_NONE:lpSkill->m_skill);

	if(damage == 0 && this->DecreaseArrow(lpObj) == 0)
	{
		return 0;
	}

	if(lpObj->Type == OBJECT_USER)
	{
		lpObj->HPAutoRecuperationTime = GetTickCount();
		lpObj->MPAutoRecuperationTime = GetTickCount();
		lpObj->BPAutoRecuperationTime = GetTickCount();
	}

	if(lpTarget->Type == OBJECT_USER)
	{
		lpTarget->HPAutoRecuperationTime = GetTickCount();
		lpTarget->MPAutoRecuperationTime = GetTickCount();
		lpTarget->BPAutoRecuperationTime = GetTickCount();
	}

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonSetEnemy(lpObj,lpTarget->Index);
	}

	if(lpObj->Type == OBJECT_USER)
	{
		gDarkSpirit[lpObj->Index].SetTarget(lpTarget->Index);
	}

	bool duel = gDuel.CheckDuel(lpObj,lpTarget);

	if(lpObj->Type == OBJECT_USER && duel != 0)
	{
		lpObj->DuelTickCount = GetTickCount();
	}

	if(lpTarget->Type == OBJECT_USER && duel != 0)
	{
		lpTarget->DuelTickCount = GetTickCount();
	}

	gEffectManager.DelEffect(lpObj,EFFECT_INVISIBILITY);

	if(gEffectManager.CheckEffect(lpTarget,EFFECT_ORDER_OF_PROTECTION) != 0 && lpSkill == 0)
	{
		this->MissSend(lpObj,lpTarget,lpSkill,send,count);
		return 1;
	}

	if(gEffectManager.CheckEffect(lpTarget,EFFECT_ORDER_OF_PROTECTION) != 0 && lpSkill != 0)
	{
		this->MissSend(lpObj,lpTarget,lpSkill,send,count);
		return 1;
	}

	if(gEffectManager.CheckEffect(lpTarget,EFFECT_PHYSI_DAMAGE_IMMUNITY) != 0 && lpSkill == 0)
	{
		this->MissSend(lpObj,lpTarget,lpSkill,send,count);
		return 1;
	}

	if(gEffectManager.CheckEffect(lpTarget,EFFECT_MAGIC_DAMAGE_IMMUNITY) != 0 && lpSkill != 0)
	{
		this->MissSend(lpObj,lpTarget,lpSkill,send,count);
		return 1;
	}

	if(lpTarget->Type == OBJECT_MONSTER)
	{
		if(lpTarget->Class == 200 && lpSkill == 0)
		{
			gObjMonsterStateProc(lpTarget,6,lpObj->Index,0);
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 1;
		}

		if(lpTarget->Class == 200 && lpSkill != 0)
		{
			gObjMonsterStateProc(lpTarget,7,lpObj->Index,0);
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 1;
		}

		if(gEffectManager.CheckEffect(lpTarget,EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY) != 0 && skill == SKILL_NONE)
		{
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 1;
		}

		if(gEffectManager.CheckEffect(lpTarget,EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY) != 0 && skill != SKILL_NONE)
		{
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 1;
		}

		if(lpTarget->MonsterSkillElementOption.CheckImmuneTime() != 0)
		{
			if(lpTarget->MonsterSkillElementOption.m_SkillElementImmuneNumber == skill)
			{
				this->MissSend(lpObj,lpTarget,lpSkill,send,count);
				return 1;
			}
		}
	}

	#pragma endregion

	#pragma region DAMAGE_CALC

	flag = 0;

	BYTE miss = 0;

	WORD effect = 0;

	if(damage == 0)
	{
		if((lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER) && this->MissCheck(lpObj,lpTarget,lpSkill,send,count,&miss) == 0)
		{
			return 1;
		}

		if((lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER) && this->MissCheckPvP(lpObj,lpTarget,lpSkill,send,count,&miss) == 0)
		{
			return 1;
		}

		int defense = this->GetTargetDefense(lpObj,lpTarget,&effect);

		if((lpObj->Class == CLASS_DW || lpObj->Class == CLASS_MG) && skill != SKILL_NONE && skill != SKILL_FALLING_SLASH && skill != SKILL_LUNGE && skill != SKILL_UPPERCUT && skill != SKILL_CYCLONE && skill != SKILL_SLASH && skill != SKILL_TWISTING_SLASH && skill != SKILL_IMPALE && skill != SKILL_FIRE_SLASH && skill != SKILL_POWER_SLASH && skill != SKILL_SPIRAL_SLASH)
		{
			damage = this->GetAttackDamageWizard(lpObj,lpTarget,lpSkill,&effect,defense);
		}
		else
		{
			damage = this->GetAttackDamage(lpObj,lpTarget,lpSkill,&effect,defense);
		}

		if(damage > 0)
		{
			this->WeaponDurabilityDown(lpObj,lpTarget);
		}

		if(miss != 0)
		{
			damage = (damage*30)/100;
		}

		for(int n=0;n < MAX_DAMAGE_REDUCTION;n++)
		{
			damage -= (damage*lpTarget->DamageReduction[n])/100;
		}

		if(lpTarget->EffectOption.AddDamageReduction > 0)
		{
			damage -= (damage*lpTarget->EffectOption.AddDamageReduction)/100;
		}

		if((GetTickCount()-lpTarget->ShieldDamageReductionTime) < ((DWORD)(gServerInfo.m_DefenseTimeConstA*1000)))
		{
			damage -= (damage*lpTarget->ShieldDamageReduction)/100;
		}

		this->WingSprite(lpObj,lpTarget,&damage);

		this->HelperSprite(lpObj,lpTarget,&damage);

		int MinDamage = lpObj->Level/10;

		MinDamage = ((MinDamage<1)?1:MinDamage);

		damage = ((damage<MinDamage)?MinDamage:damage);

		this->DamageSprite(lpTarget,damage);

		if(skill == SKILL_FALLING_SLASH || skill == SKILL_LUNGE || skill == SKILL_UPPERCUT || skill == SKILL_CYCLONE || skill == SKILL_SLASH || skill == SKILL_TWISTING_SLASH || skill == SKILL_RAGEFUL_BLOW || skill == SKILL_DEATH_STAB || skill == SKILL_CRESCENT_MOON_SLASH || skill == SKILL_STAR_FALL || skill == SKILL_IMPALE || skill == SKILL_FIRE_BREATH || skill == SKILL_ICE_ARROW || skill == SKILL_PENETRATION || skill == SKILL_FIRE_SLASH || skill == SKILL_POWER_SLASH || skill == SKILL_SPIRAL_SLASH)
		{
			if(skill != SKILL_IMPALE || lpObj->Inventory[8].m_Index == GET_ITEM(13,2) || lpObj->Inventory[8].m_Index == GET_ITEM(13,3))
			{
				if(lpObj->Class == CLASS_DK)
				{
					damage = (damage*lpObj->DKDamageMultiplierRate)/100;
				}
				else
				{
					damage = (damage*200)/100;
				}
			}
		}
		else if(skill == SKILL_FORCE || skill == SKILL_FIRE_BURST || skill == SKILL_EARTHQUAKE || skill == SKILL_ELECTRIC_SPARK || skill == SKILL_FIRE_BLAST)
		{
			damage = (damage*lpObj->DLDamageMultiplierRate)/100;
		}
		
		if((GetLargeRand()%100) < ((lpObj->DoubleDamageRate)-lpTarget->ResistDoubleDamageRate))
		{
			effect |= 0x40;
			damage += damage;
		}

		if((GetLargeRand()%100) < lpObj->TripleDamageRate)
		{
			effect |= 0x100;
			damage += damage+damage;
		}

		if(combo != 0)
		{
			effect |= 0x80;
			damage += damage;
			damage += (((lpObj->Strength+lpObj->AddStrength)+(lpObj->Dexterity+lpObj->AddDexterity)+(lpObj->Energy+lpObj->AddEnergy))/gServerInfo.m_ComboDamageConstA)*gServerInfo.m_ComboDamageConstB;
			skill = SKILL_COMBO;
		}
	}
	else
	{
		if(skill != SKILL_EXPLOSION)
		{
			effect = 0x04;
			damage = (damage*((lpObj->Type==OBJECT_USER&&lpTarget->Type==OBJECT_USER)?gServerInfo.m_ReflectDamageRatePvP:gServerInfo.m_ReflectDamageRatePvM))/100;
		}
	}

	#pragma endregion

	#pragma region DAMAGE_CONFIG

	if(lpObj->Type == OBJECT_USER)
	{
		if(lpTarget->Type == OBJECT_USER)
		{
			damage = (damage*gServerInfo.m_GeneralDamageRatePvP)/100;
			damage = (damage*gServerInfo.m_DamageRatePvP[lpObj->Class])/100;
			damage = (damage*gServerInfo.m_DamageRateTo[lpObj->Class][lpTarget->Class])/100;

			if(gDuel.CheckDuel(lpObj,lpTarget) != 0)
			{
				damage = (damage*gServerInfo.m_DuelDamageRate)/100;
			}
			else if(CC_MAP_RANGE(lpObj->Map) != 0 && CC_MAP_RANGE(lpTarget->Map) != 0)
			{
				damage = (damage*gServerInfo.m_ChaosCastleDamageRate)/100;
			}
			#if(GAMESERVER_TYPE==1)
			else if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE && lpObj->Map == MAP_CASTLE_SIEGE && lpTarget->Map == MAP_CASTLE_SIEGE)
			{
				if(lpObj->CsJoinSide == 0 || lpTarget->CsJoinSide == 0 || lpObj->CsJoinSide != lpTarget->CsJoinSide)
				{
					damage = (damage*gServerInfo.m_CastleSiegeDamageRate1)/100;
				}
				else
				{
					damage = (damage*gServerInfo.m_CastleSiegeDamageRate2)/100;
				}
			}
			#endif
		}
		else
		{
			damage = (damage*gServerInfo.m_GeneralDamageRatePvM)/100;
			damage = (damage*gServerInfo.m_DamageRatePvM[lpObj->Class])/100;
		}
	}

	#pragma endregion

	#pragma region DAMAGE_FINISH

	if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
	{
		if(lpTarget->Class == 277 || lpTarget->Class == 283) // Castle Gate,Guardian Statue
		{
			if(gEffectManager.CheckEffect(lpObj,EFFECT_BLESS_POTION) != 0)
			{
				damage += (damage*20)/100;
			}

			damage = (damage*gServerInfo.m_CastleSiegeDamageRate3)/100;

			lpObj->AccumulatedDamage = ((lpObj->AccumulatedDamage>100)?0:(lpObj->AccumulatedDamage+damage));
		}
	}

	damage = ((damage<0)?0:damage);

	#pragma endregion

	#pragma region APPLY_EFFECT

	if(lpObj->Type == OBJECT_USER && effect != 4)
	{
		if((GetLargeRand()%100) < lpObj->OffensiveFullHPRestoreRate)
		{
			lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
			GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);
		}

		if((GetLargeRand()%100) < lpObj->OffensiveFullMPRestoreRate)
		{
			lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
			GCManaSend(lpObj->Index,0xFF,(int)lpObj->Mana,lpObj->BP);
		}

		if((GetLargeRand()%100) < lpObj->OffensiveFullBPRestoreRate)
		{
			lpObj->BP = lpObj->MaxBP+lpObj->AddBP;
			GCManaSend(lpObj->Index,0xFF,(int)lpObj->Mana,lpObj->BP);
		}
	}

	if(lpSkill != 0 && count <= 1)
	{
		if(this->ApplySkillEffect(lpObj,lpTarget,lpSkill,damage) == 0)
		{
			if(send != 0)
			{
				gSkillManager.GCSkillAttackSend(lpObj,lpSkill->m_skill,lpTarget->Index,0);
			}
		}
		else
		{
			if(send != 0)
			{
				gSkillManager.GCSkillAttackSend(lpObj,lpSkill->m_skill,lpTarget->Index,1);
			}
		}
	}

	#pragma endregion

	#pragma region DAMAGE_APPLY

	if(lpTarget->Life < damage)
	{
		lpTarget->Life = 0;
	}
	else
	{
		lpTarget->Life -= damage;
	}

	if(lpTarget->Type == OBJECT_MONSTER)
	{
		lpTarget->LastAttackerID = lpObj->Index;

		gObjAddMsgSendDelay(lpTarget,0,lpObj->Index,100,0);

		if(lpTarget->CurrentAI != 0)
		{
			lpTarget->Agro.IncAgro(lpObj->Index,(damage/50));
		}
	}

	#pragma endregion

	#pragma region CHECK_SELF_DEFENSE

	if(damage > 0)
	{
		if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER && lpObj->Index != lpTarget->Index)
		{
			bool CheckSelfDefense = 1;

			if(effect == 4)
			{
				CheckSelfDefense = 0;
			}

			if(gDuel.CheckDuel(lpObj,lpTarget) != 0)
			{
				CheckSelfDefense = 0;
			}

			if(gObjGetRelationShip(lpObj,lpTarget) == 2)
			{
				CheckSelfDefense = 0;
			}

			if(gObjTargetGuildWarCheck(lpObj,lpTarget) != 0)
			{
				CheckSelfDefense = 0;
			}

			#if(GAMESERVER_TYPE==1)

			if(lpObj->Map == MAP_CASTLE_SIEGE && gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE && lpObj->CsJoinSide != 0)
			{
				CheckSelfDefense = 0;
			}

			#endif

			if(CC_MAP_RANGE(lpObj->Map) != 0 && CC_MAP_RANGE(lpTarget->Map) != 0)
			{
				CheckSelfDefense = 0;
			}

			if(gMapManager.GetMapNonOutlaw(lpObj->Map) != 0)
			{
				CheckSelfDefense = 0;
			}

			if(CheckSelfDefense != 0)
			{
				gObjCheckSelfDefense(lpObj,lpTarget->Index);
			}
		}

		this->ArmorDurabilityDown(lpObj,lpTarget);
	}

	lpObj->Rest = 0;

	#pragma endregion

	#pragma region ATTACK_FINISH

	if(damage > 0)
	{
		gEffectManager.DelEffect(lpTarget,EFFECT_SLEEP);

		if(effect != 4 && lpTarget->Type == OBJECT_USER)
		{
			if((GetLargeRand()%100) < lpTarget->FullDamageReflectRate)
			{
				gObjAddMsgSendDelay(lpTarget,10,lpObj->Index,10,damage);
			}
			else if((lpTarget->DamageReflect+lpTarget->EffectOption.AddDamageReflect) > 0)
			{
				gObjAddMsgSendDelay(lpTarget,10,lpObj->Index,10,((damage*(lpTarget->DamageReflect+lpTarget->EffectOption.AddDamageReflect))/100));
			}

			if((GetLargeRand()%100) < lpTarget->DefensiveFullHPRestoreRate)
			{
				lpTarget->Life = lpTarget->MaxLife+lpTarget->AddLife;
				GCLifeSend(lpTarget->Index,0xFF,(int)lpTarget->Life);
			}

			if((GetLargeRand()%100) < lpTarget->DefensiveFullMPRestoreRate)
			{
				lpTarget->Mana = lpTarget->MaxMana+lpTarget->AddMana;
				GCManaSend(lpTarget->Index,0xFF,(int)lpTarget->Mana,lpTarget->BP);
			}

			if((GetLargeRand()%100) < lpTarget->DefensiveFullBPRestoreRate)
			{
				lpTarget->BP = lpTarget->MaxBP+lpTarget->AddBP;
				GCManaSend(lpTarget->Index,0xFF,(int)lpTarget->Mana,lpTarget->BP);
			}

			if(lpTarget->Inventory[8].IsItem() == 0 || (lpTarget->Inventory[8].m_Index != GET_ITEM(13,2) && lpTarget->Inventory[8].m_Index != GET_ITEM(13,3) && lpTarget->Inventory[8].m_Index != GET_ITEM(13,4)))
			{
				if((GetLargeRand()%100) < gServerInfo.m_DamageStuckRate[lpTarget->Class])
				{
					flag = 1;
				}
			}
		}

		gObjectManager.CharacterLifeCheck(lpObj,lpTarget,damage,0,flag,effect,((lpSkill==0)?0:lpSkill->m_skill));
	}
	else
	{
		GCDamageSend(lpObj->Index,lpTarget->Index,0,0,effect);
	}

	if(lpObj->Type == OBJECT_USER && lpObj->Life <= 0 && lpObj->CheckLifeTime <= 0)
	{
		lpObj->AttackObj = lpTarget;
		lpObj->AttackerKilled = ((lpTarget->Type==OBJECT_USER)?1:0);
		lpObj->CheckLifeTime = 3;
	}

	#pragma endregion

	return 1;
}

bool CAttack::DecreaseArrow(LPOBJ lpObj) // OK
{
	if(lpObj->Type != OBJECT_USER || lpObj->Class != CLASS_FE)
	{
		return 1;
	}

	if(gEffectManager.CheckEffect(lpObj,EFFECT_INFINITY_ARROW) != 0 || gEffectManager.CheckEffect(lpObj,EFFECT_INFINITY_ARROW_IMPROVED) != 0)
	{
		return 1;
	}

	if(lpObj->Inventory[0].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[0].m_Index < GET_ITEM(5,0) && lpObj->Inventory[0].m_Index != GET_ITEM(4,15) && lpObj->Inventory[0].m_Slot == 0)
	{
		if(lpObj->Inventory[1].m_Index != GET_ITEM(4,7) || lpObj->Inventory[1].m_Durability < 1)
		{
			return 0;
		}
		else
		{
			gItemManager.DecreaseItemDur(lpObj,1,1);
		}
	}

	if(lpObj->Inventory[1].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[1].m_Index < GET_ITEM(5,0) && lpObj->Inventory[1].m_Index != GET_ITEM(4,7) && lpObj->Inventory[1].m_Slot == 1)
	{
		if(lpObj->Inventory[0].m_Index != GET_ITEM(4,15) || lpObj->Inventory[0].m_Durability < 1)
		{
			return 0;
		}
		else
		{
			gItemManager.DecreaseItemDur(lpObj,0,1);
		}
	}

	return 1;
}

void CAttack::WingSprite(LPOBJ lpObj,LPOBJ lpTarget,int* damage) // OK
{
	if(lpObj != 0 && lpObj->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpObj->Inventory[7];

		if(lpItem->IsItem() != 0 && lpItem->m_Durability > 0)
		{
			if(lpObj->Class == CLASS_DW || lpObj->Class == CLASS_FE)
			{
				lpObj->Life -= 1;
			}
			else
			{
				lpObj->Life -= 3;
			}

			GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);

			if((lpItem->m_Index >= GET_ITEM(12,0) && lpItem->m_Index <= GET_ITEM(12,2))) // 1st wing
			{
				(*damage) = ((*damage)*(112+(lpItem->m_Level*2)))/100;
			}
			else if((lpItem->m_Index >= GET_ITEM(12,3) && lpItem->m_Index <= GET_ITEM(12,6))) // 2sd wing
			{
				(*damage) = ((*damage)*(132+(lpItem->m_Level*1)))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,30)) // Cloak of Lord
			{
				(*damage) = ((*damage)*(120+(lpItem->m_Level*2)))/100;
			}
		}
	}

	if(lpTarget != 0 && lpTarget->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpTarget->Inventory[7];

		if(lpItem->IsItem() != 0 && lpItem->m_Durability > 0)
		{
			if((lpItem->m_Index >= GET_ITEM(12,0) && lpItem->m_Index <= GET_ITEM(12,2))) // 1st wing
			{
				(*damage) = ((*damage)*(88-(lpItem->m_Level*2)))/100;
			}
			else if((lpItem->m_Index >= GET_ITEM(12,3) && lpItem->m_Index <= GET_ITEM(12,6))) // 2sd wing
			{
				(*damage) = ((*damage)*(75-(lpItem->m_Level*2)))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,30)) // Cloak of Lord
			{
				(*damage) = ((*damage)*(90-(lpItem->m_Level*1)))/100;
			}
		}
	}
}

void CAttack::HelperSprite(LPOBJ lpObj,LPOBJ lpTarget,int* damage) // OK
{
	if(lpObj != 0 && lpObj->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpObj->Inventory[8];

		if(lpItem->IsItem() != 0 && lpItem->m_Durability > 0)
		{
			if(lpItem->m_Index == GET_ITEM(13,1)) // Satan
			{
				lpObj->Life -= 3;

				GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);

				(*damage) = ((*damage)*(100+gServerInfo.m_SatanIncDamageConstA))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,3)) // Dinorant
			{
				lpObj->Life -= 1;

				GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);

				(*damage) = ((*damage)*(100+gServerInfo.m_DinorantIncDamageConstA))/100;
			}
		}
	}

	if(lpTarget != 0 && lpTarget->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpTarget->Inventory[8];

		if(lpItem->IsItem() != 0 && lpItem->m_Durability > 0)
		{
			if(lpItem->m_Index == GET_ITEM(13,0)) // Angel
			{
				(*damage) = ((*damage)*(100-gServerInfo.m_AngelDecDamageConstA))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,3)) // Dinorant
			{
				(*damage) = ((*damage)*(100-gServerInfo.m_DinorantDecDamageConstA-(((lpItem->m_NewOption & 1)==0)?0:gServerInfo.m_DinorantDecDamageConstB)))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,4)) // Dark Horse
			{
				(*damage) = ((*damage)*(100-((gServerInfo.m_DarkHorseDecDamageConstA+lpItem->m_PetItemLevel)/gServerInfo.m_DarkHorseDecDamageConstB)))/100;
			}
		}
	}
}

void CAttack::DamageSprite(LPOBJ lpObj,int damage) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return;
	}

	CItem* lpItem = &lpObj->Inventory[8];

	if(lpItem->IsItem() == 0 || lpItem->m_IsPeriodicItem != 0)
	{
		return;
	}

	float DurabilityValue = (1.0f/gServerInfo.m_GuardianDurabilityRate)*100;

	DurabilityValue = (DurabilityValue/lpObj->GuardianDurabilityRate)*100;

	if(lpItem->m_Index == GET_ITEM(13,0)) // Angel
	{
		lpItem->m_Durability -= (damage*(3.0f*DurabilityValue))/100;
	}
	else if(lpItem->m_Index == GET_ITEM(13,1)) // Satan
	{
		lpItem->m_Durability -= (damage*(2.0f*DurabilityValue))/100;
	}
	else if(lpItem->m_Index == GET_ITEM(13,2)) // Uniria
	{
		lpItem->m_Durability -= (damage*(1.0f*DurabilityValue))/100;
	}
	else if(lpItem->m_Index == GET_ITEM(13,3)) // Dinorant
	{
		lpItem->m_Durability -= (damage*(1.0f*DurabilityValue))/100;
	}
	else if(lpItem->m_Index == GET_ITEM(13,4)) // Dark Horse
	{
		if(this->DarkHorseSprite(lpObj,damage) == 0)
		{
			return;
		}
	}
	else
	{
		return;
	}

	gItemManager.GCItemDurSend(lpObj->Index,8,(BYTE)lpItem->m_Durability,0);

	if(lpItem->m_Durability < 1)
	{
		gItemManager.InventoryDelItem(lpObj->Index,8);

		gItemManager.GCItemDeleteSend(lpObj->Index,8,0);

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		gItemManager.GCItemChangeSend(lpObj->Index,8);

		if(lpObj->Map == MAP_ICARUS && lpObj->Inventory[7].IsItem() == 0 && lpItem->m_Index == GET_ITEM(13,3))
		{
			gObjMoveGate(lpObj->Index,22);
		}
	}
}

bool CAttack::DarkHorseSprite(LPOBJ lpObj,int damage) // OK
{
	CItem* lpItem = &lpObj->Inventory[8];

	if(lpItem->m_Durability < 1)
	{
		return 0;
	}

	lpItem->m_DurabilitySmall += ((damage*2)/100)+1;

	int MaxSmallDur = (1500*gServerInfo.m_PetDurabilityRate)/100;

	MaxSmallDur = (MaxSmallDur*lpObj->PetDurabilityRate)/100;

	if(lpItem->m_DurabilitySmall > MaxSmallDur)
	{
		lpItem->m_Durability = (((--lpItem->m_Durability)<1)?0:lpItem->m_Durability);
		lpItem->m_DurabilitySmall = 0;

		if(lpItem->CheckDurabilityState() != 0)
		{
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}

		gItemManager.GCItemDurSend(lpObj->Index,8,(BYTE)lpItem->m_Durability,0);

		GCPetItemInfoSend(lpObj->Index,1,0,8,lpItem->m_PetItemLevel,lpItem->m_PetItemExp,(BYTE)lpItem->m_Durability);
	}

	return 0;
}

void CAttack::WeaponDurabilityDown(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return;
	}

	if(gEffectManager.GetEffect(lpObj,EFFECT_TALISMAN_OF_PROTECTION) != 0)
	{
		return;
	}

	for(int n=0;n < 2;n++)
	{
		if(lpObj->Inventory[n].IsItem() != 0)
		{
			bool result = 0;

			switch((lpObj->Inventory[n].m_Index/MAX_ITEM_TYPE))
			{
				case 0:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,0);
					break;
				case 1:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,0);
					break;
				case 2:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,0);
					break;
				case 3:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,0);
					break;
				case 4:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,1);
					break;
				case 5:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,((lpObj->Inventory[n].m_Slot==0)?2:3));
					break;
			}

			if(result != 0)
			{
				gItemManager.GCItemDurSend(lpObj->Index,n,(BYTE)lpObj->Inventory[n].m_Durability,0);
			}
		}
	}
}

void CAttack::ArmorDurabilityDown(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	if(gEffectManager.GetEffect(lpTarget,EFFECT_TALISMAN_OF_PROTECTION) != 0)
	{
		return;
	}

	int slot = 1+(GetLargeRand()%6);

	if(lpTarget->Inventory[slot].IsItem() != 0)
	{
		if(slot != 1 || (lpTarget->Inventory[slot].m_Index >= GET_ITEM(6,0) && lpTarget->Inventory[slot].m_Index < GET_ITEM(7,0)))
		{
			if(lpTarget->Inventory[slot].ArmorDurabilityDown(lpTarget->Index,lpObj->PhysiDamageMin) != 0)
			{
				gItemManager.GCItemDurSend(lpTarget->Index,slot,(BYTE)lpTarget->Inventory[slot].m_Durability,0);
			}
		}
	}
}

bool CAttack::CheckPlayerTarget(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return 1;
	}

	if(lpTarget->Authority == 32)
	{
		return 0;
	}
	
	if(gObjGetRelationShip(lpObj,lpTarget) == 2 && gMapManager.GetMapNonPK(lpTarget->Map) == 0)
	{
		return 1;
	}

	if(lpObj->Guild != 0 && lpTarget->Guild != 0)
	{
		if(lpObj->Guild->WarState != 0 && lpTarget->Guild->WarState != 0)
		{
			if(lpObj->Guild->Number == lpTarget->Guild->Number)
			{
				return 0;
			}
		}
	}

	if(gObjTargetGuildWarCheck(lpObj,lpTarget) == 0)
	{
		if(lpTarget->Guild != 0 && lpTarget->Guild->WarState != 0)
		{
			if(lpTarget->Guild->WarType == 1 && lpTarget->Map != MAP_ARENA && gMapManager.GetMapNonPK(lpTarget->Map) == 0)
			{
				return 1;
			}

			if(CC_MAP_RANGE(lpTarget->Map) == 0)
			{
				return 0;
			}
		}
	}

	if(gDuel.CheckDuel(lpObj,lpTarget) == 0 && OBJECT_RANGE(lpTarget->DuelUser) != 0)
	{
		return 0;
	}
	
	if(DS_MAP_RANGE(lpObj->Map) != 0 && DS_MAP_RANGE(lpTarget->Map) != 0)
	{
		return 0;
	}

	if(BC_MAP_RANGE(lpObj->Map) != 0 && BC_MAP_RANGE(lpTarget->Map) != 0)
	{
		return 0;
	}

	if(CC_MAP_RANGE(lpObj->Map) != 0 && CC_MAP_RANGE(lpTarget->Map) != 0)
	{
		return ((gChaosCastle.GetState(GET_CC_LEVEL(lpObj->Map))==CC_STATE_START)?1:0);
	}

	#if(GAMESERVER_TYPE==1)

	if(lpObj->Map == MAP_CASTLE_SIEGE && lpTarget->Map == MAP_CASTLE_SIEGE)
	{
		if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			if(lpObj->CsJoinSide != 0 && lpTarget->CsJoinSide != 0)
			{
				if(gServerInfo.m_CastleSiegeDamageRate2 == 0 && lpObj->CsJoinSide == lpTarget->CsJoinSide)
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
		}
	}

	#endif
	
	if(lpObj->Level <= 5 || lpTarget->Level <= 5)
	{
		return 0;
	}

	if(gMapManager.GetMapNonPK(lpTarget->Map) != 0)
	{
		return 0;
	}

	return 1;
}

void CAttack::MissSend(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count) // OK
{
	WORD effect = 0;

	GCDamageSend(lpObj->Index,lpTarget->Index,0,0,effect);

	if(send != 0 && lpSkill != 0)
	{
		gSkillManager.GCSkillAttackSend(lpObj,lpSkill->m_skill,lpTarget->Index,0);
	}
}

bool CAttack::MissCheck(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss) // OK
{
	int AttackSuccessRate = lpObj->AttackSuccessRate;
	AttackSuccessRate += lpObj->EffectOption.AddAttackSuccessRate;
	AttackSuccessRate += (AttackSuccessRate*lpObj->EffectOption.MulAttackSuccessRate)/100;
	AttackSuccessRate -= (AttackSuccessRate*lpObj->EffectOption.DivAttackSuccessRate)/100;
	AttackSuccessRate = ((AttackSuccessRate<0)?0:AttackSuccessRate);

	int DefenseSuccessRate = lpTarget->DefenseSuccessRate;
	DefenseSuccessRate += lpTarget->EffectOption.AddDefenseSuccessRate;
	DefenseSuccessRate += (DefenseSuccessRate*lpTarget->EffectOption.MulDefenseSuccessRate)/100;
	DefenseSuccessRate -= (DefenseSuccessRate*lpTarget->EffectOption.DivDefenseSuccessRate)/100;
	DefenseSuccessRate = ((DefenseSuccessRate<0)?0:DefenseSuccessRate);
	if(AttackSuccessRate < DefenseSuccessRate)
	{
		(*miss) = 1;

		if((GetLargeRand()%100) >= 5)
		{
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 0;
		}
	}
	else
	{
		(*miss) = 0;

		if((GetLargeRand()%((AttackSuccessRate==0)?1:AttackSuccessRate)) < DefenseSuccessRate)
		{
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 0;
		}
	}

	return 1;
}

bool CAttack::MissCheckPvP(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss) // OK
{
	(*miss) = 0;

	int AttackSuccessRate = (int)(((100*(((lpObj->AttackSuccessRatePvP*10000.0f)/(lpObj->AttackSuccessRatePvP+lpTarget->DefenseSuccessRatePvP))/10000.0f))*gServerInfo.m_ShieldGaugeAttackRate)*(((lpObj->Level*10000.0f)/(lpObj->Level+lpTarget->Level))/10000.0f));

	if((lpTarget->Level-lpObj->Level) >= 100)
	{
		AttackSuccessRate -= 5;
	}
	else if((lpTarget->Level-lpObj->Level) >= 200)
	{
		AttackSuccessRate -= 10;
	}
	else if((lpTarget->Level-lpObj->Level) >= 300)
	{
		AttackSuccessRate -= 15;
	}

	if((GetLargeRand()%100) > AttackSuccessRate)
	{
		this->MissSend(lpObj,lpTarget,lpSkill,send,count);

		if(gServerInfo.m_ShieldGaugeAttackComboMiss != 0 && lpObj->ComboSkill.m_index >= 0)
		{
			lpObj->ComboSkill.Init();
		}

		return 0;
	}

	return 1;
}

bool CAttack::ApplySkillEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage) // OK
{
	if(lpTarget->Type != OBJECT_USER && ((lpTarget->Class >= 204 && lpTarget->Class <= 209) || (lpTarget->Class >= 215 && lpTarget->Class <= 219) || lpTarget->Class == 277 || lpTarget->Class == 278 || lpTarget->Class == 283 || lpTarget->Class == 288))
	{
		return 0;
	}

	if(lpTarget->Type == OBJECT_USER && (gEffectManager.CheckEffect(lpTarget,EFFECT_IRON_DEFENSE) != 0 || gEffectManager.CheckEffect(lpTarget,EFFECT_IRON_DEFENSE_IMPROVED) != 0))
	{
		return 0;
	}

	if(gSkillManager.GetSkillType(lpSkill->m_skill) != -1 && gObjCheckResistance(lpTarget,gSkillManager.GetSkillType(lpSkill->m_skill)) != 0)
	{
		return 0;
	}

	switch(lpSkill->m_skill)
	{
		case SKILL_POISON:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_skill),20,lpObj->Index,2,3,0);
			break;
		case SKILL_METEORITE:
			gSkillManager.ApplyMeteoriteEffect(lpObj,lpTarget,lpSkill,damage);
			break;
		case SKILL_LIGHTNING:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_ICE:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_skill),10,0,0,0,0);
			break;
		case SKILL_FALLING_SLASH:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_LUNGE:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_UPPERCUT:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_CYCLONE:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_SLASH:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_DECAY:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_skill),10,lpObj->Index,2,3,0);
			break;
		case SKILL_ICE_STORM:
			gSkillManager.ApplyIceStormEffect(lpObj,lpTarget,lpSkill,damage);
			break;
		case SKILL_ICE_ARROW:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_skill),7,0,0,0,0);
			break;
		case SKILL_TWISTING_SLASH:
			gSkillManager.ApplyTwistingSlashEffect(lpObj,lpTarget,lpSkill,damage);
			break;
		case SKILL_RAGEFUL_BLOW:
			gSkillManager.ApplyRagefulBlowEffect(lpObj,lpTarget,lpSkill,damage);
			break;
		case SKILL_DEATH_STAB:
			gSkillManager.ApplyDeathStabEffect(lpObj,lpTarget,lpSkill,damage);
			break;
		case SKILL_FIRE_SLASH:
			gSkillManager.ApplyFireSlashEffect(lpObj,lpTarget,lpSkill,damage);
			break;
		case SKILL_FIRE_BURST:
			gSkillManager.ApplyFireBurstEffect(lpObj,lpTarget,lpSkill,damage);
			break;
		case SKILL_EARTHQUAKE:
			gSkillManager.ApplyEarthquakeEffect(lpObj,lpTarget,lpSkill,damage);
			break;
	}

	return 1;
}

int CAttack::GetTargetDefense(LPOBJ lpObj,LPOBJ lpTarget,WORD* effect) // OK
{
	int defense = lpTarget->Defense;

	defense += lpTarget->EffectOption.AddDefense;
	defense -= lpTarget->EffectOption.SubDefense;

	gSkillManager.SkillSwordPowerGetDefense(lpObj->Index,&defense);

	if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
	{
		defense += lpTarget->DefensePvP;
	}

	if(lpTarget->MonsterSkillElementOption.CheckDefenseTime() != 0)
	{
		defense += lpTarget->MonsterSkillElementOption.m_SkillElementDefense;
	}

	if(lpTarget->EffectOption.MulDefense > 0)
	{
		defense += (defense*lpTarget->EffectOption.MulDefense)/100;
	}

	if(lpTarget->EffectOption.DivDefense > 0)
	{
		defense -= (defense*lpTarget->EffectOption.DivDefense)/100;
	}

	if(lpTarget->Type == OBJECT_USER)
	{
		defense = (defense*50)/100;
	}

	if((GetLargeRand()%100) < ((lpObj->IgnoreDefenseRate+lpObj->EffectOption.AddIgnoreDefenseRate)-lpTarget->ResistIgnoreDefenseRate))
	{
		(*effect) = 1;

		defense = 0;
	}

	defense = ((defense<0)?0:defense);

	return defense;
}

int CAttack::GetAttackDamage(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense) // OK
{
	CItem* Right = &lpObj->Inventory[0];
	CItem* Left = &lpObj->Inventory[1];

	int damage = 0;
	int DamageMin = 0;
	int DamageMax = 0;
	int SkillDamageMin = 0;
	int SkillDamageMax = 0;
	bool DualHandWeapon = 0;

	if(lpObj->Type == OBJECT_MONSTER || lpObj->Type == OBJECT_NPC)
	{
		DamageMin = lpObj->PhysiDamageMin;
		DamageMax = lpObj->PhysiDamageMax;

		DamageMin += lpObj->EffectOption.AddPhysiDamage;
		DamageMax += lpObj->EffectOption.AddPhysiDamage;

		DamageMin += lpObj->EffectOption.AddMinPhysiDamage;
		DamageMax += lpObj->EffectOption.AddMaxPhysiDamage;

		DamageMin += (DamageMin*lpObj->EffectOption.MulPhysiDamage)/100;
		DamageMax += (DamageMax*lpObj->EffectOption.MulPhysiDamage)/100;

		DamageMin -= (DamageMin*lpObj->EffectOption.DivPhysiDamage)/100;
		DamageMax -= (DamageMax*lpObj->EffectOption.DivPhysiDamage)/100;

		int range = (DamageMax-DamageMin);

		range = ((range<1)?1:range);

		damage = DamageMin+(GetLargeRand()%range);
	}
	else
	{
		if(lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL)
		{
			if(Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(4,0) && Left->m_Index >= GET_ITEM(0,0) && Left->m_Index < GET_ITEM(4,0))
			{
				if(Right->m_IsValidItem != 0 && Left->m_IsValidItem != 0)
				{
					DualHandWeapon = 1;
				}
			}
		}

		if(lpSkill != 0)
		{
			SkillDamageMin = lpSkill->m_DamageMin;
			SkillDamageMax = lpSkill->m_DamageMax;

			SkillDamageMin += lpObj->SkillDamageBonus;
			SkillDamageMax += lpObj->SkillDamageBonus;

			int type = gSkillManager.GetSkillType(lpSkill->m_skill);

			if(CHECK_RANGE(type,MAX_RESISTANCE_TYPE) != 0)
			{
				SkillDamageMin += lpObj->AddResistance[type];
				SkillDamageMax += lpObj->AddResistance[type];
			}

			if(lpObj->Class == CLASS_DL)
			{
				switch(lpSkill->m_skill)
				{
					case SKILL_EARTHQUAKE:
						SkillDamageMin += ((lpObj->Strength+lpObj->AddStrength)/gServerInfo.m_EarthquakeDamageConstA)+((lpObj->Leadership+lpObj->AddLeadership)/gServerInfo.m_EarthquakeDamageConstB)+(lpObj->Inventory[8].m_PetItemLevel*gServerInfo.m_EarthquakeDamageConstC);
						SkillDamageMax += ((lpObj->Strength+lpObj->AddStrength)/gServerInfo.m_EarthquakeDamageConstA)+((lpObj->Leadership+lpObj->AddLeadership)/gServerInfo.m_EarthquakeDamageConstB)+(lpObj->Inventory[8].m_PetItemLevel*gServerInfo.m_EarthquakeDamageConstC);
						break;
					case SKILL_ELECTRIC_SPARK:
						SkillDamageMin += ((lpObj->Leadership+lpObj->AddLeadership)/gServerInfo.m_ElectricSparkDamageConstA)+gServerInfo.m_ElectricSparkDamageConstB;
						SkillDamageMax += ((lpObj->Leadership+lpObj->AddLeadership)/gServerInfo.m_ElectricSparkDamageConstA)+gServerInfo.m_ElectricSparkDamageConstB;
						break;
					default:
						SkillDamageMin += ((lpObj->Strength+lpObj->AddStrength)/gServerInfo.m_DLSkillDamageConstA)+((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_DLSkillDamageConstB);
						SkillDamageMax += ((lpObj->Strength+lpObj->AddStrength)/gServerInfo.m_DLSkillDamageConstA)+((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_DLSkillDamageConstB);
						break;
				}
			}
		}

		if(DualHandWeapon != 0)
		{
			DamageMin = lpObj->PhysiDamageMinRight+lpObj->PhysiDamageMinLeft+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxRight+lpObj->PhysiDamageMaxLeft+SkillDamageMax;
		}
		else if((Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(4,0)) || (Right->m_Index >= GET_ITEM(5,0) && Right->m_Index < GET_ITEM(6,0)))
		{
			DamageMin = lpObj->PhysiDamageMinRight+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxRight+SkillDamageMax;
		}
		else if(Right->m_Index >= GET_ITEM(4,0) && Right->m_Index < GET_ITEM(5,0) && Right->m_Index != GET_ITEM(4,15) && Right->m_Slot == 0)
		{
			DamageMin = lpObj->PhysiDamageMinRight+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxRight+SkillDamageMax;
		}
		else if(Left->m_Index >= GET_ITEM(4,0) && Left->m_Index < GET_ITEM(5,0) && Left->m_Index != GET_ITEM(4,7) && Left->m_Slot == 1)
		{
			DamageMin = lpObj->PhysiDamageMinLeft+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxLeft+SkillDamageMax;
		}
		else
		{
			DamageMin = lpObj->PhysiDamageMinLeft+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxLeft+SkillDamageMax;
		}

		DamageMin += lpObj->EffectOption.AddPhysiDamage;
		DamageMax += lpObj->EffectOption.AddPhysiDamage;

		DamageMin += lpObj->EffectOption.AddMinPhysiDamage;
		DamageMax += lpObj->EffectOption.AddMaxPhysiDamage;

		gSkillManager.SkillSwordPowerGetPhysiDamage(lpObj->Index,&DamageMin,&DamageMax);

		DamageMin += (DamageMin*lpObj->EffectOption.MulPhysiDamage)/100;
		DamageMax += (DamageMax*lpObj->EffectOption.MulPhysiDamage)/100;

		DamageMin -= (DamageMin*lpObj->EffectOption.DivPhysiDamage)/100;
		DamageMax -= (DamageMax*lpObj->EffectOption.DivPhysiDamage)/100;

		int range = (DamageMax-DamageMin);

		range = ((range<1)?1:range);

		damage = DamageMin+(GetLargeRand()%range);

		if((GetLargeRand()%100) < ((lpObj->CriticalDamageRate+lpObj->EffectOption.AddCriticalDamageRate)-lpTarget->ResistCriticalDamageRate))
		{
			(*effect) = 3;

			damage = DamageMax;
			damage += lpObj->CriticalDamage;
			damage += lpObj->EffectOption.AddCriticalDamage;
		}

		if((GetLargeRand()%100) < ((lpObj->ExcellentDamageRate+lpObj->EffectOption.AddExcellentDamageRate)-lpTarget->ResistExcellentDamageRate))
		{
			(*effect) = 2;

			damage = (DamageMax*120)/100;
			damage += lpObj->ExcellentDamage;
			damage += lpObj->EffectOption.AddExcellentDamage;
		}
	}

	if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
	{
		damage += lpObj->DamagePvP;
	}

	if(lpObj->MonsterSkillElementOption.CheckAttackTime() != 0)
	{
		damage += lpObj->MonsterSkillElementOption.m_SkillElementAttack;
	}

	damage -= TargetDefense;

	damage = ((damage<0)?0:damage);

	return damage;
}

int CAttack::GetAttackDamageWizard(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense) // OK
{
	CItem* Right = &lpObj->Inventory[0];
	CItem* Left = &lpObj->Inventory[1];

	int DamageMin = lpObj->MagicDamageMin;
	int DamageMax = lpObj->MagicDamageMax;

	if(lpSkill->m_skill == SKILL_NOVA && lpObj->SkillNovaCount >= 0)
	{
		DamageMin += (gServerInfo.m_NovaDamageConstA*(lpObj->SkillNovaCount*gServerInfo.m_NovaDamageConstB))+((lpObj->Strength+lpObj->AddStrength)/gServerInfo.m_NovaDamageConstC);
		DamageMax += (gServerInfo.m_NovaDamageConstA*(lpObj->SkillNovaCount*gServerInfo.m_NovaDamageConstB))+((lpObj->Strength+lpObj->AddStrength)/gServerInfo.m_NovaDamageConstC);
	}
	else
	{
		DamageMin += lpSkill->m_DamageMin;
		DamageMax += lpSkill->m_DamageMax;
	}

	DamageMin += lpObj->SkillDamageBonus;
	DamageMax += lpObj->SkillDamageBonus;

	DamageMin += lpObj->EffectOption.AddMagicDamage;
	DamageMax += lpObj->EffectOption.AddMagicDamage;

	DamageMin += lpObj->EffectOption.AddMinMagicDamage;
	DamageMax += lpObj->EffectOption.AddMaxMagicDamage;

	gSkillManager.SkillSwordPowerGetMagicDamage(lpObj->Index,&DamageMin,&DamageMax);

	int type = gSkillManager.GetSkillType(lpSkill->m_skill);

	if(CHECK_RANGE(type,MAX_RESISTANCE_TYPE) != 0)
	{
		DamageMin += lpObj->AddResistance[type];
		DamageMax += lpObj->AddResistance[type];
	}

	DamageMin += (DamageMin*lpObj->EffectOption.MulMagicDamage)/100;
	DamageMax += (DamageMax*lpObj->EffectOption.MulMagicDamage)/100;

	DamageMin -= (DamageMin*lpObj->EffectOption.DivMagicDamage)/100;
	DamageMax -= (DamageMax*lpObj->EffectOption.DivMagicDamage)/100;

	if(Right->IsItem() != 0 && Right->m_IsValidItem != 0 && ((Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(1,0)) || (Right->m_Index >= GET_ITEM(5,0) && Right->m_Index < GET_ITEM(6,0))))
	{
		int rise = (int)(((Right->m_MagicDamageRate/2)+(Right->m_Level*2))*Right->m_CurrentDurabilityState);
		DamageMin += (DamageMin*rise)/100;
		DamageMax += (DamageMax*rise)/100;
	}

	int range = (DamageMax-DamageMin);

	range = ((range<1)?1:range);

	int damage = DamageMin+(GetLargeRand()%range);

	if((GetLargeRand()%100) < ((lpObj->CriticalDamageRate+lpObj->EffectOption.AddCriticalDamageRate)-lpTarget->ResistCriticalDamageRate))
	{
		(*effect) = 3;

		damage = DamageMax;
		damage += lpObj->CriticalDamage;
		damage += lpObj->EffectOption.AddCriticalDamage;
	}

	if((GetLargeRand()%100) < ((lpObj->ExcellentDamageRate+lpObj->EffectOption.AddExcellentDamageRate)-lpTarget->ResistExcellentDamageRate))
	{
		(*effect) = 2;

		damage = (DamageMax*120)/100;
		damage += lpObj->ExcellentDamage;
		damage += lpObj->EffectOption.AddExcellentDamage;
	}

	if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
	{
		damage += lpObj->DamagePvP;
	}

	damage -= TargetDefense;

	damage = ((damage<0)?0:damage);

	return damage;
}

int CAttack::GetAttackDamageCursed(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense) // OK
{
	CItem* Right = &lpObj->Inventory[0];
	CItem* Left = &lpObj->Inventory[1];

	int DamageMin = lpObj->CurseDamageMin;
	int DamageMax = lpObj->CurseDamageMax;

	DamageMin += lpSkill->m_DamageMin;
	DamageMax += lpSkill->m_DamageMax;

	DamageMin += lpObj->SkillDamageBonus;
	DamageMax += lpObj->SkillDamageBonus;

	DamageMin += lpObj->EffectOption.AddCurseDamage;
	DamageMax += lpObj->EffectOption.AddCurseDamage;

	DamageMin += lpObj->EffectOption.AddMinCurseDamage;
	DamageMax += lpObj->EffectOption.AddMaxCurseDamage;

	gSkillManager.SkillSwordPowerGetCurseDamage(lpObj->Index,&DamageMin,&DamageMax);

	int type = gSkillManager.GetSkillType(lpSkill->m_skill);

	if(CHECK_RANGE(type,MAX_RESISTANCE_TYPE) != 0)
	{
		DamageMin += lpObj->AddResistance[type];
		DamageMax += lpObj->AddResistance[type];
	}

	DamageMin += (DamageMin*lpObj->EffectOption.MulCurseDamage)/100;
	DamageMax += (DamageMax*lpObj->EffectOption.MulCurseDamage)/100;

	DamageMin -= (DamageMin*lpObj->EffectOption.DivCurseDamage)/100;
	DamageMax -= (DamageMax*lpObj->EffectOption.DivCurseDamage)/100;

	if(Left->IsItem() != 0 && Left->m_IsValidItem != 0 && Left->m_Index >= GET_ITEM(5,21) && Left->m_Index <= GET_ITEM(5,23))
	{
		int rise = (int)(((Left->m_MagicDamageRate/2)+(Left->m_Level*2))*Left->m_CurrentDurabilityState);
		DamageMin += (DamageMin*rise)/100;
		DamageMax += (DamageMax*rise)/100;
	}

	int range = (DamageMax-DamageMin);

	range = ((range<1)?1:range);

	int damage = DamageMin+(GetLargeRand()%range);

	if((GetLargeRand()%100) < ((lpObj->CriticalDamageRate+lpObj->EffectOption.AddCriticalDamageRate)-lpTarget->ResistCriticalDamageRate))
	{
		(*effect) = 3;

		damage = DamageMax;
		damage += lpObj->CriticalDamage;
		damage += lpObj->EffectOption.AddCriticalDamage;
	}

	if((GetLargeRand()%100) < ((lpObj->ExcellentDamageRate+lpObj->EffectOption.AddExcellentDamageRate)-lpTarget->ResistExcellentDamageRate))
	{
		(*effect) = 2;

		damage = (DamageMax*120)/100;
		damage += lpObj->ExcellentDamage;
		damage += lpObj->EffectOption.AddExcellentDamage;
	}

	if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
	{
		damage += lpObj->DamagePvP;
	}

	damage -= TargetDefense;

	damage = ((damage<0)?0:damage);

	return damage;
}

void CAttack::GetPreviewDefense(LPOBJ lpObj,DWORD* defense) // OK
{
	(*defense) = lpObj->Defense;
	(*defense) += lpObj->EffectOption.AddDefense;
	(*defense) -= lpObj->EffectOption.SubDefense;

	gSkillManager.SkillSwordPowerGetDefense(lpObj->Index,(int*)defense);

	(*defense) += ((*defense)*lpObj->EffectOption.MulDefense)/100;
	(*defense) -= ((*defense)*lpObj->EffectOption.DivDefense)/100;
}

void CAttack::GetPreviewPhysiDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage) // OK
{
	CItem* Right = &lpObj->Inventory[0];
	CItem* Left = &lpObj->Inventory[1];

	bool DualHandWeapon = 0;

	if(lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL)
	{
		if(Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(4,0) && Left->m_Index >= GET_ITEM(0,0) && Left->m_Index < GET_ITEM(4,0))
		{
			if(Right->m_IsValidItem != 0 && Left->m_IsValidItem != 0)
			{
				DualHandWeapon = 1;
			}
		}
	}

	if(DualHandWeapon != 0)
	{
		(*DamageMin) = lpObj->PhysiDamageMinRight+lpObj->PhysiDamageMinLeft;
		(*DamageMax) = lpObj->PhysiDamageMaxRight+lpObj->PhysiDamageMaxLeft;
	}
	else if((Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(4,0)) || (Right->m_Index >= GET_ITEM(5,0) && Right->m_Index < GET_ITEM(6,0)))
	{
		(*DamageMin) = lpObj->PhysiDamageMinRight;
		(*DamageMax) = lpObj->PhysiDamageMaxRight;
	}
	else if(Right->m_Index >= GET_ITEM(4,0) && Right->m_Index < GET_ITEM(5,0) && Right->m_Index != GET_ITEM(4,15) && Right->m_Slot == 0)
	{
		(*DamageMin) = lpObj->PhysiDamageMinRight;
		(*DamageMax) = lpObj->PhysiDamageMaxRight;
	}
	else if(Left->m_Index >= GET_ITEM(4,0) && Left->m_Index < GET_ITEM(5,0) && Left->m_Index != GET_ITEM(4,7) && Left->m_Slot == 1)
	{
		(*DamageMin) = lpObj->PhysiDamageMinLeft;
		(*DamageMax) = lpObj->PhysiDamageMaxLeft;
	}
	else
	{
		(*DamageMin) = lpObj->PhysiDamageMinLeft;
		(*DamageMax) = lpObj->PhysiDamageMaxLeft;
	}

	(*DamageMin) += lpObj->EffectOption.AddPhysiDamage;
	(*DamageMax) += lpObj->EffectOption.AddPhysiDamage;

	(*DamageMin) += lpObj->EffectOption.AddMinPhysiDamage;
	(*DamageMax) += lpObj->EffectOption.AddMaxPhysiDamage;

	gSkillManager.SkillSwordPowerGetPhysiDamage(lpObj->Index,(int*)DamageMin,(int*)DamageMax);

	(*MulDamage) = lpObj->EffectOption.MulPhysiDamage;
	(*DivDamage) = lpObj->EffectOption.DivPhysiDamage;
}

void CAttack::GetPreviewMagicDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage,DWORD* DamageRate) // OK
{
	CItem* Right = &lpObj->Inventory[0];
	CItem* Left = &lpObj->Inventory[1];

	(*DamageMin) = lpObj->MagicDamageMin;
	(*DamageMax) = lpObj->MagicDamageMax;

	(*DamageMin) += lpObj->EffectOption.AddMagicDamage;
	(*DamageMax) += lpObj->EffectOption.AddMagicDamage;

	(*DamageMin) += lpObj->EffectOption.AddMinMagicDamage;
	(*DamageMax) += lpObj->EffectOption.AddMaxMagicDamage;

	gSkillManager.SkillSwordPowerGetMagicDamage(lpObj->Index,(int*)DamageMin,(int*)DamageMax);

	(*MulDamage) = lpObj->EffectOption.MulMagicDamage;
	(*DivDamage) = lpObj->EffectOption.DivMagicDamage;

	if(Right->IsItem() != 0 && Right->m_IsValidItem != 0 && ((Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(1,0)) || (Right->m_Index >= GET_ITEM(5,0) && Right->m_Index < GET_ITEM(6,0))))
	{
		(*DamageRate) = (int)(((Right->m_MagicDamageRate/2)+(Right->m_Level*2))*Right->m_CurrentDurabilityState);
	}
	else
	{
		(*DamageRate) = 0;
	}
}

void CAttack::GetPreviewCurseDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage,DWORD* DamageRate) // OK
{
	CItem* Right = &lpObj->Inventory[0];
	CItem* Left = &lpObj->Inventory[1];

	(*DamageMin) = lpObj->CurseDamageMin;
	(*DamageMax) = lpObj->CurseDamageMax;

	(*DamageMin) += lpObj->EffectOption.AddCurseDamage;
	(*DamageMax) += lpObj->EffectOption.AddCurseDamage;

	(*DamageMin) += lpObj->EffectOption.AddMinCurseDamage;
	(*DamageMax) += lpObj->EffectOption.AddMaxCurseDamage;

	gSkillManager.SkillSwordPowerGetCurseDamage(lpObj->Index,(int*)DamageMin,(int*)DamageMax);

	(*MulDamage) = lpObj->EffectOption.MulCurseDamage;
	(*DivDamage) = lpObj->EffectOption.DivCurseDamage;

	if(Left->IsItem() != 0 && Left->m_IsValidItem != 0 && Left->m_Index >= GET_ITEM(5,21) && Left->m_Index <= GET_ITEM(5,23))
	{
		(*DamageRate) = (int)(((Left->m_MagicDamageRate/2)+(Left->m_Level*2))*Left->m_CurrentDurabilityState);
	}
	else
	{
		(*DamageRate) = 0;
	}
}

void CAttack::GetPreviewDamageMultiplier(LPOBJ lpObj,DWORD* DamageMultiplier,DWORD* RFDamageMultiplierA,DWORD* RFDamageMultiplierB,DWORD* RFDamageMultiplierC) // OK
{
	switch(lpObj->Class)
	{
		case CLASS_DW:
			(*DamageMultiplier) = 200;
			(*RFDamageMultiplierA) = 100;
			(*RFDamageMultiplierB) = 100;
			(*RFDamageMultiplierC) = 100;
			break;
		case CLASS_DK:
			(*DamageMultiplier) = lpObj->DKDamageMultiplierRate;
			(*RFDamageMultiplierA) = 100;
			(*RFDamageMultiplierB) = 100;
			(*RFDamageMultiplierC) = 100;
			break;
		case CLASS_FE:
			(*DamageMultiplier) = 200;
			(*RFDamageMultiplierA) = 100;
			(*RFDamageMultiplierB) = 100;
			(*RFDamageMultiplierC) = 100;
			break;
		case CLASS_MG:
			(*DamageMultiplier) = 200;
			(*RFDamageMultiplierA) = 100;
			(*RFDamageMultiplierB) = 100;
			(*RFDamageMultiplierC) = 100;
			break;
		case CLASS_DL:
			(*DamageMultiplier) = lpObj->DLDamageMultiplierRate;
			(*RFDamageMultiplierA) = 100;
			(*RFDamageMultiplierB) = 100;
			(*RFDamageMultiplierC) = 100;
			break;
	}
}

void CAttack::CGAttackRecv(PMSG_ATTACK_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpTarget->Live == 0)
	{
		return;
	}

	if(lpObj->Map != lpTarget->Map)
	{
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X,lpTarget->Y,1) != 0)
	{
		return;
	}

	if(lpObj->Type == OBJECT_USER && sqrt(pow(((float)lpObj->X-(float)lpTarget->X),2)+pow(((float)lpObj->Y-(float)lpTarget->Y),2)) > ((lpObj->Class==CLASS_FE)?6:3))
	{
		return;
	}

	lpObj->Dir = lpMsg->dir;

	lpObj->MultiSkillIndex = 0;

	lpObj->MultiSkillCount = 0;

	GCActionSend(lpObj,lpMsg->action,aIndex,bIndex);

	lpObj->ComboSkill.Init();

	this->Attack(lpObj,lpTarget,0,0,0,0,0,0);
}
