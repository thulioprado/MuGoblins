#include "Library.h"
#include "Item.h"
#include "Monster.h"
#include "Viewport.h"
#include "Player.h"
#include "Message.h"
#include "Prism.h"

BYTE SecondWeaponFixVal;
char DescriptionText[60][100];
DWORD DescriptionColor[60];
DWORD DescriptionType[60];
DWORD DescriptionCustomColor[2][60];

CItem::CItem() : ExBmd(), TransformationRings{}, TransformationRingFormat("%s [%s]"), ModelSize(0.f), ApplyPrismGlow(true)
{
	memset(this->TransformationRings, 0, sizeof(this->TransformationRings));

	SecondWeaponFixVal = 0;

	memset(DescriptionText, 0, sizeof(DescriptionText));
	memset(DescriptionColor, 0, sizeof(DescriptionColor));
	memset(DescriptionType, 0, sizeof(DescriptionType));
}

CItem::~CItem()
{
}

void CItem::Load()
{
	//
	// research pshop
	//
	/// === print background ===
	/// 0061BD7E
	/// === print ok button ===
	/// 0061BFBF
	/// === print cancel button ===
	/// 0061C053
	/// === print value ===
	/// 0061BE59
	///
	/// Parece que o main cria janelas internas (windows)
	/// e os valores dos textbox são os titulos delas

	//
	// Leitura de informações corretas de itens
	//
	this->ExBmd = std::make_unique<ItemInfoEx[]>(8192);

	if (!this->ExBmd)
	{
		MessageBox(NULL, "Insufficient memory.", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	FILE* File;
	fopen_s(&File, ".\\Data\\Local\\Item.bmdx", "rb");

	if (!File)
	{
		MessageBox(NULL, "Item.bmdx not found.", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	fread((char*)(this->ExBmd.get()), sizeof(ItemInfoEx[8192]), 1, File);
	fclose(File);
	
	for (int i = 0; i < 8192; ++i)
	{
		XorConvert((LPBYTE)(&this->ExBmd[i]), sizeof(ItemInfoEx));
	}

	//
	// Aumento de limite de descrições nos itens
	//
	Memory::Change<DWORD>(0x40C254, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40C2A4, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CD4F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CE23, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CE69, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CEAD, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CEF3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CF3B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CF81, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CFDC, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D18A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D1CD, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D334, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D350, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D36B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D4DA, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D500, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D527, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D543, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D570, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D729, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40D745, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4200F9, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x420182, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x420F2C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x42101A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4214A3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421525, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421571, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4215A1, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4215BD, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4215F6, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421621, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421668, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x42168E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4216B7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4216F0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421967, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421B18, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421CFF, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421F29, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421F55, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x421F82, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x42201A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x422045, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x42208B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x432D39, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x432D5C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x432D98, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x43310A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x43313E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4331B8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4331D4, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x433211, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x433230, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x492555, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x49258E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x4925C2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5960A2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59625F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59635E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x599689, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5996C2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5996F6, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59E661, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59E6FB, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59E8C8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59EB17, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59EE27, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59EE82, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59EEBB, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59EF11, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59EFA4, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59F04A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59F10D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59F20C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59F385, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FD77, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FDA6, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FDE7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FE0F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FE76, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FEC0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FEF3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FF34, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FF5C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x59FFC3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A01B9, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A01E3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A0228, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A0260, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A0286, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A02AC, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A0417, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A043D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A050A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A0580, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A05BC, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A21E8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A222A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A252D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2620, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A27D0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2855, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2D4C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2D72, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2DC2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2DE8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2E5D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2E81, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2EC0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2EEF, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2F46, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2F76, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2FC2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A2FE2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A300C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A302C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3069, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3089, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3157, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A31AD, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A31CD, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A31F1, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A323D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A325D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A32A1, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A32CB, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A32EB, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A332D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3365, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A33A7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A33C7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A36DB, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A39F2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3B17, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3B53, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3B8A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3BBE, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3BF0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3C2D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3C75, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3CAD, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3CDA, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3D34, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3D85, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3DD3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3E22, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3E57, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3E8C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3EC0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3EFA, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3F35, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3F9D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A3FD1, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4007, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4060, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4094, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A40CA, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4105, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A413E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4174, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A41AD, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A41E3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4217, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A424B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A429E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A42D2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A430B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A433F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A437B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A43C4, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A43F8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4431, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4465, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4493, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A44C7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4500, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4534, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A455D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A459C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A45D0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A463B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4B40, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4BF0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4C5B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4C9F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4CF5, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4D37, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4D7E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4DB2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4DF2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4E26, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4E3B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4E6F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4EAC, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4F26, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4F5A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4F9F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A4FD3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5015, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5050, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A508B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A50D0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A510B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5146, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5181, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A51BE, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A51F4, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A522F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5281, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A52B6, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A52EA, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5331, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5378, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A53AE, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A53E2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5407, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A544C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5477, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A54C9, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A54F9, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5518, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5537, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A557E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A55AE, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A55CD, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A55EC, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A561D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5670, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A56C0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A56E6, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5719, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A574D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A579D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A57D6, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A580A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A583E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5877, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A58CC, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A58F1, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5906, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5E28, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5E7F, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5F0D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5F40, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A5F8B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A60F8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A613A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6160, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6185, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A61DB, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6210, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A625C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A62C4, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A65C7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6802, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6B05, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6CD6, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6D45, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6DC8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6E34, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6EA0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6F0C, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6F78, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A6FE4, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7075, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A70A8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A70F8, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A712B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7187, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A71C0, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A720A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7243, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7283, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A730A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7350, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A736D, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7387, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A73DE, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A742E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7472, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7661, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A774B, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A77CE, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A77F7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7836, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7B29, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7DD2, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7DF7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7E19, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7E54, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7EB3, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7EED, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A7F1A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A877E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A89EC, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8A1A, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8A4E, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8A82, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8AB4, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8B04, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8B66, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8BC7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8EF9, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A8F39, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A90D6, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A9117, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A9375, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A93A7, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x5A95D1, (DWORD)(&DescriptionText));
	Memory::Change<DWORD>(0x40CD8A, (DWORD)(&DescriptionText[1]));
	Memory::Change<DWORD>(0x40D1F2, (DWORD)(&DescriptionText[1]));
	Memory::Change<DWORD>(0x420115, (DWORD)(&DescriptionText[1]));
	Memory::Change<DWORD>(0x420F53, (DWORD)(&DescriptionText[1]));
	Memory::Change<DWORD>(0x42106E, (DWORD)(&DescriptionText[1]));
	Memory::Change<DWORD>(0x4214C1, (DWORD)(&DescriptionText[1]));
	Memory::Change<DWORD>(0x5960BB, (DWORD)(&DescriptionText[1]));
	Memory::Change<DWORD>(0x5A8797, (DWORD)(&DescriptionText[1]));
	Memory::Change<DWORD>(0x40CDA8, (DWORD)(&DescriptionText[2]));
	Memory::Change<DWORD>(0x40D20E, (DWORD)(&DescriptionText[2]));
	Memory::Change<DWORD>(0x420127, (DWORD)(&DescriptionText[2]));
	Memory::Change<DWORD>(0x42143B, (DWORD)(&DescriptionText[2]));
	Memory::Change<DWORD>(0x5960D9, (DWORD)(&DescriptionText[2]));
	Memory::Change<DWORD>(0x5A87B8, (DWORD)(&DescriptionText[2]));
	Memory::Change<DWORD>(0x40D220, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x420160, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x42145D, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x59610D, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x5961CF, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x5961DB, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x5961E7, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x5961F3, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x5961FF, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x59620B, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x5A8850, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x5A892D, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x5A895D, (DWORD)(&DescriptionText[3]));
	Memory::Change<DWORD>(0x40D249, (DWORD)(&DescriptionText[4]));
	Memory::Change<DWORD>(0x596222, (DWORD)(&DescriptionText[4]));
	Memory::Change<DWORD>(0x5A8871, (DWORD)(&DescriptionText[4]));
	Memory::Change<DWORD>(0x596234, (DWORD)(&DescriptionText[5]));
	Memory::Change<DWORD>(0x5A88A4, (DWORD)(&DescriptionText[5]));
	Memory::Change<DWORD>(0x40C266, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40C2B4, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40CD7A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40CE38, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40CEC2, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40CF50, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40CF96, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40D005, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40D018, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40D183, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40D510, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40D556, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x4200DC, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x420F3F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421030, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421044, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x42105F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x4214A9, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x4214DB, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421593, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421943, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421977, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x42198A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421CE9, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421D0F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421D18, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421F47, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x421FBB, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x42202F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x422052, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x432D6C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x432DA8, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x433100, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x43311A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x433174, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x433181, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x4331EA, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x433240, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x49256F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x4925A3, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x4925DD, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59624A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5996A3, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5996D7, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x599711, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59E70B, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59EB65, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59EE9E, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59EEDA, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59EF66, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59EF76, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59F012, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59F022, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59F0A6, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59F0B6, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59F0FB, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59F223, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59F230, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59F39E, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59FDCA, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59FE43, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59FF17, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x59FF90, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A023D, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A042F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A0480, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A04E3, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A0558, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A21C2, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A263D, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A27ED, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A2807, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A2821, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A282E, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A2D34, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A2DA6, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3C15, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3C63, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3C9C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3D53, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3DA4, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3DF2, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3E37, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3E6C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3EA1, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3F14, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3F4D, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3FB2, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A3FE6, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A401C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4075, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A40A9, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A40DF, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A411F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4153, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4189, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A41C2, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A41F8, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A422C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4263, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A42B3, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A42EC, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4320, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4354, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4390, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A43D9, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4412, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4446, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A447A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A44A8, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A44E1, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4515, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4549, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4572, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A45B1, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A45E5, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4BA6, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4BBB, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4C1A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4C2C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4C70, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4CBB, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4CC8, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4D0A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4D4C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4D93, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4DC7, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4E07, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4E50, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4E84, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4EC1, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4F3B, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4F6F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4FB4, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A4FE8, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A502A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5065, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A50A0, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A50E5, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5120, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A515B, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5196, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A51D3, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5209, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5244, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5296, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A52CB, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A52FF, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A534A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A538D, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A542A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A54A0, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5555, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A560C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A564B, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A565E, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5692, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A56AE, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A572E, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5762, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A57B7, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A57EB, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A581F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5858, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A588C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A591B, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5E0C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5E45, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5EAB, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5ED4, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5F58, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A5F77, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A619A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A61F0, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6225, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6271, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6599, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A65DC, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A65E7, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6ADE, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6B1A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6B3D, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6D22, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6D5A, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6D7F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6E11, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6E49, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6E57, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6EE9, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6F21, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6F2F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6FC1, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A6FF9, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A7007, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A70C2, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A7145, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A71DA, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A725D, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A7322, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A739C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A73F3, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A780C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A7E3C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A7ECA, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8A2C, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8A60, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8A95, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8AC4, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8B14, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8B76, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8BD7, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8F14, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A8F4F, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A90F6, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A9127, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x5A9385, (DWORD)(&DescriptionColor));
	Memory::Change<DWORD>(0x40CD98, (DWORD)(&DescriptionColor[1]));
	Memory::Change<DWORD>(0x40D200, (DWORD)(&DescriptionColor[1]));
	Memory::Change<DWORD>(0x420107, (DWORD)(&DescriptionColor[1]));
	Memory::Change<DWORD>(0x5960C9, (DWORD)(&DescriptionColor[1]));
	Memory::Change<DWORD>(0x5A87A9, (DWORD)(&DescriptionColor[1]));
	Memory::Change<DWORD>(0x40CDB3, (DWORD)(&DescriptionColor[2]));
	Memory::Change<DWORD>(0x421441, (DWORD)(&DescriptionColor[2]));
	Memory::Change<DWORD>(0x59EE03, (DWORD)(&DescriptionColor[2]));
	Memory::Change<DWORD>(0x59EDFC, (DWORD)(&DescriptionColor[3]));
	Memory::Change<DWORD>(0x5A8860, (DWORD)(&DescriptionColor[3]));
	Memory::Change<DWORD>(0x5A893B, (DWORD)(&DescriptionColor[3]));
	Memory::Change<DWORD>(0x5A896C, (DWORD)(&DescriptionColor[3]));
	Memory::Change<DWORD>(0x40D246, (DWORD)(&DescriptionColor[4]));
	Memory::Change<DWORD>(0x5A8888, (DWORD)(&DescriptionColor[4]));
	Memory::Change<DWORD>(0x5A88B2, (DWORD)(&DescriptionColor[5]));
	Memory::Change<DWORD>(0x40CD80, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x40CE42, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x40CECC, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x40CF57, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x40CFA1, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x40D02B, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x40D55D, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x4200E1, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x420F49, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x421074, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x4214AF, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x4214E2, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x42194A, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x42197E, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x421991, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x421CE2, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x421D2A, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x421FC6, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x42205E, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x433121, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x43318F, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x492576, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x4925AA, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x4925E4, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x596251, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5996AA, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5996DE, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x599718, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59E666, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59E7EF, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59E8CD, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59EB1E, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59EEE0, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59EFB9, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59F073, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59F122, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59F243, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59FDD1, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59FE4F, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59FF1E, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x59FF9C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A0248, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A0436, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A045C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A2839, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A2D3B, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A2DB1, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3C0A, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3C8A, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3CC2, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3CF7, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3D5A, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3DAB, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3DF9, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3E3E, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3E73, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3EA8, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3EDB, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3F1B, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3F54, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3FB9, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A3FED, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4023, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A407C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A40B0, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A40E6, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4126, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A415A, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4190, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A41C9, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A41FF, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4233, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A426A, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A42BA, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A42F3, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4327, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A43E0, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4419, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A444D, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A44AF, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A44E8, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A451C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A457D, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A45B8, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A45EC, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4BB1, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4BC2, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4C33, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4C77, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4CCF, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4D11, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4D53, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4D9A, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4DCE, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4E0E, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4E57, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4E8F, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4ECC, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4F42, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4F76, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4FBB, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A4FEF, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5031, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A506C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A50A7, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A50EC, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5127, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5162, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A519D, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A51DA, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5210, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A524B, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A529D, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A52D2, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5306, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5351, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5394, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5735, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5769, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A57BE, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A57F2, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5826, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A585F, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5893, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5926, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5E4C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A5EDF, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A61A1, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A61F7, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A622C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6278, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A65A0, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A65EE, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6AD7, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6B44, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6D29, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6D8C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6E18, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6E64, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6EF0, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6F3C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A6FC8, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A7014, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A70C9, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A714C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A71E1, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A7264, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A732D, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A73FE, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A7813, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A7E31, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A7ED1, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8A33, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8A67, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8A9C, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8ACB, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8B1F, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8B81, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8BE2, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8F1B, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A8F5B, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A90FD, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A912E, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x5A9390, (DWORD)(&DescriptionType));
	Memory::Change<DWORD>(0x40CD9E, (DWORD)(&DescriptionType[1]));
	Memory::Change<DWORD>(0x5960CF, (DWORD)(&DescriptionType[1]));
	Memory::Change<DWORD>(0x5A87AE, (DWORD)(&DescriptionType[1]));
	Memory::Change<DWORD>(0x40CDB9, (DWORD)(&DescriptionType[2]));
	Memory::Change<DWORD>(0x421447, (DWORD)(&DescriptionType[2]));
	Memory::Change<DWORD>(0x5A8866, (DWORD)(&DescriptionType[3]));
	Memory::Change<DWORD>(0x5A8945, (DWORD)(&DescriptionType[3]));
	Memory::Change<DWORD>(0x5A8971, (DWORD)(&DescriptionType[3]));
	Memory::Change<DWORD>(0x40D23F, (DWORD)(&DescriptionType[4]));
	Memory::Change<DWORD>(0x5A888E, (DWORD)(&DescriptionType[4]));
	Memory::Change<DWORD>(0x5A88B8, (DWORD)(&DescriptionType[5]));
	//Memory::Change<DWORD>(0x4BFFCB, (DWORD)(&DescriptionType[15]));
	//Memory::Change<DWORD>(0x5AAF50, (DWORD)(&DescriptionType[15]));
	//Memory::Change<DWORD>(0x647E73, (DWORD)(&DescriptionType[15]));
	//Memory::Change<DWORD>(0x648B4B, (DWORD)(&DescriptionType[15]));
	//Memory::Change<DWORD>(0x5D3941, (DWORD)(&DescriptionType[17]));

    //
    // Ganchos
    //
	Memory::Jump(0x5CF180, this->SetModelPosition);
	Memory::Jump(0x5CF7AB, this->SetModelSize);
	Memory::Jump(0x5F99A8, this->EnableGlow);
	Memory::Call(0x5069A2, this->SetGlow);
	Memory::Call(0x5F8445, this->SetGlow);
	Memory::Jump(0x54177A, this->AllowExcellentOptions);
	Memory::Jump(0x5C6CEB, this->AllowInsertItem);
	Memory::Jump(0x5B8E66, this->AllowInsertItemGreenSlot);
	Memory::Jump(0x5A7420, this->SetDescriptionInfo);
	Memory::Jump(0x59E78D, this->SetDescriptionColor1);
	Memory::Jump(0x59EBE7, this->SetDescriptionColor2);
	Memory::Jump(0x5048CA, this->WeaponViewCheck);
	Memory::Jump(0x504A4F, this->SecondWeaponViewCheckReplacedCode);
	Memory::Hook(0x5049BC, this->SecondWeaponViewCheck);
	Memory::Hook(0x5049C5, this->SecondWeaponViewCheck);
	Memory::Jump(0x5002E8, this->SecondWeaponViewFix);
    Memory::Jump(0x5A38EF, this->TransformationRings1);
	Memory::Jump(0x5AA1AB, this->TransformationRings2);
    Memory::Jump(0x5411AB, this->TransformationRings3);
    Memory::Jump(0x59A81E, this->TransformationRings4);
    Memory::Jump(0x5A60DA, this->TransformationRings5);
    Memory::Jump(0x54181B, this->TransformationRings6);
    Memory::Jump(0x541ADE, this->TransformationRings7);
    Memory::Jump(0x5F87BB, this->TransformationRings8);
    Memory::Jump(0x5A18C7, this->TransformationRings9);
	Memory::Jump(0x5CD82C, this->ConfirmToSell);
	Memory::Jump(0x5E5CC4, this->RenderizingInventory);
	Memory::Jump(0x5E5D0B, this->RenderizingShop);

	Memory::Call(0x506DAE, this->TestEffect);
	Memory::Call(0x506DFA, this->TestEffect);
	Memory::Call(0x506E46, this->TestEffect);
}

void CItem::LoadModels()
{
	//
	// Anéis de transformação
	//
	pLoadModel(GET_ITEM_MODEL(13, 32), "Data\\Item\\", "Ring", 1);
	pLoadModel(GET_ITEM_MODEL(13, 33), "Data\\Item\\", "Ring", 1);
	pLoadModel(GET_ITEM_MODEL(13, 34), "Data\\Item\\", "Ring", 1);
	pLoadModel(GET_ITEM_MODEL(13, 35), "Data\\Item\\", "Ring", 1);
	pLoadModel(GET_ITEM_MODEL(13, 36), "Data\\Item\\", "Ring", 1);
	pLoadModel(GET_ITEM_MODEL(13, 37), "Data\\Item\\", "Ring", 1);
	pLoadModel(GET_ITEM_MODEL(13, 38), "Data\\Item\\", "Ring", 1);

	//
	// Kit de prisma
	//
	pLoadModel(GET_ITEM_MODEL(13, 39), "Data\\Item\\", "Ring", 3);
	pLoadModel(GET_ITEM_MODEL(13, 40), "Data\\Item\\", "Ring", 3);
	pLoadModel(GET_ITEM_MODEL(13, 41), "Data\\Item\\", "Necklace", 3);

	//
	// Tintas, Neutralizadores e Energias
	//
	pLoadModel(GET_ITEM_MODEL(14, 32), "Data\\Item\\", "pipe_r", -1);
	pLoadModel(GET_ITEM_MODEL(14, 33), "Data\\Item\\", "pipe_g", -1);
	pLoadModel(GET_ITEM_MODEL(14, 34), "Data\\Item\\", "pipe_b", -1);
	pLoadModel(GET_ITEM_MODEL(14, 35), "Data\\Item\\", "pipe_r", -1);
	pLoadModel(GET_ITEM_MODEL(14, 36), "Data\\Item\\", "pipe_g", -1);
	pLoadModel(GET_ITEM_MODEL(14, 37), "Data\\Item\\", "pipe_b", -1);
	pLoadModel(GET_ITEM_MODEL(14, 38), "Data\\Item\\", "pipe_g", -1);
	pLoadModel(GET_ITEM_MODEL(14, 39), "Data\\Item\\", "pipe_g", -1);
}

void CItem::LoadTextures()
{
	//
	// Anéis de transformação
	//
	pLoadTexture(GET_ITEM_MODEL(13, 32), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(13, 33), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(13, 34), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(13, 35), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(13, 36), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(13, 37), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(13, 38), "Item\\", GL_REPEAT, GL_NEAREST, 1);

	//
	// Kit de prisma
	//
	pLoadTexture(GET_ITEM_MODEL(13, 39), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(13, 40), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(13, 41), "Item\\", GL_REPEAT, GL_NEAREST, 1);

	//
	// Tintas, Neutralizadores e Energias
	//
	pLoadTexture(GET_ITEM_MODEL(14, 32), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 33), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 34), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 35), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 36), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 37), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 38), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 39), "Item\\", GL_REPEAT, GL_NEAREST, 1);
}

bool CItem::GetModelPosition(DWORD Index)
{
	//
	// Anéis de transformação
	//
	if (Index == GET_ITEM_MODEL(13, 10) || (Index >= GET_ITEM_MODEL(13, 32) && Index <= GET_ITEM_MODEL(13, 38)))
	{
		pModelPositionX = 270.f;
		pModelPositionY = -10.f;
		pModelPositionZ = 0.f;

		return true;
	}

	//
	// Anel de Prisma
	//
	if (Index == GET_ITEM_MODEL(13, 39) || Index == GET_ITEM_MODEL(13, 40))
	{
		pModelPositionX = 270.f;
		pModelPositionY = -10.f;
		pModelPositionZ = 0.f;

		return true;
	}

	//
	// Colar de Prisma
	//
	if (Index == GET_ITEM_MODEL(13, 41))
	{
		pModelPositionX = 270.f;
		pModelPositionY = 0.f;
		pModelPositionZ = 0.f;

		return true;
	}

	return false;
}

bool CItem::GetModelSize(DWORD Index)
{
	if ((Index == GET_ITEM_MODEL(13, 10) || (Index >= GET_ITEM_MODEL(13, 32) && Index <= GET_ITEM_MODEL(13, 38))) ||		// Aneis de transformação
		(Index == GET_ITEM_MODEL(13, 39) || Index == GET_ITEM_MODEL(13, 40)))												// Aneis de prisma
	{
		this->ModelSize = 0.0025f;

		return true;
	}

	if (Index == GET_ITEM_MODEL(13, 41))	// Colar de prisma
	{
		this->ModelSize = 0.0013f;

		return true;
	}

	return false;
}

void CItem::SetDescription(ItemInfo* Item)
{
	CustomItemInfo* Custom = null;

	switch (pCursorInterface)
	{
		case CursorInterface::Inventory:
		{
			if (pCursorSlot >= INVENTORY_SIZE)
			{
				return;
			}

			Custom = &Player.Inventory[pCursorSlot];
			break;
		}
		case CursorInterface::Warehouse:
		case CursorInterface::TradeSource:
		case CursorInterface::ChaosMachine:
		case CursorInterface::Shop:
		{
			if (pCursorSlot >= WAREHOUSE_SIZE)
			{
				return;
			}

			Custom = &Player.TempSource[pCursorSlot];
			break;
		}
		case CursorInterface::TradeTarget:
		case CursorInterface::PersonalShop:
		{
			if (pCursorSlot >= WAREHOUSE_SIZE)
			{
				return;
			}

			Custom = &Player.TempTarget[pCursorSlot];
			break;
		}
	}

	if (Custom)
	{
		if ((Item->Index <= GET_ITEM(12, 6) && Item->Index != GET_ITEM(4, 7) && Item->Index != GET_ITEM(4, 15)) ||
			(Item->Index >= GET_ITEM(13, 8) && Item->Index <= GET_ITEM(13, 9)) ||
			(Item->Index >= GET_ITEM(13, 11) && Item->Index <= GET_ITEM(13, 12)) ||
			(Item->Index >= GET_ITEM(13, 21) && Item->Index <= GET_ITEM(13, 28)) ||
			Item->Index == GET_ITEM(13, 30) ||
			(Item->Index >= GET_ITEM(13, 39) && Item->Index <= GET_ITEM(13, 41)))
		{
			char Name[100];
			BYTE Level = (Item->Code & 0x78) >> 3;

			switch (Item->Index)
			{
				case GET_ITEM(13, 39):
				{
					if (Level > 0)
					{
						if (Item->NewOption)
						{
							wsprintf(Name, "%s %s +%d [%s]", (char*)(0x76CF434), this->ExBmd[Item->Index].Name, Level, Message.Get(9));
							this->SetNameColor(0, 0, Name);
						}
						else
						{
							this->SetNameColor(0, 0, this->ExBmd[Item->Index].Name);
						}
					}
					else
					{
						if (Item->NewOption)
						{
							wsprintf(Name, "%s %s [%s]", (char*)(0x76CF434), this->ExBmd[Item->Index].Name, Message.Get(9));
							this->SetNameColor(0, 0, Name);
						}
						else
						{
							this->SetNameColor(0, 0, ExBmd[Item->Index].Name);
						}
					}

					break;
				}
				case GET_ITEM(13, 40):
				{
					if (Level > 0)
					{
						if (Item->NewOption)
						{
							wsprintf(Name, "%s %s +%d [%s]", (char*)(0x76CF434), this->ExBmd[Item->Index].Name, Level, Message.Get(10));
							this->SetNameColor(0, 0, Name);
						}
						else
						{
							this->SetNameColor(0, 0, this->ExBmd[Item->Index].Name);
						}
					}
					else
					{
						if (Item->NewOption)
						{
							wsprintf(Name, "%s %s [%s]", (char*)(0x76CF434), this->ExBmd[Item->Index].Name, Message.Get(10));
							this->SetNameColor(0, 0, Name);
						}
						else
						{
							this->SetNameColor(0, 0, ExBmd[Item->Index].Name);
						}
					}

					break;
				}
				default:
				{
					if (Level > 0)
					{
						if (Item->NewOption)
						{
							wsprintf(Name, "%s %s +%d", (char*)(0x76CF434), this->ExBmd[Item->Index].Name, Level);
							this->SetNameColor(0, 0, Name);
						}
						else
						{
							this->SetNameColor(0, 0, this->ExBmd[Item->Index].Name);
						}
					}
					else
					{
						if (Item->NewOption)
						{
							wsprintf(Name, "%s %s", (char*)(0x76CF434), ExBmd[Item->Index].Name);
							this->SetNameColor(0, 0, Name);
						}
						else
						{
							this->SetNameColor(0, 0, ExBmd[Item->Index].Name);
						}
					}

					break;
				}
			}
		}

		switch (Item->Index)
		{
			case GET_ITEM(13, 39):	// Anel de Prisma [Armadura]
			case GET_ITEM(13, 40):	// Anel de Prisma [Arma]
			{
				if ((Custom->Prism.Color[0].Red != 0 || Custom->Prism.Color[0].Green != 0 || Custom->Prism.Color[0].Blue != 0) &&
					(Custom->Prism.Color[1].Red != 0 || Custom->Prism.Color[1].Green != 0 || Custom->Prism.Color[1].Blue != 0))
				{
					this->AddLine();
					this->AddDescription(RGBA(0, 0, 0, 0), RGBA(255, 255, 255, 255), Message.Get(4));
					this->AddDescription(RGBA(Custom->Prism.Color[0].Red, Custom->Prism.Color[0].Green, Custom->Prism.Color[0].Blue, 255), RGBA(255, 255, 255, 255), "R[+%02d]   G[+%02d]   B[+%02d]", (Custom->Prism.Color[0].Red / 0x11), (Custom->Prism.Color[0].Green / 0x11), (Custom->Prism.Color[0].Blue / 0x11));
					this->AddDescription(RGBA(Custom->Prism.Color[1].Red, Custom->Prism.Color[1].Green, Custom->Prism.Color[1].Blue, 255), RGBA(255, 255, 255, 255), "R[+%02d]   G[+%02d]   B[+%02d]", (Custom->Prism.Color[1].Red / 0x11), (Custom->Prism.Color[1].Green / 0x11), (Custom->Prism.Color[1].Blue / 0x11));
				}
				else if (Custom->Prism.Color[0].Red != 0 || Custom->Prism.Color[0].Green != 0 || Custom->Prism.Color[0].Blue != 0)
				{
					this->AddLine();
					this->AddDescription(RGBA(0, 0, 0, 0), RGBA(255, 255, 255, 255), Message.Get(3));
					this->AddDescription(RGBA(Custom->Prism.Color[0].Red, Custom->Prism.Color[0].Green, Custom->Prism.Color[0].Blue, 255), RGBA(255, 255, 255, 255), "R[+%02d]   G[+%02d]   B[+%02d]", (Custom->Prism.Color[0].Red / 0x11), (Custom->Prism.Color[0].Green / 0x11), (Custom->Prism.Color[0].Blue / 0x11));
				}
				else if (Custom->Prism.Color[1].Red != 0 || Custom->Prism.Color[1].Green != 0 || Custom->Prism.Color[1].Blue != 0)
				{
					this->AddLine();
					this->AddDescription(RGBA(0, 0, 0, 0), RGBA(255, 255, 255, 255), Message.Get(3));
					this->AddDescription(RGBA(Custom->Prism.Color[1].Red, Custom->Prism.Color[1].Green, Custom->Prism.Color[1].Blue, 255), RGBA(255, 255, 255, 255), "R[+%02d]   G[+%02d]   B[+%02d]", (Custom->Prism.Color[1].Red / 0x11), (Custom->Prism.Color[1].Green / 0x11), (Custom->Prism.Color[1].Blue / 0x11));
				}

				break;
			}
			case GET_ITEM(13, 41): // Colar de Prisma
			{
				if (Custom->Prism.Speed[0] != 0 || Custom->Prism.Speed[1] != 0)
				{
					this->AddLine();
				}

				if (Custom->Prism.Speed[0] != 0)
				{
					this->AddDescription(RGBA(0, 0, 0, 0), RGBA(230, 230, 0, 255), Message.Get((Custom->Prism.Speed[0] > 0) ? 11 : 12), Custom->Prism.Speed[0]);
				}

				if (Custom->Prism.Speed[1] != 0)
				{
					this->AddDescription(RGBA(0, 0, 0, 0), RGBA(0, 235, 235, 255), Message.Get((Custom->Prism.Speed[1] > 0) ? 13 : 14), Custom->Prism.Speed[1]);
				}

				break;
			}
			case GET_ITEM(14, 32): // Tinta Vermelha
			case GET_ITEM(14, 33): // Tinta Verde
			case GET_ITEM(14, 34): // Tinta Azul
			{
				this->AddDescription(RGBA(0, 0, 0, 0), RGBA(255, 255, 255, 255), Message.Get(5));
				break;
			}
			case GET_ITEM(14, 35): // Neutralizador Vermelho
			case GET_ITEM(14, 36): // Neutralizador Verde
			case GET_ITEM(14, 37): // Neutralizador Azul
			{
				this->AddDescription(RGBA(0, 0, 0, 0), RGBA(255, 255, 255, 255), Message.Get(6));
				break;
			}
			case GET_ITEM(14, 38): // Energia Positiva
			{
				this->AddDescription(RGBA(0, 0, 0, 0), RGBA(255, 255, 255, 255), Message.Get(7));
				break;
			}
			case GET_ITEM(14, 39): // Energia Negativa
			{
				this->AddDescription(RGBA(0, 0, 0, 0), RGBA(255, 255, 255, 255), Message.Get(8));
				break;
			}
		}
	}
}

void CItem::SetNameColor(DWORD Background, DWORD Foreground, const char* Name)
{
	int Line = 1;

	switch (pCursorInterface)
	{
		case CursorInterface::Inventory:
		{
			if (pCursorSlot >= 76)
			{
				Line = 3;
			}

			if (pCheckInterface(0x10000))
			{
				Line = 3;
			}

			break;
		}
		case CursorInterface::Shop:
		case CursorInterface::PersonalShop:
		{
			Line = 3;
			break;
		}
	}

	if (Name)
	{
		strcpy_s(DescriptionText[Line], Name);
	}
	  	 
	if (Background != 0 || Foreground != 0)
	{
		DescriptionColor[Line] = 100;
		DescriptionType[Line] = 2;
		DescriptionCustomColor[0][Line] = Background;
		DescriptionCustomColor[1][Line] = Foreground;
	}
}

void CItem::AddLine()
{
	auto Current = pCurrentMessage;

	DescriptionColor[Current] = 100;
	DescriptionType[Current] = 0;
	DescriptionCustomColor[0][Current] = 0;
	DescriptionCustomColor[1][Current] = 0;

	strcpy_s(DescriptionText[Current], "\n");

	++pCurrentMessage;
}

void CItem::AddDescription(DWORD Background, DWORD Foreground, const char* Format, ...)
{
	auto Current = pCurrentMessage;

	DescriptionColor[Current] = 100;
	DescriptionType[Current] = 0;
	DescriptionCustomColor[0][Current] = Background;
	DescriptionCustomColor[1][Current] = Foreground;

	va_list Arguments;
	va_start(Arguments, Format);
	vsprintf_s(DescriptionText[Current], Format, Arguments);
	va_end(Arguments);

	++pCurrentMessage;
}

void __declspec(naked) CItem::SetModelPosition()
{
	static DWORD Back[2] = {0x5CF186, 0x5CF70E};
	static DWORD Index;

	__asm
	{
		MOV Index, ESI;
		PUSHAD;
	}

	if (Item.GetModelPosition(Index))
	{
		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}	
	
	__asm
	{
		POPAD;
		CMP ESI, 0xA0A;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::SetModelSize()
{
	static DWORD Back[2] = {0x5CF7B8, 0x5CFC58};
	static DWORD Index;

	__asm
	{
		MOV BYTE PTR DS : [ECX + 0xA0] , 0x00;
		MOV Index, ESI;
		PUSHAD;
	}

	if (Item.GetModelSize(Index))
	{
		__asm
		{
			POPAD;
			MOV EAX, Item.ModelSize;
			MOV DWORD PTR SS : [EBP + 0x20] , EAX;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		CMP ESI, 0x1003;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::SetDescriptionInfo()
{
	static DWORD Back = 0x5A7428;
	static ItemInfo* Info;

	__asm
	{
		PUSHAD;
		MOV Info, EDI;
	}

	Item.SetDescription(Info);

	__asm
	{
		POPAD;
		MOV EAX, DWORD PTR DS : [0x788C850] ;
		LEA EAX, DWORD PTR DS : [EAX + EAX * 0x4] ;
		PUSH 0x6B8B64;
		JMP Back;
	}
}

void __declspec(naked) CItem::SetDescriptionColor1()
{
	static DWORD Back[2] = {0x59E794, 0x59E7EC};
	static DWORD Index, Current;
	static BYTE R, G, B;

	__asm
	{
		MOV EAX, DWORD PTR DS : [EDI * 0x4 + DescriptionColor] ;
		PUSHAD;
		MOV Index, EAX;
		MOV Current, EDI;
	}

	if (Index == 100)
	{
		R = (BYTE)(DescriptionCustomColor[1][Current] & 0xFF);
		G = (BYTE)((DescriptionCustomColor[1][Current] & 0xFF00) >> 8);
		B = (BYTE)((DescriptionCustomColor[1][Current] & 0xFF0000) >> 16);

		glColor3f(R / 255.f, G / 255.f, B / 255.f);
		
		pMessageBackgroundColor = DescriptionCustomColor[0][Current];
		pMessageForegroundColor = DescriptionCustomColor[1][Current];

		DescriptionCustomColor[0][Current] = 0;
		DescriptionCustomColor[1][Current] = 0;

		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		MOV EAX, DWORD PTR DS : [EDI * 0x4 + DescriptionColor] ;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::SetDescriptionColor2()
{
	static DWORD Back[2] = {0x59EBEE, 0x59EC46};
	static DWORD Index, Current;
	static BYTE R, G, B;

	__asm
	{
		MOV EAX, DWORD PTR DS : [EBX * 0x4 + DescriptionColor] ;
		PUSHAD;
		MOV Index, EAX;
		MOV Current, EBX;
	}

	if (Index == 100)
	{
		R = (BYTE)(DescriptionCustomColor[1][Current] & 0xFF);
		G = (BYTE)((DescriptionCustomColor[1][Current] & 0xFF00) >> 8);
		B = (BYTE)((DescriptionCustomColor[1][Current] & 0xFF0000) >> 16);

		glColor3f(R / 255.f, G / 255.f, B / 255.f);

		pMessageBackgroundColor = DescriptionCustomColor[0][Current];
		pMessageForegroundColor = DescriptionCustomColor[1][Current];

		DescriptionCustomColor[0][Current] = 0;
		DescriptionCustomColor[1][Current] = 0;

		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::EnableGlow()
{
	static DWORD Back[4] = {0x5F9D77, 0x5F9D77, 0x5F99B1, 0x5F9C65};
	static WORD Index;
	static DWORD Level;
	static BYTE Excellent;

	__asm
	{
		MOV Level, EBX;
		PUSHAD;
		XOR EAX, EAX;
		MOV AX, WORD PTR DS : [ESI + 0x2] ;
		MOV Index, AX;
		MOV AL, BYTE PTR SS : [EBP + 0x1C] ;
		MOV Excellent, AL;
	}

	if ((Index == GET_ITEM_MODEL(13, 10) || (Index >= GET_ITEM_MODEL(13, 32) && Index <= GET_ITEM_MODEL(13, 38))) ||	// Aneis de transformação
		(Index >= GET_ITEM_MODEL(14, 32) && Index <= GET_ITEM_MODEL(14, 34)) ||											// Tintas
		(Index == GET_ITEM_MODEL(14, 38)))																				// Energia Positiva
	{
		__asm
		{
			POPAD;
			JMP Back[12];
		}
	}

	if ((Index >= GET_ITEM_MODEL(14, 35) && Index <= GET_ITEM_MODEL(14, 37)) ||		// Neutralizadores
		(Index == GET_ITEM_MODEL(14, 39)))											// Energia Negativa
	{
		__asm
		{
			POPAD;
			JMP Back[8];
		}
	}

	__asm
	{
		POPAD;
		CMP EBX, 0x3;
		JL Less;
		JMP Back[8];

	Less:
		JMP Back[4];
	}
}

void CItem::SetGlow(int ItemModel, float Alpha, DWORD Unk2, FloatColor* Color, DWORD Unk3)
{
	switch (ItemModel)
	{
		case GET_ITEM_MODEL(13, 39):	// Anel de Prisma
		case GET_ITEM_MODEL(13, 40):	// Anel de Prisma
		case GET_ITEM_MODEL(13, 41):	// Colar de Prisma
		{
			Color->Red = Prism.InterpolateValue(0.6f, 1.f, ((1.0f + sin(TickCount / 500.f)) * 0.5f));
			Color->Green = 0.f;
			Color->Blue = Prism.InterpolateValue(1.0f, 0.f, ((1.0f + sin(TickCount / 500.f)) * 0.5f));
			return;
		}
		case GET_ITEM_MODEL(14, 32):	// Tinta Vermelha
		case GET_ITEM_MODEL(14, 35):	// Neutralizador Vermelho
		{
			Color->Red = 0.2f;
			Color->Green = 0.0f;
			Color->Blue = 0.0f;
			return;
		}
		case GET_ITEM_MODEL(14, 33):	// Tinta Verde
		case GET_ITEM_MODEL(14, 36):	// Neutralizador Verde
		{
			Color->Red = 0.0f;
			Color->Green = 0.2f;
			Color->Blue = 0.0f;
			return;
		}
		case GET_ITEM_MODEL(14, 34):	// Tinta Azul
		case GET_ITEM_MODEL(14, 37):	// Neutralizador Azul
		{
			Color->Red = 0.0f;
			Color->Green = 0.0f;
			Color->Blue = 0.2f;
			return;
		}
		case GET_ITEM_MODEL(14, 38):	// Energia Positiva
		case GET_ITEM_MODEL(14, 39):	// Energia Negativa
		{
			Color->Red = 1.0f;
			Color->Green = 1.0f;
			Color->Blue = 0.0f;
			return;
		}
	}

	if (Viewport.Renderizing && Viewport.Renderizing->Type == 1 && Item.ApplyPrismGlow)
	{		
		if (Viewport.Renderizing->Index == Player.Index)
		{
			if (Viewport.Renderizing->Armor == ItemModel || Viewport.Renderizing->Helm == ItemModel || Viewport.Renderizing->Gloves == ItemModel || Viewport.Renderizing->Pants == ItemModel || Viewport.Renderizing->Boots == ItemModel)
			{
				if (Prism.GetEffect(Color, Player.PrismArmor.Color, Player.Index, Player.PrismArmor.Speed))
				{
					return;
				}
			}
			else if (Viewport.Renderizing->LeftWeapon == ItemModel || Viewport.Renderizing->RightWeapon == ItemModel)
			{
				if (Prism.GetEffect(Color, Player.PrismWeapon.Color, Player.Index, Player.PrismWeapon.Speed))
				{
					return;
				}
			}
		}
		else
		{
			if (pState == STATE_SELECT_CHARACTER && (Viewport.Renderizing->Index >= 0 && Viewport.Renderizing->Index <= 5))
			{
				if (Viewport.Renderizing->Armor == ItemModel || Viewport.Renderizing->Helm == ItemModel || Viewport.Renderizing->Gloves == ItemModel || Viewport.Renderizing->Pants == ItemModel || Viewport.Renderizing->Boots == ItemModel)
				{
					if (Prism.GetEffect(Color, Player.CharacterList[Viewport.Renderizing->Index].PrismArmor.Color, Viewport.Renderizing->Index, Player.CharacterList[Viewport.Renderizing->Index].PrismArmor.Speed))
					{
						return;
					}
				}
				else if (Viewport.Renderizing->LeftWeapon == ItemModel || Viewport.Renderizing->RightWeapon == ItemModel)
				{
					if (Prism.GetEffect(Color, Player.CharacterList[Viewport.Renderizing->Index].PrismWeapon.Color, Viewport.Renderizing->Index, Player.CharacterList[Viewport.Renderizing->Index].PrismWeapon.Speed))
					{
						return;
					}
				}
			}
			else
			{
				auto Target = Viewport.GetCustom(Viewport.Renderizing->Index);

				if (Target)
				{
					if (Viewport.Renderizing->Armor == ItemModel || Viewport.Renderizing->Helm == ItemModel || Viewport.Renderizing->Gloves == ItemModel || Viewport.Renderizing->Pants == ItemModel || Viewport.Renderizing->Boots == ItemModel)
					{
						if (Prism.GetEffect(Color, Target->PrismArmor.Color, Viewport.Renderizing->Index, Target->PrismArmor.Speed))
						{
							return;
						}
					}
					else if (Viewport.Renderizing->LeftWeapon == ItemModel || Viewport.Renderizing->RightWeapon == ItemModel)
					{
						if (Prism.GetEffect(Color, Target->PrismWeapon.Color, Viewport.Renderizing->Index, Target->PrismWeapon.Speed))
						{
							return;
						}
					}
				}
			}
		}
	}

	pSetGlow(ItemModel, Alpha, Unk2, Color, Unk3);
}

void __declspec(naked) CItem::AllowExcellentOptions()
{
	static DWORD Back[2] = {0x54180B , 0x541784};
	static WORD Index;

	__asm
	{
		MOV Index, AX;
		PUSHAD;
	}

	if (Index >= GET_ITEM(13, 39) && Index <= GET_ITEM(13, 41)) // Kit de prisma
	{
		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		CMP AX, 0x1A18;
		JG Greater;
		JMP Back[4];

	Greater:
		JMP Back[0];
	}
}

void __declspec(naked) CItem::AllowInsertItem()
{
	static DWORD Back[2] = {0x5C6CF0, 0x5C6D65};
	static WORD Index;

	__asm
	{
		MOV Index, DX;
		PUSHAD;
	}

	if (Index >= GET_ITEM(14, 32) && Index <= GET_ITEM(14, 41))	// Tintas, Neutralizadores e Energias
	{
		__asm
		{
			POPAD;
			IMUL EBX, EDI;
			MOV EAX, DWORD PTR SS : [EBP - 0x34] ;
			MOV ECX, DWORD PTR SS : [EBP + 0x10] ;
			ADD EBX, EAX;
			LEA EAX, DWORD PTR DS : [EBX + EBX * 0x8] ;
			MOV BL, 0x1;
			MOV BYTE PTR SS : [EBP - 0x2D] , BL;
			MOVSX ESI, WORD PTR DS : [ECX + EAX * 0x8] ;
			LEA ECX, DWORD PTR DS : [ECX + EAX * 0x8] ;
			MOV EAX, DWORD PTR DS : [ECX + 0x4] ;
			SAR EAX, 0x3;
			AND EAX, 0xF;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		CMP DX, 0x1C0D;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::AllowInsertItemGreenSlot()
{
	static DWORD Back[2] = {0x5B8E6B, 0x5B8E92};
	static WORD Index;

	__asm
	{
		MOV Index, SI;
		PUSHAD;
	}

	if (Index >= GET_ITEM(14, 32) && Index <= GET_ITEM(14, 41))	// Tintas, Neutralizadores e Energias
	{
		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		CMP SI, 0x1C0D;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::WeaponViewCheck()
{
	static DWORD Back = 0x504962;

	__asm
	{
		MOV BYTE PTR SS : [EBP + 0x13] , 0;
		CMP DWORD PTR SS : [EBP - 0x1C] , 0;
		JE RightHand;
		MOV BYTE PTR SS : [EBP + 0x13] , 1;

	RightHand:
		CMP EAX, 0;
		JNZ LeftHand;
		CMP WORD PTR DS : [ESI + 0x2A8] , GET_ITEM_MODEL(7, 0);
		JGE NoDisplay;
		CMP WORD PTR DS : [ESI + 0x2A8] , GET_ITEM_MODEL(4, 15);
		JE Display;
		JMP End;

	LeftHand:
		CMP WORD PTR DS : [ESI + 0x2C8] , GET_ITEM_MODEL(7, 0);
		JGE NoDisplay;
		CMP WORD PTR DS : [ESI + 0x2C8] , GET_ITEM_MODEL(4, 7);
		JE Display;
		JMP End;

	Display:
		MOV BYTE PTR SS : [EBP + 0x13] , 1;
		JMP End;

	NoDisplay:
		MOV BYTE PTR SS : [EBP + 0x13] , 0;

	End:
		JMP Back;
	}
}

void __declspec(naked) CItem::SecondWeaponViewCheck()
{
	static DWORD Back = 0x504A54;

	__asm
	{
		CMP SecondWeaponFixVal, 0;
		JNZ End;
		MOV EAX, 1;
		MOV SecondWeaponFixVal, 1;
		JMP WeaponViewCheck;

	End:
		MOV SecondWeaponFixVal, 0;
		MOV EAX, DWORD PTR DS : [0x6B8D48] ;
		JMP Back;
	}
}

void __declspec(naked) CItem::SecondWeaponViewCheckReplacedCode()
{
	__asm
	{
		JMP SecondWeaponViewCheck;
	}
}

void __declspec(naked) CItem::SecondWeaponViewFix()
{
	static DWORD Back = 0x5002ED;
	static DWORD Function[2] = {0x5EAE50, 0x5EAFF0};
	static BYTE IsShield = 0;

	__asm
	{
		PUSHAD;
		MOV IsShield, 0;
		CMP EDI, GET_ITEM_MODEL(6, 0);
		JL FlipCheck;
		CMP EDI, GET_ITEM_MODEL(7, 0);
		JGE FlipCheck;
		MOV IsShield, 1;
		CMP EDI, GET_ITEM_MODEL(6, 16);
		JNZ CheckShield1;
		MOV DWORD PTR SS : [EBP - 0x30] , 0x41F00000;
		MOV DWORD PTR SS : [EBP - 0x2C] , 0;
		MOV DWORD PTR SS : [EBP - 0x28] , 0x42B40000;
		LEA EDX, DWORD PTR SS : [EBP - 0x64] ;
		LEA EAX, DWORD PTR SS : [EBP - 0x30] ;
		PUSH EDX;
		PUSH EAX;
		CALL Function[0];
		ADD ESP, 0x8;
		MOV DWORD PTR SS : [EBP - 0x58] , 0xC1A00000;
		MOV DWORD PTR SS : [EBP - 0x48] , 0;
		MOV DWORD PTR SS : [EBP - 0x38] , 0xC1A00000;
		JMP FlipCheck;

	CheckShield1:
		CMP EDI, GET_ITEM_MODEL(6, 15);
		JE IsShield1;
		CMP EDI, GET_ITEM_MODEL(6, 14);
		JNZ CheckShield2;

	IsShield1:
		MOV DWORD PTR SS : [EBP - 0x30] , 0x42480000;
		MOV DWORD PTR SS : [EBP - 0x2C] , 0;
		MOV DWORD PTR SS : [EBP - 0x28] , 0x42B40000;
		LEA EDX, DWORD PTR SS : [EBP - 0x64] ;
		LEA EAX, DWORD PTR SS : [EBP - 0x30] ;
		PUSH EDX;
		PUSH EAX;
		CALL Function[0];
		ADD ESP, 8;
		MOV DWORD PTR SS : [EBP - 0x58] , 0xC1E00000;
		MOV DWORD PTR SS : [EBP - 0x48] , 0;
		MOV DWORD PTR SS : [EBP - 0x38] , 0xC1C80000;
		JMP FlipCheck;

	CheckShield2:
		CMP EDI, GET_ITEM_MODEL(6, 6);
		JNZ IsShield2;
		MOV DWORD PTR SS : [EBP - 0x30] , 0x41F00000;
		MOV DWORD PTR SS : [EBP - 0x2C] , 0;
		MOV DWORD PTR SS : [EBP - 0x28] , 0x42B40000;
		LEA EDX, DWORD PTR SS : [EBP - 0x64] ;
		LEA EAX, DWORD PTR SS : [EBP - 0x30] ;
		PUSH EDX;
		PUSH EAX;
		CALL Function[0];
		ADD ESP, 8;
		MOV DWORD PTR SS : [EBP - 0x58] , 0xC1700000;
		MOV DWORD PTR SS : [EBP - 0x48] , 0;
		MOV DWORD PTR SS : [EBP - 0x38] , 0xC1C80000;
		JMP FlipCheck;

	IsShield2:
		MOV DWORD PTR SS : [EBP - 0x58] , 0xC1200000;
		MOV DWORD PTR SS : [EBP - 0x48] , 0;
		MOV DWORD PTR SS : [EBP - 0x38] , 0;

	FlipCheck:
		CMP IsShield, 1;
		JE End;
		CMP SecondWeaponFixVal, 0;
		JE End;

		SUB ESP, 0x3C;
		MOV DWORD PTR SS : [ESP + 0x30] , 0x43110000;
		MOV DWORD PTR SS : [ESP + 0x34] , 0;
		MOV DWORD PTR SS : [ESP + 0x38] , 0x43898000;
		LEA EAX, DWORD PTR SS : [ESP] ;
		LEA ECX, DWORD PTR SS : [ESP + 0x30] ;
		PUSH EAX;
		PUSH ECX;
		CALL Function[0];
		ADD ESP, 8;

		MOV DWORD PTR SS : [ESP + 0x0C] , 0;
		MOV DWORD PTR SS : [ESP + 0x1C] , 0x41200000;
		MOV DWORD PTR SS : [ESP + 0x2C] , 0xC1F00000;
		LEA EDX, DWORD PTR SS : [EBP - 0x64] ;
		LEA EAX, DWORD PTR SS : [ESP] ;
		LEA ECX, DWORD PTR SS : [EBP - 0x64] ;
		PUSH EDX;
		PUSH EAX;
		PUSH ECX;
		CALL Function[4];
		ADD ESP, 0x48;

	End:
		POPAD;
		PUSH 0x68B19FC;
		JMP Back;
	}
}

void __declspec(naked) CItem::TransformationRings1()
{
	static DWORD Back[2] = {0x5A393E, 0x5A3C19};
	static DWORD Function = 0x68EFC7;
	static WORD Index;
	static int Level;
	static int Mob;
	static char* Name;

	__asm
	{
		MOV Index, CX;
		PUSHAD;
	}

	if (Index == GET_ITEM(13, 10) || (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38)))
	{
		__asm
		{
			POPAD;
			MOV EDI, DWORD PTR SS : [EBP - 0x10] ;
			XOR EDX, EDX;
			MOV ECX, 0x7666A80;
			MOV Level, EDI;
			PUSHAD;
		}

		if (Index == GET_ITEM(13, 10))
		{
			Mob = Item.TransformationRings[0][Level];
		}
		else if (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))
		{
			Index -= GET_ITEM(13, 32);
			Mob = Item.TransformationRings[Index + 1][Level];
		}

		Name = Monster.GetName(Mob);

		__asm
		{
			POPAD;
			MOV ESI, Mob;
			MOV ECX, Name;
			LEA EAX, DWORD PTR DS : [EAX + EAX * 0x4] ;
			LEA EDX, DWORD PTR DS : [EAX + EAX * 0x4] ;
			LEA EAX, DWORD PTR DS : [EDX * 0x4 + DescriptionText] ;
			PUSH ECX;
			PUSH 0x76A9658;
			PUSH Item.TransformationRingFormat;
			PUSH EAX;
			CALL Function;
			ADD ESP, 0x10;
			MOV EAX, DWORD PTR DS : [0x788C850] ;
			XOR EBX, EBX;
			MOV DWORD PTR DS : [EAX * 0x4 + DescriptionType] , 0x1;
			MOV DWORD PTR DS : [EAX * 0x4 + DescriptionColor] , 0x3;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::TransformationRings2()
{
	static DWORD Back[2] = {0x5AA201, 0x5AA1F3};
	static WORD Index;
	static int Level;
	static int Mob;
	static char* Name;

	__asm
	{
		MOV Index, AX;
		PUSHAD;
	}

	if (Index == GET_ITEM_MODEL(13, 10) || (Index >= GET_ITEM_MODEL(13, 32) && Index <= GET_ITEM_MODEL(13, 38)))
	{
		__asm
		{
			POPAD;
			MOV Level, EBX;
			XOR ECX, ECX;
			MOV EAX, 0x7666A80;
			PUSHAD;
		}

		if (Index == GET_ITEM_MODEL(13, 10))
		{
			Mob = Item.TransformationRings[0][Level];
		}
		else if (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))
		{
			Index -= GET_ITEM(13, 32);
			Mob = Item.TransformationRings[Index + 1][Level];
		}

		Name = Monster.GetName(Mob);

		__asm
		{
			POPAD;
			LEA EDX, DWORD PTR SS : [EBP - 0x44] ;
			MOV EBX, Mob;
			MOV ECX, Name;
			PUSH ECX;
			PUSH 0x76A9658;
			PUSH Item.TransformationRingFormat;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::TransformationRings3()
{
	static DWORD Back[2] = {0x5411B0, 0x5411B2};
	static WORD Index;

	__asm
	{
		MOV Index, CX;
		PUSHAD;
	}

	if (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))
	{
		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		CMP CX, 0x1A0A;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::TransformationRings4()
{
	static DWORD Back[2] = {0x59A826, 0x59A828};
	static WORD Index;

	__asm
	{
		MOV DWORD PTR SS : [EBP + 0x8] , ECX;
		MOV CX, WORD PTR DS : [ESI] ;
		MOV Index, CX;
		MOV ECX, DWORD PTR SS : [EBP + 0x8] ;
		PUSHAD;
	}

	if (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))
	{
		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		CMP WORD PTR DS : [ESI] , 0x1A0A;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::TransformationRings5()
{
	static DWORD Back[2] = {0x5A60DF, 0x5A60E1};
	static WORD Index;

	__asm
	{
		MOV Index, CX;
		PUSHAD;
	}

	if (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))
	{
		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		CMP CX, 0x1A0A;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::TransformationRings6()
{
	static DWORD Back[3] = {0x541827, 0x541821, 0x54197A};
	static WORD Index;

	__asm
	{
		MOV Index, AX;
		PUSHAD;
	}

	if (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))
	{
		__asm
		{
			POPAD;
			JMP Back[8];
		}
	}

	__asm
	{
		POPAD;
		CMP AX, 0x1A0C;
		JL Less;
		JMP Back[4];

	Less:
		JMP Back[0];
	}
}

void __declspec(naked) CItem::TransformationRings7()
{
	static DWORD Back[2] = {0x541AF8, 0x541AE4};
	static WORD Index;

	__asm
	{
		MOV Index, AX;
		PUSHAD;
	}

	if (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))
	{
		__asm
		{
			POPAD;
			JMP Back[0];
		}
	}

	__asm
	{
		POPAD;
		CMP AX, 0x1A0C;
		JL Less;
		JMP Back[4];

	Less:
		JMP Back[0];
	}
}

void __declspec(naked) CItem::TransformationRings8()
{
	static DWORD Back[3] = {0x5F87C1, 0x5F87E0, 0x5F87DB};
	static DWORD Index;

	__asm
	{
		MOV Index, ECX;
		PUSHAD;
	}

	if (Index >= GET_ITEM_MODEL(13, 32) && Index <= GET_ITEM_MODEL(13, 38))
	{
		__asm
		{
			POPAD;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		CMP ECX, 0x1C21;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::TransformationRings9()
{
	static DWORD Back[2] = {0x5A1910, 0x5A190B};
	static DWORD Function = 0x68EFC7;
	static DWORD Index;
	static int Level;
	static int Mob;
	static char* Name;

	__asm
	{
		MOV Index, EAX;
		PUSHAD;
	}

	if (Index == GET_ITEM(13, 10) || (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38)))
	{
		__asm
		{
			POPAD;
			PUSH EBX;
			MOV EBX, DWORD PTR SS : [EBP + 0xC] ;
			PUSH EDI;
			MOV EDI, DWORD PTR SS : [EBP + 0x10] ;
			MOV Level, EBX;
			PUSHAD;
		}

		if (Index == GET_ITEM(13, 10))
		{
			Mob = Item.TransformationRings[0][Level];
		}
		else if (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))
		{
			Index -= GET_ITEM(13, 32);
			Mob = Item.TransformationRings[Index + 1][Level];
		}

		Name = Monster.GetName(Mob);

		__asm
		{
			POPAD;
			MOV ESI, Name;
			PUSH ESI;
			PUSH 0x76A9658;
			PUSH Item.TransformationRingFormat;
			PUSH EDI;
			CALL Function;
			ADD ESP, 0x10;
			JMP Back[4];
		}
	}

	__asm
	{
		POPAD;
		JMP Back[0];
	}
}

void __declspec(naked) CItem::ConfirmToSell()
{
	static DWORD Back[2] = {0x5CDE0D, 0x5CD833};
	static WORD Index;

	__asm
	{
		MOV Index, CX;
		PUSHAD;
	}

	if ((Index == GET_ITEM(13, 10) || (Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38))) ||		// Aneis de transformação
		(Index == GET_ITEM(13, 39) || Index == GET_ITEM(13, 40)))										// Aneis de prisma
	{
		__asm
		{
			POPAD;
			JMP Back[0];
		}
	}

	__asm
	{
		POPAD;
		TEST BYTE PTR DS : [0x785ACE3] , 0x3F;
		JMP Back[4];
	}
}

void __declspec(naked) CItem::RenderizingInventory()
{
	static DWORD Back = 0x5E5CC9;
	static DWORD Function = 0x5D1EF0;

	__asm
	{
		MOV Item.ApplyPrismGlow, 0;
		CALL Function;
		MOV Item.ApplyPrismGlow, 1;
		JMP Back;
	}
}

void __declspec(naked) CItem::RenderizingShop()
{
	static DWORD Back = 0x5E5D10;
	static DWORD Function = 0x5D1EF0;

	__asm
	{
		MOV Item.ApplyPrismGlow, 0;
		CALL Function;
		MOV Item.ApplyPrismGlow, 1;
		JMP Back;
	}
}

int CItem::TestEffect(int Index, DWORD Unk1, DWORD Unk2, float* Color, DWORD Count, float Size, DWORD Unk3)
{
	Prism.GetEffect((FloatColor*)(Color), Player.PrismArmor.Color, Player.Index, Player.PrismArmor.Speed);
	return pShowEffect2(Index, Unk1, Unk2, Color, Count, Size, Unk3);
}

CItem Item;