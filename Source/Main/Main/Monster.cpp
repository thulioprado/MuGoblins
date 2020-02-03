#include "Library.h"
#include "Monster.h"
#include "Viewport.h"
#include "Model.h"

#define NPC_FOLDER		"Data\\Npc\\"
#define MONSTER_FOLDER	"Data\\Monster\\"

CMonster::CMonster() : Monsters(), Health()
{
}

CMonster::~CMonster()
{
}

void CMonster::Load()
{
	int Count = MODEL_MONSTER - 1;

	//
	// Monsters
	//
	this->Monsters[50] = {++Count, MONSTER_FOLDER, "Monster50", false};

	//
	// Ganchos
	//
	Memory::Call(0x596CEA, this->DrawHealth);
	Memory::Call(0x50ACE6, this->Renderize);
	Memory::Jump(0x50AC73, this->SetType);
	Memory::Jump(0x55B769, this->CanTalk);
}

char* CMonster::GetName(WORD Index) const
{
	static MonsterName* Monster = (MonsterName*)(MONSTER_NAME_BASE);

	for (short i = 0; i < 512; ++i)
	{
		if (Monster[i].Index == Index)
		{
			return Monster[i].Name;
		}
	}

	return (char*)("");
}

MonsterInfo* CMonster::Get(WORD Index)
{
	if (this->Monsters.count(Index) > 0)
	{
		return &this->Monsters.at(Index);
	}

	return null;
}

void CMonster::ResetHealth()
{
	this->Health.clear();
}

void CMonster::AddHealth(WORD Index, BYTE Percent)
{
	this->Health[Index] = Percent;
}

BYTE CMonster::GetHealthPercent(WORD Index) const
{
	if (this->Health.count(Index) > 0)
	{
		return this->Health.at(Index);
	}

	return (BYTE)(-1);
}

void CMonster::DrawHealth()
{
	SelectObject(pHDC, pDefaultFont);

	int X = 0, Y = 0, Width = 80;
	Angle Angle;
	SIZE Size;
	ViewportInfo* Viewport;
	BYTE Percent;

	for (int i = 0; i < VIEWPORT_MAX; ++i)
	{
		Viewport = ::Viewport.GetByPosition(i);

		if (!Viewport->Live)
		{
			continue;
		}

		if (Viewport->Type == VIEWPORT_MONSTER)
		{
			Percent = Monster.GetHealthPercent(Viewport->Index);

			if (Percent == (BYTE)(-1))
			{
				continue;
			}

			Angle.X = Viewport->Angle1.X;
			Angle.Y = Viewport->Angle1.Y;
			Angle.Z = Viewport->Angle1.Z + Viewport->Angle2.Z + 100.f;

			pGetPositionFromAngle(&Angle, &X, &Y);

			X -= (int)(floor(Width / 2.f));

			pEnableAlpha(1);
			glColor4f(0.f, 0.f, 0.f, 0.5f);
			pDrawForm((float)(X - 1), (float)(Y - 12), 2.f + (float)(Width), 17.f);

			glColor3f(0.35f, 0.25f, 0.25f);
			pDrawForm((float)(X), 1.f + Y, (float)(Width), 3.f);

			glColor3f(0.9f, 0.f, 0.f);
			pDrawForm((float)(X), 1.f + Y, (float)((Percent * Width) / 100.f), 3.f);
			pDisableAlpha();

			GetTextExtentPoint(pHDC, Viewport->Name, strlen(Viewport->Name), &Size);
			Size.cx = 80 - (LONG)((float)(Size.cx) / pScreenDivisorX);

			if (Size.cx < 0)
			{
				Size.cx = 0;
			}
			else
			{
				Size.cx /= 2;
			}

			/*switch (Monster->Level) {
				case 7:		// Boss
					glColor4f(0.2f, 0.6f, 1.0f, 1.0f);
					break;
				case 6:		// Extremely Hard
					glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				case 5:		// Very Hard
					glColor4f(1.0f, 0.40f, 0.40f, 1.0f);
					break;
				case 4:		// Hard
					glColor4f(1.0f, 0.71f, 0.71f, 1.0f);
					break;
				case 3:		// Easy
					glColor4f(0.71f, 0.94f, 0.71f, 1.0f);
					break;
				case 2:		// Very Easy
					glColor4f(0.40f, 0.94f, 0.40f, 1.0f);
					break;
				case 1:		// Extremely Easy
					glColor4f(0.0f, 0.94f, 0.0f, 1.0f);
					break;
				default:	// Normal
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					break;
			}*/

			glColor4f(1.f, 1.f, 1.f, 1.f);

			X += Size.cx;
			Y -= 9;

			if ((X >= 0 && X <= (pScreenWidthBase - (Size.cx / 2))) && (Y >= 0 && Y <= pScreenHeightBase))
			{
				pDrawText(X, Y, Viewport->Name, 0, 0, 0);
			}
		}
	}

	pDisableAlphaBlend();
	glColor3f(1.f, 1.f, 1.f);

	pDrawMonsterHP();
}

ViewportInfo* CMonster::Renderize(int Index, DWORD a2, int a3, int a4)
{
	auto Mob = Monster.Get((WORD)(Index));

	if (Mob)
	{
		auto Model = (ModelInfo*)(Mob->Model * sizeof(ModelInfo) + MODEL_BASE);

		if (!*(BYTE*)(0x6B8D84) || Model->UnkWord26 <= 0)
		{
			pLoadModel(Mob->Model, Mob->Folder, Mob->File, -1);

			if (Model->UnkWord26 > 0)
			{
				DWORD Address = 0;

				for (short i = 0; i < Model->UnkWord26; ++i)
				{
					*(float*)(Address + Model->UnkDword30 + 4) = 0.25f;
					Address += 16;
				}
			}
		}

		if (Model->UnkWord24 > 0)
		{
			pLoadTexture(Mob->Model, &Mob->Folder[5], GL_REPEAT, GL_NEAREST, 1);
		}

		auto Result = pRenderizeModel(a4, Mob->Model, a2, a3, 0);

		switch (Index)
		{
			case 50:
			{
				Result->Size = 0.60f;
				break;
			}
			case 259:
			{
				Result->Size = 0.95f;
				Result->Angle1.Z += 140.f;
				break;
			}
			case 350:
			{
				Result->Size = 0.95f;
				break;
			}
			case 351:
			{
				Result->Size = 0.85f;
				break;
			}
		}

		return Result;
	}

	return pRenderizeMonster(Index, a2, a3, a4);
}

void __declspec(naked) CMonster::SetType()
{
	static DWORD Back[3] = {0x50AC79, 0x50AC92, 0x50ACBA};
	static int Index;
	static MonsterInfo* Mob;

	__asm
	{
		MOV Index, ESI;
		PUSHAD;
	}

	Mob = Monster.Get((WORD)(Index));

	if (Mob)
	{
		if (Mob->IsNPC)
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
			JMP Back[8];
		}
	}

	__asm
	{
		POPAD;
		CMP ESI, 0xC8;
		JMP Back[0];
	}
}

void __declspec(naked) CMonster::CanTalk()
{
	static DWORD Back[3] = {0x55B76F, 0x55B782, 0x55B789};
	static WORD Index;
	static MonsterInfo* Mob;

	__asm
	{
		MOV Index, AX;
		PUSHAD;
	}

	Mob = Monster.Get(Index);

	if (Mob)
	{
		if (Mob->IsNPC)
		{
			__asm
			{
				POPAD;
				MOV BYTE PTR DS : [0x788C829] , 0x01;
				JMP Back[8];
			}
		}
		
		__asm
		{
			POPAD;
			MOV BYTE PTR DS : [0x788C829] , 0x00;
			JMP Back[8];
		}
	}

	__asm
	{
		POPAD;
		CMP AX, 0xF3;
		JE Equal;
		JMP Back[0];

	Equal:
		JMP Back[4];
	}
}

CMonster Monster;