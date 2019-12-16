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

private:
	ResolutionInfo Info;
};

extern CResolution Resolution;