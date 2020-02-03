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

struct ItemInfoEx
{
	char Name[50];
	WORD Defense;
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
	void SetNameColor(DWORD Background, DWORD Foreground, const char* Name = null);
	void AddLine();
	void AddDescription(DWORD Background, DWORD Foreground, const char* Format, ...);

public:
	static void SetModelPosition();
	static void SetModelSize();
	static void SetDescriptionInfo();
	static void SetDescriptionColor1();
	static void SetDescriptionColor2();
	static void EnableGlow();
	static void SetGlow(int ItemModel, float Alpha, DWORD Unk2, FloatColor* Color, DWORD Unk3);
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
	static int FullArmorGlowEffect(int Unk1, DWORD Unk2, struct ViewportInfo* Viewport, struct FloatColor* Color, DWORD Unk4, float Unk5, DWORD Unk6);
	
public:
	std::unique_ptr<ItemInfoEx[]> ExBmd;
	WORD TransformationRings[8][16];
	const char* TransformationRingFormat;
	float ModelSize;
	bool ApplyPrismGlow;
};

extern CItem Item;