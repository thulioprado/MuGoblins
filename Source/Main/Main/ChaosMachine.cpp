#include "Library.h"
#include "ChaosMachine.h"
#include "Message.h"

CChaosMachine::CChaosMachine() : Items(), Total(0), Title(""), Index(0xFF), Money(0), Rate(0), X(0), Y(0)
{
}

CChaosMachine::~CChaosMachine()
{
}

void CChaosMachine::Load()
{
    //
    // Ganchos
    //
    Memory::Jump(0x5EBA91, this->ClearItems);
    Memory::Jump(0x5EBB68, this->CountItems);
    Memory::Jump(0x5EC6D7, this->CheckCombination);
    Memory::Jump(0x5ECC52, this->ClickMix);
    Memory::Jump(0x5ED0A7, this->Description);
    Memory::Jump(0x61DFBB, this->Confirm);
}

void CChaosMachine::Clear()
{
    this->Total = 0;

    this->Items[GET_ITEM(12, 15)] = 0;  // Jewel of Chaos
    this->Items[GET_ITEM(12, 13)] = 0;  // Jewel of Bless
    this->Items[GET_ITEM(12, 14)] = 0;  // Jewel of Soul
    this->Items[GET_ITEM(12, 16)] = 0;  // Jewel of Life
    this->Items[GET_ITEM(14, 22)] = 0;  // Jewel of Creation
    this->Items[GET_ITEM(14, 31)] = 0;  // Jewel of Guardian
    this->Items[GET_ITEM(13, 39)] = 0;  // Prism Ring
    this->Items[GET_ITEM(13, 40)] = 0;  // Prism Ring
}

void CChaosMachine::Add(WORD Index)
{
    ++this->Items.at(Index);
    ++this->Total;
}

void CChaosMachine::Check()
{
    this->Index = 0xFF;

    //
    // Combinação de anéis de prisma
    //
    if (this->Total == 3 &&
        this->Items.at(GET_ITEM(12, 15)) == 1 &&
        (this->Items.at(GET_ITEM(13, 39)) == 2 || this->Items.at(GET_ITEM(13, 40)) == 2))
    {
        this->Title = Message.Get(0);
        this->Index = 24;
        this->Money = 5000000;
        this->Rate = 100;
        this->X = 1;
        this->Y = 1;

        return;
    }
}

void __declspec(naked) CChaosMachine::ClearItems()
{
    static DWORD Back = 0x5EBA96;

    __asm
    {
        PUSHAD;
    }

    ChaosMachine.Clear();

    __asm
    {
        POPAD;
        MOV EDX, 1;
        JMP Back;
    }
}

void __declspec(naked) CChaosMachine::CountItems()
{
    static DWORD Back = 0x5EBB6F;
    static WORD Index;

    __asm
    {
        AND EDI, 0xF;
        MOV Index, AX;
        PUSHAD;
    }

    ChaosMachine.Add(Index);

    __asm
    {
        POPAD;
        CMP AX, 0x1800;
        JMP Back;
    }
}

void __declspec(naked) CChaosMachine::CheckCombination()
{
    static DWORD Back[2] = {0x5EC70A, 0x5EC6DC};

    __asm
    {
        PUSHAD;
    }

    ChaosMachine.Check();

    if (ChaosMachine.Index != 0xFF)
    {
        __asm
        {
            POPAD;
            POP EDI;
            POP ESI;
            MOV EAX, ChaosMachine.Index;
            POP EBX;
            MOV ESP, EBP;
            POP EBP;
            RETN;
        }
    }

    __asm
    {
        POPAD;
        CMP EAX, 0x4;
        JNZ NotZero;
        JMP Back[4];
    NotZero:
        JMP Back[0];
    }
}

void __declspec(naked) CChaosMachine::ClickMix()
{
    static DWORD Back[2] = {0x5ECC58, 0x5ECCB4};

    __asm
    {
        PUSHAD;
    }
    
    if (ChaosMachine.Index != 0xFF)
    {
        __asm
        {
            POPAD;
            MOV EAX, ChaosMachine.X;
            MOV ECX, ChaosMachine.Y;
            JMP Back[4];
        }
    }

    __asm
    {
        POPAD;
        LEA EAX, DWORD PTR DS : [EDX + 0x18] ;
        CMP EAX, 0x30;
        JMP Back[0];
    }
}

void __declspec(naked) CChaosMachine::Description()
{
    static DWORD Back[2] = {0x5ED0AD, 0x5ED42E};
    static DWORD Function[2] = {0x5A0930, 0x68EFC7};

    __asm
    {
        PUSHAD;
    }

    if (ChaosMachine.Index != 0xFF)
    {
        __asm
        {
            POPAD;
            MOV ESI, ChaosMachine.Money;
            MOV DWORD PTR DS : [0x7D0E9A0] , ESI;
            MOV ESI, ChaosMachine.Rate;
            LEA EDX, DWORD PTR SS : [EBP - 0x48] ;
            PUSH EDX;
            PUSH ChaosMachine.Money;
            CALL Function[0];
            ADD ESP, 0x8;
            MOV ECX, DWORD PTR DS : [0x7D0E998] ;
            MOV EAX, 0x7;
            MOV DWORD PTR DS : [0x6B7DF0] , 0xFF30FFFF;
            MOV EBX, 0x80282828;
            MOV DWORD PTR DS : [0x6B7DF8] , EBX;
            LEA EAX, DWORD PTR SS : [EBP - 0xAC] ;
            PUSH ChaosMachine.Title;
            PUSH EAX;
            CALL Function[4];
            ADD ESP, 0x8;
            JMP Back[4];
        }
    }

    __asm
    {
        POPAD;
        LEA EAX, DWORD PTR DS : [EDX + 0x18] ;
        CMP EAX, 0x30;
        JMP Back[0];
    }
}

void __declspec(naked) CChaosMachine::Confirm()
{
    static DWORD Back[3] = {0x61DFC0, 0x61DFCC, 0x61DFD9};

    __asm
    {
        PUSHAD;
    }

    if (ChaosMachine.Index != 0xFF)
    {
        __asm
        {
            POPAD;
            LEA EDX, DWORD PTR SS : [EBP - 0x9C] ;
            PUSH ChaosMachine.Title;
            PUSH EDX;
            JMP Back[4];
        }
    }

    __asm
    {
        POPAD;
        CMP EAX, 0xC;
        JNZ NotZero;
        JMP Back[0];
    NotZero:
        JMP Back[8];
    }
}

CChaosMachine ChaosMachine;