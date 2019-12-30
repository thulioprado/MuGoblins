#pragma once

class CSign
{
public:
	CSign();
	~CSign();

public:
	void Load();
	void LoadModels();
	void LoadTextures();

public:
	static LPBYTE Create(struct ViewportInfo* Info);
	static void Destroy(struct ViewportInfo* Info);
	static void Glow();
};

extern CSign Sign;