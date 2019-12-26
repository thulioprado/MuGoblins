// JewelMix.cpp: implementation of the CJewelMix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JewelMix.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "Util.h"

CJewelMix gJewelMix;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJewelMix::CJewelMix() // OK
{

}

CJewelMix::~CJewelMix() // OK
{

}

int CJewelMix::GetJewelSimpleIndex(int type) // OK
{
	switch (type)
	{
		case 0:
			return GET_ITEM(14, 13);
		case 1:
			return GET_ITEM(14, 14);
		case 2:
			return GET_ITEM(14, 16);
		case 3:
			return GET_ITEM(14, 22);
		case 4:
			return GET_ITEM(14, 31);
		case 7:
			return GET_ITEM(12, 15);
	}

	return -1;
}

int CJewelMix::GetJewelBundleIndex(int type) // OK
{
	switch (type)
	{
		case 0:
			return GET_ITEM(12, 30);
		case 1:
			return GET_ITEM(12, 31);
	}

	return -1;
}

void CJewelMix::CGJewelMixRecv(PMSG_JEWEL_MIX_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_COMMON)
	{
		return;
	}

	if (lpObj->ChaosLock != 0)
	{
		this->GCJewelMixResultSend(aIndex, 0);
		return;
	}

	if (lpMsg->type < 0 || lpMsg->type > 9)
	{
		this->GCJewelMixResultSend(aIndex, 2);
		return;
	}

	if (lpMsg->level < 0 || lpMsg->level > 2)
	{
		this->GCJewelMixResultSend(aIndex, 3);
		return;
	}

	lpObj->ChaosLock = 1;

	int count = gItemManager.GetInventoryItemCount(lpObj, this->GetJewelSimpleIndex(lpMsg->type), 0);

	int JewelCount = (lpMsg->level + 1) * 10;

	if (count < JewelCount)
	{
		lpObj->ChaosLock = 0;
		this->GCJewelMixResultSend(aIndex, 4);
		return;
	}

	int MixMoney = (lpMsg->level + 1) * 500000;

	if (lpObj->Money < ((DWORD)MixMoney))
	{
		lpObj->ChaosLock = 0;
		this->GCJewelMixResultSend(aIndex, 5);
		return;
	}

	lpObj->Money -= MixMoney;

	GCMoneySend(aIndex, lpObj->Money);

	gItemManager.DeleteInventoryItemCount(lpObj, this->GetJewelSimpleIndex(lpMsg->type), 0, JewelCount);

	GDCreateItemSend(aIndex, 0xEB, 0, 0, this->GetJewelBundleIndex(lpMsg->type), lpMsg->level, 0, 0, 0, 0, -1, 0, 0, 0, 0);

	this->GCJewelMixResultSend(aIndex, 1);

	lpObj->ChaosLock = 0;
}

void CJewelMix::CGJewelUnMixRecv(PMSG_JEWEL_UNMIX_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_COMMON)
	{
		return;
	}

	if (lpObj->ChaosLock != 0)
	{
		this->GCJewelUnMixResultSend(aIndex, 0);
		return;
	}

	if (lpMsg->type < 0 || lpMsg->type > 9)
	{
		this->GCJewelUnMixResultSend(aIndex, 2);
		return;
	}

	if (lpMsg->level < 0 || lpMsg->level > 2)
	{
		this->GCJewelUnMixResultSend(aIndex, 3);
		return;
	}

	if (INVENTORY_FULL_RANGE(lpMsg->slot) == 0)
	{
		this->GCJewelUnMixResultSend(aIndex, 5);
		return;
	}

	lpObj->ChaosLock = 1;

	if (lpObj->Inventory[lpMsg->slot].IsItem() == 0)
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex, 4);
		return;
	}

	if (lpObj->Inventory[lpMsg->slot].m_Level != lpMsg->level)
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex, 3);
		return;
	}

	if (lpObj->Inventory[lpMsg->slot].m_Index != this->GetJewelBundleIndex(lpMsg->type))
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex, 6);
		return;
	}

	int count = gItemManager.GetInventoryEmptySlotCount(lpObj);

	int JewelCount = (lpMsg->level + 1) * 10;

	if (count < JewelCount)
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex, 7);
		return;
	}

	int MixMoney = 1000000;

	if (lpObj->Money < ((DWORD)MixMoney))
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex, 8);
		return;
	}

	lpObj->Money -= MixMoney;

	GCMoneySend(aIndex, lpObj->Money);

	gItemManager.InventoryDelItem(aIndex, lpMsg->slot);

	gItemManager.GCItemDeleteSend(aIndex, lpMsg->slot, 1);

	for (int n = 0; n < JewelCount; n++)
	{
		GDCreateItemSend(aIndex, 0xEB, 0, 0, this->GetJewelSimpleIndex(lpMsg->type), 0, 0, 0, 0, 0, -1, 0, 0, 0, 0);
	}

	this->GCJewelUnMixResultSend(aIndex, 1);

	lpObj->ChaosLock = 0;
}

void CJewelMix::GCJewelMixResultSend(int aIndex, BYTE result) // OK
{
	PMSG_JEWEL_MIX_RESULT_SEND pMsg;

	pMsg.header.set(0xBC, 0x00, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CJewelMix::GCJewelUnMixResultSend(int aIndex, BYTE result) // OK
{
	PMSG_JEWEL_MIX_RESULT_SEND pMsg;

	pMsg.header.set(0xBC, 0x01, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}
