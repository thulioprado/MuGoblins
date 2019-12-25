#include "Library.h"
#include "Item.h"
#include "Monster.h"
#include "Viewport.h"
#include "Player.h"

BYTE CItem::SecondWeaponFixVal;
char CItem::DescriptionText[60][100];
DWORD CItem::DescriptionColor[60];
DWORD CItem::DescriptionType[60];

CItem::CItem() : TransformationRings{}, TransformationRingFormat("%s [%s]")
{
	this->SecondWeaponFixVal = 0;

	memset(this->DescriptionText, 0, sizeof(this->DescriptionText));
	memset(this->DescriptionColor, 0, sizeof(this->DescriptionColor));
	memset(this->DescriptionType, 0, sizeof(this->DescriptionType));
	memset(this->TransformationRings, 0, sizeof(this->TransformationRings));
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
	// Aumento de limite de descrições nos itens
	//
	Memory::Change<DWORD>(0x40C254, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40C2A4, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CD4F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CE23, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CE69, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CEAD, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CEF3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CF3B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CF81, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CFDC, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D18A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D1CD, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D334, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D350, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D36B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D4DA, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D500, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D527, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D543, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D570, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D729, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40D745, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4200F9, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x420182, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x420F2C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x42101A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4214A3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421525, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421571, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4215A1, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4215BD, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4215F6, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421621, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421668, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x42168E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4216B7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4216F0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421967, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421B18, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421CFF, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421F29, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421F55, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x421F82, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x42201A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x422045, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x42208B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x432D39, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x432D5C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x432D98, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x43310A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x43313E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4331B8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4331D4, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x433211, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x433230, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x492555, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x49258E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x4925C2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5960A2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59625F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59635E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x599689, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5996C2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5996F6, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59E661, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59E6FB, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59E8C8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59EB17, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59EE27, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59EE82, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59EEBB, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59EF11, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59EFA4, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59F04A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59F10D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59F20C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59F385, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FD77, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FDA6, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FDE7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FE0F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FE76, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FEC0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FEF3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FF34, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FF5C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x59FFC3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A01B9, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A01E3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A0228, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A0260, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A0286, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A02AC, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A0417, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A043D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A050A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A0580, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A05BC, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A21E8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A222A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A252D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2620, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A27D0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2855, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2D4C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2D72, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2DC2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2DE8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2E5D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2E81, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2EC0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2EEF, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2F46, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2F76, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2FC2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A2FE2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A300C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A302C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3069, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3089, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3157, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A31AD, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A31CD, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A31F1, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A323D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A325D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A32A1, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A32CB, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A32EB, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A332D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3365, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A33A7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A33C7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A36DB, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A39F2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3B17, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3B53, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3B8A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3BBE, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3BF0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3C2D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3C75, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3CAD, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3CDA, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3D34, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3D85, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3DD3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3E22, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3E57, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3E8C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3EC0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3EFA, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3F35, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3F9D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A3FD1, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4007, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4060, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4094, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A40CA, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4105, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A413E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4174, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A41AD, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A41E3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4217, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A424B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A429E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A42D2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A430B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A433F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A437B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A43C4, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A43F8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4431, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4465, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4493, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A44C7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4500, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4534, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A455D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A459C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A45D0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A463B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4B40, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4BF0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4C5B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4C9F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4CF5, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4D37, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4D7E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4DB2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4DF2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4E26, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4E3B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4E6F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4EAC, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4F26, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4F5A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4F9F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A4FD3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5015, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5050, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A508B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A50D0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A510B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5146, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5181, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A51BE, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A51F4, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A522F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5281, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A52B6, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A52EA, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5331, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5378, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A53AE, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A53E2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5407, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A544C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5477, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A54C9, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A54F9, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5518, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5537, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A557E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A55AE, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A55CD, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A55EC, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A561D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5670, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A56C0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A56E6, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5719, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A574D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A579D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A57D6, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A580A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A583E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5877, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A58CC, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A58F1, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5906, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5E28, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5E7F, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5F0D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5F40, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A5F8B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A60F8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A613A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6160, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6185, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A61DB, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6210, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A625C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A62C4, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A65C7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6802, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6B05, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6CD6, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6D45, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6DC8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6E34, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6EA0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6F0C, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6F78, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A6FE4, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7075, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A70A8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A70F8, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A712B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7187, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A71C0, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A720A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7243, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7283, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A730A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7350, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A736D, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7387, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A73DE, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A742E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7472, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7661, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A774B, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A77CE, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A77F7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7836, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7B29, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7DD2, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7DF7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7E19, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7E54, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7EB3, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7EED, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A7F1A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A877E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A89EC, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8A1A, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8A4E, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8A82, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8AB4, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8B04, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8B66, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8BC7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8EF9, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A8F39, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A90D6, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A9117, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A9375, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A93A7, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x5A95D1, (DWORD)(&this->DescriptionText));
	Memory::Change<DWORD>(0x40CD8A, (DWORD)(&this->DescriptionText[1]));
	Memory::Change<DWORD>(0x40D1F2, (DWORD)(&this->DescriptionText[1]));
	Memory::Change<DWORD>(0x420115, (DWORD)(&this->DescriptionText[1]));
	Memory::Change<DWORD>(0x420F53, (DWORD)(&this->DescriptionText[1]));
	Memory::Change<DWORD>(0x42106E, (DWORD)(&this->DescriptionText[1]));
	Memory::Change<DWORD>(0x4214C1, (DWORD)(&this->DescriptionText[1]));
	Memory::Change<DWORD>(0x5960BB, (DWORD)(&this->DescriptionText[1]));
	Memory::Change<DWORD>(0x5A8797, (DWORD)(&this->DescriptionText[1]));
	Memory::Change<DWORD>(0x40CDA8, (DWORD)(&this->DescriptionText[2]));
	Memory::Change<DWORD>(0x40D20E, (DWORD)(&this->DescriptionText[2]));
	Memory::Change<DWORD>(0x420127, (DWORD)(&this->DescriptionText[2]));
	Memory::Change<DWORD>(0x42143B, (DWORD)(&this->DescriptionText[2]));
	Memory::Change<DWORD>(0x5960D9, (DWORD)(&this->DescriptionText[2]));
	Memory::Change<DWORD>(0x5A87B8, (DWORD)(&this->DescriptionText[2]));
	Memory::Change<DWORD>(0x40D220, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x420160, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x42145D, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x59610D, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x5961CF, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x5961DB, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x5961E7, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x5961F3, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x5961FF, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x59620B, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x5A8850, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x5A892D, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x5A895D, (DWORD)(&this->DescriptionText[3]));
	Memory::Change<DWORD>(0x40D249, (DWORD)(&this->DescriptionText[4]));
	Memory::Change<DWORD>(0x596222, (DWORD)(&this->DescriptionText[4]));
	Memory::Change<DWORD>(0x5A8871, (DWORD)(&this->DescriptionText[4]));
	Memory::Change<DWORD>(0x596234, (DWORD)(&this->DescriptionText[5]));
	Memory::Change<DWORD>(0x5A88A4, (DWORD)(&this->DescriptionText[5]));
	Memory::Change<DWORD>(0x40C266, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40C2B4, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40CD7A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40CE38, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40CEC2, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40CF50, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40CF96, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40D005, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40D018, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40D183, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40D510, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40D556, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x4200DC, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x420F3F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421030, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421044, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x42105F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x4214A9, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x4214DB, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421593, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421943, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421977, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x42198A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421CE9, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421D0F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421D18, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421F47, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x421FBB, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x42202F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x422052, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x432D6C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x432DA8, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x433100, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x43311A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x433174, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x433181, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x4331EA, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x433240, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x49256F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x4925A3, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x4925DD, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59624A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5996A3, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5996D7, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x599711, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59E70B, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59EB65, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59EE9E, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59EEDA, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59EF66, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59EF76, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59F012, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59F022, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59F0A6, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59F0B6, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59F0FB, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59F223, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59F230, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59F39E, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59FDCA, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59FE43, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59FF17, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x59FF90, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A023D, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A042F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A0480, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A04E3, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A0558, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A21C2, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A263D, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A27ED, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A2807, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A2821, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A282E, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A2D34, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A2DA6, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3C15, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3C63, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3C9C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3D53, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3DA4, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3DF2, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3E37, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3E6C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3EA1, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3F14, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3F4D, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3FB2, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A3FE6, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A401C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4075, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A40A9, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A40DF, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A411F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4153, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4189, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A41C2, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A41F8, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A422C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4263, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A42B3, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A42EC, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4320, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4354, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4390, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A43D9, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4412, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4446, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A447A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A44A8, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A44E1, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4515, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4549, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4572, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A45B1, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A45E5, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4BA6, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4BBB, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4C1A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4C2C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4C70, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4CBB, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4CC8, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4D0A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4D4C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4D93, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4DC7, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4E07, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4E50, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4E84, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4EC1, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4F3B, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4F6F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4FB4, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A4FE8, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A502A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5065, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A50A0, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A50E5, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5120, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A515B, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5196, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A51D3, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5209, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5244, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5296, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A52CB, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A52FF, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A534A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A538D, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A542A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A54A0, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5555, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A560C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A564B, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A565E, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5692, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A56AE, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A572E, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5762, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A57B7, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A57EB, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A581F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5858, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A588C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A591B, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5E0C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5E45, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5EAB, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5ED4, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5F58, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A5F77, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A619A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A61F0, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6225, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6271, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6599, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A65DC, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A65E7, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6ADE, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6B1A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6B3D, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6D22, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6D5A, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6D7F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6E11, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6E49, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6E57, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6EE9, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6F21, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6F2F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6FC1, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A6FF9, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A7007, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A70C2, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A7145, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A71DA, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A725D, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A7322, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A739C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A73F3, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A780C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A7E3C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A7ECA, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8A2C, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8A60, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8A95, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8AC4, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8B14, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8B76, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8BD7, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8F14, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A8F4F, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A90F6, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A9127, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x5A9385, (DWORD)(&this->DescriptionColor));
	Memory::Change<DWORD>(0x40CD98, (DWORD)(&this->DescriptionColor[1]));
	Memory::Change<DWORD>(0x40D200, (DWORD)(&this->DescriptionColor[1]));
	Memory::Change<DWORD>(0x420107, (DWORD)(&this->DescriptionColor[1]));
	Memory::Change<DWORD>(0x5960C9, (DWORD)(&this->DescriptionColor[1]));
	Memory::Change<DWORD>(0x5A87A9, (DWORD)(&this->DescriptionColor[1]));
	Memory::Change<DWORD>(0x40CDB3, (DWORD)(&this->DescriptionColor[2]));
	Memory::Change<DWORD>(0x421441, (DWORD)(&this->DescriptionColor[2]));
	Memory::Change<DWORD>(0x59EE03, (DWORD)(&this->DescriptionColor[2]));
	Memory::Change<DWORD>(0x59EDFC, (DWORD)(&this->DescriptionColor[3]));
	Memory::Change<DWORD>(0x5A8860, (DWORD)(&this->DescriptionColor[3]));
	Memory::Change<DWORD>(0x5A893B, (DWORD)(&this->DescriptionColor[3]));
	Memory::Change<DWORD>(0x5A896C, (DWORD)(&this->DescriptionColor[3]));
	Memory::Change<DWORD>(0x40D246, (DWORD)(&this->DescriptionColor[4]));
	Memory::Change<DWORD>(0x5A8888, (DWORD)(&this->DescriptionColor[4]));
	Memory::Change<DWORD>(0x5A88B2, (DWORD)(&this->DescriptionColor[5]));
	Memory::Change<DWORD>(0x40CD80, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x40CE42, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x40CECC, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x40CF57, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x40CFA1, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x40D02B, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x40D55D, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x4200E1, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x420F49, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x421074, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x4214AF, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x4214E2, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x42194A, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x42197E, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x421991, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x421CE2, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x421D2A, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x421FC6, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x42205E, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x433121, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x43318F, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x492576, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x4925AA, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x4925E4, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x596251, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5996AA, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5996DE, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x599718, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59E666, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59E7EF, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59E8CD, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59EB1E, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59EEE0, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59EFB9, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59F073, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59F122, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59F243, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59FDD1, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59FE4F, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59FF1E, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x59FF9C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A0248, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A0436, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A045C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A2839, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A2D3B, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A2DB1, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3C0A, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3C8A, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3CC2, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3CF7, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3D5A, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3DAB, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3DF9, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3E3E, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3E73, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3EA8, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3EDB, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3F1B, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3F54, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3FB9, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A3FED, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4023, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A407C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A40B0, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A40E6, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4126, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A415A, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4190, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A41C9, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A41FF, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4233, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A426A, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A42BA, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A42F3, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4327, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A43E0, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4419, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A444D, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A44AF, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A44E8, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A451C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A457D, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A45B8, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A45EC, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4BB1, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4BC2, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4C33, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4C77, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4CCF, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4D11, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4D53, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4D9A, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4DCE, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4E0E, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4E57, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4E8F, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4ECC, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4F42, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4F76, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4FBB, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A4FEF, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5031, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A506C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A50A7, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A50EC, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5127, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5162, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A519D, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A51DA, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5210, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A524B, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A529D, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A52D2, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5306, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5351, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5394, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5735, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5769, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A57BE, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A57F2, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5826, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A585F, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5893, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5926, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5E4C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A5EDF, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A61A1, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A61F7, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A622C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6278, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A65A0, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A65EE, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6AD7, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6B44, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6D29, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6D8C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6E18, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6E64, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6EF0, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6F3C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A6FC8, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A7014, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A70C9, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A714C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A71E1, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A7264, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A732D, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A73FE, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A7813, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A7E31, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A7ED1, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8A33, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8A67, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8A9C, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8ACB, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8B1F, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8B81, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8BE2, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8F1B, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A8F5B, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A90FD, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A912E, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x5A9390, (DWORD)(&this->DescriptionType));
	Memory::Change<DWORD>(0x40CD9E, (DWORD)(&this->DescriptionType[1]));
	Memory::Change<DWORD>(0x5960CF, (DWORD)(&this->DescriptionType[1]));
	Memory::Change<DWORD>(0x5A87AE, (DWORD)(&this->DescriptionType[1]));
	Memory::Change<DWORD>(0x40CDB9, (DWORD)(&this->DescriptionType[2]));
	Memory::Change<DWORD>(0x421447, (DWORD)(&this->DescriptionType[2]));
	Memory::Change<DWORD>(0x5A8866, (DWORD)(&this->DescriptionType[3]));
	Memory::Change<DWORD>(0x5A8945, (DWORD)(&this->DescriptionType[3]));
	Memory::Change<DWORD>(0x5A8971, (DWORD)(&this->DescriptionType[3]));
	Memory::Change<DWORD>(0x40D23F, (DWORD)(&this->DescriptionType[4]));
	Memory::Change<DWORD>(0x5A888E, (DWORD)(&this->DescriptionType[4]));
	Memory::Change<DWORD>(0x5A88B8, (DWORD)(&this->DescriptionType[5]));
	//Memory::Change<DWORD>(0x4BFFCB, (DWORD)(&this->DescriptionType[15]));
	//Memory::Change<DWORD>(0x5AAF50, (DWORD)(&this->DescriptionType[15]));
	//Memory::Change<DWORD>(0x647E73, (DWORD)(&this->DescriptionType[15]));
	//Memory::Change<DWORD>(0x648B4B, (DWORD)(&this->DescriptionType[15]));
	//Memory::Change<DWORD>(0x5D3941, (DWORD)(&this->DescriptionType[17]));

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
	// Anel de Prisma
	//
	pLoadModel(GET_ITEM_MODEL(13, 39), "Data\\Item\\", "Ring", 3);

	//
	// Tintas e Neutralizadores
	//
	pLoadModel(GET_ITEM_MODEL(14, 32), "Data\\Item\\", "pipe_r", -1);
	pLoadModel(GET_ITEM_MODEL(14, 33), "Data\\Item\\", "pipe_g", -1);
	pLoadModel(GET_ITEM_MODEL(14, 34), "Data\\Item\\", "pipe_b", -1);
	pLoadModel(GET_ITEM_MODEL(14, 35), "Data\\Item\\", "pipe_r", -1);
	pLoadModel(GET_ITEM_MODEL(14, 36), "Data\\Item\\", "pipe_g", -1);
	pLoadModel(GET_ITEM_MODEL(14, 37), "Data\\Item\\", "pipe_b", -1);
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
	// Anel de Prisma
	//
	pLoadTexture(GET_ITEM_MODEL(13, 39), "Item\\", GL_REPEAT, GL_NEAREST, 1);

	//
	// Tintas e Neutralizadores
	//
	pLoadTexture(GET_ITEM_MODEL(14, 32), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 33), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 34), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 35), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 36), "Item\\", GL_REPEAT, GL_NEAREST, 1);
	pLoadTexture(GET_ITEM_MODEL(14, 37), "Item\\", GL_REPEAT, GL_NEAREST, 1);
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
	if (Index == GET_ITEM_MODEL(13, 39))
	{
		pModelPositionX = 270.f;
		pModelPositionY = -10.f;
		pModelPositionZ = 0.f;

		return true;
	}

	return false;
}

bool CItem::GetModelSize(DWORD Index)
{
	//
	// Anéis de transformação
	//
	if (Index == GET_ITEM_MODEL(13, 10) || (Index >= GET_ITEM_MODEL(13, 32) && Index <= GET_ITEM_MODEL(13, 38)))
	{
		this->ModelSize = 0.0025f;

		return true;
	}

	//
	// Anel de Prisma
	//
	if (Index == GET_ITEM_MODEL(13, 39))
	{
		this->ModelSize = 0.0025f;

		return true;
	}

	return false;
}

void CItem::SetDescription(ItemInfo* Item)
{
	switch (Item->Index)
	{
		case GET_ITEM(13, 39):	// Anel de Prisma [Armadura]
		{
			break;
		}
		case GET_ITEM(13, 40):	// Anel de Prisma [Arma]
		{
			break;
		}
	}
}

float CItem::InterpolateValue(float Start, float End, float Timer)
{
	return Start + (End - Start) * Timer;
}

bool CItem::Equals(float A, float B)
{
	return fabs(A - B) < 0.001f;
}

bool CItem::InterpolateColors(GlowColor* Color, float Timer, float R1, float G1, float B1, float R2, float G2, float B2)
{
	Color->Red = this->InterpolateValue(R1, R2, Timer);
	Color->Green = this->InterpolateValue(G1, G2, Timer);
	Color->Blue = this->InterpolateValue(B1, B2, Timer);

	return Equals(Color->Red, R2) && Equals(Color->Green, G2) && Equals(Color->Blue, B2);
}

bool CItem::ApplyPrismEffect(GlowColor* Color, PrismEffect* Prism, int Index)
{
	int Rand = (Index * 18765 + 44162) % RAND_MAX;
	float Timer = (1.0f + sin((GetTickCount() + (Rand % 750)) / (750.0f + (float)(Rand % 50)))) * 0.5f;

	if ((Prism[0].Red != 0.f || Prism[0].Green != 0.f || Prism[0].Blue != 0.f) && 
		(Prism[1].Red != 0.f || Prism[1].Green != 0.f || Prism[1].Blue != 0.f))
	{
		this->InterpolateColors(Color, Timer, Prism[0].Red, Prism[0].Green, Prism[0].Blue, Prism[1].Red, Prism[1].Green, Prism[1].Blue);
	}
	else if (Prism[0].Red != 0.f || Prism[0].Green != 0.f || Prism[0].Blue != 0.f)
	{
		Color->Red = Prism[0].Red;
		Color->Green = Prism[0].Green;
		Color->Blue = Prism[0].Blue;
	}
	else if (Prism[1].Red != 0.f || Prism[1].Green != 0.f || Prism[1].Blue != 0.f)
	{
		Color->Red = Prism[1].Red;
		Color->Green = Prism[1].Green;
		Color->Blue = Prism[1].Blue;
	}
	else
	{
		return false;
	}

	return true;
}

void CItem::ApplyPrismValue(PrismEffect* Prism, BYTE Red, BYTE Green, BYTE Blue)
{
	BYTE RedValue[] = {GET_NIBBLE_X(Red), GET_NIBBLE_Y(Red)};
	BYTE GreenValue[] = {GET_NIBBLE_X(Green), GET_NIBBLE_Y(Green)};
	BYTE BlueValue[] = {GET_NIBBLE_X(Blue), GET_NIBBLE_Y(Blue)};

	Prism[0].Red = float(RedValue[0] * 0x11) / 255.f;
	Prism[1].Red = float(RedValue[1] * 0x11) / 255.f;
	Prism[0].Green = float(GreenValue[0] * 0x11) / 255.f;
	Prism[1].Green = float(GreenValue[1] * 0x11) / 255.f;
	Prism[0].Blue = float(BlueValue[0] * 0x11) / 255.f;
	Prism[1].Blue = float(BlueValue[1] * 0x11) / 255.f;
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
	static float MSize;

	__asm
	{
		MOV BYTE PTR DS : [ECX + 0xA0] , 0x00;
		MOV Index, ESI;
		PUSHAD;
	}

	if (Item.GetModelSize(Index))
	{
		MSize = Item.ModelSize;

		__asm
		{
			POPAD;
			MOV EAX, MSize;
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

	//
	// Aneis de transformação
	//
	if (Index == GET_ITEM_MODEL(13, 10) || (Index >= GET_ITEM_MODEL(13, 32) && Index <= GET_ITEM_MODEL(13, 38)))
	{
		__asm
		{
			POPAD;
			JMP Back[12];
		}
	}

	//
	// Tintas
	//
	if (Index >= GET_ITEM_MODEL(14, 32) && Index <= GET_ITEM_MODEL(14, 34))
	{
		__asm
		{
			POPAD;
			JMP Back[12];
		}
	}

	//
	// Neutralizadores
	//
	if (Index >= GET_ITEM_MODEL(14, 35) && Index <= GET_ITEM_MODEL(14, 37))
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

void CItem::SetGlow(int ItemModel, float Alpha, DWORD Unk2, GlowColor* Color, DWORD Unk3)
{
	switch (ItemModel)
	{
		case GET_ITEM_MODEL(14, 32):	// Tinta Vermelha
		{
			Color->Red = 0.1f;
			Color->Green = 0.0f;
			Color->Blue = 0.0f;
			return;
		}
		case GET_ITEM_MODEL(14, 33):	// Tinta Verde
		{
			Color->Red = 0.0f;
			Color->Green = 0.1f;
			Color->Blue = 0.0f;
			return;
		}
		case GET_ITEM_MODEL(14, 34):	// Tinta Azul
		{
			Color->Red = 0.0f;
			Color->Green = 0.0f;
			Color->Blue = 0.1f;
			return;
		}
	}

	if (Viewport.Renderizing && Viewport.Renderizing->Type == 1 && !Viewport.RenderizingInventory)
	{		
		if (Viewport.Renderizing->Index == Player.Index)
		{
			if (Viewport.Renderizing->Armor == ItemModel || Viewport.Renderizing->Helm == ItemModel || Viewport.Renderizing->Gloves == ItemModel || Viewport.Renderizing->Pants == ItemModel || Viewport.Renderizing->Boots == ItemModel)
			{
				if (Item.ApplyPrismEffect(Color, Player.PrismArmor, Player.Index))
				{
					return;
				}
			}
			else if (Viewport.Renderizing->LeftWeapon == ItemModel || Viewport.Renderizing->RightWeapon == ItemModel)
			{
				if (Item.ApplyPrismEffect(Color, Player.PrismWeapon, Player.Index))
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
					if (Item.ApplyPrismEffect(Color, Player.CharacterList[Viewport.Renderizing->Index].PrismArmor, Viewport.Renderizing->Index))
					{
						return;
					}
				}
				else if (Viewport.Renderizing->LeftWeapon == ItemModel || Viewport.Renderizing->RightWeapon == ItemModel)
				{
					if (Item.ApplyPrismEffect(Color, Player.CharacterList[Viewport.Renderizing->Index].PrismWeapon, Viewport.Renderizing->Index))
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
						if (Item.ApplyPrismEffect(Color, Target->PrismArmor, Viewport.Renderizing->Index))
						{
							return;
						}
					}
					else if (Viewport.Renderizing->LeftWeapon == ItemModel || Viewport.Renderizing->RightWeapon == ItemModel)
					{
						if (Item.ApplyPrismEffect(Color, Target->PrismWeapon, Viewport.Renderizing->Index))
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

	if (Index == GET_ITEM(13, 39) || Index == GET_ITEM(13, 40)) // Aneis de Prisma
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

	if ((Index >= GET_ITEM(14, 32) && Index <= GET_ITEM(14, 34)) ||		// Tintas
		(Index >= GET_ITEM(14, 35) && Index <= GET_ITEM(14, 37)))		// Neutralizadores
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

	if ((Index >= GET_ITEM(14, 32) && Index <= GET_ITEM(14, 34)) ||		// Tintas
		(Index >= GET_ITEM(14, 35) && Index <= GET_ITEM(14, 37)))		// Neutralizadores
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

	if (Index == GET_ITEM(13, 10) || 
		(Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38)))
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

	if (Index == GET_ITEM_MODEL(13, 10) || 
		(Index >= GET_ITEM_MODEL(13, 32) && Index <= GET_ITEM_MODEL(13, 38)))
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

	if (Index == GET_ITEM(13, 10) || 
		(Index >= GET_ITEM(13, 32) && Index <= GET_ITEM(13, 38)))
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

CItem Item;