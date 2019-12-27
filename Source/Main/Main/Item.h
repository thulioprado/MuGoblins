#pragma once

#pragma pack(push, 1)
struct ItemInfo
{
	WORD Index;				    // 0
	BYTE Unknown1[0x2];		    // 2
	DWORD Code;				    // 4
	BYTE Slot;                  // 8
	BYTE Unknown2;	            // 9
	BYTE TwoHand;               // A
	BYTE Unknown3;              // B
	WORD DamageMin;             // C
	WORD DamageMax;             // E
	WORD DefenseSuccessRate;	// 10
	WORD Defense;               // 12
	WORD MagicDefense;          // 14
	BYTE MagicDamageRate;       // 16
	BYTE AttackSpeed;           // 17
	WORD WalkSpeed;             // 18
	BYTE Durability;		    // 1A
	BYTE NewOption;			    // 1B
	BYTE SetOption;			    // 1C
};
#pragma pack(pop)

class CItem
{
public:
	CItem();
	~CItem();

public:
	void Load();
	void LoadModels();
	void LoadTextures();
	bool GetModelPosition(DWORD Index);
	bool GetModelSize(DWORD Index);
	void SetDescription(ItemInfo* Item);
	void SetNameColor(DWORD Background, DWORD Foreground);
	void AddLine();
	void AddDescription(DWORD Background, DWORD Foreground, const char* Format, ...);
	float InterpolateValue(float Start, float End, float Timer);
	bool Equals(float A, float B);
	bool InterpolateColors(GlowColor* Color, float Timer, float R1, float G1, float B1, float R2, float G2, float B2);
	bool ApplyPrismEffect(GlowColor* Color, struct PrismEffect* Prism, int Index);
	void ApplyPrismValue(struct PrismEffect* Prism, BYTE Red, BYTE Green, BYTE Blue);

public:
	static void SetModelPosition();
	static void SetModelSize();
	static void SetDescriptionInfo();
	static void SetDescriptionColor1();
	static void SetDescriptionColor2();
	static void EnableGlow();
	static void SetGlow(int ItemModel, float Alpha, DWORD Unk2, GlowColor* Color, DWORD Unk3);
	static void AllowExcellentOptions();
	static void AllowInsertItem();
	static void AllowInsertItemGreenSlot();
	static void WeaponViewCheck();
	static void SecondWeaponViewCheckReplacedCode();
	static void SecondWeaponViewCheck();
	static void SecondWeaponViewFix();
	static void TransformationRings1();
	static void TransformationRings2();
	static void TransformationRings3();
	static void TransformationRings4();
	static void TransformationRings5();
	static void TransformationRings6();
	static void TransformationRings7();
	static void TransformationRings8();
	static void TransformationRings9();
	static void ConfirmToSell();
	static void RenderizingInventory();
	static void RenderizingShop();
	
public:
	WORD TransformationRings[8][16];
	const char* TransformationRingFormat;
	float ModelSize;
	bool ApplyPrismGlow;
};

extern CItem Item;