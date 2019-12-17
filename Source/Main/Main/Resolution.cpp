#include "Library.h"
#include "Resolution.h"
#include "Window.h"

CResolution::CResolution() : Info{}
{
}

CResolution::~CResolution()
{
}

void CResolution::Load()
{	
	//
	// Resoluções
	//				width, height, font size, move list sizes
	auto Resolution = Registry::Read<DWORD>("Resolution");

	switch (Resolution)
	{
		case 1:
		{
			this->Info = {800, 600, 13, {170, 39, 99, 144}};
			break;
		}
		case 2:
		{
			this->Info = {1024, 768, 14, {150, 34, 89, 128}};
			break;
		}
		case 3:
		{
			this->Info = {1152, 864, 14, {130, 29, 74, 109}};
			break;
		}
		case 4:
		{
			this->Info = {1280, 720, 15, {130, 29, 74, 109}};
			break;
		}
		case 5:
		{
			this->Info = {1280, 800, 15, {130, 29, 74, 109}};
			break;
		}
		case 6:
		{
			this->Info = {1920, 1080, 18, {130, 29, 74, 109}};
			break;
		}
		default:
		{
			this->Info = {640, 480, 12, {190, 44, 104, 154}};
			break;
		}
	}

	//
	// Correção de seleção de servidores (widescreen)
	//
	if (this->Info.Width >= 1280)
	{
		Memory::Change<DWORD>(0x6224C8, 0x1A0);
		Memory::Change<BYTE>(0x6224D2, 0x20);
	}

	//
	// Ganchos
	//
	Memory::Jump(0x4B3AAE, this->WindowSize);
	Memory::Jump(0x4B4D5F, this->FontSize);
	Memory::Jump(0x443B70, this->MoveListSize);
	Memory::Jump(0x44315B, this->MoveListClick);
	Memory::Call(0x60F066, this->DrawLogo);
}

void __declspec(naked) CResolution::WindowSize()
{
	static DWORD Back = 0x4B3B2A;

	__asm
	{
		PUSHAD;
	}

	pScreenWidth = Resolution.Info.Width;
	pScreenHeight = Resolution.Info.Height;

	if (Resolution.Info.Width >= 1280)
	{
		pClipPrecisionX += 1500.f;
	}

	__asm
	{
		POPAD;
		JMP Back;
	}
}

void __declspec(naked) CResolution::FontSize()
{
	static DWORD Back = 0x4B4DCD;

	__asm
	{
		PUSHAD;
	}

	pFontSize = Resolution.Info.Font;

	__asm
	{
		POPAD;
		JMP Back;
	}
}

void __declspec(naked) CResolution::MoveListSize()
{
	static DWORD Back = 0x443C14;

	__asm
	{
		MOVZX EAX, Resolution.Info.Move[0];
		MOV DWORD PTR SS : [EBP - 0x8] , EAX;
		MOVZX EAX, Resolution.Info.Move[2];
		MOV DWORD PTR SS : [EBP - 0xC] , EAX;
		MOVZX EAX, Resolution.Info.Move[4];
		MOV DWORD PTR SS : [EBP - 0x10] , EAX;
		MOVZX EAX, Resolution.Info.Move[6];
		MOV DWORD PTR SS : [EBP - 0x14] , EAX;
		JMP Back;
	}
}

void __declspec(naked) CResolution::MoveListClick()
{
	static DWORD Back = 0x44319D;

	__asm
	{
		MOV DWORD PTR SS : [EBP - 0x58] , ESI;
		MOVZX EBX, Resolution.Info.Move[0];
		JMP Back;
	}
}

void CResolution::DrawLogo(int Index, float X, float Y, float Width, float Height, float Unk1, float Unk2, float ScaleX, float ScaleY, char Unk3, float Unk4)
{
	RECT Rect;

	if (GetWindowRect(Window.GetHandle(), &Rect))
	{
		X = ((Rect.right - Rect.left) / 2) - (Width / 2);
		Y = ((Rect.bottom - Rect.top) / 2) - (Height / 2);
	}
	else
	{
		X = (Resolution.Info.Width / 2) - (Width / 2);
		Y = (Resolution.Info.Height / 2) - (Height / 2);
	}

	pDrawImage(Index, X, Y, Width, Height, Unk1, Unk2, ScaleX, ScaleY, Unk3, Unk4);
}

CResolution Resolution;