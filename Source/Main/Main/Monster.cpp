#include "Library.h"
#include "Monster.h"
#include "Viewport.h"

CMonster::CMonster() : Monsters(), Count(0), Current(0)
{
}

CMonster::~CMonster()
{
}

void CMonster::Load()
{
	//
	// Ganchos
	//
	Memory::Call(0x596CEA, this->DrawHealth);
}

void CMonster::Reset(BYTE Count)
{
	this->Count = Count;
	this->Current = 0;
}

void CMonster::Add(WORD Index, BYTE Percent)
{
	this->Monsters[this->Current].Index = Index;
	this->Monsters[this->Current].Percent = Percent;
	
	++this->Current;
}

MonsterInfo* CMonster::Search(WORD Index)
{
	for (BYTE i = 0; i < this->Count; ++i)
	{
		if (this->Monsters[i].Index == Index)
		{
			return &this->Monsters[i];
		}
	}

	return null;
}

void CMonster::DrawHealth()
{
	SelectObject(pHDC, pDefaultFont);

	int X = 0, Y = 0, Width = 80;
	Angle Angle;
	SIZE Size;
	ViewportInfo* Viewport;
	MonsterInfo* Info;

	for (int i = 0; i < VIEWPORT_MAX; ++i)
	{
		Viewport = ::Viewport.GetByPosition(i);

		if (!Viewport->Live)
		{
			continue;
		}

		if (Viewport->Type == VIEWPORT_MONSTER)
		{
			Info = Monster.Search(Viewport->Index);

			if (!Info || Info->Percent == (BYTE)(-1))
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
			pDrawForm((float)(X), 1.f + Y, (float)((Info->Percent * Width) / 100.f), 3.f);
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

CMonster Monster;