// ItemValue.cpp: implementation of the CItemValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemValue.h"
#include "ItemManager.h"
#include "ItemStack.h"
#include "MemScript.h"
#include "Util.h"

CItemValue gItemValue;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemValue::CItemValue() // OK
{
	this->m_ItemValueInfo.clear();
}

CItemValue::~CItemValue() // OK
{

}

void CItemValue::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_ItemValueInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			ITEM_VALUE_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.Level = lpMemScript->GetAsNumber();

			info.Grade = lpMemScript->GetAsNumber();

			info.Value = lpMemScript->GetAsNumber();

			this->m_ItemValueInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemValue::GetItemValue(CItem* lpItem,int* value) // OK
{
	for(std::vector<ITEM_VALUE_INFO>::iterator it=this->m_ItemValueInfo.begin();it != this->m_ItemValueInfo.end();it++)
	{
		if(it->Index == lpItem->m_Index)
		{
			if(it->Level == -1 || it->Level == lpItem->m_Level)
			{
				if(it->Grade == -1 || it->Grade == lpItem->m_NewOption)
				{
					if(gItemStack.GetItemMaxStack(it->Index) == 0 || it->Index == GET_ITEM(4,7) || it->Index == GET_ITEM(4,15))
					{
						(*value) = it->Value;
						return 1;
					}
					else
					{
						(*value) = (int)(it->Value*lpItem->m_Durability);
						return 1;
					}
				}
			}
		}
	}

	return 0;
}
