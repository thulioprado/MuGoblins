#pragma once

struct MonsterName
{
	short Index;
	char Name[52];
};

class CMonster
{
public:
	CMonster();
	~CMonster();

public:
	void Load();
	void Reset();
	void Add(WORD Index, BYTE Percent);
	char* GetName(WORD Index) const;
	BYTE GetPercent(WORD Index) const;

public:
	static void DrawHealth();

private:
	std::unordered_map<WORD, BYTE> Monsters;
};

extern CMonster Monster;