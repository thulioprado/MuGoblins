#pragma once

#pragma pack(push, 1)
struct PlayerObject
{
	char Name[11];				// 0
	BYTE Class;					// B
	BYTE Unknown[2];			// C
	short Level;				// E
	DWORD Experience;			// 10
	DWORD NextExperience;		// 14
	WORD Strength;				// 18
	WORD Dexterity;				// 1A
	WORD Vitality;				// 1C
	WORD Energy;				// 1E
	WORD Leadership;			// 20
	WORD HP;					// 22
	WORD MP;					// 24
	WORD MaxHP;					// 26
	WORD MaxMP;					// 28
	short AddStrength;			// 2A
	short AddDexterity;			// 2C
	short AddVitality;			// 2E
	short AddEnergy;			// 30
	short AddLeadership;		// 32
	BYTE Unknown5[4];			// 34
	WORD BP;					// 38
	WORD MaxBP;					// 3A
	BYTE IsUnkByte1;			// 3C
	BYTE IsUnkByte2;			// 3D
	WORD IsUnkWord1;			// 3E
	WORD IsUnkWord2;			// 40
	WORD IsUnkWord3;			// 42
	WORD AddPoint;				// 44
	WORD MaxAddPoint;			// 46
	WORD MinusPoint;			// 48
	WORD MaxMinusPoint;			// 4A
	WORD AttackSpeed;			// 4C
	WORD AttackRate;			// 4E
	WORD AttackDamageMinLeft;	// 50
	WORD AttackDamageMaxLeft;	// 52
	WORD AttackDamageMinRight;	// 54
	WORD AttackDamageMaxRight;	// 56
	WORD MagicSpeed;			// 58
	WORD MagicDamageMin;		// 5A
	WORD MagicDamageMax;		// 5C
	BYTE Unknown4[0x2];			// 5E
	WORD DefenseRate;			// 60
	WORD Defense;				// 62
	BYTE Unknown6[0x4];			// 64
	WORD LevelUpPoint;			// 68
	BYTE Unknown7[0xEA1];		// 6A
};
#pragma pack(pop)

struct CharacterList
{
	PrismColor PrismWeapon;
	PrismColor PrismArmor;
	BYTE Sign;
};

struct PrismColorByte
{
	BYTE Red;
	BYTE Green;
	BYTE Blue;
};

struct PrismInfo
{
	BYTE Type;
	PrismColorByte Color[2];
	char Speed[2];
};

struct CustomItemInfo
{
	PrismInfo Prism;
};

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

public:
	void Load();
	void Execute(int Index, std::function<void(PlayerObject*)> Callback);
	void SetInventory(BYTE Slot, LPBYTE ItemInfo);
	void SetTempSource(BYTE Slot, LPBYTE ItemInfo);
	void SetTempTarget(BYTE Slot, LPBYTE ItemInfo);

public:
	static void Update();
	static void DrawDamage(int Unk1, int Damage, int Unk2, int Unk3, char Unk4);
	static void DrawHP1();
	static void DrawHP2();
	static void DrawHP3();
	static void DrawHP4();
	static void DrawHP5();
	static void DrawMP1();
	static void DrawMP2();
	static void DrawMP3();
	static void DrawMP4();
	static void CheckMP1();
	static void CheckMP2();
	static void CheckMP3();
	static void CheckMP4();
	static void CheckMP5();
	static void CheckMP6();
	static void CheckMP7();
	static void DrawBP1();
	static void DrawBP2();
	static void DrawBP3();
	static void CheckBP1();
	static void CheckBP2();
	static void CheckBP3();
	static void CheckBP4();
	static void CheckBP5();
	static void CheckBP6();
	static void DrawLevelUpPoints();
	static void DrawResets();
	static void DrawExperience(char* Buffer, char* Format, DWORD Experience, DWORD NextExperience);
	static void DrawExperienceBar(float X, float Y, float Width, float Height);
	static void GetX();
	static void GetY();

public:
	CharacterList CharacterList[5];
	WORD Index;
	DWORD HP;
	DWORD MaxHP;
	DWORD MP;
	DWORD MaxMP;
	DWORD BP;
	DWORD MaxBP;
	DWORD LevelUpPoints;
	int Damage;
	DWORD X;
	DWORD Y;
	QWORD Experience;
	QWORD NextExperience;
	QWORD PreviousNextExperience;
	DWORD PercentExperience;
	DWORD Resets;
	PrismColor PrismWeapon;
	PrismColor PrismArmor;
	BYTE Sign;

	CustomItemInfo Inventory[INVENTORY_SIZE];
	CustomItemInfo TempSource[WAREHOUSE_SIZE];
	CustomItemInfo TempTarget[WAREHOUSE_SIZE];

public:
	std::function<void(PlayerObject*)> Callback;
};

extern CPlayer Player;