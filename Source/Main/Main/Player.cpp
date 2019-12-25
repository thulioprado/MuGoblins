#include "Library.h"
#include "Player.h"
#include "Protocol.h"

CPlayer::CPlayer() : Index(0), HP(0), MaxHP(0), MP(0), MaxMP(0), BP(0), MaxBP(0), LevelUpPoints(0), Damage(0), X(0), Y(0), Experience(0), NextExperience(0), PreviousNextExperience(0), PercentExperience(0), Resets(0), PrismWeapon{}, PrismArmor{}
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Load()
{
	Memory::Jump(0x4CC640, this->Update);

	//
	// Dano
	//
	Memory::Call(0x4C4375, this->DrawDamage);
	Memory::Call(0x4C4C01, this->DrawDamage); // Miss
	Memory::Call(0x4C4D66, this->DrawDamage);
	Memory::Call(0x4C4DB6, this->DrawDamage);
	Memory::Call(0x4C4E3F, this->DrawDamage);
	Memory::Call(0x4C4E8F, this->DrawDamage);
	Memory::Call(0x4C4EAE, this->DrawDamage);
	Memory::Call(0x4C83F1, this->DrawDamage);
	Memory::Call(0x4C8D46, this->DrawDamage);
	Memory::Call(0x4C8D7B, this->DrawDamage);
	Memory::Call(0x4C9828, this->DrawDamage);
	Memory::Call(0x4C985D, this->DrawDamage);

	//
	// HP
	//
	Memory::Jump(0x5DEA11, this->DrawHP1);
	Memory::Jump(0x599361, this->DrawHP2);
	Memory::Jump(0x5990B5, this->DrawHP3);
	Memory::Jump(0x599250, this->DrawHP4);
	Memory::Jump(0x59A077, this->DrawHP5);
	Memory::Nop<6>(0x599234);
	Memory::Nop<6>(0x59923F);
	Memory::Change<DWORD>(0x59A06E, 0x41100000); // Tamanho	- Life
	Memory::Change<DWORD>(0x59A073, 0x41000000); // Tamanho	- Life
	Memory::Change<DWORD>(0x59A082, 0x42FC0000); // Posição X - Life
	Memory::Change<DWORD>(0x59A07D, 0x43E98000); // Posição Y - Life

	//
	// MP
	//
	Memory::Jump(0x5DEB24, this->DrawMP1);
	Memory::Jump(0x599426, this->DrawMP2);
	Memory::Jump(0x5990D2, this->DrawMP3);
	Memory::Jump(0x59A09C, this->DrawMP4);
	Memory::Jump(0x54E49F, this->CheckMP1);
	Memory::Jump(0x54DC33, this->CheckMP2);
	Memory::Jump(0x556113, this->CheckMP3);
	Memory::Jump(0x5703B1, this->CheckMP4);
	Memory::Jump(0x573723, this->CheckMP5);
	Memory::Jump(0x57A4E1, this->CheckMP6);
	Memory::Jump(0x5834AA, this->CheckMP7);
	Memory::Nop<6>(0x5990BF);
	Memory::Nop<6>(0x5990DE);
	Memory::Change<DWORD>(0x59A093, 0x41100000); // Tamanho	- Mana
	Memory::Change<DWORD>(0x59A098, 0x41000000); // Tamanho	- Mana
	Memory::Change<DWORD>(0x59A0A9, 0x44020000); // Posição X - Mana
	Memory::Change<DWORD>(0x59A0A2, 0x43E98000); // Posição Y - Mana

	//
	// BP
	//
	Memory::Jump(0x5989B5, this->DrawBP1);
	Memory::Jump(0x5987EE, this->DrawBP2);
	Memory::Jump(0x598809, this->DrawBP3);
	Memory::Jump(0x54E3AA, this->CheckBP1);
	Memory::Jump(0x556985, this->CheckBP2);
	Memory::Jump(0x570B2E, this->CheckBP3);
	Memory::Jump(0x573E78, this->CheckBP4);
	Memory::Jump(0x57AC05, this->CheckBP5);
	Memory::Jump(0x583C89, this->CheckBP6);
	Memory::Nop<5>(0x598801);
	Memory::Change<DWORD>(0x598964, 0x41100000); // Tamanho	- BP
	Memory::Change<DWORD>(0x598969, 0x41000000); // Tamanho	- BP
	Memory::Change<DWORD>(0x598974, 0x440D0000); // Posição X - BP
	Memory::Change<DWORD>(0x59896F, 0x43E98000); // Posição Y - BP

	//
	// Pontos
	//
	Memory::Jump(0x5DEE89, this->DrawLevelUpPoints);

	//
	// Resets
	//
	Memory::Jump(0x5DD2B2, this->DrawResets);

	//
	// Experiência
	//
	Memory::Call(0x59B897, this->DrawExperience);
	Memory::Call(0x59B7E3, this->DrawExperienceBar);
	Memory::Nop<5>(0x59B820);						// Remove a numeração da barra de experiência

	//
	// Coordenadas
	//
	Memory::Jump(0x599EAE, this->GetX);
	Memory::Jump(0x599F81, this->GetY);
}

void CPlayer::Execute(int Index, std::function<void(PlayerObject*)> Callback)
{
	this->Callback = Callback;

	PMSG_LEVEL_UP_POINT_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x06, sizeof(pMsg));
	pMsg.result = 16;

	pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

void __declspec(naked) CPlayer::Update()
{
	static DWORD Back = 0x4CC6BD;

	__asm
	{
		PUSHAD;
	}

	Player.Callback((PlayerObject*)(PLAYER_OBJECT_BASE));

	__asm
	{
		POPAD;
		MOV EAX, DWORD PTR DS : [0x7666A78];
		JMP Back;
	}
}

void CPlayer::DrawDamage(int Unk1, int Damage, int Unk2, int Unk3, char Unk4)
{
	if (Player.Damage > 0)
	{
		Damage = Player.Damage;
	}

	pDrawDamage(Unk1, Damage, Unk2, Unk3, Unk4);
}

void __declspec(naked) CPlayer::DrawHP1()
{
	static DWORD Back = 0x5DEA19;

	__asm
	{
		MOV ECX, Player.MaxHP;
		MOV EDX, Player.HP;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawHP2()
{
	static DWORD Back = 0x599367;

	__asm
	{
		MOV ECX, Player.MaxHP;
		MOV ESI, Player.HP;
		LEA EDX, [EBP - 0x94];
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawHP3()
{
	static DWORD Back = 0x5990C5;

	__asm
	{
		MOV EDX, Player.MaxHP;
		MOV EAX, Player.HP;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawHP4()
{
	static DWORD Back = 0x599255;

	__asm
	{
		XOR EDI, EDI;
		CMP EAX, EDI;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawHP5()
{
	static DWORD Back = 0x59A07C;

	__asm
	{
		MOV EAX, Player.HP;
		PUSH EAX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawMP1()
{
	static DWORD Back = 0x5DEB2C;

	__asm
	{
		MOV EDX, Player.MaxMP;
		MOV ECX, Player.MP;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawMP2()
{
	static DWORD Back = 0x59942C;

	__asm
	{
		MOV EDX, Player.MaxMP;
		MOV EBX, Player.MP;
		LEA EAX, [EBP - 0x94];
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawMP3()
{
	static DWORD Back = 0x5990E4;

	__asm
	{
		MOV ECX, Player.MaxMP;
		MOV EAX, Player.MP;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawMP4()
{
	static DWORD Back = 0x59A0A1;

	__asm
	{
		MOV EDX, Player.MP;
		PUSH EDX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckMP1()
{
	static DWORD Back = 0x54E4A6;

	__asm
	{
		MOV EDX, Player.MP;
		MOV EAX, DWORD PTR SS : [EBP - 0x44] ;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckMP2()
{
	static DWORD Back = 0x54DC39;

	__asm
	{
		MOV ECX, Player.MP;
		CMP ECX, EDX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckMP3()
{
	static DWORD Back = 0x556119;

	__asm
	{
		MOV ECX, Player.MP;
		CMP ECX, EDX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckMP4()
{
	static DWORD Back = 0x5703B7;

	__asm
	{
		MOV EDX, Player.MP;
		CMP ECX, EDX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckMP5()
{
	static DWORD Back = 0x573729;

	__asm
	{
		MOV EDX, Player.MP;
		CMP ECX, EDX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckMP6()
{
	static DWORD Back = 0x57A4E7;

	__asm
	{
		MOV EDX, Player.MP;
		CMP ECX, EDX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckMP7()
{
	static DWORD Back = 0x5834B0;

	__asm
	{
		MOV ECX, Player.MP;
		CMP EDX, ECX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawBP1()
{
	static DWORD Back = 0x5989BA;

	__asm
	{
		MOV EDI, Player.MaxBP;
		MOV ESI, Player.BP;
		PUSH EDI;
		PUSH ESI;
		LEA ECX, DWORD PTR SS : [EBP - 0x7C] ;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawBP2()
{
	static DWORD Back = 0x5987F6;

	__asm
	{
		MOV EAX, Player.MaxBP;
		CMP EAX, 1;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawBP3()
{
	static DWORD Back = 0x59880F;

	__asm
	{
		MOV EAX, Player.BP;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckBP1()
{
	static DWORD Back = 0x54E3B1;

	__asm
	{
		MOV EDX, Player.BP;
		MOV EAX, DWORD PTR SS : [EBP - 0x3C] ;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckBP2()
{
	static DWORD Back = 0x55698F;

	__asm
	{
		MOV ECX, Player.BP;
		MOV EAX, DWORD PTR SS : [EBP - 0x2D0] ;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckBP3()
{
	static DWORD Back = 0x570B35;

	__asm
	{
		MOV EDX, Player.BP;
		MOV EAX, DWORD PTR SS : [EBP - 0x7C] ;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckBP4()
{
	static DWORD Back = 0x573E7F;

	__asm
	{
		MOV ECX, Player.BP;
		MOV EAX, DWORD PTR SS : [EBP - 0x78] ;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckBP5()
{
	static DWORD Back = 0x57AC0C;

	__asm
	{
		MOV ECX, Player.BP;
		MOV EAX, DWORD PTR SS : [EBP - 0x54] ;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::CheckBP6()
{
	static DWORD Back = 0x583C90;

	__asm
	{
		MOV ECX, Player.BP;
		MOV EAX, DWORD PTR SS : [EBP - 0x54] ;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawLevelUpPoints()
{
	static DWORD Back = 0x5DEE8E;

	__asm
	{
		MOV EAX, Player.LevelUpPoints;
		PUSH EAX;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::DrawResets()
{
	static DWORD Back = 0x5DD2BF;
	static const char* Resets = "Resets: %u";

	__asm
	{
		MOV EDX, 0;
		MOV EAX, Player.Resets;
		PUSH EDX;
		PUSH EAX;
		PUSH Resets;
		JMP Back;
	}
}

void CPlayer::DrawExperience(char* Buffer, char* Format, DWORD Experience, DWORD NextExperience)
{
	sprintf_s(Buffer, 80, Format, Player.Experience, Player.NextExperience);
}

void CPlayer::DrawExperienceBar(float X, float Y, float Width, float Height)
{
	pEnableAlpha(1);
	glColor4f(0.92f, 0.80f, 0.34f, 1.f);
	pDrawForm(X, Y, (float)(Player.PercentExperience * 2), Height);
}

void __declspec(naked) CPlayer::GetX()
{
	static DWORD Back = 0x599EB3;

	__asm
	{
		PUSH 0x41000000;
		MOV Player.X, EDI;
		JMP Back;
	}
}

void __declspec(naked) CPlayer::GetY()
{
	static DWORD Back = 0x599F86;

	__asm
	{
		PUSH 0x41000000;
		MOV Player.Y, EDI;
		JMP Back;
	}
}

CPlayer Player;