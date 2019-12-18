#pragma once

struct MonsterInfo
{
	WORD Index;
	BYTE Percent;
};

class CMonster
{
public:
	CMonster();
	~CMonster();

public:
	void Load();
	void Reset(BYTE Count);
	void Add(WORD Index, BYTE Percent);
	MonsterInfo* Search(WORD Index);

public:
	static void DrawHealth();

private:
	MonsterInfo Monsters[75];
	BYTE Count;
	BYTE Current;
};

extern CMonster Monster;