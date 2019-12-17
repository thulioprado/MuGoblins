// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define INVENTORY_WEAR_SIZE 12
#define INVENTORY_MAIN_SIZE 76
#define INVENTORY_EXT1_SIZE 76
#define INVENTORY_EXT2_SIZE 76
#define INVENTORY_EXT3_SIZE 76
#define INVENTORY_EXT4_SIZE 76
#define INVENTORY_FULL_SIZE 108
#define INVENTORY_SIZE 108
#define TRADE_SIZE 32
#define WAREHOUSE_EXT1_SIZE 120
#define WAREHOUSE_SIZE 120
#define CHAOS_BOX_SIZE 32
#define MAX_EXC_OPTION 6
#define MAX_SOCKET_OPTION 5
#define MAX_SPECIAL 14
#define MAX_RESISTANCE_TYPE 7
#define MAX_PET_LEVEL 50
#define MAX_ITEM_PRICE 3000000000

enum eSpecialNumber
{
	SPECIAL_OPTION1 = 0,
	SPECIAL_OPTION2 = 1,
	SPECIAL_OPTION3 = 2,
	SPECIAL_EXCELLENT1 = 3,
	SPECIAL_EXCELLENT2 = 4,
	SPECIAL_EXCELLENT3 = 5,
	SPECIAL_EXCELLENT4 = 6,
	SPECIAL_EXCELLENT5 = 7,
	SPECIAL_EXCELLENT6 = 8,
	SPECIAL_COMMON1 = 9,
	SPECIAL_COMMON2 = 10,
	SPECIAL_COMMON3 = 11,
	SPECIAL_COMMON4 = 12,
	SPECIAL_COMMON5 = 13,
};

class CPetItemExp
{
public:
	CPetItemExp() // OK
	{
		this->m_DarkSpiritExpTable[0] = 0;
		this->m_DarkSpiritExpTable[1] = 0;

		for(int n=2;n < MAX_PET_LEVEL+2;n++)
		{
			this->m_DarkSpiritExpTable[n] = ((((n+10)*n)*n)*n)*100;
		}

		this->m_DarkHorseExpTable[0] = 0;
		this->m_DarkHorseExpTable[1] = 0;

		for(int n=2;n < MAX_PET_LEVEL+2;n++)
		{
			this->m_DarkHorseExpTable[n] = ((((n+10)*n)*n)*n)*100;
		}
	};
public:
	int m_DarkSpiritExpTable[MAX_PET_LEVEL+2];
	int m_DarkHorseExpTable[MAX_PET_LEVEL+2];
};

class CItem
{
public:
	CItem();
	void Clear();
	bool IsItem();
	bool IsExcItem();
	bool IsSetItem();
	bool IsClass(int Class,int ChangeUp);
	void Convert(int index,BYTE Option1,BYTE Option2,BYTE Option3,BYTE NewOption,BYTE SetOption);
	void Value();
	void OldValue();
	void PetValue();
	void SetPetItemInfo(int PetLevel,int PetExp);
	int GetDamageMin();
	int GetDamageMax();
	int GetDefense();
	int GetDefenseSuccessRate();
	int GetBookSuccessRate();
	bool WeaponDurabilityDown(int aIndex,int defense,int type);
	bool ArmorDurabilityDown(int aIndex,int damage);
	bool WingDurabilityDown(int aIndex,int decrease);
	bool PendantDurabilityDown(int aIndex,int decrease);
	bool RingDurabilityDown(int aIndex,int decrease);
	bool CheckDurabilityState();
	bool AddPetItemExp(int amount);
	bool DecPetItemExp(int amount);
public:
	DWORD m_Serial;
	short m_Index;
	short m_Level;
	BYTE m_Slot;
	BYTE m_Class;
	BYTE m_TwoHand;
	BYTE m_AttackSpeed;
	BYTE m_WalkSpeed;
	WORD m_DamageMin;
	WORD m_DamageMax;
	BYTE m_DefenseSuccessRate;
	WORD m_Defense;
	WORD m_MagicDefense;
	BYTE m_Speed;
	WORD m_BaseDamageMin;
	WORD m_BaseDefense;
	WORD m_MagicDamageRate;
	float m_Durability;
	float m_BaseDurability;
	WORD m_DurabilitySmall;
	WORD m_SpecialIndex[MAX_SPECIAL];
	WORD m_SpecialValue[MAX_SPECIAL];
	WORD m_RequireStrength;
	WORD m_RequireDexterity;
	WORD m_RequireEnergy;
	WORD m_RequireLevel;
	WORD m_RequireVitality;
	WORD m_RequireLeadership;
	WORD m_Leadership;
	BYTE m_Resistance[MAX_RESISTANCE_TYPE];
	DWORD m_SellMoney;
	DWORD m_BuyMoney;
	int m_Value;
	int m_PShopValue;
	int m_PcPointValue;
	int m_ItemExist;
	int m_OldSellMoney;
	int m_OldBuyMoney;
	BYTE m_Option1;
	BYTE m_Option2;
	BYTE m_Option3;
	BYTE m_NewOption;
	float m_DurabilityState[4];
	float m_CurrentDurabilityState;
	bool m_SkillChange;
	bool m_QuestItem;
	BYTE m_SetOption;
	BYTE m_SetAddStat;
	bool m_IsValidItem;
	bool m_IsExpiredItem;
	bool m_IsPeriodicItem;
	bool m_LoadPeriodicItem;
	int m_PeriodicItemTime;
	int m_LoadPetItemInfo;
	int m_PetItemLevel;
	int m_PetItemExp;
};
