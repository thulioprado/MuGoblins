#pragma once

class CChaosMachine
{
public:
	CChaosMachine();
	~CChaosMachine();

public:
	void Load();
	void Clear();
	void Add(struct ItemInfo* Item);
	void Check();

public:
	static void ClearItems();
	static void CountItems();
	static void CheckCombination();
	static void ClickMix();
	static void Description();
	static void Confirm();

private:
	std::unordered_map<WORD, int> Items;
	DWORD Total;
	const char* Title;
	DWORD Index;
	DWORD Money;
	DWORD Rate;
	DWORD X;
	DWORD Y;
};

extern CChaosMachine ChaosMachine;