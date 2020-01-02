#include "Library.h"
#include "Viewport.h"
#include "Player.h"
#include "Item.h"
#include "Prism.h"

CViewport::CViewport() : Renderizing(null), Custom()
{
}

CViewport::~CViewport()
{
}

void CViewport::Load()
{
	Memory::Jump(0x50273B, this->SetRenderizing);
}

void CViewport::Clear()
{
	for (int i = 0; i < VIEWPORT_MAX; ++i)
	{
		((ViewportInfo*)(VIEWPORT_BASE + sizeof(ViewportInfo) * i))->Live = false;
	}
}

ViewportInfo* CViewport::GetMyself()
{
	return (ViewportInfo*)(pMyViewport);
}

ViewportInfo* CViewport::GetByPosition(int Position)
{
	if (Position >= VIEWPORT_MAX)
	{
		return null;
	}

	return (ViewportInfo*)(VIEWPORT_BASE + sizeof(ViewportInfo) * Position);
}

ViewportInfo* CViewport::GetByIndex(WORD Index)
{
	ViewportInfo* Viewport = null;

	for (int i = 0; i < VIEWPORT_MAX; ++i)
	{
		Viewport = (ViewportInfo*)(VIEWPORT_BASE + sizeof(ViewportInfo) * i);

		if (Viewport->Live && Viewport->Index == Index)
		{
			break;
		}
	}

	return Viewport;
}

ViewportInfo* CViewport::GetByName(char* Name)
{
	ViewportInfo* Viewport = null;

	for (int i = 0; i < VIEWPORT_MAX; ++i)
	{
		Viewport = (ViewportInfo*)(VIEWPORT_BASE + sizeof(ViewportInfo) * i);

		if (Viewport->Live && !strcmp(Viewport->Name, Name))
		{
			break;
		}
	}

	return Viewport;
}

ViewportCustom* CViewport::GetCustom(WORD Index)
{
	auto it = this->Custom.find(Index);

	if (it != this->Custom.end())
	{
		return &it->second;
	}

	return nullptr;
}

void CViewport::AddCustom(WORD Index, LPBYTE CharSet)
{
	if (Index == Player.Index)
	{
		Prism.GetValue(Player.PrismArmor.Color, CharSet[18], CharSet[19], CharSet[20]);
		Prism.GetValue(Player.PrismWeapon.Color, CharSet[21], CharSet[22], CharSet[23]);

		Player.PrismArmor.Speed = ((char)(CharSet[24]) * -1) * 33;
		Player.PrismWeapon.Speed = ((char)(CharSet[25]) * -1) * 33;
	}
	else
	{
		if (this->Custom.count(Index) < 1)
		{
			this->Custom[Index] = {};
		}

		auto Info = &this->Custom.at(Index);

		if (Info)
		{
			Prism.GetValue(Info->PrismArmor.Color, CharSet[18], CharSet[19], CharSet[20]);
			Prism.GetValue(Info->PrismWeapon.Color, CharSet[21], CharSet[22], CharSet[23]);

			Info->PrismArmor.Speed = ((char)(CharSet[24]) * -1) * 33;
			Info->PrismWeapon.Speed = ((char)(CharSet[25]) * -1) * 33;
		}
	}
}

void __declspec(naked) CViewport::SetRenderizing()
{
	static DWORD Back = 0x502740;

	__asm
	{
		MOV ESI, DWORD PTR SS : [EBP + 0x8] ;
		XOR EAX, EAX;
		MOV Viewport.Renderizing, ESI;
		JMP Back;
	}
}

CViewport Viewport;