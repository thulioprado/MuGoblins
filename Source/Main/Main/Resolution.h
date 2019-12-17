#pragma once

#define MAX_RESOLUTION_INFO 7

struct ResolutionInfo
{
	WORD Width;
	WORD Height;
	WORD Font;
	WORD Move[4];
};

class CResolution
{
public:
	CResolution();
	~CResolution();

public:
	void Load();

public:
	static void WindowSize();
	static void FontSize();
	static void MoveListSize();
	static void MoveListClick();
	static void DrawLogo(int Index, float X, float Y, float Width, float Height, float Unk1, float Unk2, float ScaleX, float ScaleY, char Unk3, float Unk4);

private:
	ResolutionInfo Info;
};

extern CResolution Resolution;