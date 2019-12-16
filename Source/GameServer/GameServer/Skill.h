// Skill.h: interface for the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CSkill
{
public:
	CSkill();
	virtual ~CSkill();
	void Clear();
	bool IsSkill();
	bool Set(BYTE skill,int level);

public:
	BYTE m_level;
	BYTE m_skill;
	int m_DamageMin;
	int m_DamageMax;
};
