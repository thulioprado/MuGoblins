#include "Library.h"
#include "Sign.h"
#include "Viewport.h"
#include "Player.h"

CSign::CSign()
{
}

CSign::~CSign()
{
}

void CSign::Load()
{
	//
	// Ganchos
	//
	Memory::Call(0x504AE7, this->Create);
	Memory::Call(0x4124C9, this->Destroy);
	Memory::Call(0x428E32, this->Destroy);
	Memory::Call(0x428E63, this->Destroy);
	Memory::Call(0x428E94, this->Destroy);
	Memory::Call(0x428ECA, this->Destroy);
	Memory::Call(0x428EFD, this->Destroy);
	Memory::Call(0x429068, this->Destroy);
	Memory::Call(0x42907B, this->Destroy);
	Memory::Call(0x429115, this->Destroy);
	Memory::Call(0x42914A, this->Destroy);
	Memory::Call(0x429339, this->Destroy);
	Memory::Call(0x492446, this->Destroy);
	Memory::Call(0x507B72, this->Destroy);
	Memory::Call(0x507BFD, this->Destroy);
	Memory::Call(0x507C58, this->Destroy);
	Memory::Call(0x507DCA, this->Destroy);
	Memory::Call(0x507E1E, this->Destroy);
	Memory::Call(0x50949D, this->Destroy);
	Memory::Call(0x50A216, this->Destroy);
	Memory::Call(0x50A7E2, this->Destroy);
	Memory::Call(0x5AB8C3, this->Destroy);
	Memory::Call(0x5AC298, this->Destroy);
	Memory::Call(0x5AD2DA, this->Destroy);
	Memory::Jump(0x5F010F, this->Glow);
}

void CSign::LoadModels()
{
	//pLoadModel(1, "Data\\Skill\\", "afksign", -1);
}

void CSign::LoadTextures()
{
	//pLoadTexture(1, "Skill\\", GL_REPEAT, GL_NEAREST, 1);
}

LPBYTE CSign::Create(ViewportInfo* Info)
{
	if (pState == STATE_SELECT_SERVER)
	{
		return null;
	}

	if (Info->Type == 1)
	{
		switch (pState)
		{
			case STATE_SELECT_CHARACTER:
			{
				if (Info->SignID == 0)
				{
					if (Info->Sign)
					{
						Destroy(Info);
					}

					if (Player.CharacterList[Info->Index].Sign != 0)
					{
						Info->SignID = Player.CharacterList[Info->Index].Sign;
					}
					else if ((Info->ViewSkillState & 0x1000000) == 0x1000000 || Info->CtlCode == 8 || Info->CtlCode == 16 || Info->CtlCode == 32 || Info->CtlCode == 64)
					{
						Info->SignID = 5;
					}
				}
				else
				{
					if (Player.CharacterList[Info->Index].Sign != 0)
					{
						if (Info->SignID != Player.CharacterList[Info->Index].Sign)
						{
							if (Info->Sign)
							{
								Destroy(Info);
							}

							Info->SignID = Player.CharacterList[Info->Index].Sign;
						}
					}
					else if (Info->SignID != 5 && (Info->ViewSkillState & 0x1000000) == 0x1000000 || Info->CtlCode == 8 || Info->CtlCode == 16 || Info->CtlCode == 32 || Info->CtlCode == 64)
					{
						if (Info->Sign)
						{
							Destroy(Info);
						}

						Info->SignID = 5;
					}
				}

				break;
			}
			case STATE_PLAYING:
			{
				if (Info->Index == Player.Index)
				{
					if (Info->SignID == 0)
					{
						if (Info->Sign)
						{
							Destroy(Info);
						}

						if (Player.Sign != 0)
						{
							Info->SignID = Player.Sign;
						}
						else if ((Info->ViewSkillState & 0x1000000) == 0x1000000 || Info->CtlCode == 8 || Info->CtlCode == 16 || Info->CtlCode == 32 || Info->CtlCode == 64)
						{
							Info->SignID = 5;
						}
					}
					else
					{
						if (Info->SignID != 5 && (Info->ViewSkillState & 0x1000000) == 0x1000000 || Info->CtlCode == 8 || Info->CtlCode == 16 || Info->CtlCode == 32 || Info->CtlCode == 64)
						{
							if (Info->Sign)
							{
								Destroy(Info);
							}

							Info->SignID = 5;
						}
						else
						{
							if (!(Info->SignID == 5 && (Info->ViewSkillState & 0x1000000) == 0x1000000 || Info->CtlCode == 8 || Info->CtlCode == 16 || Info->CtlCode == 32 || Info->CtlCode == 64))
							{
								if (Info->SignID != Player.Sign)
								{
									if (Info->Sign)
									{
										Destroy(Info);
									}

									Info->SignID = Player.Sign;
								}
							}
						}
					}
				}
				else
				{
					auto Player = Viewport.GetCustom(Info->Index);

					if (Player)
					{
						if (Info->SignID == 0)
						{
							if (Info->Sign)
							{
								Destroy(Info);
							}

							if (Player->Sign != 0)
							{
								Info->SignID = Player->Sign;
							}
							else if ((Info->ViewSkillState & 0x1000000) == 0x1000000 || Info->CtlCode == 8 || Info->CtlCode == 16 || Info->CtlCode == 32 || Info->CtlCode == 64)
							{
								Info->SignID = 5;
							}
						}
						else
						{
							if (Player->Sign != 0)
							{
								if (Info->SignID != Player->Sign)
								{
									if (Info->Sign)
									{
										Destroy(Info);
									}

									Info->SignID = Player->Sign;
								}
							}
							else if (Info->SignID != 5 && (Info->ViewSkillState & 0x1000000) == 0x1000000 || Info->CtlCode == 8 || Info->CtlCode == 16 || Info->CtlCode == 32 || Info->CtlCode == 64)
							{
								if (Info->Sign)
								{
									Destroy(Info);
								}

								Info->SignID = 5;
							}
						}
					}
				}

				break;
			}
		}

		if (!Info->Sign)
		{
			if (Info->SignID >= 5 && Info->SignID < 255)
			{
				if (Info->SignID == 14 || Info->SignID == 15)
				{
				}
				else
				{
					BYTE* Buffer = new BYTE[472];

					if (Buffer)
					{
						if (Info->SignID == 5)
						{
							Info->Sign = pCreateSign(Buffer, 0x128, 20, 1, 70.f, -5.f, 0.0f, 0, 0, 45.f);
						}
						else if (Info->SignID >= 6 && Info->SignID <= 13)
						{
							Info->Sign = pCreateSign2(Buffer, 0x637, (Info->SignID + 1) % 7, 20, 120.f, 0.0f, 0.0f);
						}
						/*else if (Info->SignID == 16)	// AFK Sign
						{
							Info->Sign = pCreateSign(Buffer, 0xFF00, 20, 1, 70.f, -5.f, 0.0f, 0, 0, 45.f);
						}*/
					}
				}
			}
		}

		return Info->Sign;
	}

	return null;
}

void CSign::Destroy(ViewportInfo* Info)
{
	if (Info->Sign)
	{
		delete[] Info->Sign;

		Info->SignID = 0;
		Info->Sign = null;
	}
}

void __declspec(naked) CSign::Glow()
{
	static DWORD Back[2] = {0x5F015C, 0x5F0115};
	static WORD Index;

	__asm
	{
		MOV Index, AX;
		PUSHAD;
	}

	if (false) // Glow
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
		CMP AX, 0x128;
		JNZ NotZero;
		JMP Back[4];

	NotZero:
		JMP Back[0];
	}
}

CSign Sign;