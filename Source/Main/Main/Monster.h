#pragma once

struct MonsterName
{
	short Index;
	char Name[52];
};

struct MonsterInfo
{
	int Model;
	const char* Folder;
	const char* File;
	bool IsNPC;
};

class CMonster
{
public:
	CMonster();
	~CMonster();

public:
	void Load();
	char* GetName(WORD Index) const;
	MonsterInfo* Get(WORD Index);
	void ResetHealth();
	void AddHealth(WORD Index, BYTE Percent);
	BYTE GetHealthPercent(WORD Index) const;

public:
	static void DrawHealth();
	static struct ViewportInfo* Renderize(int Index, DWORD a2, int a3, int a4);
	static void SetType();
	static void CanTalk();

private:
	std::unordered_map<WORD, MonsterInfo> Monsters;
	std::unordered_map<WORD, BYTE> Health;
};

extern CMonster Monster;