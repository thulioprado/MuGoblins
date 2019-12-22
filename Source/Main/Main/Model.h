#pragma once

#define MODEL_BASE			*(DWORD*)(0x5756AB8)
#define MODEL_SIGN			0x4064

struct ModelInfo
{
	BYTE Unknown1[0x24];	// 0
	short UnkWord24;		// 24
	short UnkWord26;		// 26
	BYTE Unknown2[0x8];		// 28
	DWORD UnkDword30;		// 30
	BYTE Unknown3[0xAC];	// 34
};

class CModel
{
public:
	CModel();
	~CModel();

public:
	void Load();
	ModelInfo* Get(int Model) const;

public:
	static void LoadModels();
};

extern CModel Model;