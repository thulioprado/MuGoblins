#include "Library.h"
#include "Message.h"

CMessage::CMessage() : Text{0}
{
}

CMessage::~CMessage()
{
}

void CMessage::Load()
{
	//
	// Ganchos
	//
    Memory::Jump(0x457710, this->ShowInTab);
    Memory::Jump(0x45779D, this->SetColor1);
    Memory::Jump(0x45796F, this->SetColor2);
    Memory::Jump(0x457AFE, this->SetColor3);
    Memory::Jump(0x549A03, this->SetColor4);
}

const char* CMessage::Get(int Index) const
{
    switch (Index)
	{
		case 0:
			return "Combinação: Anéis de Prisma";
		case 1:
			return "Auto-click ativado.";
		case 2:
			return "Auto-click desativado.";
        case 3:
            return "Cor:";
        case 4:
            return "Cores:";
        case 5:
            return "Usado no Anel de Prisma para aumentar um canal de cor.";
        case 6:
            return "Usado no Anel de Prisma para diminuir um canal de cor.";
	}

	return "";
}

void CMessage::Success(int Index, ...)
{
    auto Format = this->Get(Index);

    va_list Arguments;
    va_start(Arguments, Format);
    vsprintf_s(this->Text, Format, Arguments);
    va_end(Arguments);

    pShowMessage("", this->Text, 95);
}

void CMessage::Error(int Index, ...)
{
    auto Format = this->Get(Index);

    va_list Arguments;
    va_start(Arguments, Format);
    vsprintf_s(this->Text, Format, Arguments);
    va_end(Arguments);

    pShowMessage("", this->Text, 2);
}

void CMessage::Info(int Index, ...)
{
    auto Format = this->Get(Index);

    va_list Arguments;
    va_start(Arguments, Format);
    vsprintf_s(this->Text, Format, Arguments);
    va_end(Arguments);

    pShowMessage("", this->Text, 1);
}

void CMessage::SetColor(int Index)
{
    switch (Index)
    {
        case 0:
        {
            pMessageBackgroundColor = RGBA(255, 200, 50, 150);
            pMessageForegroundColor = RGBA(0, 0, 0, 255);

            break;
        }
        case 1:
        {
            pMessageBackgroundColor = RGBA(0, 0, 0, 150);
            pMessageForegroundColor = RGBA(100, 150, 255, 255);

            break;
        }
        case 2:
        {
            pMessageBackgroundColor = RGBA(0, 0, 0, 150);
            pMessageForegroundColor = RGBA(255, 30, 0, 255);

            break;
        }
        case 3:
        {
            pMessageBackgroundColor = RGBA(0, 0, 0, 150);
            pMessageForegroundColor = RGBA(205, 220, 239, 255);

            break;
        }
        case 4:
        {
            pMessageBackgroundColor = RGBA(0, 200, 255, 150);
            pMessageForegroundColor = RGBA(0, 0, 0, 255);

            break;
        }
        case 5:
        {
            pMessageBackgroundColor = RGBA(0, 255, 150, 200);
            pMessageForegroundColor = RGBA(0, 0, 0, 255);

            break;
        }
        case 6:
        {
            pMessageBackgroundColor = RGBA(200, 200, 0, 200);
            pMessageForegroundColor = RGBA(0, 0, 0, 255);

            break;
        }
        case 95:
        {
            pMessageBackgroundColor = RGBA(0, 0, 0, 150);
            pMessageForegroundColor = RGBA(0, 220, 0, 255);

            break;
        }
        case 96:
        {
            pMessageBackgroundColor = RGBA(219, 112, 147, 200);
            pMessageForegroundColor = RGBA(240, 240, 240, 255);

            break;
        }
        case 97:
        {
            pMessageBackgroundColor = RGBA(255, 255, 255, 200);
            pMessageForegroundColor = RGBA(0, 0, 0, 255);

            break;
        }
    }
}

void __declspec(naked) CMessage::ShowInTab()
{
    static DWORD Back[2] = {0x457715, 0x45775A};

    __asm
    {
        CMP EAX, 2;
        JE DontShow;
        CMP EAX, 95;
        JE DontShow;
        JMP Back[4];

    DontShow:
        JMP Back[0];
    }
}

void __declspec(naked) CMessage::SetColor1()
{
    static DWORD Back = 0x457848;
    static DWORD Index;

    __asm
    {
        MOV Index, EAX;
        PUSHAD;
    }

    Message.SetColor(Index);

    __asm
    {
        POPAD;
        JMP Back;
    }
}

void __declspec(naked) CMessage::SetColor2()
{
    static DWORD Back = 0x457A07;
    static DWORD Index;

    __asm
    {
        MOV Index, EAX;
        PUSHAD;
    }

    Message.SetColor(Index);

    __asm
    {
        POPAD;
        JMP Back;
    }
}

void __declspec(naked) CMessage::SetColor3()
{
    static DWORD Back[2] = {0x457B03, 0x457B80};
    static DWORD Index;

    __asm
    {
        CMP EAX, 5;
        JA Above;
        JMP Back[0];
    Above:
        MOV Index, EAX;
        PUSHAD;
    }

    Message.SetColor(Index);

    __asm
    {
        POPAD;
        JMP Back[4];
    }
}

void __declspec(naked) CMessage::SetColor4()
{
    static DWORD Back[2] = {0x549A0C, 0x549A95};
    static DWORD Index;

    __asm
    {
        CMP ECX, 5;
        JA Above;
        JMP Back[0];
    Above:
        MOV Index, ECX;
        PUSHAD;
    }

    Message.SetColor(Index);

    __asm
    {
        POPAD;
        JMP Back[4];
    }
}

CMessage Message;