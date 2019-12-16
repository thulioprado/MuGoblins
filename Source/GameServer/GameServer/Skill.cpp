// Skill.cpp: implementation of the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Skill.h"
#include "SkillManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkill::CSkill() // OK
{
	this->Clear();
}

CSkill::~CSkill() // OK
{

}

void CSkill::Clear() // OK
{
	this->m_level = 0;
	this->m_skill = SKILL_NONE;
	this->m_DamageMin = 0;
	this->m_DamageMax = 0;
}

bool CSkill::IsSkill() // OK
{
	return (this->m_skill != SKILL_NONE);
}

bool CSkill::Set(BYTE skill, int level) // OK
{
	if (this->IsSkill() != 0)
	{
		return false;
	}

	this->m_level = level;
	this->m_skill = skill;
	this->m_DamageMin = gSkillManager.GetSkillDamage(skill);
	this->m_DamageMax = this->m_DamageMin + (this->m_DamageMin / 2);

	return true;
}
