#include "Library.h"
#include "Patch.h"

BYTE SecondWeaponFixVal = 0;
char ItemDescription[60][100];
DWORD ItemDescriptionColor[60];
DWORD ItemDescriptionType[60];

void __declspec(naked) CPU()
{
	static DWORD Back = 0x628F9F;

	__asm
	{
		PUSH 1;
		CALL DWORD PTR DS : [0x6A7148];
		CALL DWORD PTR DS : [0x6A7144];
		JMP Back;
	}
}

void __declspec(naked) Summon()
{
	static DWORD Back = 0x59AEB1;
	static DWORD Function = 0x5AA760;

	__asm
	{
		PUSHAD;
	}

	glColor3f(1.0f, 1.0f, 1.0f);

	__asm
	{
		POPAD;
		CALL Function;
		JMP Back;
	}
}

void __declspec(naked) WeaponViewCheck()
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
		CMP WORD PTR DS : [ESI + 0x2A8] , pGetItemModel(7, 0);
		JGE NoDisplay;
		CMP WORD PTR DS : [ESI + 0x2A8] , pGetItemModel(4, 15);
		JE Display;
		JMP End;

	LeftHand:
		CMP WORD PTR DS : [ESI + 0x2C8] , pGetItemModel(7, 0);
		JGE NoDisplay;
		CMP WORD PTR DS : [ESI + 0x2C8] , pGetItemModel(4, 7);
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

void __declspec(naked) SecondWeaponViewCheck()
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

void __declspec(naked) SecondWeaponViewCheckReplacedCode()
{
	__asm
	{
		JMP SecondWeaponViewCheck;
	}
}

void __declspec(naked) SecondWeaponViewFix()
{
	static DWORD Back = 0x5002ED;
	static DWORD Function[2] = {0x5EAE50, 0x5EAFF0};
	static BYTE IsShield = 0;

	__asm
	{
		PUSHAD;
		MOV IsShield, 0;
		CMP EDI, pGetItemModel(6, 0);
		JL FlipCheck;
		CMP EDI, pGetItemModel(7, 0);
		JGE FlipCheck;
		MOV IsShield, 1;
		CMP EDI, pGetItemModel(6, 16);
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
		CMP EDI, pGetItemModel(6, 15);
		JE IsShield1;
		CMP EDI, pGetItemModel(6, 14);
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
		CMP EDI, pGetItemModel(6, 6);
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

bool Patch()
{
	if (!Memory::Writable(0x401000, 0x300000) ||
		!Memory::Writable(0x7D171A0, sizeof(float)))
	{
		return false;
	}

	//
	// Crack
	//
	Memory::Change<BYTE>(0x4B4305, 0xEB);
	Memory::Change<BYTE>(0x4B47FE, 0x75);
	Memory::Change<BYTE>(0x4B4856, 0xE9);
	Memory::Change<BYTE>(0x4B4857, 0xA6);
	Memory::Change<BYTE>(0x4B4858, 0x00);
	Memory::Change<BYTE>(0x4B485B, 0x90);
	Memory::Change<BYTE>(0x4BEA9F, 0xEB);

	//
	// CRC32
	//
	Memory::Change<BYTE>(0x40ABE3, 0xEB);
	Memory::Change<BYTE>(0x40ADD6, 0xEB);
	Memory::Change<BYTE>(0x53E994, 0xEB);
	Memory::Change<BYTE>(0x53ECD4, 0xEB);
	Memory::Change<BYTE>(0x53F8B3, 0xEB);
	Memory::Change<BYTE>(0x540593, 0xEB);
	Memory::Change<BYTE>(0x607090, 0xEB);
	Memory::Change<BYTE>(0x607095, 0xEB);
	Memory::Change<BYTE>(0x607141, 0xEB);
	Memory::Change<BYTE>(0x607146, 0xEB);
	Memory::Change<BYTE>(0x6071B3, 0xEB);
	Memory::Change<BYTE>(0x6071B8, 0xEB);
	Memory::Change<BYTE>(0x607090, 0xEB);
	Memory::Change<BYTE>(0x607095, 0xEB);
	Memory::Change<BYTE>(0x607141, 0xEB);
	Memory::Change<BYTE>(0x607146, 0xEB);
	Memory::Change<BYTE>(0x6071B3, 0xEB);
	Memory::Change<BYTE>(0x6071B8, 0xEB);

	//
	// Conexão
	//
	Memory::Change<WORD>(0x6C41BC, 3000);							// ConnectServer Porta
	Memory::Change<const char*>(0x6B6694, "127.0.0.1");				// ConnectServer IP
	Memory::Change<const char*>(0x6B7248, "2274:");					// Version
	Memory::Change<const char*>(0x6B7250, "TbYehR2hFUPBKgZj");		// Serial
	Memory::Change<BYTE>(0x4E0F1B, 0xEB);							// Liberar conexão com ip local

	//
	// Screenshot Path
	//
	CreateDirectory(".\\ScreenShots", null);

	static char screen_path[] = "ScreenShots\\[%02d-%02d][%02d-%02d][%04d].jpg";
	Memory::Change<DWORD>(0x628B89, DWORD(&screen_path));

	//
	// Mensagens na seleção de personagens
	//
	Memory::Change<DWORD>(0x62590A, DWORD(&""));					// Frase 1
	Memory::Change<DWORD>(0x625918, DWORD(&""));					// Frase 1
	Memory::Change<DWORD>(0x62593C, DWORD(&""));					// Frase 1
	Memory::Change<DWORD>(0x625957, DWORD(&""));					// Frase 2
	Memory::Change<DWORD>(0x625964, DWORD(&""));					// Frase 2
	Memory::Change<DWORD>(0x625988, DWORD(&""));					// Frase 2
	Memory::Change<DWORD>(0x6259A3, DWORD(&""));					// Frase 3
	Memory::Change<DWORD>(0x6259B0, DWORD(&""));					// Frase 3
	Memory::Change<DWORD>(0x6259D4, DWORD(&""));					// Frase 3

	//
	// Centralizar lista de servidores
	//
	Memory::Change<DWORD>(0x61F3DA, 0xFF);
	Memory::Change<DWORD>(0x61F3E0, 0xFF);
	Memory::Change<DWORD>(0x61F431, 0x120);
	Memory::Change<BYTE>(0x61F463, 0x4C);
	Memory::Change<DWORD>(0x61F7E0, 0xFC);
	Memory::Change<DWORD>(0x61F814, 0x16D);
	Memory::Change<DWORD>(0x61F81F, 0x1F3);
	Memory::Change<DWORD>(0x61FE1D, 0xFF);
	Memory::Change<DWORD>(0x61FE24, 0xFF);
	Memory::Change<DWORD>(0x61FE8F, 0x118);
	Memory::Change<DWORD>(0x61FE9E, 0x127);
	Memory::Change<DWORD>(0x61FEC7, 0x127);
	Memory::Change<DWORD>(0x61FE8F, 0x11D);
	Memory::Change<DWORD>(0x61FF12, 0x3F700000);
	Memory::Change<DWORD>(0x6201B7, 0x3F700000);
	Memory::Change<DWORD>(0x6201D6, 0x43B68000);
	Memory::Change<DWORD>(0x620251, 0x1B0);
	Memory::Change<float>(0x6A9788, 383.0f);

	//
	// Qualidade das fontes
	//
	Memory::Change<DWORD>(0x6B6654, DEFAULT_CHARSET);
	Memory::Change<BYTE>(0x4B4DEE, ANTIALIASED_QUALITY);
	Memory::Change<BYTE>(0x4B4E2E, ANTIALIASED_QUALITY);
	Memory::Change<BYTE>(0x4B4E6E, ANTIALIASED_QUALITY);
	Memory::Change<BYTE>(0x62F7B8, ANTIALIASED_QUALITY);

	//
	// Correção de lag utilizando ctrl
	//
	Memory::Change<BYTE>(0x4519F8, 0x02);

	//
	// Correção de recuo
	//
	Memory::Change<BYTE>(0x4ECA80, 0xC3);

	//
	// Carregar NPCs do Castle Siege em qualquer mapa
	//
	Memory::Change<BYTE>(0x42B2D0, 0xEB);

	//
	// Correção de personagem deslizando
	//
	Memory::Change<BYTE>(0x550B25, 0xE9);
	Memory::Change<BYTE>(0x550B26, 0xEE);
	Memory::Change<BYTE>(0x550B27, 0x00);
	Memory::Change<BYTE>(0x550B28, 0x00);
	Memory::Change<BYTE>(0x550B29, 0x00);
	Memory::Nop(0x550B2A);
	Memory::Change<BYTE>(0x58D019, 0xE9);
	Memory::Change<BYTE>(0x58D01A, 0xD9);
	Memory::Change<BYTE>(0x58D01B, 0x00);
	Memory::Change<BYTE>(0x58D01C, 0x00);
	Memory::Change<BYTE>(0x58D01D, 0x00);
	Memory::Nop(0x58D01E);

	//
	// Correção de cursor
	//
	Memory::Nop(0x4CBC47);
	Memory::Nop<7>(0x4CBC65);
	Memory::Nop(0x4CE934);
	Memory::Nop(0x4CE958);
	Memory::Nop<6>(0x4CE96B);

	//
	// Correção de crash [audio]
	//
	if (waveOutGetNumDevs() < 1)
	{
		Memory::Change<BYTE>(0x5750324, 0x00);
	}

	//
	// Correção no brilho de itens
	//
	Memory::Change<BYTE>(0x5F9CB9, 0x83);
	Memory::Change<BYTE>(0x5F9CBA, 0xFB);
	Memory::Change<BYTE>(0x5F9CBB, 0x10);
	Memory::Nop<5>(0x44AE85);
	Memory::Nop<5>(0x44AEAE);
	Memory::Nop<5>(0x44B81D);
	Memory::Nop<5>(0x44B859);
	Memory::Nop<5>(0x44B8AC);
	Memory::Nop<5>(0x44B8FF);
	Memory::Nop<5>(0x4A784A);

	//
	// Correção no alvo dos monstros
	//
	Memory::Change<std::vector<BYTE>>(0x4C4FF8, {0x83, 0xC4, 0x04, 0xC6, 0x86, 0x71, 0x03, 0x00, 0x00, 0x01, 0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0xC7, 0x86, 0x94, 0x03, 0x00, 0x00, 0xFF, 0xFF});
	Memory::Nop<4>(0x4C5015);

	//
	// Correção na animação de ataque
	//
	Memory::Nop(0x42FBAF);
	Memory::Nop<14>(0x42FBB6);

	//
	// Correção de personagem usar skill de arma sem conter uma
	//
	Memory::Change<WORD>(0x50A5D7, 0xFFF);
	Memory::Change<WORD>(0x50A619, 0xFFF);
	Memory::Change<WORD>(0x5AAFF8, 0xFFF);
	Memory::Change<WORD>(0x5AB5C9, 0xFFF);

	//
	// Remove mensagem de item bloqueado para GMs
	//
	Memory::Nop<167>(0x62582B);

	//
	// Remove mensagem de entrada no mapa
	//
	Memory::Nop<5>(0x4C18F7);
	Memory::Nop<49>(0x4C18FF);
	Memory::Nop<54>(0x4BEAFD);

	//
	// Remove nome dos monstros no topo da tela
	//
	Memory::Nop<74>(0x5AAB4B);

	//
	// Permite exclusão de personagem em qualquer nível
	//
	Memory::Change<BYTE>(0x623492, 0xEB);

	//
	// Otimização no uso de CPU
	//
	Memory::Jump(0x628F99, CPU);

	//
	// Altera posição do nome do summon (elf)
	//
	Memory::Nop<6>(0x59AECD);

	//
	// Correção de cor do nome do summon (elf) ao equipar pet
	//
	Memory::Jump(0x59AEAC, Summon);

	//
	// Correção de animação de skills
	//
	//Memory::Change<BYTE>(0x4F0B83, 2);

	//
	// Remove trava do jogo
	//
	Memory::Nop<7>(0x610520);
	Memory::Nop<7>(0x610CA8);
	Memory::Nop<7>(0x611352);
	Memory::Nop<7>(0x6378D2);

	//
	// Exibição de duas armas equipadas nas costas
	//
	Memory::Jump(0x5048CA, WeaponViewCheck);
	Memory::Jump(0x504A4F, SecondWeaponViewCheckReplacedCode);
	Memory::Hook(0x5049BC, SecondWeaponViewCheck);
	Memory::Hook(0x5049C5, SecondWeaponViewCheck);
	Memory::Jump(0x5002E8, SecondWeaponViewFix);

	//
	// Fix Valley of Loren on some PC's
	//
	//Memory::Nop<3>(0x691217);

	//
	// Add new maps
	//
	//Memory::Hook<true>(0x64C6DB, this->LoadMapName);

	//
	// Aumento de limite de descrições nos itens
	//
	memset(ItemDescription, 0, sizeof(ItemDescription));
	memset(ItemDescriptionColor, 0, sizeof(ItemDescriptionColor));
	memset(ItemDescriptionType, 0, sizeof(ItemDescriptionType));

	Memory::Change<DWORD>(0x40C254, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40C2A4, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CD4F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CE23, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CE69, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CEAD, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CEF3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CF3B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CF81, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CFDC, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D18A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D1CD, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D334, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D350, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D36B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D4DA, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D500, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D527, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D543, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D570, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D729, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40D745, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4200F9, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x420182, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x420F2C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x42101A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4214A3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421525, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421571, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4215A1, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4215BD, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4215F6, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421621, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421668, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x42168E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4216B7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4216F0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421967, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421B18, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421CFF, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421F29, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421F55, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x421F82, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x42201A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x422045, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x42208B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x432D39, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x432D5C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x432D98, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x43310A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x43313E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4331B8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4331D4, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x433211, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x433230, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x492555, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x49258E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x4925C2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5960A2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59625F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59635E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x599689, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5996C2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5996F6, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59E661, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59E6FB, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59E8C8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59EB17, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59EE27, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59EE82, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59EEBB, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59EF11, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59EFA4, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59F04A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59F10D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59F20C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59F385, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FD77, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FDA6, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FDE7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FE0F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FE76, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FEC0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FEF3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FF34, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FF5C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x59FFC3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A01B9, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A01E3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A0228, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A0260, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A0286, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A02AC, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A0417, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A043D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A050A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A0580, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A05BC, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A21E8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A222A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A252D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2620, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A27D0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2855, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2D4C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2D72, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2DC2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2DE8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2E5D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2E81, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2EC0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2EEF, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2F46, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2F76, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2FC2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A2FE2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A300C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A302C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3069, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3089, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3157, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A31AD, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A31CD, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A31F1, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A323D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A325D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A32A1, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A32CB, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A32EB, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A332D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3365, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A33A7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A33C7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A36DB, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A39F2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3B17, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3B53, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3B8A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3BBE, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3BF0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3C2D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3C75, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3CAD, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3CDA, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3D34, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3D85, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3DD3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3E22, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3E57, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3E8C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3EC0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3EFA, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3F35, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3F9D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A3FD1, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4007, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4060, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4094, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A40CA, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4105, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A413E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4174, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A41AD, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A41E3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4217, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A424B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A429E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A42D2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A430B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A433F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A437B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A43C4, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A43F8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4431, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4465, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4493, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A44C7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4500, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4534, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A455D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A459C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A45D0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A463B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4B40, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4BF0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4C5B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4C9F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4CF5, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4D37, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4D7E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4DB2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4DF2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4E26, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4E3B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4E6F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4EAC, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4F26, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4F5A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4F9F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A4FD3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5015, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5050, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A508B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A50D0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A510B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5146, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5181, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A51BE, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A51F4, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A522F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5281, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A52B6, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A52EA, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5331, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5378, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A53AE, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A53E2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5407, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A544C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5477, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A54C9, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A54F9, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5518, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5537, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A557E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A55AE, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A55CD, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A55EC, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A561D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5670, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A56C0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A56E6, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5719, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A574D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A579D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A57D6, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A580A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A583E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5877, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A58CC, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A58F1, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5906, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5E28, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5E7F, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5F0D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5F40, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A5F8B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A60F8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A613A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6160, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6185, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A61DB, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6210, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A625C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A62C4, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A65C7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6802, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6B05, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6CD6, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6D45, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6DC8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6E34, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6EA0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6F0C, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6F78, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A6FE4, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7075, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A70A8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A70F8, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A712B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7187, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A71C0, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A720A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7243, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7283, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A730A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7350, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A736D, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7387, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A73DE, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A742E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7472, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7661, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A774B, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A77CE, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A77F7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7836, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7B29, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7DD2, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7DF7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7E19, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7E54, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7EB3, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7EED, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A7F1A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A877E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A89EC, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8A1A, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8A4E, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8A82, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8AB4, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8B04, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8B66, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8BC7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8EF9, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A8F39, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A90D6, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A9117, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A9375, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A93A7, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x5A95D1, (DWORD)(&ItemDescription));
	Memory::Change<DWORD>(0x40CD8A, (DWORD)(&ItemDescription[1]));
	Memory::Change<DWORD>(0x40D1F2, (DWORD)(&ItemDescription[1]));
	Memory::Change<DWORD>(0x420115, (DWORD)(&ItemDescription[1]));
	Memory::Change<DWORD>(0x420F53, (DWORD)(&ItemDescription[1]));
	Memory::Change<DWORD>(0x42106E, (DWORD)(&ItemDescription[1]));
	Memory::Change<DWORD>(0x4214C1, (DWORD)(&ItemDescription[1]));
	Memory::Change<DWORD>(0x5960BB, (DWORD)(&ItemDescription[1]));
	Memory::Change<DWORD>(0x5A8797, (DWORD)(&ItemDescription[1]));
	Memory::Change<DWORD>(0x40CDA8, (DWORD)(&ItemDescription[2]));
	Memory::Change<DWORD>(0x40D20E, (DWORD)(&ItemDescription[2]));
	Memory::Change<DWORD>(0x420127, (DWORD)(&ItemDescription[2]));
	Memory::Change<DWORD>(0x42143B, (DWORD)(&ItemDescription[2]));
	Memory::Change<DWORD>(0x5960D9, (DWORD)(&ItemDescription[2]));
	Memory::Change<DWORD>(0x5A87B8, (DWORD)(&ItemDescription[2]));
	Memory::Change<DWORD>(0x40D220, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x420160, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x42145D, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x59610D, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x5961CF, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x5961DB, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x5961E7, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x5961F3, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x5961FF, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x59620B, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x5A8850, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x5A892D, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x5A895D, (DWORD)(&ItemDescription[3]));
	Memory::Change<DWORD>(0x40D249, (DWORD)(&ItemDescription[4]));
	Memory::Change<DWORD>(0x596222, (DWORD)(&ItemDescription[4]));
	Memory::Change<DWORD>(0x5A8871, (DWORD)(&ItemDescription[4]));
	Memory::Change<DWORD>(0x596234, (DWORD)(&ItemDescription[5]));
	Memory::Change<DWORD>(0x5A88A4, (DWORD)(&ItemDescription[5]));
	Memory::Change<DWORD>(0x40C266, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40C2B4, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40CD7A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40CE38, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40CEC2, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40CF50, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40CF96, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40D005, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40D018, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40D183, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40D510, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40D556, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x4200DC, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x420F3F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421030, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421044, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x42105F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x4214A9, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x4214DB, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421593, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421943, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421977, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x42198A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421CE9, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421D0F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421D18, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421F47, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x421FBB, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x42202F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x422052, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x432D6C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x432DA8, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x433100, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x43311A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x433174, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x433181, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x4331EA, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x433240, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x49256F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x4925A3, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x4925DD, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59624A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5996A3, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5996D7, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x599711, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59E70B, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59EB65, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59EE9E, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59EEDA, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59EF66, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59EF76, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59F012, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59F022, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59F0A6, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59F0B6, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59F0FB, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59F223, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59F230, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59F39E, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59FDCA, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59FE43, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59FF17, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x59FF90, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A023D, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A042F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A0480, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A04E3, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A0558, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A21C2, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A263D, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A27ED, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A2807, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A2821, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A282E, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A2D34, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A2DA6, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3C15, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3C63, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3C9C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3D53, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3DA4, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3DF2, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3E37, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3E6C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3EA1, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3F14, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3F4D, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3FB2, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A3FE6, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A401C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4075, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A40A9, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A40DF, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A411F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4153, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4189, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A41C2, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A41F8, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A422C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4263, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A42B3, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A42EC, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4320, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4354, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4390, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A43D9, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4412, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4446, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A447A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A44A8, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A44E1, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4515, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4549, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4572, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A45B1, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A45E5, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4BA6, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4BBB, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4C1A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4C2C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4C70, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4CBB, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4CC8, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4D0A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4D4C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4D93, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4DC7, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4E07, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4E50, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4E84, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4EC1, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4F3B, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4F6F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4FB4, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A4FE8, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A502A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5065, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A50A0, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A50E5, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5120, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A515B, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5196, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A51D3, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5209, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5244, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5296, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A52CB, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A52FF, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A534A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A538D, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A542A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A54A0, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5555, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A560C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A564B, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A565E, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5692, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A56AE, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A572E, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5762, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A57B7, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A57EB, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A581F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5858, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A588C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A591B, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5E0C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5E45, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5EAB, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5ED4, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5F58, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A5F77, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A619A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A61F0, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6225, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6271, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6599, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A65DC, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A65E7, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6ADE, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6B1A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6B3D, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6D22, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6D5A, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6D7F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6E11, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6E49, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6E57, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6EE9, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6F21, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6F2F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6FC1, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A6FF9, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A7007, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A70C2, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A7145, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A71DA, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A725D, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A7322, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A739C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A73F3, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A780C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A7E3C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A7ECA, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8A2C, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8A60, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8A95, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8AC4, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8B14, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8B76, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8BD7, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8F14, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A8F4F, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A90F6, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A9127, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x5A9385, (DWORD)(&ItemDescriptionColor));
	Memory::Change<DWORD>(0x40CD98, (DWORD)(&ItemDescriptionColor[1]));
	Memory::Change<DWORD>(0x40D200, (DWORD)(&ItemDescriptionColor[1]));
	Memory::Change<DWORD>(0x420107, (DWORD)(&ItemDescriptionColor[1]));
	Memory::Change<DWORD>(0x5960C9, (DWORD)(&ItemDescriptionColor[1]));
	Memory::Change<DWORD>(0x5A87A9, (DWORD)(&ItemDescriptionColor[1]));
	Memory::Change<DWORD>(0x40CDB3, (DWORD)(&ItemDescriptionColor[2]));
	Memory::Change<DWORD>(0x421441, (DWORD)(&ItemDescriptionColor[2]));
	Memory::Change<DWORD>(0x59EE03, (DWORD)(&ItemDescriptionColor[2]));
	Memory::Change<DWORD>(0x59EDFC, (DWORD)(&ItemDescriptionColor[3]));
	Memory::Change<DWORD>(0x5A8860, (DWORD)(&ItemDescriptionColor[3]));
	Memory::Change<DWORD>(0x5A893B, (DWORD)(&ItemDescriptionColor[3]));
	Memory::Change<DWORD>(0x5A896C, (DWORD)(&ItemDescriptionColor[3]));
	Memory::Change<DWORD>(0x40D246, (DWORD)(&ItemDescriptionColor[4]));
	Memory::Change<DWORD>(0x5A8888, (DWORD)(&ItemDescriptionColor[4]));
	Memory::Change<DWORD>(0x5A88B2, (DWORD)(&ItemDescriptionColor[5]));
	Memory::Change<DWORD>(0x40CD80, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x40CE42, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x40CECC, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x40CF57, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x40CFA1, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x40D02B, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x40D55D, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x4200E1, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x420F49, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x421074, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x4214AF, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x4214E2, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x42194A, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x42197E, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x421991, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x421CE2, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x421D2A, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x421FC6, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x42205E, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x433121, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x43318F, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x492576, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x4925AA, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x4925E4, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x596251, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5996AA, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5996DE, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x599718, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59E666, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59E7EF, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59E8CD, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59EB1E, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59EEE0, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59EFB9, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59F073, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59F122, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59F243, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59FDD1, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59FE4F, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59FF1E, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x59FF9C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A0248, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A0436, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A045C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A2839, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A2D3B, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A2DB1, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3C0A, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3C8A, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3CC2, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3CF7, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3D5A, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3DAB, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3DF9, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3E3E, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3E73, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3EA8, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3EDB, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3F1B, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3F54, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3FB9, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A3FED, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4023, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A407C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A40B0, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A40E6, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4126, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A415A, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4190, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A41C9, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A41FF, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4233, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A426A, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A42BA, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A42F3, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4327, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A43E0, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4419, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A444D, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A44AF, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A44E8, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A451C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A457D, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A45B8, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A45EC, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4BB1, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4BC2, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4C33, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4C77, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4CCF, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4D11, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4D53, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4D9A, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4DCE, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4E0E, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4E57, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4E8F, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4ECC, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4F42, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4F76, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4FBB, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A4FEF, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5031, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A506C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A50A7, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A50EC, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5127, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5162, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A519D, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A51DA, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5210, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A524B, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A529D, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A52D2, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5306, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5351, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5394, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5735, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5769, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A57BE, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A57F2, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5826, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A585F, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5893, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5926, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5E4C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A5EDF, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A61A1, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A61F7, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A622C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6278, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A65A0, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A65EE, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6AD7, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6B44, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6D29, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6D8C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6E18, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6E64, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6EF0, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6F3C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A6FC8, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A7014, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A70C9, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A714C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A71E1, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A7264, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A732D, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A73FE, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A7813, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A7E31, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A7ED1, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8A33, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8A67, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8A9C, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8ACB, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8B1F, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8B81, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8BE2, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8F1B, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A8F5B, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A90FD, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A912E, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x5A9390, (DWORD)(&ItemDescriptionType));
	Memory::Change<DWORD>(0x40CD9E, (DWORD)(&ItemDescriptionType[1]));
	Memory::Change<DWORD>(0x5960CF, (DWORD)(&ItemDescriptionType[1]));
	Memory::Change<DWORD>(0x5A87AE, (DWORD)(&ItemDescriptionType[1]));
	Memory::Change<DWORD>(0x40CDB9, (DWORD)(&ItemDescriptionType[2]));
	Memory::Change<DWORD>(0x421447, (DWORD)(&ItemDescriptionType[2]));
	Memory::Change<DWORD>(0x5A8866, (DWORD)(&ItemDescriptionType[3]));
	Memory::Change<DWORD>(0x5A8945, (DWORD)(&ItemDescriptionType[3]));
	Memory::Change<DWORD>(0x5A8971, (DWORD)(&ItemDescriptionType[3]));
	Memory::Change<DWORD>(0x40D23F, (DWORD)(&ItemDescriptionType[4]));
	Memory::Change<DWORD>(0x5A888E, (DWORD)(&ItemDescriptionType[4]));
	Memory::Change<DWORD>(0x5A88B8, (DWORD)(&ItemDescriptionType[5]));
	//Memory::Change<DWORD>(0x4BFFCB, (DWORD)(&ItemDescriptionType[15]));
	//Memory::Change<DWORD>(0x5AAF50, (DWORD)(&ItemDescriptionType[15]));
	//Memory::Change<DWORD>(0x647E73, (DWORD)(&ItemDescriptionType[15]));
	//Memory::Change<DWORD>(0x648B4B, (DWORD)(&ItemDescriptionType[15]));
	//Memory::Change<DWORD>(0x5D3941, (DWORD)(&ItemDescriptionType[17]));

	//
	// Drop de Ring of Warrior em qualquer nível
	//
	Memory::Change<BYTE>(0x5CE192, 0xE9);
	Memory::Change<BYTE>(0x5CE193, 0xF7);
	Memory::Change<BYTE>(0x5CE194, 0x00);
	Memory::Change<BYTE>(0x5CE195, 0x00);
	Memory::Change<BYTE>(0x5CE196, 0x00);
	Memory::Change<BYTE>(0x5CE197, 0x90);
	Memory::Change<BYTE>(0x5CE292, 0xEB);

	return true;
}