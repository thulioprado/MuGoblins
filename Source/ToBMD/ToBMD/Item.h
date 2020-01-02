#pragma once

class CItem
{
private:
	struct BMD
	{
		char Name[30];
		BYTE TwoHand;
		BYTE Level;
		BYTE Slot;
		BYTE Skill;
		BYTE Width;
		BYTE Height;
		BYTE DamageMin;
		BYTE DamageMax;
		BYTE DefenseSuccessRate;
		BYTE Defense;
		BYTE MagicDefense;
		BYTE AttackSpeed;
		BYTE WalkSpeed;
		BYTE Durability;
		BYTE MagicDurability;
		BYTE MagicDamageRate;
		WORD RequireStrength;
		WORD RequireDexterity;
		WORD RequireEnergy;
		WORD RequireVitality;
		WORD RequireLeadership;
		WORD RequireLevel;
		WORD Value;
		DWORD BuyMoney;
		BYTE Attribute;
		BYTE RequireClass[MAX_CLASS];
		BYTE Resistance[MAX_RESISTANCE_TYPE];
		BYTE Unk1;
		BYTE Unk2;
		BYTE Unk3;
	};

#pragma pack(push, 1)
	struct ExtraBMD
	{
		char Name[50];
		WORD Defense;
	};
#pragma pack(pop)

public:
	CItem();
	~CItem();

public:
	void Convert(const char* From, const char* To, const char* To2);
};

extern CItem Item;