#pragma once

class CItem
{
public:
	CItem();
	~CItem();

public:
	void Load();
	void LoadModels();
	void LoadTextures();

public:
	static void EnableGlow();
	static void SetGlow(int ItemModel, float Alpha, DWORD Unk2, GlowColor* Colors, DWORD Unk3);
	static void AllowExcellentOptions();
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
	static void TransformationRings10();

public:
	static BYTE SecondWeaponFixVal;
	static char DescriptionText[60][100];
	static DWORD DescriptionColor[60];
	static DWORD DescriptionType[60];

public:
	WORD TransformationRings[8][16];
	const char* TransformationRingFormat;
};

extern CItem Item;