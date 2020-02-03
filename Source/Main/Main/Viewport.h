#pragma once

#pragma pack(push, 1)
struct Angle
{
	float X;
	float Y;
	float Z;
};

struct ViewportInfo
{
	bool Live;					// 0
	BYTE Unknown1[0xB];			// 1
	float Size;					// C
	Angle Angle1;				// 10
	Angle EffectAngle;			// 1C
	BYTE Unknown2[0x3C];		// 28
	DWORD UnkValue64;			// 64
	BYTE Unknown3[0x10];		// 68
	DWORD ViewSkillState;		// 78
	BYTE Unknown4[0x8];			// 7C
	BYTE Type;					// 84
	BYTE Unknown5[0xA7];		// 85
	Angle Angle2;				// 12C
	BYTE Unknown6[0x8C];		// 138
	BYTE Class;                 // 1C4
	BYTE Unknown23;             // 1C5
	WORD Level;     			// 1C6
	BYTE CtlCode;				// 1C8
	char Name[25];				// 1C9
	short GuildIndex;			// 1E2
	short Index;				// 1E4
	BYTE Unknown7[0x22];		// 1E6
	short Helm;					// 208
	BYTE HelmLevel;				// 20A
	BYTE HelmExcellent;			// 20B
	BYTE HelmAncient;			// 20C
	BYTE Unknown8[0x1B];		// 20D
	short Armor;				// 228
	BYTE ArmorLevel;			// 22A
	BYTE ArmorExcellent;		// 22B
	BYTE ArmorAncient;			// 22C
	BYTE Unknown9[0x1B];		// 22D
	short Pants;				// 248
	BYTE PantsLevel;			// 24A
	BYTE PantsExcellent;		// 24B
	BYTE PantsAncient;			// 24C
	BYTE Unknown10[0x1B];		// 24D
	short Gloves;				// 268
	BYTE GlovesLevel;			// 26A
	BYTE GlovesExcellent;		// 26B
	BYTE GlovesAncient;			// 26C
	BYTE Unknown11[0x1B];		// 26D
	short Boots;				// 288
	BYTE BootsLevel;			// 28A
	BYTE BootsExcellent;		// 28B
	BYTE BootsAncient;			// 28C
	BYTE Unknown12[0x1B];		// 28D
	short RightWeapon;			// 2A8
	BYTE RightWeaponLevel;		// 2AA
	BYTE RightWeaponExcellent;	// 2AB
	BYTE RightWeaponAncient;	// 2AC
	BYTE Unknown13[0x1B];		// 2AD
	short LeftWeapon;			// 2C8
	BYTE LeftWeaponLevel;		// 2CA
	BYTE LeftWeaponExcellent;	// 2CB
	BYTE LeftWeaponAncient;		// 2CC
	BYTE Unknown14[0x1B];		// 2CD
	short Wings;				// 2E8
	BYTE WingsLevel;			// 2EA
	BYTE WingsExcellent;		// 2EB
	BYTE WingsAncient;			// 2EC
	BYTE Unknown15[0x1B];		// 2ED
	short Pet;					// 308
	BYTE PetLevel;				// 30A
	BYTE PetExcellent;			// 30B
	BYTE PetAncient;			// 30C
	BYTE Unknown16[0x43];		// 30D
	BYTE SignID;				// 350
	BYTE Unknown17[0x03];		// 351
	LPBYTE Sign;				// 354
	BYTE GuildLevel;			// 358
	BYTE Unknown18_1[0x07];		// 359
	BYTE GuildRelationship;		// 360
	BYTE Unknown18_2[0x04];		// 361
	BYTE PkLevel;				// 365
	BYTE Unknown21[0x02];		// 366
	BYTE UnkValue368;			// 368
	BYTE Unknown19[0x10];		// 369
	BYTE UnkValue379;			// 379
	BYTE Unknown20[0x3];		// 37A
	bool SkillSuccess;			// 37D
	int MagicNumber;			// 37E
	BYTE Unknwon22[0x8A];		// 382
	DWORD X;                    // 40C
	DWORD Y;                    // 410
	BYTE Unknown24[0x10];       // 414
};
#pragma pack(pop)

struct ViewportCustom
{
	PrismColor PrismWeapon;
	PrismColor PrismArmor;
	BYTE Sign;
};

class CViewport
{
public:
	CViewport();
	~CViewport();

public:
	void Load();
	void Clear();
	ViewportInfo* GetMyself();
	ViewportInfo* GetByPosition(int Position);
	ViewportInfo* GetByIndex(WORD Index);
	ViewportInfo* GetByName(char* Name);
	ViewportCustom* GetCustom(WORD Index);
	void AddCustom(WORD Index, LPBYTE CharSet);

public:
	static void SetRenderizing();

public:
	ViewportInfo* Renderizing;

private:
	ViewportCustom Custom[MAX_OBJECT];
};

extern CViewport Viewport;