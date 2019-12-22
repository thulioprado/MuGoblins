#include "Library.h"
#include "Texture.h"
#include "Item.h"

TextureImage CTexture::Textures[TEXTURE_MAX];

CTexture::CTexture()
{
	memset(&this->Textures, 0, sizeof(this->Textures));
}

CTexture::~CTexture()
{
}

void CTexture::Load()
{
	//
	// Aumento de texturas
	//
	Memory::Change<DWORD>(0x4A982D, TEXTURE_MAX);
	Memory::Change<DWORD>(0x6086EF, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x534410, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x534A39, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x534A48, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x5E000D, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x5E9780, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x5FA7E9, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x5FA844, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x5FA9BC, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x62C4E6, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x62C93B, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x62CADD, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x62CB5C, (DWORD)(&Textures));
	Memory::Change<DWORD>(0x44D909, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x44D9CA, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x44DA8A, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x460BB8, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x461570, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x53D284, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x53DF56, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x5484E4, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x5485F6, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x636CA2, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x636CC0, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x636CE9, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x636CFE, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x5E0006, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x5E9779, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x630A78, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x636D4F, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x636F6D, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x636FA0, (DWORD)(&Textures->Width));
	Memory::Change<DWORD>(0x44D923, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x44D9B2, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x44DA74, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x460BA0, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x46142D, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x46155A, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x461B06, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x53D290, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x53DF5F, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x548403, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x5484CC, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x5485E0, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x5E90D9, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x636CB1, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x636CD4, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x636CF2, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x636D0D, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x53BACE, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x636D72, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x636FE5, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x63701A, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x6598D0, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x6599B5, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x659B2F, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x659B69, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x659BA2, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x659BEC, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x659C28, (DWORD)(&Textures->Height));
	Memory::Change<DWORD>(0x44D9D7, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x451F6B, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x451FF3, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x4529AA, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x4529FE, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x460BC5, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x4E5487, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x4E54D4, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x4E606B, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x4E60B8, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x4E7ACD, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x4E7B03, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x53D2A5, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x53DD95, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x5484F1, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x60D718, (DWORD)(&Textures->UnkDword28));
	Memory::Change<DWORD>(0x44D997, (DWORD)(&Textures->Texture));
	Memory::Change<DWORD>(0x5484B4, (DWORD)(&Textures->Texture));
	Memory::Change<DWORD>(0x60C935, (DWORD)(&Textures->Texture));
	Memory::Change<DWORD>(0x60C991, (DWORD)(&Textures->Texture));
	Memory::Change<DWORD>(0x637259, (DWORD)(&Textures->Texture));
	Memory::Change<DWORD>(0x460B0D, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x4631D9, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x463209, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x60EF87, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x44D95F, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x44D9AC, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x4603BA, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x460B9A, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x46321E, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x548422, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x5484C6, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x5E90C5, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x534407, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x62CAD4, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x62CB53, (DWORD)(&Textures->UnkPByte34));
	Memory::Change<DWORD>(0x5DFDF8, (DWORD)(&Textures) + 0x790);
	Memory::Change<DWORD>(0x5E0375, (DWORD)(&Textures) + 0x790);
	Memory::Change<DWORD>(0x5DFE06, (DWORD)(&Textures) + 0x794);
	Memory::Change<DWORD>(0x5E0383, (DWORD)(&Textures) + 0x794);
	Memory::Change<DWORD>(0x5DFF72, (DWORD)(&Textures) + 0x798);
	Memory::Change<DWORD>(0x5E0432, (DWORD)(&Textures) + 0x798);
	Memory::Change<DWORD>(0x5DFF56, (DWORD)(&Textures) + 0x79C);
	Memory::Change<DWORD>(0x5E0419, (DWORD)(&Textures) + 0x79C);
	Memory::Change<DWORD>(0x5DFE16, (DWORD)(&Textures) + 0x7A4);
	Memory::Change<DWORD>(0x5DFF68, (DWORD)(&Textures) + 0x7A4);
	Memory::Change<DWORD>(0x5E0393, (DWORD)(&Textures) + 0x7A4);
	Memory::Change<DWORD>(0x5E03CB, (DWORD)(&Textures) + 0x7A4);
	Memory::Change<DWORD>(0x5E042B, (DWORD)(&Textures) + 0x7A4);
	Memory::Change<DWORD>(0x5E8C12, (DWORD)(&Textures) + 0x7A8);
	Memory::Change<DWORD>(0x59CD95, (DWORD)(&Textures) + 0x7D0);
	Memory::Change<DWORD>(0x6044AA, (DWORD)(&Textures) + 0xE38);
	Memory::Change<DWORD>(0x6044B3, (DWORD)(&Textures) + 0x1068);
	Memory::Change<DWORD>(0x6044FF, (DWORD)(&Textures) + 0x1068);
	Memory::Change<DWORD>(0x604552, (DWORD)(&Textures) + 0x1538);

	//
	// Ganchos
	//
	Memory::Call(0x60ABC0, this->LoadTextures);
}

void CTexture::LoadTextures()
{
	pLoadTextures();

	Item.LoadTextures();
}

CTexture Texture;