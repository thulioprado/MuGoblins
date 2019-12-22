#pragma once

#define TEXTURE_MAX				0x8000
#define TEXTURE_START_CUSTOM	0x488A

struct TextureImage
{
	char Name[32];
	float Width;			// 20
	float Height;			// 24
	DWORD UnkDword28;		// 28
	GLuint Texture;			// 2C
	BYTE UnkByte30;			// 30
	BYTE Unknown3[0x03];	// 31
	BYTE* UnkPByte34;		// 34
};

class CTexture
{
public:
	CTexture();
	~CTexture();

public:
	void Load();

public:
	static void LoadTextures();

public:
	static TextureImage Textures[TEXTURE_MAX];
};

extern CTexture Texture;