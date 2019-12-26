#include "Library.h"
#include "Message.h"

CMessage::CMessage()
{
}

CMessage::~CMessage()
{
}

void CMessage::Load()
{
	this->Language = Registry::Read<BYTE>("Language");
}

const char* CMessage::Get(int Index) const
{
	switch (this->Language)
	{
		case 0:
		{
			switch (Index)
			{
				case 0:
					return "Combinação: Anéis de Prisma";
			}

			break;
		}
	}

	return "";
}

CMessage Message;