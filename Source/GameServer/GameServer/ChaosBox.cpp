// ChaosBox.cpp: implementation of the CChaosBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "DSProtocol.h"
#include "ItemBagManager.h"
#include "ItemOptionRate.h"
#include "Log.h"
#include "Notice.h"
#include "RandomManager.h"
#include "ServerInfo.h"
#include "SetItemType.h"
#include "Shop.h"
#include "Util.h"

CChaosBox gChaosBox;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChaosBox::CChaosBox() // OK
{
	this->m_SeniorMixLimitDay = 0;

	this->m_SeniorMixLimitMonth = 0;

	this->m_SeniorMixLimitYear = 0;
}

CChaosBox::~CChaosBox() // OK
{

}

void CChaosBox::ChaosBoxInit(LPOBJ lpObj) // OK
{
	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		lpObj->ChaosBox[n].Clear();
		lpObj->ChaosBoxMap[n] = 0xFF;
	}
}

void CChaosBox::ChaosBoxItemDown(LPOBJ lpObj,int slot) // OK
{
	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(n != slot)
		{
			lpObj->ChaosBox[n].Clear();
			lpObj->ChaosBoxMap[n] = 0xFF;
			continue;
		}

		CItem* lpItem = &lpObj->ChaosBox[n];
		
		lpItem->m_Level = 0;

		float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index,lpItem->m_Level,lpItem->IsExcItem(),lpItem->IsSetItem());

		lpItem->m_Durability = dur*(lpItem->m_Durability/lpItem->m_BaseDurability);

		lpItem->Convert(lpItem->m_Index,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption,lpItem->m_SetOption);
	}
}

void CChaosBox::ChaosBoxItemKeep(LPOBJ lpObj,int slot) // OK
{
	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(n != slot)
		{
			lpObj->ChaosBox[n].Clear();
			lpObj->ChaosBoxMap[n] = 0xFF;
			continue;
		}
	}
}

void CChaosBox::ChaosBoxItemSave(LPOBJ lpObj) // OK
{
	if(gObjInventoryCommit(lpObj->Index) != 0)
	{
		for(int n=0;n < CHAOS_BOX_SIZE;n++)
		{
			if(lpObj->ChaosBox[n].IsItem() != 0)
			{
				gItemManager.InventoryInsertItem(lpObj->Index,lpObj->ChaosBox[n]);
			}
		}
	}
}

void CChaosBox::ChaosItemMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int ItemCount = 0;
	int ItemMoney = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
			lpObj->ChaosBox[n].OldValue();
			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13) || lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			ItemCount++;
			lpObj->ChaosBox[n].OldValue();
			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if(lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
		{
			ItemCount++;
			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if(ChaosCount == 0 || ItemCount == 0)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(gServerInfo.m_ChaosItemMixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (ItemMoney/20000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_ChaosItemMixRate[lpObj->AccountLevel];
	}


	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate*10000;

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		WORD ItemIndex = 0;
		BYTE ItemLevel = 0;
		BYTE ItemOption1 = 0;
		BYTE ItemOption2 = 0;
		BYTE ItemOption3 = 0;

		CRandomManager RandomManager;

		RandomManager.AddElement(GET_ITEM(2,6),1);

		RandomManager.AddElement(GET_ITEM(4,6),1);

		RandomManager.AddElement(GET_ITEM(5,7),1);

		RandomManager.GetRandomElement(&ItemIndex);

		gItemOptionRate.GetItemOption0(3,&ItemLevel);

		gItemOptionRate.GetItemOption1(3,&ItemOption1);

		gItemOptionRate.GetItemOption2(3,&ItemOption2);

		gItemOptionRate.GetItemOption3(3,&ItemOption3);

		GDCreateItemSend(lpObj->Index,0xFF,0,0,ItemIndex,ItemLevel,0,ItemOption1,ItemOption2,ItemOption3,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[ChaosItemMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[ChaosItemMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::DevilSquareMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int EyeCount = 0;
	int EyeLevel = 0;
	int KeyCount = 0;
	int KeyLevel = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,17))
		{
			EyeCount++;
			EyeLevel = lpObj->ChaosBox[n].m_Level;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,18))
		{
			KeyCount++;
			KeyLevel = lpObj->ChaosBox[n].m_Level;
		}
	}

	if(ChaosCount != 1 || EyeCount != 1 || KeyCount != 1 || EyeLevel != KeyLevel)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(EyeLevel > 7 || KeyLevel > 7)
	{
		this->GCChaosMixSend(lpObj->Index,8,0);
		return;
	}

	switch(EyeLevel)
	{
		case 1:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate1[lpObj->AccountLevel];
			lpObj->ChaosMoney = 100000;
			break;
		case 2:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate2[lpObj->AccountLevel];
			lpObj->ChaosMoney = 200000;
			break;
		case 3:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate3[lpObj->AccountLevel];
			lpObj->ChaosMoney = 400000;
			break;
		case 4:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate4[lpObj->AccountLevel];
			lpObj->ChaosMoney = 700000;
			break;
		case 5:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate5[lpObj->AccountLevel];
			lpObj->ChaosMoney = 1100000;
			break;
		case 6:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate6[lpObj->AccountLevel];
			lpObj->ChaosMoney = 1600000;
			break;
		case 7:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate7[lpObj->AccountLevel];
			lpObj->ChaosMoney = 2000000;
			break;
	}

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		GDCreateItemSend(lpObj->Index,0xFF,0,0,GET_ITEM(14,19),EyeLevel,1,0,0,0,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[DevilSquareMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[DevilSquareMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::PlusItemLevelMix(LPOBJ lpObj,int type) // OK
{
	int ChaosCount = 0;
	int BlessCount = 0;
	int SoulCount = 0;
	int ItemCount = 0;
	int ItemSlot = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13))
		{
			BlessCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			SoulCount++;
		}
		else if(lpObj->ChaosBox[n].m_Level == (9+type))
		{
			ItemCount++;
			ItemSlot = n;
		}
	}

	if(ChaosCount != 1 || SoulCount < (type+1) || BlessCount < (type+1) || ItemCount != 1)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	switch(type)
	{
		case 0:
			lpObj->ChaosSuccessRate = gServerInfo.m_PlusItemLevelMixRate1[lpObj->AccountLevel];
			break;
		case 1:
			lpObj->ChaosSuccessRate = gServerInfo.m_PlusItemLevelMixRate2[lpObj->AccountLevel];
			break;
		case 2:
			lpObj->ChaosSuccessRate = gServerInfo.m_PlusItemLevelMixRate3[lpObj->AccountLevel];
			break;
		case 3:
			lpObj->ChaosSuccessRate = gServerInfo.m_PlusItemLevelMixRate4[lpObj->AccountLevel];
			break;
		case 4:
			lpObj->ChaosSuccessRate = gServerInfo.m_PlusItemLevelMixRate5[lpObj->AccountLevel];
			break;
		case 5:
			lpObj->ChaosSuccessRate = gServerInfo.m_PlusItemLevelMixRate6[lpObj->AccountLevel];
			break;
	}

	if(lpObj->ChaosBox[ItemSlot].m_Option2 != 0)
	{
		lpObj->ChaosSuccessRate += gServerInfo.m_AddLuckSuccessRate2[lpObj->AccountLevel];
	}

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 2000000*(type+1);

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		CItem item = lpObj->ChaosBox[ItemSlot];

		item.m_Level++;

		item.m_Durability = (float)gItemManager.GetItemDurability(item.m_Index,item.m_Level,item.IsExcItem(),item.IsSetItem());

		item.m_Durability = (item.m_Durability*lpObj->ChaosBox[ItemSlot].m_Durability)/item.m_BaseDurability;

		this->ChaosBoxInit(lpObj);

		gItemManager.ChaosBoxAddItem(lpObj->Index,item,0);

		this->GCChaosMixSend(lpObj->Index,1,&item);

		gObjCustomLogPlusChaosMix(lpObj,type,item.m_Index);

		gLog.Output(LOG_CHAOS_MIX,"[PlusItemLevelMix][Success][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);
		this->GCChaosBoxSend(lpObj,0);
		this->GCChaosMixSend(lpObj->Index,0,0);
		gLog.Output(LOG_CHAOS_MIX,"[PlusItemLevelMix][Failure][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::DinorantMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int UniriaCount = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,2) && lpObj->ChaosBox[n].m_Durability == 255)
		{
			UniriaCount++;
		}
	}

	if(ChaosCount != 1 || UniriaCount != 10)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	lpObj->ChaosSuccessRate = gServerInfo.m_DinorantMixRate[lpObj->AccountLevel];

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 500000;

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		WORD ItemIndex = GET_ITEM(13,3);
		BYTE ItemNewOption = 0;

		gItemOptionRate.GetItemOption4(3,&ItemNewOption);

		gItemOptionRate.MakeNewOption(ItemIndex,ItemNewOption,&ItemNewOption);

		GDCreateItemSend(lpObj->Index,0xFF,0,0,ItemIndex,0,255,0,0,0,-1,ItemNewOption,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[DinorantMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[DinorantMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::FruitMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int CreationCount = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,22))
		{
			CreationCount++;
		}
	}

	if(ChaosCount != 1 || CreationCount != 1)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	lpObj->ChaosSuccessRate = gServerInfo.m_FruitMixRate[lpObj->AccountLevel];
	
	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 3000000;

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		BYTE ItemLevel = GetLargeRand()%5;

		GDCreateItemSend(lpObj->Index,0xFF,0,0,GET_ITEM(13,15),ItemLevel,0,0,0,0,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[FruitMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[FruitMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::Wing2Mix(LPOBJ lpObj,int type) // OK
{
	int ChaosCount = 0;
	int FeatherCount = 0;
	int SleeveCount = 0;
	int WingItemCount = 0;
	int WingItemMoney = 0;
	int ItemCount = 0;
	int ItemMoney = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,14) && lpObj->ChaosBox[n].m_Level == 0)
		{
			FeatherCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,14) && lpObj->ChaosBox[n].m_Level == 1)
		{
			SleeveCount++;
		}
		else if((lpObj->ChaosBox[n].m_Index >= GET_ITEM(12,0) && lpObj->ChaosBox[n].m_Index <= GET_ITEM(12,2)))
		{
			WingItemCount++;
			WingItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
		else if(lpObj->ChaosBox[n].IsExcItem() != 0 && lpObj->ChaosBox[n].m_Level >= 4)
		{
			ItemCount++;
			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if(ChaosCount != 1 || (type == 0 && FeatherCount != 1) || (type == 1 && SleeveCount != 1) || WingItemCount != 1)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(gServerInfo.m_Wing2MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (WingItemMoney/4000000)+(ItemMoney/40000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_Wing2MixRate[lpObj->AccountLevel];
	}

	if(gServerInfo.m_Wing2MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>90)?90:lpObj->ChaosSuccessRate);
	}
	else
	{
		lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);
	}

	lpObj->ChaosMoney = 5000000;
	
	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		if(type == 0)
		{
			WORD ItemIndex = 0;
			BYTE ItemOption2 = 0;
			BYTE ItemOption3 = 0;
			BYTE ItemNewOption = 0;

			CRandomManager RandomManager;

			RandomManager.AddElement(GET_ITEM(12,3),1);
			RandomManager.AddElement(GET_ITEM(12,4),1);
			RandomManager.AddElement(GET_ITEM(12,5),1);
			RandomManager.AddElement(GET_ITEM(12,6),1);
			RandomManager.GetRandomElement(&ItemIndex);

			gItemOptionRate.GetItemOption2(4,&ItemOption2);
			gItemOptionRate.GetItemOption3(4,&ItemOption3);
			gItemOptionRate.GetItemOption4(4,&ItemNewOption);
			gItemOptionRate.MakeNewOption(ItemIndex,ItemNewOption,&ItemNewOption);

			GDCreateItemSend(lpObj->Index,0xFF,0,0,ItemIndex,0,0,0,ItemOption2,ItemOption3,-1,(ItemNewOption+(32*(GetLargeRand()%2))),0,0);

			gLog.Output(LOG_CHAOS_MIX,"[Wing2Mix][Success][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
		}
		else
		{
			WORD ItemIndex = 0;
			BYTE ItemOption2 = 0;
			BYTE ItemOption3 = 0;
			BYTE ItemNewOption = 0;

			CRandomManager RandomManager;

			RandomManager.AddElement(GET_ITEM(13,30),1);
			RandomManager.GetRandomElement(&ItemIndex);

			gItemOptionRate.GetItemOption2(4,&ItemOption2);
			gItemOptionRate.GetItemOption3(4,&ItemOption3);
			gItemOptionRate.GetItemOption4(5,&ItemNewOption);
			gItemOptionRate.MakeNewOption(ItemIndex,ItemNewOption,&ItemNewOption);

			GDCreateItemSend(lpObj->Index,0xFF,0,0,ItemIndex,0,0,0,ItemOption2,ItemOption3,-1,(ItemNewOption+32),0,0);

			gLog.Output(LOG_CHAOS_MIX,"[Wing2Mix][Success][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
		}
	}
	else
	{
		this->ChaosBoxInit(lpObj);
		this->GCChaosBoxSend(lpObj,0);
		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[Wing2Mix][Failure][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::BloodCastleMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int ScrollCount = 0;
	int ScrollLevel = 0;
	int BoneCount = 0;
	int BoneLevel = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,16))
		{
			ScrollCount++;
			ScrollLevel = lpObj->ChaosBox[n].m_Level;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,17))
		{
			BoneCount++;
			BoneLevel = lpObj->ChaosBox[n].m_Level;
		}
	}

	if(ChaosCount != 1 || ScrollCount != 1 || BoneCount != 1 || ScrollLevel != BoneLevel)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(ScrollLevel > 8 || BoneLevel > 8)
	{
		this->GCChaosMixSend(lpObj->Index,8,0);
		return;
	}

	switch(ScrollLevel)
	{
		case 1:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate1[lpObj->AccountLevel];
			lpObj->ChaosMoney = 50000;
			break;
		case 2:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate2[lpObj->AccountLevel];
			lpObj->ChaosMoney = 80000;
			break;
		case 3:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate3[lpObj->AccountLevel];
			lpObj->ChaosMoney = 150000;
			break;
		case 4:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate4[lpObj->AccountLevel];
			lpObj->ChaosMoney = 250000;
			break;
		case 5:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate5[lpObj->AccountLevel];
			lpObj->ChaosMoney = 400000;
			break;
		case 6:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate6[lpObj->AccountLevel];
			lpObj->ChaosMoney = 600000;
			break;
		case 7:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate7[lpObj->AccountLevel];
			lpObj->ChaosMoney = 850000;
			break;
		case 8:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate8[lpObj->AccountLevel];
			lpObj->ChaosMoney = 1050000;
			break;
	}
	
	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) <= lpObj->ChaosSuccessRate)
	{
		GDCreateItemSend(lpObj->Index,0xFF,0,0,GET_ITEM(13,18),ScrollLevel,1,0,0,0,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[BloodCastleMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[BloodCastleMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::Wing1Mix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int ChaosItem = 0;
	int ItemCount = 0;
	int ItemMoney = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
			lpObj->ChaosBox[n].OldValue();
			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13) || lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			ItemCount++;
			lpObj->ChaosBox[n].OldValue();
			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if((lpObj->ChaosBox[n].m_Index == GET_ITEM(2,6) || lpObj->ChaosBox[n].m_Index == GET_ITEM(4,6) || lpObj->ChaosBox[n].m_Index == GET_ITEM(5,7)) && lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
		{
			ChaosItem++;
			ItemCount++;
			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
		else if(lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
		{
			ItemCount++;
			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if(ChaosCount == 0 || ChaosItem == 0)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(gServerInfo.m_Wing1MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (ItemMoney/20000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_Wing1MixRate[lpObj->AccountLevel];
	}
	
	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate*10000;

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		WORD ItemIndex = 0;
		BYTE ItemOption2 = 0;
		BYTE ItemOption3 = 0;

		CRandomManager RandomManager;

		RandomManager.AddElement(GET_ITEM(12,0),1);
		RandomManager.AddElement(GET_ITEM(12,1),1);
		RandomManager.AddElement(GET_ITEM(12,2),1);
		RandomManager.GetRandomElement(&ItemIndex);

		gItemOptionRate.GetItemOption2(5,&ItemOption2);
		gItemOptionRate.GetItemOption3(5,&ItemOption3);

		GDCreateItemSend(lpObj->Index,0xFF,0,0,ItemIndex,0,0,0,ItemOption2,ItemOption3,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[Wing1Mix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);
		this->GCChaosBoxSend(lpObj,0);
		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[Wing1Mix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::PetMix(LPOBJ lpObj,int type) // OK
{
	int ChaosCount = 0;
	int SoulOfDarkHorseCount = 0;
	int SoulOfDarkSpiritCount = 0;
	int BlessCount = 0;
	int SoulCount = 0;
	int CreationCount = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,31) && lpObj->ChaosBox[n].m_Level == 0)
		{
			SoulOfDarkHorseCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,31) && lpObj->ChaosBox[n].m_Level == 1)
		{
			SoulOfDarkSpiritCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13))
		{
			BlessCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			SoulCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,22))
		{
			CreationCount++;
		}
	}

	if(ChaosCount != 1 || (type == 0 && SoulOfDarkHorseCount != 1) || (type == 1 && SoulOfDarkSpiritCount != 1) || BlessCount != (5-(type*3)) || SoulCount != (5-(type*3)) || CreationCount != 1)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	lpObj->ChaosSuccessRate = gServerInfo.m_PetMixRate[lpObj->AccountLevel];
	
	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 5000000-(4000000*type);

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		GDCreateItemSend(lpObj->Index,0xFE,0,0,(GET_ITEM(13,4)+type),0,0,0,0,0,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[PetMix][Success][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,1);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[PetMix][Failure][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::SiegePotionMix(LPOBJ lpObj,int type) // OK
{
	int BlessCount = 0;
	int SoulCount = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13))
		{
			BlessCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			SoulCount++;
		}
	}

	if((type == 0 && (BlessCount == 0 || BlessCount > 25)) || (type == 1 && (SoulCount == 0 || SoulCount > 25)))
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	lpObj->ChaosMoney = 100000-(50000*type);

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	GDCreateItemSend(lpObj->Index,0xFF,0,0,GET_ITEM(14,7),type,((BlessCount+SoulCount)*10),0,0,0,-1,0,0,0);

	gLog.Output(LOG_CHAOS_MIX,"[SiegePotionMix][Success][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
}

void CChaosBox::LifeStoneMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int BlessCount = 0;
	int SoulCount = 0;
	int GuardianCount = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13))
		{
			BlessCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			SoulCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,31))
		{
			GuardianCount++;
		}
	}

	if(ChaosCount != 1 || BlessCount != 5 || SoulCount != 5 || GuardianCount != 1)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	lpObj->ChaosMoney = 5000000;

	int TaxMoney = (lpObj->ChaosMoney*gCastleSiegeSync.GetTaxRateChaos(lpObj->Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(TaxMoney);

	GDCreateItemSend(lpObj->Index,0xFF,0,0,GET_ITEM(13,11),1,0,0,0,0,-1,0,0,0);

	gLog.Output(LOG_CHAOS_MIX,"[LifeStoneMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
}

void CChaosBox::SeniorMix(LPOBJ lpObj) // OK
{
	if(gCastleSiegeSync.CheckCastleOwnerMember(lpObj->Index) == 0 || lpObj->GuildStatus != 0x80)
	{
		this->GCChaosMixSend(lpObj->Index,0,0);
		return;
	}

	SYSTEMTIME SystemTime;

	GetSystemTime(&SystemTime);

	if(this->m_SeniorMixLimitDay == SystemTime.wDay && this->m_SeniorMixLimitMonth == SystemTime.wMonth && this->m_SeniorMixLimitYear == SystemTime.wYear)
	{
		this->GCChaosMixSend(lpObj->Index,0,0);
		return;
	}

	int SoulPack10 = 0;
	int BlessPack10 = 0;
	int GuardianCount = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,30))
		{
			BlessPack10 += lpObj->ChaosBox[n].m_Level+1;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,31))
		{
			SoulPack10 += lpObj->ChaosBox[n].m_Level+1;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,31))
		{
			GuardianCount++;
		}
	}

	if(SoulPack10 != 3 || BlessPack10 != 3 || GuardianCount != 30)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	lpObj->ChaosMoney = 1000000000;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	CItem item;

	if(gItemBagManager.GetItemBySpecialValue(ITEM_BAG_SENIOR_MIX,lpObj,&item) == 0)
	{
		this->GCChaosMixSend(lpObj->Index,0,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	this->m_SeniorMixLimitDay = SystemTime.wDay;

	this->m_SeniorMixLimitMonth = SystemTime.wMonth;

	this->m_SeniorMixLimitYear = SystemTime.wYear;

	GDCreateItemSend(lpObj->Index,0xFF,0,0,item.m_Index,(BYTE)item.m_Level,0,item.m_Option1,item.m_Option2,item.m_Option3,-1,item.m_NewOption,item.m_SetOption,0);

	gLog.Output(LOG_CHAOS_MIX,"[SeniorMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
}

void CChaosBox::CGChaosMixRecv(PMSG_CHAOS_MIX_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->ChaosLock != 0)
	{
		return;
	}

	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->ChaosLock = 1;

	lpObj->ChaosMoney = 0;

	lpObj->ChaosSuccessRate = 0;

	lpObj->IsChaosMixCompleted = 1;

	switch(lpMsg->type)
	{
		case CHAOS_MIX_CHAOS_ITEM:
			this->ChaosItemMix(lpObj);
			break;
		case CHAOS_MIX_DEVIL_SQUARE:
			this->DevilSquareMix(lpObj);
			break;
		case CHAOS_MIX_PLUS_ITEM_LEVEL1:
			this->PlusItemLevelMix(lpObj,0);
			break;
		case CHAOS_MIX_PLUS_ITEM_LEVEL2:
			this->PlusItemLevelMix(lpObj,1);
			break;
		case CHAOS_MIX_DINORANT:
			this->DinorantMix(lpObj);
			break;
		case CHAOS_MIX_FRUIT:
			this->FruitMix(lpObj);
			break;
		case CHAOS_MIX_WING1:
			this->Wing2Mix(lpObj,0);
			break;
		case CHAOS_MIX_BLOOD_CASTLE:
			this->BloodCastleMix(lpObj);
			break;
		case CHAOS_MIX_WING2:
			this->Wing1Mix(lpObj);
			break;
		case CHAOS_MIX_PET1:
			this->PetMix(lpObj,0);
			break;
		case CHAOS_MIX_PET2:
			this->PetMix(lpObj,1);
			break;
		case CHAOS_MIX_SIEGE_POTION1:
			this->SiegePotionMix(lpObj,0);
			break;
		case CHAOS_MIX_SIEGE_POTION2:
			this->SiegePotionMix(lpObj,1);
			break;
		case CHAOS_MIX_LIFE_STONE:
			this->LifeStoneMix(lpObj);
			break;
		case CHAOS_MIX_SENIOR:
			this->SeniorMix(lpObj);
			break;
		case CHAOS_MIX_PLUS_ITEM_LEVEL3:
			this->PlusItemLevelMix(lpObj,2);
			break;
		case CHAOS_MIX_PLUS_ITEM_LEVEL4:
			this->PlusItemLevelMix(lpObj,3);
			break;
	}
}

void CChaosBox::CGChaosMixCloseRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOS_BOX)
	{
		return;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;

	this->ChaosBoxInit(lpObj);

	gObjInventoryCommit(aIndex);

	lpObj->ChaosLock = 0;

	lpObj->IsChaosMixCompleted = 0;

	lpObj->IsCastleNPCUpgradeCompleted = 0;

	PBMSG_HEAD pMsg;

	pMsg.set(0x87,sizeof(pMsg));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.size);
}

void CChaosBox::GCChaosBoxSend(LPOBJ lpObj,BYTE type) // OK
{
	if((type == 0 && (lpObj->Interface.type != INTERFACE_CHAOS_BOX || lpObj->Interface.state == 1)) || (type != 0 && (lpObj->Interface.type != INTERFACE_TRAINER || lpObj->Interface.state == 1)))
	{
		return;
	}

	BYTE send[2048];

	PMSG_SHOP_ITEM_LIST_SEND pMsg;

	pMsg.header.set(0x31,0);

	int size = sizeof(pMsg);

	pMsg.type = ((type==0)?3:5);

	pMsg.count = 0;

	PMSG_SHOP_ITEM_LIST info;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		info.slot = n;

		gItemManager.ItemByteConvert(info.ItemInfo,lpObj->ChaosBox[n]);

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpObj->Index,send,size);
}

void CChaosBox::GCChaosMixSend(int aIndex,BYTE result,CItem* lpItem) // OK
{
	PMSG_CHAOS_MIX_SEND pMsg;

	pMsg.header.set(0x86,sizeof(pMsg));

	pMsg.result = result;

	if(lpItem == 0)
	{
		memset(pMsg.ItemInfo,0xFF,sizeof(pMsg.ItemInfo));
	}
	else
	{
		gItemManager.ItemByteConvert(pMsg.ItemInfo,(*lpItem));
	}

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	gObj[aIndex].ChaosLock = 0;
}
