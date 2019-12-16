#include "Library.h"
#include "Patch.h"

void __declspec(naked) cpu()
{
	static DWORD back = 0x628F9F;

	__asm
	{
		push 1;
		call DWORD ptr ds : [0x6A7148] ;
		call DWORD ptr ds : [0x6A7144] ;
		jmp back;
	}
}

void __declspec(naked) summon()
{
	static DWORD back = 0x59AEB1;
	static DWORD function = 0x5AA760;

	__asm
	{
		pushad;
	}

	glColor3f(1.0f, 1.0f, 1.0f);

	__asm
	{
		popad;
		call function;
		jmp back;
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
	Memory::Change<const char*>(0x6B6694, "127.0.0.1");			// ConnectServer IP
	Memory::Change<const char*>(0x6B7248, "2274:");				// Version
	Memory::Change<const char*>(0x6B7250, "TbYehR2hFUPBKgZj");		// Serial
	Memory::Change<BYTE>(0x4E0F1B, 0xEB);							// Liberar conexão com ip local

	//
	// Screenshot Path
	//
	CreateDirectory(".\\ScreenShots", null);

	static const char* screen_path = "ScreenShots\\[%02d-%02d][%02d-%02d][%04d].jpg";
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
	Memory::Jump(0x628F99, cpu);

	//
	// Altera posição do nome do summon (elf)
	//
	Memory::Nop<6>(0x59AECD);

	//
	// Correção de cor do nome do summon (elf) ao equipar pet
	//
	Memory::Jump(0x59AEAC, summon);

	//
	// Correção de animação de skills
	//
	Memory::Change<BYTE>(0x4F0B83, 2);

	//
	// Remove trava do jogo
	//
	Memory::Nop<7>(0x610520);
	Memory::Nop<7>(0x610CA8);
	Memory::Nop<7>(0x611352);
	Memory::Nop<7>(0x6378D2);

	//
	// Fix Valley of Loren on some PC's
	//
	//Memory::Nop<3>(0x691217);

	//
	// Add new maps
	//
	//Memory::Hook<true>(0x64C6DB, this->LoadMapName);

	return true;
}