// ComboSkill.h: interface for the CComboSkill class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CComboSkill
{
public:
	void Init();
	int GetSkillType(BYTE skill);
	bool CheckCombo(BYTE skill);
public:
	DWORD m_time;
	BYTE m_skill[2];
	int m_index;
};
