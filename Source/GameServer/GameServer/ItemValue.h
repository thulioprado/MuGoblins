// ItemValue.h: interface for the CItemValue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Item.h"

struct ITEM_VALUE_INFO
{
	int Index;
	int Level;
	int Grade;
	int Value;
};

class CItemValue
{
public:
	CItemValue();
	virtual ~CItemValue();
	void Load(char* path);
	bool GetItemValue(CItem* lpItem,int* value);
private:
	std::vector<ITEM_VALUE_INFO> m_ItemValueInfo;
};

extern CItemValue gItemValue;
