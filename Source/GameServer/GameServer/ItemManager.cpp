// ItemManager.cpp: implementation of the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemManager.h"
#include "BloodCastle.h"
#include "CastleSiegeSync.h"
#include "DSProtocol.h"
#include "Duel.h"
#include "EffectManager.h"
#include "Fruit.h"
#include "ItemBagManager.h"
#include "ItemMove.h"
#include "ItemStack.h"
#include "Kalima.h"
#include "LifeStone.h"
#include "Map.h"
#include "MemScript.h"
#include "Mercenary.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "PersonalShop.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "ServerInfo.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "Trade.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"

CItemManager gItemManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemManager::CItemManager() // OK
{
	this->m_ItemInfo.clear();
}

CItemManager::~CItemManager() // OK
{

}

void CItemManager::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_ItemInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (strcmp("end", lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				ITEM_INFO info;

				memset(&info, 0, sizeof(info));

				info.Index = SafeGetItem(GET_ITEM(section, lpMemScript->GetNumber()));
				info.Slot = lpMemScript->GetAsNumber();
				info.Skill = lpMemScript->GetAsNumber();
				info.Width = lpMemScript->GetAsNumber();
				info.Height = lpMemScript->GetAsNumber();
				info.HaveSerial = lpMemScript->GetAsNumber();
				info.HaveOption = lpMemScript->GetAsNumber();
				info.DropItem = lpMemScript->GetAsNumber();

				strcpy_s(info.Name, lpMemScript->GetAsString());

				if (section >= 0 && section <= 5)
				{
					info.Level = lpMemScript->GetAsNumber();
					info.DamageMin = lpMemScript->GetAsNumber();
					info.DamageMax = lpMemScript->GetAsNumber();
					info.AttackSpeed = lpMemScript->GetAsNumber();
					info.Durability = lpMemScript->GetAsNumber();
					info.MagicDurability = lpMemScript->GetAsNumber();
					info.MagicDamageRate = lpMemScript->GetAsNumber();
					info.RequireLevel = lpMemScript->GetAsNumber();
					info.RequireStrength = lpMemScript->GetAsNumber();
					info.RequireDexterity = lpMemScript->GetAsNumber();
					info.RequireEnergy = lpMemScript->GetAsNumber();
					info.RequireVitality = lpMemScript->GetAsNumber();
					info.RequireLeadership = lpMemScript->GetAsNumber();
					info.TwoHand = ((info.Width >= 2) ? 1 : 0);
					info.Durability += info.MagicDurability;
				}
				else if (section >= 6 && section <= 11)
				{
					info.Level = lpMemScript->GetAsNumber();

					if (section == 6)
					{
						info.Defense = lpMemScript->GetAsNumber();
						info.DefenseSuccessRate = lpMemScript->GetAsNumber();
					}
					else if (section >= 7 && section <= 9)
					{
						info.Defense = lpMemScript->GetAsNumber();
						info.MagicDefense = lpMemScript->GetAsNumber();
					}
					else if (section == 10)
					{
						info.Defense = lpMemScript->GetAsNumber();
						info.AttackSpeed = lpMemScript->GetAsNumber();
					}
					else if (section == 11)
					{
						info.Defense = lpMemScript->GetAsNumber();
						info.WalkSpeed = lpMemScript->GetAsNumber();
					}

					info.Durability = lpMemScript->GetAsNumber();
					info.RequireLevel = lpMemScript->GetAsNumber();
					info.RequireStrength = lpMemScript->GetAsNumber();
					info.RequireDexterity = lpMemScript->GetAsNumber();
					info.RequireEnergy = lpMemScript->GetAsNumber();
					info.RequireVitality = lpMemScript->GetAsNumber();
					info.RequireLeadership = lpMemScript->GetAsNumber();
				}
				else if (section == 12)
				{
					info.Level = lpMemScript->GetAsNumber();
					info.Defense = lpMemScript->GetAsNumber();
					info.Durability = lpMemScript->GetAsNumber();
					info.RequireLevel = lpMemScript->GetAsNumber();
					info.RequireEnergy = lpMemScript->GetAsNumber();
					info.RequireStrength = lpMemScript->GetAsNumber();
					info.RequireDexterity = lpMemScript->GetAsNumber();
					info.RequireLeadership = lpMemScript->GetAsNumber();
					info.BuyMoney = lpMemScript->GetAsNumber();
				}
				else if (section == 13)
				{
					info.Level = lpMemScript->GetAsNumber();
					info.Durability = lpMemScript->GetAsNumber();

					for (int n = 0; n < MAX_RESISTANCE_TYPE; n++)
					{
						info.Resistance[n] = lpMemScript->GetAsNumber();
					}

					info.RequireLevel = info.Level;
				}
				else if (section == 14)
				{
					info.Value = lpMemScript->GetAsNumber();
					info.Level = lpMemScript->GetAsNumber();
					info.Durability = 1;
				}
				else if (section == 15)
				{
					info.Level = lpMemScript->GetAsNumber();
					info.RequireLevel = lpMemScript->GetAsNumber();
					info.RequireEnergy = lpMemScript->GetAsNumber();
					info.BuyMoney = lpMemScript->GetAsNumber();
					info.DamageMin = info.Level;
					info.DamageMax = info.Level + (info.Level / 2);
				}

				if (section <= 11 || section == 13)
				{
					lpMemScript->GetToken();
				}

				if (section != 14)
				{
					for (int n = 0; n < MAX_CLASS; n++)
					{
						info.RequireClass[n] = lpMemScript->GetAsNumber();
					}
				}

				this->m_ItemInfo[info.Index] = info;
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemManager::GetInfo(int index, ITEM_INFO* lpInfo) // OK
{
	auto it = this->m_ItemInfo.find(index);

	if (it == this->m_ItemInfo.end())
	{
		return 0;
	}
	else
	{
		(*lpInfo) = it->second;
		return 1;
	}
}

int CItemManager::GetItemSkill(int index) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return -1;
	}
	else
	{
		return ItemInfo.Skill;
	}
}

int CItemManager::GetItemTwoHand(int index) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return -1;
	}
	else
	{
		return ItemInfo.TwoHand;
	}
}

int CItemManager::GetItemDurability(int index, int level, int NewOption, int SetOption) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return 0;
	}


	if (index == GET_ITEM(14, 21) || index == GET_ITEM(14, 29)) // Rena,Symbol of Kundun
	{
		return 1;
	}

	if (index == GET_ITEM(13, 18) || index == GET_ITEM(13, 29) || index == GET_ITEM(14, 19)) // Invisibility Cloak,Armor of Guardsman,Devil's Invitation
	{
		return 1;
	}

	int dur = 0;

	if (level >= 5)
	{
		if (level == 10)
		{
			dur = ItemInfo.Durability + ((level * 2) - 3);
		}
		else if (level == 11)
		{
			dur = ItemInfo.Durability + ((level * 2) - 1);
		}
		else if (level == 12)
		{
			dur = ItemInfo.Durability + ((level * 2) + 2);
		}
		else if (level == 13)
		{
			dur = ItemInfo.Durability + ((level * 2) + 6);
		}
		else if (level == 14)
		{
			dur = ItemInfo.Durability + ((level * 2) + 11);
		}
		else if (level == 15)
		{
			dur = ItemInfo.Durability + ((level * 2) + 17);
		}
		else
		{
			dur = ItemInfo.Durability + ((level * 2) - 4);
		}
	}
	else
	{
		dur = ItemInfo.Durability + level;
	}

	if (index != GET_ITEM(0, 19) && index != GET_ITEM(2, 13) && index != GET_ITEM(4, 18) && index != GET_ITEM(5, 10) && ItemInfo.Slot != 7) // Sword of Archangel,Scepter of Archangel,Crossbow of Archangel,Staff of Archangel
	{
		if (SetOption != 0)
		{
			dur += 20;
		}
		else if (NewOption != 0)
		{
			dur += 15;
		}
	}

	return ((dur > 255) ? 255 : dur);
}

int CItemManager::GetItemRepairMoney(CItem* lpItem, int type) // OK
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (lpItem->m_Durability == lpItem->m_BaseDurability)
	{
		return 0;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(lpItem->m_Index, &ItemInfo) == 0)
	{
		return 0;
	}

	if (ItemInfo.Durability == 0)
	{
		return 0;
	}

	int RepairMoney = 0;

	if (lpItem->m_Index == GET_ITEM(13, 4) || lpItem->m_Index == GET_ITEM(13, 5)) // Dark Horse,Dark Reaven
	{
		RepairMoney = lpItem->m_BuyMoney / 1;
	}
	else
	{
		RepairMoney = lpItem->m_BuyMoney / 3;
	}

	RepairMoney = ((RepairMoney > MAX_MONEY) ? MAX_MONEY : RepairMoney);

	RepairMoney = ((RepairMoney >= 100) ? ((RepairMoney / 10) * 10) : RepairMoney);

	RepairMoney = ((RepairMoney >= 1000) ? ((RepairMoney / 100) * 100) : RepairMoney);

	float sq1 = sqrt((float)RepairMoney);
	float sq2 = sqrt(sq1);

	float value = (float)((((3.0 * sq1) * sq2) * (1 - (lpItem->m_Durability / lpItem->m_BaseDurability))) + 1);

	if (lpItem->m_Durability <= 0)
	{
		if (lpItem->m_Index == GET_ITEM(13, 4) || lpItem->m_Index == GET_ITEM(13, 5)) // Dark Horse,Dark Reaven
		{
			value *= (float)2.0f;
		}
		else
		{
			value *= (float)1.4f;
		}
	}

	int money = (int)((type == 1) ? (value * 2.5f) : value);

	money = ((money >= 100) ? ((money / 10) * 10) : money);

	money = ((money >= 1000) ? ((money / 100) * 100) : money);

	return money;
}

int CItemManager::GetInventoryItemSlot(LPOBJ lpObj, int index, int level) // OK
{
	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = 0; n < MaxValue; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Index == index && (level == -1 || lpObj->Inventory[n].m_Level == level))
			{
				return n;
			}
		}
	}

	return -1;
}

int CItemManager::GetInventoryItemCount(LPOBJ lpObj, int index, int level) // OK
{
	int count = 0;

	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = 0; n < MaxValue; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Index == index && (level == -1 || lpObj->Inventory[n].m_Level == level))
			{
				if (gItemStack.GetItemMaxStack(lpObj->Inventory[n].m_Index) == 0)
				{
					count++;
				}
				else
				{
					count += (int)lpObj->Inventory[n].m_Durability;
				}
			}
		}
	}

	return count;
}

int CItemManager::GetInventoryEmptySlotCount(LPOBJ lpObj) // OK
{
	int count = 0;

	int MaxValue = this->GetInventoryMaxValue(lpObj) - INVENTORY_WEAR_SIZE;

	for (int n = 0; n < MaxValue; n++)
	{
		if (lpObj->InventoryMap[n] == 0xFF)
		{
			count++;
		}
	}

	return count;
}

int CItemManager::GetInventoryMaxValue(LPOBJ lpObj) // OK
{
	int MaxValue = INVENTORY_MAIN_SIZE;

	switch (lpObj->ExtInventory)
	{
		case 0:
			MaxValue = INVENTORY_MAIN_SIZE;
			break;
		case 1:
			MaxValue = INVENTORY_EXT1_SIZE;
			break;
		case 2:
			MaxValue = INVENTORY_EXT2_SIZE;
			break;
		case 3:
			MaxValue = INVENTORY_EXT3_SIZE;
			break;
		case 4:
			MaxValue = INVENTORY_EXT4_SIZE;
			break;
		default:
			MaxValue = INVENTORY_EXT4_SIZE;
			break;
	}

	return MaxValue;
}

bool CItemManager::CheckItemRequireLevel(LPOBJ lpObj, CItem* lpItem) // OK
{
	if (lpObj->Level >= lpItem->m_RequireLevel)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireStrength(LPOBJ lpObj, CItem* lpItem) // OK
{
	if ((lpObj->Strength + lpObj->AddStrength) >= lpItem->m_RequireStrength)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireDexterity(LPOBJ lpObj, CItem* lpItem) // OK
{
	if ((lpObj->Dexterity + lpObj->AddDexterity) >= lpItem->m_RequireDexterity)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireVitality(LPOBJ lpObj, CItem* lpItem) // OK
{
	if ((lpObj->Vitality + lpObj->AddVitality) >= lpItem->m_RequireVitality)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireEnergy(LPOBJ lpObj, CItem* lpItem) // OK
{
	if ((lpObj->Energy + lpObj->AddEnergy) >= lpItem->m_RequireEnergy)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireLeadership(LPOBJ lpObj, CItem* lpItem) // OK
{
	if ((lpObj->Leadership + lpObj->AddLeadership) >= lpItem->m_RequireLeadership)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireClass(LPOBJ lpObj, int index) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return 0;
	}

	if (CHECK_RANGE(lpObj->Class, MAX_CLASS) == 0)
	{
		return 0;
	}

	if (ItemInfo.RequireClass[lpObj->Class] == 0)
	{
		return 0;
	}

	if ((lpObj->ChangeUp + 1) >= ItemInfo.RequireClass[lpObj->Class])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemMoveToInventory(LPOBJ lpObj, CItem* lpItem, int slot) // OK
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		return 1;
	}

	if (this->CheckItemRequireLevel(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireStrength(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireDexterity(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireVitality(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireEnergy(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireLeadership(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireClass(lpObj, lpItem->m_Index) == 0)
	{
		return 0;
	}

	if (lpItem->m_Slot != slot && slot != 0 && slot != 1 && slot != 10 && slot != 11)
	{
		return 0;
	}

	if (lpItem->m_Slot != slot && ((slot == 0 || slot == 1) && lpItem->m_Slot != 0 && lpItem->m_Slot != 1))
	{
		return 0;
	}

	if (lpItem->m_Slot != slot && ((slot == 10 || slot == 11) && lpItem->m_Slot != 10 && lpItem->m_Slot != 11))
	{
		return 0;
	}

	if (slot == 0 && lpObj->Inventory[1].IsItem() != 0)
	{
		if ((lpItem->m_TwoHand != 0 && lpObj->Inventory[1].m_Index != GET_ITEM(4, 7)) || (lpObj->Inventory[1].m_TwoHand != 0 && lpItem->m_Index != GET_ITEM(4, 15)))
		{
			return 0;
		}
	}

	if (slot == 1 && lpObj->Inventory[0].IsItem() != 0)
	{
		if ((lpItem->m_TwoHand != 0 && lpObj->Inventory[0].m_Index != GET_ITEM(4, 15)) || (lpObj->Inventory[0].m_TwoHand != 0 && lpItem->m_Index != GET_ITEM(4, 7)))
		{
			return 0;
		}
	}

	if (lpObj->Map == MAP_ATLANS)
	{
		if (lpItem->m_Index == GET_ITEM(13, 2) || lpItem->m_Index == GET_ITEM(13, 3)) // Uniria,Dinorant
		{
			return 0;
		}
	}

	if (CC_MAP_RANGE(lpObj->Map) != 0)
	{
		if (lpItem->m_Index == GET_ITEM(13, 10) || (lpItem->m_Index >= GET_ITEM(13, 32) && lpItem->m_Index <= GET_ITEM(13, 38))) // Rings
		{
			return 0;
		}
	}

	if (lpItem->m_Index == GET_ITEM(13, 10) || (lpItem->m_Index >= GET_ITEM(13, 32) && lpItem->m_Index <= GET_ITEM(13, 38)) || (lpItem->m_Index == GET_ITEM(13, 20) && lpItem->m_Level == 0)) // Rings
	{
		if (lpObj->Inventory[slot ^ 1].IsItem() != 0 && lpObj->Inventory[slot ^ 1].m_Index == lpItem->m_Index)
		{
			return 0;
		}
	}

	return 1;
}

bool CItemManager::CheckItemMoveToTrade(LPOBJ lpObj, CItem* lpItem, BYTE TargetFlag) // OK
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (lpItem->m_IsPeriodicItem != 0)
	{
		return 0;
	}

	if (gItemMove.CheckItemMoveAllowTrade(lpItem->m_Index) == 0)
	{
		return 0;
	}

	if (this->CheckItemMoveToBlock(lpObj, lpItem) == 0)
	{
		return 0;
	}

	return 1;
}

bool CItemManager::CheckItemMoveToVault(LPOBJ lpObj, CItem* lpItem, BYTE TargetFlag) // OK
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (lpItem->m_IsPeriodicItem != 0)
	{
		return 0;
	}

	if (gItemMove.CheckItemMoveAllowVault(lpItem->m_Index) == 0)
	{
		return 0;
	}

	return 1;
}

bool CItemManager::CheckItemMoveToChaos(LPOBJ lpObj, CItem* lpItem, BYTE TargetFlag) // OK
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (lpItem->m_IsPeriodicItem != 0)
	{
		return 0;
	}

	return 1;
}

bool CItemManager::CheckItemMoveToBlock(LPOBJ lpObj, CItem* lpItem) // OK
{
	if (gServerInfo.m_TradeItemBlock == 0 || lpObj->Authority == 32)
	{
		return 1;
	}

	if (gServerInfo.m_TradeItemBlockExc != 0 && lpItem->IsExcItem() != 0 && GetNewOptionCount(lpItem->m_NewOption) >= gServerInfo.m_TradeItemBlockExc)
	{
		return 0;
	}

	if (gServerInfo.m_TradeItemBlockSet != 0 && lpItem->IsSetItem() != 0)
	{
		return 0;
	}

	return 1;
}

bool CItemManager::CheckItemInventorySpace(LPOBJ lpObj, int index) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return 0;
	}

	int MaxY = (this->GetInventoryMaxValue(lpObj) - INVENTORY_WEAR_SIZE) / 8;

	for (int y = 0; y < MaxY; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (lpObj->InventoryMap[((y * 8) + x)] == 0xFF)
			{
				if (this->InventoryRectCheck(lpObj->Index, x, y, ItemInfo.Width, ItemInfo.Height) != 0xFF)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

bool CItemManager::CheckItemInventorySpace(LPOBJ lpObj, int width, int height) // OK
{
	int MaxY = (this->GetInventoryMaxValue(lpObj) - INVENTORY_WEAR_SIZE) / 8;

	for (int y = 0; y < MaxY; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (lpObj->InventoryMap[((y * 8) + x)] == 0xFF)
			{
				if (this->InventoryRectCheck(lpObj->Index, x, y, width, height) != 0xFF)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

void CItemManager::InventoryItemSet(int aIndex, int slot, BYTE type) // OK
{
	if (INVENTORY_BASE_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(gObj[aIndex].Inventory[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = (slot - INVENTORY_WEAR_SIZE) % 8;
	int y = (slot - INVENTORY_WEAR_SIZE) / 8;

	if (INVENTORY_MAIN_RANGE(slot) != 0 && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 8))
	{
		return;
	}

	if (INVENTORY_EXT1_RANGE(slot) != 0 && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 12))
	{
		return;
	}

	if (INVENTORY_EXT2_RANGE(slot) != 0 && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 16))
	{
		return;
	}

	if (INVENTORY_EXT3_RANGE(slot) != 0 && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 20))
	{
		return;
	}

	if (INVENTORY_EXT4_RANGE(slot) != 0 && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 24))
	{
		return;
	}

	if (INVENTORY_SHOP_RANGE(slot) != 0 && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 28))
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			gObj[aIndex].InventoryMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CItemManager::InventoryRectCheck(int aIndex, int x, int y, int width, int height) // OK
{
	int slot = ((y * 8) + x) + INVENTORY_WEAR_SIZE;

	if (INVENTORY_MAIN_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 8))
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 12))
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 16))
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 20))
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 24))
	{
		return 0xFF;
	}

	if (INVENTORY_SHOP_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 28))
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (gObj[aIndex].InventoryMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return slot;
}

BYTE CItemManager::InventoryInsertItem(int aIndex, CItem item) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int MaxY = (this->GetInventoryMaxValue(&gObj[aIndex]) - INVENTORY_WEAR_SIZE) / 8;

	for (int y = 0; y < MaxY; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gObj[aIndex].InventoryMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->InventoryRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					gObj[aIndex].Inventory[slot] = item;
					this->InventoryItemSet(aIndex, slot, 1);
					return slot;
				}
			}
		}
	}

	return 0xFF;
}

BYTE CItemManager::InventoryAddItem(int aIndex, CItem item, int slot) // OK
{
	if (INVENTORY_RANGE(slot) == 0)
	{
		return 0xFF;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Inventory[slot].IsItem() != 0 || item.IsItem() == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		ITEM_INFO ItemInfo;

		if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
		{
			return 0xFF;
		}

		int x = (slot - INVENTORY_WEAR_SIZE) % 8;
		int y = (slot - INVENTORY_WEAR_SIZE) / 8;

		if (this->InventoryRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height) == 0xFF)
		{
			return 0xFF;
		}

		lpObj->Inventory[slot] = item;

		this->InventoryItemSet(aIndex, slot, 1);

		return slot;
	}
	else
	{
		if (this->CheckItemRequireClass(lpObj, item.m_Index) == 0)
		{
			return 0xFF;
		}

		lpObj->Inventory[slot] = item;

		if (lpObj->Inventory[slot].m_Option1 != 0)
		{
			gSkillManager.AddSkillWeapon(lpObj, lpObj->Inventory[slot].m_SpecialIndex[SPECIAL_OPTION1], lpObj->Inventory[slot].m_Level);
		}

		return slot;
	}
}

void CItemManager::InventoryDelItem(int aIndex, int slot) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		this->InventoryItemSet(aIndex, slot, 0xFF);
		lpObj->Inventory[slot].Clear();
	}
	else
	{
		if (lpObj->Inventory[slot].m_Option1 != 0)
		{
			gSkillManager.DelSkillWeapon(lpObj, lpObj->Inventory[slot].m_SpecialIndex[SPECIAL_OPTION1], lpObj->Inventory[slot].m_Level);
		}
		lpObj->Inventory[slot].Clear();
	}
}

bool CItemManager::InventoryInsertItemStack(LPOBJ lpObj, CItem* lpItem) // OK
{
	int MaxStack, CreateItemIndex;

	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if ((MaxStack = gItemStack.GetItemMaxStack(lpItem->m_Index)) <= 0)
	{
		return 0;
	}

	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = INVENTORY_WEAR_SIZE; n < MaxValue; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Index == lpItem->m_Index && lpObj->Inventory[n].m_Level == lpItem->m_Level)
			{
				if (lpObj->Inventory[n].m_Durability < MaxStack)
				{
					int AddDur = (int)(MaxStack - lpObj->Inventory[n].m_Durability);

					AddDur = ((AddDur > lpItem->m_Durability) ? (int)lpItem->m_Durability : AddDur);

					lpItem->m_Durability -= AddDur;

					lpObj->Inventory[n].m_Durability += AddDur;

					if (lpObj->Inventory[n].m_Durability == MaxStack && (CreateItemIndex = gItemStack.GetCreateItemIndex(lpItem->m_Index)) != -1)
					{
						this->InventoryDelItem(lpObj->Index, n);
						this->GCItemDeleteSend(lpObj->Index, n, 1);

						GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, CreateItemIndex, (BYTE)lpItem->m_Level, 1, 0, 0, 0, lpObj->Index, 0, 0, 0, 0);

						if (lpItem->m_Durability < 1)
						{
							return 1;
						}
					}
					else
					{
						this->GCItemDurSend(lpObj->Index, n, (BYTE)lpObj->Inventory[n].m_Durability, 0);

						if (lpItem->m_Durability < 1)
						{
							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}

bool CItemManager::InventoryAddItemStack(LPOBJ lpObj, int SourceSlot, int TargetSlot) // OK
{
	int MaxStack, CreateItemIndex;

	if (lpObj->Inventory[SourceSlot].IsItem() == 0 || lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[SourceSlot].m_Index != lpObj->Inventory[TargetSlot].m_Index || lpObj->Inventory[SourceSlot].m_Level != lpObj->Inventory[TargetSlot].m_Level)
	{
		return 0;
	}

	if ((MaxStack = gItemStack.GetItemMaxStack(lpObj->Inventory[SourceSlot].m_Index)) <= 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].m_Durability >= MaxStack)
	{
		return 0;
	}

	int AddDur = (int)(MaxStack - lpObj->Inventory[TargetSlot].m_Durability);

	AddDur = ((AddDur > lpObj->Inventory[SourceSlot].m_Durability) ? (int)lpObj->Inventory[SourceSlot].m_Durability : AddDur);

	lpObj->Inventory[SourceSlot].m_Durability -= AddDur;

	lpObj->Inventory[TargetSlot].m_Durability += AddDur;

	if (lpObj->Inventory[TargetSlot].m_Durability == MaxStack && (CreateItemIndex = gItemStack.GetCreateItemIndex(lpObj->Inventory[SourceSlot].m_Index)) != -1)
	{
		this->InventoryDelItem(lpObj->Index, TargetSlot);
		this->GCItemDeleteSend(lpObj->Index, TargetSlot, 1);

		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, CreateItemIndex, (BYTE)lpObj->Inventory[SourceSlot].m_Level, 0, 0, 0, 0, lpObj->Index, 0, 0, 0, 0);
	}
	else
	{
		this->GCItemDurSend(lpObj->Index, TargetSlot, (BYTE)lpObj->Inventory[TargetSlot].m_Durability, 0);
	}

	if (lpObj->Inventory[SourceSlot].m_Durability < 1)
	{
		this->InventoryDelItem(lpObj->Index, SourceSlot);
		this->GCItemDeleteSend(lpObj->Index, SourceSlot, 1);
	}

	return 1;
}

void CItemManager::TradeItemSet(int aIndex, int slot, BYTE type) // OK
{
	if (TRADE_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(gObj[aIndex].Trade[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = slot % 8;
	int y = slot / 8;

	if ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 4)
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			gObj[aIndex].TradeMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CItemManager::TradeRectCheck(int aIndex, int x, int y, int width, int height) // OK
{
	if ((x + width) > 8 || (y + height) > 4)
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (gObj[aIndex].TradeMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return ((y * 8) + x);
}

BYTE CItemManager::TradeInsertItem(int aIndex, CItem item) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gObj[aIndex].TradeMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->TradeRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					gObj[aIndex].Trade[slot] = item;
					this->TradeItemSet(aIndex, slot, 1);
					return slot;
				}
			}
		}
	}

	return 0xFF;
}

BYTE CItemManager::TradeAddItem(int aIndex, CItem item, int slot) // OK
{
	if (TRADE_RANGE(slot) == 0)
	{
		return 0xFF;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Trade[slot].IsItem() != 0 || item.IsItem() == 0)
	{
		return 0xFF;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int x = slot % 8;
	int y = slot / 8;

	if (this->TradeRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height) == 0xFF)
	{
		return 0xFF;
	}

	lpObj->Trade[slot] = item;
	this->TradeItemSet(aIndex, slot, 1);
	return slot;
}

void CItemManager::TradeDelItem(int aIndex, int slot) // OK
{
	this->TradeItemSet(aIndex, slot, 0xFF);
	gObj[aIndex].Trade[slot].Clear();
}

void CItemManager::WarehouseItemSet(int aIndex, int slot, BYTE type) // OK
{
	if (WAREHOUSE_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(gObj[aIndex].Warehouse[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = slot % 8;
	int y = slot / 8;

	if (WAREHOUSE_MAIN_RANGE(slot) != 0 && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 15))
	{
		return;
	}

	if (WAREHOUSE_EXT1_RANGE(slot) != 0 && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 30))
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			gObj[aIndex].WarehouseMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CItemManager::WarehouseRectCheck(int aIndex, int x, int y, int width, int height) // OK
{
	int slot = ((y * 8) + x);

	if (WAREHOUSE_MAIN_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 15))
	{
		return 0xFF;
	}

	if (WAREHOUSE_EXT1_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 30))
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (gObj[aIndex].WarehouseMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return slot;
}

BYTE CItemManager::WarehouseInsertItem(int aIndex, CItem item) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int MaxY = ((gObj[aIndex].ExtWarehouse == 0) ? WAREHOUSE_EXT1_SIZE : WAREHOUSE_SIZE) / 8;

	for (int y = 0; y < MaxY; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gObj[aIndex].WarehouseMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->WarehouseRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					gObj[aIndex].Warehouse[slot] = item;
					this->WarehouseItemSet(aIndex, slot, 1);
					return slot;
				}
			}
		}
	}

	return 0xFF;
}

BYTE CItemManager::WarehouseAddItem(int aIndex, CItem item, int slot) // OK
{
	if (WAREHOUSE_RANGE(slot) == 0)
	{
		return 0xFF;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Warehouse[slot].IsItem() != 0 || item.IsItem() == 0)
	{
		return 0xFF;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int x = slot % 8;
	int y = slot / 8;

	if (this->WarehouseRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height) == 0xFF)
	{
		return 0xFF;
	}

	lpObj->Warehouse[slot] = item;
	this->WarehouseItemSet(aIndex, slot, 1);
	return slot;
}

void CItemManager::WarehouseDelItem(int aIndex, int slot) // OK
{
	this->WarehouseItemSet(aIndex, slot, 0xFF);
	gObj[aIndex].Warehouse[slot].Clear();
}

void CItemManager::ChaosBoxItemSet(int aIndex, int slot, BYTE type) // OK
{
	if (CHAOS_BOX_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(gObj[aIndex].ChaosBox[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = slot % 8;
	int y = slot / 8;

	if ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 4)
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			gObj[aIndex].ChaosBoxMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CItemManager::ChaosBoxRectCheck(int aIndex, int x, int y, int width, int height) // OK
{
	if ((x + width) > 8 || (y + height) > 4)
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (gObj[aIndex].ChaosBoxMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return ((y * 8) + x);
}

BYTE CItemManager::ChaosBoxInsertItem(int aIndex, CItem item) // OK
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gObj[aIndex].ChaosBoxMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->ChaosBoxRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					gObj[aIndex].ChaosBox[slot] = item;
					this->ChaosBoxItemSet(aIndex, slot, 1);
					return slot;
				}
			}
		}
	}

	return 0xFF;
}

BYTE CItemManager::ChaosBoxAddItem(int aIndex, CItem item, int slot) // OK
{
	if (CHAOS_BOX_RANGE(slot) == 0)
	{
		return 0xFF;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->ChaosBox[slot].IsItem() != 0 || item.IsItem() == 0)
	{
		return 0xFF;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int x = slot % 8;
	int y = slot / 8;

	if (this->ChaosBoxRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height) == 0xFF)
	{
		return 0xFF;
	}

	lpObj->ChaosBox[slot] = item;
	this->ChaosBoxItemSet(aIndex, slot, 1);
	return slot;
}

void CItemManager::ChaosBoxDelItem(int aIndex, int slot) // OK
{
	this->ChaosBoxItemSet(aIndex, slot, 0xFF);
	gObj[aIndex].ChaosBox[slot].Clear();
}

void CItemManager::ItemByteConvert(BYTE* lpMsg, CItem item) // OK
{
	memset(lpMsg, 0, 16);

	lpMsg[0] = item.m_Index & 0xFF;

	lpMsg[1] |= item.m_Level * 8;
	lpMsg[1] |= item.m_Option1 * 128;
	lpMsg[1] |= item.m_Option2 * 4;
	lpMsg[1] |= item.m_Option3 & 3;

	lpMsg[2] = (BYTE)item.m_Durability;

	lpMsg[3] |= (item.m_Index & 256) >> 1;
	lpMsg[3] |= ((item.m_Option3 > 3) ? 0x40 : 0);
	lpMsg[3] |= item.m_NewOption;

	lpMsg[4] = item.m_SetOption;

	lpMsg[5] |= (item.m_Index & GET_ITEM(15, 0)) >> 5;
	/*lpMsg[5] |= ((item.m_ItemOptionEx & 128) >> 4);
	lpMsg[5] |= ((item.m_IsPeriodicItem & 1) << 1);
	lpMsg[5] |= ((item.m_IsExpiredItem & 1) << 2);*/

	if (item.m_Index == GET_ITEM(13, 39) || item.m_Index == GET_ITEM(13, 40))
	{
		lpMsg[6] = item.m_Prism[0];
		lpMsg[7] = item.m_Prism[1];
		lpMsg[8] = item.m_Prism[2];
	}
}

void CItemManager::DBItemByteConvert(BYTE* lpMsg, CItem* lpItem) // OK
{
	if (lpItem->IsItem() == 0)
	{
		memset(lpMsg, 0xFF, 16);
		return;
	}

	if (lpItem->m_Index == GET_ITEM(13, 19))
	{
		memset(lpMsg, 0xFF, 16);
		return;
	}

	memset(lpMsg, 0, 16);

	lpMsg[0] = lpItem->m_Index & 0xFF;

	lpMsg[1] |= lpItem->m_Level * 8;
	lpMsg[1] |= lpItem->m_Option1 * 128;
	lpMsg[1] |= lpItem->m_Option2 * 4;
	lpMsg[1] |= lpItem->m_Option3 & 3;

	lpMsg[2] = (BYTE)lpItem->m_Durability;

	lpMsg[3] = SET_NUMBERHB(SET_NUMBERHW(lpItem->m_Serial));
	lpMsg[4] = SET_NUMBERLB(SET_NUMBERHW(lpItem->m_Serial));
	lpMsg[5] = SET_NUMBERHB(SET_NUMBERLW(lpItem->m_Serial));
	lpMsg[6] = SET_NUMBERLB(SET_NUMBERLW(lpItem->m_Serial));

	lpMsg[7] |= (lpItem->m_Index & 256) >> 1;
	lpMsg[7] |= ((lpItem->m_Option3 > 3) ? 64 : 0);
	lpMsg[7] |= lpItem->m_NewOption;

	lpMsg[8] |= lpItem->m_SetOption & 15;

	lpMsg[9] |= (lpItem->m_Index & GET_ITEM(15, 0)) >> 5;
	/*lpMsg[9] |= ((lpItem->m_ItemOptionEx & 128) >> 4);
	lpMsg[9] |= ((lpItem->m_IsPeriodicItem & 1) << 1);
	lpMsg[9] |= ((lpItem->m_IsExpiredItem & 1) << 2);*/

	if (lpItem->m_Index == GET_ITEM(13, 39) || lpItem->m_Index == GET_ITEM(13, 40))
	{
		lpMsg[10] = lpItem->m_Prism[0];
		lpMsg[11] = lpItem->m_Prism[1];
		lpMsg[12] = lpItem->m_Prism[2];
	}
}

bool CItemManager::ConvertItemByte(CItem* lpItem, BYTE* lpMsg) // OK
{
	if (lpMsg[0] == 0xFF && (lpMsg[7] & 0x80) == 0x80 && (lpMsg[9] & 0xF0) == 0xF0)
	{
		return 0;
	}

	lpItem->Clear();

	lpItem->m_Index = lpMsg[0] | ((lpMsg[9] & 0xF0) * 32) | ((lpMsg[7] & 0x80) * 2);
	lpItem->m_Option1 = (lpMsg[1] / 128) & 1;
	lpItem->m_Option2 = (lpMsg[1] / 4) & 1;
	lpItem->m_Option3 = (lpMsg[1] & 3) + ((lpMsg[7] & 64) / 16);
	lpItem->m_Level = (lpMsg[1] / 8) & 15;
	lpItem->m_Durability = lpMsg[2];
	lpItem->m_Durability = lpItem->m_Durability;

	int MaxStack = gItemStack.GetItemMaxStack(lpItem->m_Index);

	lpItem->m_Durability = ((MaxStack == 0) ? lpItem->m_Durability : ((lpItem->m_Durability > MaxStack) ? MaxStack : lpItem->m_Durability));
	lpItem->m_Serial = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg[3], lpMsg[4]), MAKE_NUMBERW(lpMsg[5], lpMsg[6]));
	lpItem->m_NewOption = lpMsg[7] & 63;
	lpItem->m_SetOption = lpMsg[8] & 15;

	/*lpItem->m_ItemOptionEx = (lpMsg[9] & 8) * 16;
	lpItem->m_IsPeriodicItem = ((lpMsg[9] & 2) / 2) & 1;
	lpItem->m_IsExpiredItem = ((lpMsg[9] & 4) / 4) & 1;

	if (lpMsg[11] == 0 && lpMsg[12] == 0 && lpMsg[13] == 0 && lpMsg[14] == 0 && lpMsg[15] == 0)
	{
		memset(lpItem->m_SocketOption, 0xFF, MAX_SOCKET_OPTION);
	}
	else
	{
		memcpy(lpItem->m_SocketOption, &lpMsg[11], MAX_SOCKET_OPTION);
	}*/

	if (lpItem->m_Index == GET_ITEM(13, 39) || lpItem->m_Index == GET_ITEM(13, 40))
	{
		memcpy(lpItem->m_Prism, &lpMsg[10], sizeof(lpItem->m_Prism));
	}

	lpItem->Convert(lpItem->m_Index, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption);
	return 1;
}

bool CItemManager::IsValidItem(LPOBJ lpObj, CItem* lpItem) // OK
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireLevel(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireStrength(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireDexterity(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireVitality(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireEnergy(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireLeadership(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireClass(lpObj, lpItem->m_Index) == 0)
	{
		return 0;
	}

	return 1;
}

void CItemManager::UpdateInventoryViewport(int aIndex, int slot) // OK
{
	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		return;
	}

	if (slot == 10 || slot == 11)
	{
		if (gSkillManager.SkillChangeUse(aIndex) != 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
			gObjectManager.CharacterUpdateMapEffect(&gObj[aIndex]);
		}
	}

	this->GCItemChangeSend(aIndex, slot);
}

void CItemManager::DeleteInventoryItemCount(LPOBJ lpObj, int index, int level, int count) // OK
{
	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = 0; n < MaxValue, count > 0; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Index == index && lpObj->Inventory[n].m_Level == level)
			{
				if (gItemStack.GetItemMaxStack(lpObj->Inventory[n].m_Index) <= 0)
				{
					this->InventoryDelItem(lpObj->Index, n);
					this->GCItemDeleteSend(lpObj->Index, n, 1);
					count--;
				}
				else
				{
					if (lpObj->Inventory[n].m_Durability > count)
					{
						lpObj->Inventory[n].m_Durability -= count;
						this->GCItemDurSend(lpObj->Index, n, (BYTE)lpObj->Inventory[n].m_Durability, 0);
						count = 0;
					}
					else
					{
						count -= (int)lpObj->Inventory[n].m_Durability;
						this->InventoryDelItem(lpObj->Index, n);
						this->GCItemDeleteSend(lpObj->Index, n, 1);
					}
				}
			}
		}
	}
}

void CItemManager::DecreaseItemDur(LPOBJ lpObj, int slot, int dur) // OK
{
	if (INVENTORY_FULL_RANGE(slot) == 0)
	{
		return;
	}

	if (lpObj->Inventory[slot].IsItem() == 0)
	{
		return;
	}

	if (lpObj->Inventory[slot].m_Durability > dur)
	{
		lpObj->Inventory[slot].m_Durability -= dur;
		this->GCItemDurSend(lpObj->Index, slot, (BYTE)lpObj->Inventory[slot].m_Durability, 0);
	}
	else
	{
		this->InventoryDelItem(lpObj->Index, slot);
		this->GCItemDeleteSend(lpObj->Index, slot, 1);
	}
}

int CItemManager::RepairItem(LPOBJ lpObj, CItem* lpItem, int slot, int type) // OK
{
	if (lpItem->m_Slot == 0xFF || INVENTORY_WEAR_RANGE(lpItem->m_Slot) == 0)
	{
		return 0;
	}

	if (lpItem->m_Slot == 8 && lpItem->m_Index != GET_ITEM(13, 4))
	{
		return 0;
	}

	if (lpItem->m_IsPeriodicItem != 0)
	{
		return 0;
	}

	if (lpItem->m_Index == GET_ITEM(4, 7) || lpItem->m_Index == GET_ITEM(4, 15)) // Bolt,Arrow
	{
		return 0;
	}

	if (lpObj->Interface.type != INTERFACE_TRAINER && (lpItem->m_Index == GET_ITEM(13, 4) || lpItem->m_Index == GET_ITEM(13, 5)))
	{
		return 0;
	}

	if (lpItem->m_Index == GET_ITEM(13, 20) && (lpItem->m_Level >= 0 && lpItem->m_Level <= 2)) // Wizard's Ring,Starter Ring
	{
		return 0;
	}

	if (lpItem->m_Index == GET_ITEM(13, 10) || (lpItem->m_Index >= GET_ITEM(13, 32) && lpItem->m_Index <= GET_ITEM(13, 38))) // Rings
	{
		return 0;
	}

	int money = this->GetItemRepairMoney(lpItem, type);

	money = ((money < 0) ? 0 : money);

	if (lpObj->Money < ((DWORD)money))
	{
		return 0;
	}

	lpObj->Money -= money;

	lpItem->Convert(lpItem->m_Index, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption);

	lpItem->m_Durability = lpItem->m_BaseDurability;

	if (INVENTORY_WEAR_RANGE(slot) != 0 && lpItem->CheckDurabilityState() != 0)
	{
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}

	this->GCItemDurSend(lpObj->Index, slot, (BYTE)lpItem->m_Durability, 0);

	if (lpItem->m_Index == GET_ITEM(13, 4))
	{
		GCPetItemInfoSend(lpObj->Index, 1, 0, slot, lpItem->m_PetItemLevel, lpItem->m_PetItemExp, (BYTE)lpItem->m_Durability);
	}

	if (lpItem->m_Index == GET_ITEM(13, 5))
	{
		GCPetItemInfoSend(lpObj->Index, 0, 0, slot, lpItem->m_PetItemLevel, lpItem->m_PetItemExp, (BYTE)lpItem->m_Durability);
	}

	return lpObj->Money;
}

BYTE CItemManager::MoveItemToInventoryFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->Inventory[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItemStack(lpObj, SourceSlot, TargetSlot) != 0)
	{
		return 0xFF;
	}

	BYTE map[INVENTORY_SIZE];

	memcpy(map, lpObj->InventoryMap, INVENTORY_SIZE);

	this->InventoryItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->InventoryAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->InventoryMap, map, INVENTORY_SIZE);
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);
	this->InventoryItemSet(lpObj->Index, TargetSlot, 1);
	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);
	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToInventoryFromTrade(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (TRADE_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Trade[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Trade[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->Trade[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItem(lpObj->Index, lpObj->Trade[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->TradeDelItem(lpObj->Index, SourceSlot);

	if (OBJECT_RANGE(lpObj->TargetNumber) != 0)
	{
		lpObj->TradeOk = 0;

		gTrade.GCTradeOkButtonSend(lpObj->Index, 0);

		gObj[lpObj->TargetNumber].TradeOk = 0;

		gTrade.GCTradeOkButtonSend(lpObj->TargetNumber, 2);
		gTrade.GCTradeItemDelSend(lpObj->TargetNumber, SourceSlot);
	}

	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToInventoryFromWarehouse(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (WAREHOUSE_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (WAREHOUSE_EXT1_RANGE(SourceSlot) != 0 && lpObj->ExtWarehouse < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	int tax = gWarehouse.GetWarehouseTaxMoney(lpObj->Level, lpObj->WarehousePW);

	if (lpObj->Money < ((DWORD)tax) && lpObj->WarehouseMoney < tax)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Warehouse[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Warehouse[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->Warehouse[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItem(lpObj->Index, lpObj->Warehouse[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->WarehouseDelItem(lpObj->Index, SourceSlot);

	if (lpObj->Money >= ((DWORD)tax))
	{
		lpObj->Money -= tax;
		gWarehouse.GCWarehouseMoneySend(lpObj->Index, 1, lpObj->Money, lpObj->WarehouseMoney);
	}
	else if (lpObj->WarehouseMoney >= tax)
	{
		lpObj->WarehouseMoney -= tax;
		gWarehouse.GCWarehouseMoneySend(lpObj->Index, 1, lpObj->Money, lpObj->WarehouseMoney);
	}

	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToInventoryFromChaosBox(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (CHAOS_BOX_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->ChaosBox[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->ChaosBox[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->ChaosBox[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItem(lpObj->Index, lpObj->ChaosBox[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->ChaosBoxDelItem(lpObj->Index, SourceSlot);
	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToInventoryFromPersonalShop(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (INVENTORY_SHOP_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(TargetSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->Inventory[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);
	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToTradeFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (TRADE_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToTrade(lpObj, &lpObj->Inventory[SourceSlot], TargetFlag) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->TradeAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);

	BYTE ItemInfo[MAX_ITEM_INFO];

	this->ItemByteConvert(ItemInfo, lpObj->Trade[TargetSlot]);

	gTrade.GCTradeItemAddSend(lpObj->TargetNumber, TargetSlot, ItemInfo);

	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);

	return 1;
}

BYTE CItemManager::MoveItemToTradeFromTrade(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (TRADE_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (TRADE_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Trade[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Trade[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	BYTE map[TRADE_SIZE];

	memcpy(map, lpObj->TradeMap, TRADE_SIZE);

	this->TradeItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->TradeAddItem(lpObj->Index, lpObj->Trade[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->TradeMap, map, TRADE_SIZE);
		return 0xFF;
	}

	this->TradeDelItem(lpObj->Index, SourceSlot);
	this->TradeItemSet(lpObj->Index, TargetSlot, 1);

	gTrade.GCTradeItemDelSend(lpObj->TargetNumber, SourceSlot);

	BYTE ItemInfo[MAX_ITEM_INFO];

	this->ItemByteConvert(ItemInfo, lpObj->Trade[TargetSlot]);

	gTrade.GCTradeItemAddSend(lpObj->TargetNumber, TargetSlot, ItemInfo);

	return 1;
}

BYTE CItemManager::MoveItemToWarehouseFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (WAREHOUSE_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	if (WAREHOUSE_EXT1_RANGE(TargetSlot) != 0 && lpObj->ExtWarehouse < 1)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToVault(lpObj, &lpObj->Inventory[SourceSlot], TargetFlag) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->WarehouseAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);
	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);

	return 2;
}

BYTE CItemManager::MoveItemToWarehouseFromWarehouse(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (WAREHOUSE_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (WAREHOUSE_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (WAREHOUSE_EXT1_RANGE(TargetSlot) != 0 && lpObj->ExtWarehouse < 1)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Warehouse[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Warehouse[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	BYTE map[WAREHOUSE_SIZE];

	memcpy(map, lpObj->WarehouseMap, WAREHOUSE_SIZE);

	this->WarehouseItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->WarehouseAddItem(lpObj->Index, lpObj->Warehouse[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->WarehouseMap, map, WAREHOUSE_SIZE);
		return 0xFF;
	}

	this->WarehouseDelItem(lpObj->Index, SourceSlot);
	this->WarehouseItemSet(lpObj->Index, TargetSlot, 1);

	return 2;
}

BYTE CItemManager::MoveItemToChaosBoxFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (CHAOS_BOX_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT1_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 1)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT2_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 2)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT3_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 3)
	{
		return 0xFF;
	}

	if (INVENTORY_EXT4_RANGE(SourceSlot) != 0 && lpObj->ExtInventory < 4)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToChaos(lpObj, &lpObj->Inventory[SourceSlot], TargetFlag) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->ChaosBoxAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);
	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToChaosBoxFromChaosBox(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (CHAOS_BOX_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (CHAOS_BOX_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->ChaosBox[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->ChaosBox[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	BYTE map[CHAOS_BOX_SIZE];

	memcpy(map, lpObj->ChaosBoxMap, CHAOS_BOX_SIZE);

	this->ChaosBoxItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->ChaosBoxAddItem(lpObj->Index, lpObj->ChaosBox[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->ChaosBoxMap, map, CHAOS_BOX_SIZE);
		return 0xFF;
	}

	this->ChaosBoxDelItem(lpObj->Index, SourceSlot);
	this->ChaosBoxItemSet(lpObj->Index, TargetSlot, 1);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToPersonalShopFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (INVENTORY_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_SHOP_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToTrade(lpObj, &lpObj->Inventory[SourceSlot], TargetFlag) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);
	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);

	return 0;
}

BYTE CItemManager::MoveItemToPersonalShopFromPersonalShop(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag) // OK
{
	if (INVENTORY_SHOP_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_SHOP_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	BYTE map[INVENTORY_SIZE];

	memcpy(map, lpObj->InventoryMap, INVENTORY_SIZE);

	this->InventoryItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->InventoryAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->InventoryMap, map, INVENTORY_SIZE);
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);
	this->InventoryItemSet(lpObj->Index, TargetSlot, 1);

	return 0;
}

void CItemManager::CGItemGetRecv(PMSG_ITEM_GET_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_GET_SEND pMsg;

	pMsg.header.setE(0x22, sizeof(pMsg));

	pMsg.result = 0xFF;

	memset(pMsg.ItemInfo, 0, sizeof(pMsg.ItemInfo));

	if (lpObj->DieRegen != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type != INTERFACE_SHOP)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	int index = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	if (MAP_ITEM_RANGE(index) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (MAP_RANGE(lpObj->Map) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (gMap[lpObj->Map].CheckItemGive(aIndex, index) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	CMapItem* lpItem = &gMap[lpObj->Map].m_Item[index];

	if (gQuestObjective.CheckQuestObjectiveItemCount(lpObj, lpItem->m_Index, lpItem->m_Level) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if ((lpItem->m_Index == GET_ITEM(13, 20) && lpItem->m_Level == 0)) // Rings
	{
		if (this->GetInventoryItemCount(lpObj, lpItem->m_Index, lpItem->m_Level) > 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}
	}

	if (lpItem->m_Index == GET_ITEM(14, 15)) // Money
	{
		gMap[lpObj->Map].ItemGive(aIndex, index);

		if (gObjCheckMaxMoney(aIndex, lpItem->m_BuyMoney) == 0)
		{
			lpObj->Money = MAX_MONEY;
		}
		else
		{
			lpObj->Money += lpItem->m_BuyMoney;
		}

		pMsg.result = 0xFE;

		pMsg.ItemInfo[0] = SET_NUMBERHB(SET_NUMBERHW(lpObj->Money));
		pMsg.ItemInfo[1] = SET_NUMBERLB(SET_NUMBERHW(lpObj->Money));
		pMsg.ItemInfo[2] = SET_NUMBERHB(SET_NUMBERLW(lpObj->Money));
		pMsg.ItemInfo[3] = SET_NUMBERLB(SET_NUMBERLW(lpObj->Money));

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	CItem item = (*lpItem);

	if (this->InventoryInsertItemStack(lpObj, &item) != 0)
	{
		gMap[lpObj->Map].ItemGive(aIndex, index);

		pMsg.result = 0xFD;
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	pMsg.result = this->InventoryInsertItem(aIndex, item);

	if (pMsg.result == 0xFF)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	gMap[lpObj->Map].ItemGive(aIndex, index);

	this->ItemByteConvert(pMsg.ItemInfo, item);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	GCPartyItemInfoSend(aIndex, &item);

	if (BC_MAP_RANGE(lpObj->Map) != 0)
	{
		if (gBloodCastle.CheckEventItemSerial(lpObj->Map, lpItem) != 0)
		{
			gBloodCastle.GetEventItem(lpObj->Map, aIndex, lpItem);
		}
	}
}

void CItemManager::CGItemDropRecv(PMSG_ITEM_DROP_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_DROP_SEND pMsg;

	pMsg.header.set(0x23, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.slot = lpMsg->slot;

	if (lpObj->DieRegen != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type != INTERFACE_SHOP)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (INVENTORY_FULL_RANGE(lpMsg->slot) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	CItem* lpItem = &lpObj->Inventory[lpMsg->slot];

	if (lpItem->IsItem() == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (lpItem->m_IsPeriodicItem != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (gItemMove.CheckItemMoveAllowDrop(lpItem->m_Index) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if ((lpItem->m_Index < GET_ITEM(12, 0) && lpItem->m_Level > 4) || lpItem->IsExcItem() != 0 || lpItem->IsSetItem() != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (gItemBagManager.DropItemByItemIndex(lpItem->m_Index, lpItem->m_Level, lpObj, lpObj->Map, lpMsg->x, lpMsg->y) != 0)
	{
		this->InventoryDelItem(aIndex, lpMsg->slot);
	}
	else if (lpItem->m_Index == GET_ITEM(13, 7) && (lpItem->m_Level == 0 || lpItem->m_Level == 1)) // Siege Summon
	{
	#if(GAMESERVER_TYPE==1)

		if (gMercenary.CreateMercenary(aIndex, 286 + lpItem->m_Level, lpMsg->x, lpMsg->y) == 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}

		this->InventoryDelItem(aIndex, lpMsg->slot);

	#endif
	}
	else if (lpItem->m_Index == GET_ITEM(13, 11) && lpItem->m_Level == 1) // Life Stone
	{
	#if(GAMESERVER_TYPE==1)

		if (gLifeStone.CreateLifeStone(aIndex) == 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}

		this->InventoryDelItem(aIndex, lpMsg->slot);

	#endif
	}
	else if (lpItem->m_Index == GET_ITEM(13, 20) && lpItem->m_Level == 1) // Starter Ring 40
	{
		if (lpObj->Level < 40)
		{
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(257));
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}

		this->InventoryDelItem(aIndex, lpMsg->slot);
		gItemBagManager.DropItemBySpecialValue(ITEM_BAG_STARTER_RING1, lpObj, lpObj->Map, lpMsg->x, lpMsg->y);
	}
	else if (lpItem->m_Index == GET_ITEM(13, 20) && lpItem->m_Level == 2) // Starter Ring 80
	{
		if (lpObj->Level < 80)
		{
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(258));
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}

		this->InventoryDelItem(aIndex, lpMsg->slot);
		gItemBagManager.DropItemBySpecialValue(ITEM_BAG_STARTER_RING2, lpObj, lpObj->Map, lpMsg->x, lpMsg->y);
	}
	else if (lpItem->m_Index == GET_ITEM(14, 28)) // Lost Map
	{
		if (gKalima.CreateKalimaGate(aIndex, lpItem->m_Level, lpMsg->x, lpMsg->y) == 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}

		this->InventoryDelItem(aIndex, lpMsg->slot);
	}
	else
	{
		if (gMap[lpObj->Map].ItemDrop(lpItem->m_Index, lpItem->m_Level, lpItem->m_Durability, lpMsg->x, lpMsg->y, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption, lpItem->m_Serial, aIndex, lpItem->m_PetItemLevel, lpItem->m_PetItemExp, lpItem->m_PeriodicItemTime) == 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}

		this->InventoryDelItem(aIndex, lpMsg->slot);
	}

	pMsg.result = 1;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	this->UpdateInventoryViewport(aIndex, lpMsg->slot);
}

void CItemManager::CGItemMoveRecv(PMSG_ITEM_MOVE_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_MOVE_SEND pMsg;

	pMsg.header.setE(0x24, sizeof(pMsg));

	pMsg.result = 0xFF;
	pMsg.SourceFlag = lpMsg->SourceFlag;
	pMsg.SourceSlot = lpMsg->SourceSlot;
	pMsg.TargetFlag = lpMsg->TargetFlag;
	pMsg.TargetSlot = lpMsg->TargetSlot;

	memset(pMsg.ItemInfo, 0xFF, sizeof(pMsg.ItemInfo));

	if (lpObj->DieRegen != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	int index = lpMsg->ItemInfo[0] + ((lpMsg->ItemInfo[3] & 0x80) * 2) + ((lpMsg->ItemInfo[5] & 0xF0) * 32);

	if (lpMsg->SourceFlag == 1 || lpMsg->TargetFlag == 1) // Trade
	{
		if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0 || OBJECT_RANGE(lpObj->TargetNumber) == 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}
	}

	if (lpMsg->SourceFlag == 2 || lpMsg->TargetFlag == 2) // Warehouse
	{
		if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_WAREHOUSE || lpObj->Interface.state == 0 || lpObj->LoadWarehouse == 0 || lpObj->WarehouseLock == 1)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}
	}

	if (lpMsg->SourceFlag == 3 || (lpMsg->SourceFlag >= 6 && lpMsg->SourceFlag <= 20) || lpMsg->TargetFlag == 3 || (lpMsg->TargetFlag >= 6 && lpMsg->TargetFlag <= 20)) // Chaos Box
	{
		if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOS_BOX || lpObj->ChaosLock != 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}
	}

	if (lpMsg->SourceFlag == 4 || lpMsg->TargetFlag == 4) // Personal Shop
	{
		if (lpObj->PShopOpen != 0 || lpObj->PShopTransaction != 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}
	}

	if (lpMsg->SourceFlag == 5 || lpMsg->TargetFlag == 5) // Trainer
	{
		if (lpObj->Interface.use == INTERFACE_NONE || lpObj->Interface.type != INTERFACE_TRAINER || lpObj->ChaosLock != 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}
	}

	if (lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 0) // Inventory -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 1) // Inventory -> Trade
	{
		if ((pMsg.result = this->MoveItemToTradeFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Trade[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 2) // Inventory -> Warehouse
	{
		if ((pMsg.result = this->MoveItemToWarehouseFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Warehouse[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 0 && (lpMsg->TargetFlag == 3 || (lpMsg->TargetFlag >= 5 && lpMsg->TargetFlag <= 20))) // Inventory -> Chaos Box
	{
		if ((pMsg.result = this->MoveItemToChaosBoxFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->ChaosBox[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 4) // Inventory -> Personal Shop
	{
		if ((pMsg.result = this->MoveItemToPersonalShopFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 1 && lpMsg->TargetFlag == 0) // Trade -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromTrade(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 1 && lpMsg->TargetFlag == 1) // Trade -> Trade
	{
		if ((pMsg.result = this->MoveItemToTradeFromTrade(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Trade[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 2 && lpMsg->TargetFlag == 0) // Warehouse -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromWarehouse(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 2 && lpMsg->TargetFlag == 2) // Warehouse -> Warehouse
	{
		if ((pMsg.result = this->MoveItemToWarehouseFromWarehouse(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Warehouse[lpMsg->TargetSlot]);
		}
	}
	else if ((lpMsg->SourceFlag == 3 || (lpMsg->SourceFlag >= 5 && lpMsg->SourceFlag <= 20)) && lpMsg->TargetFlag == 0) // Chaos Box -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromChaosBox(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if ((lpMsg->SourceFlag == 3 || (lpMsg->SourceFlag >= 5 && lpMsg->SourceFlag <= 20)) && (lpMsg->TargetFlag == 3 || (lpMsg->TargetFlag >= 5 && lpMsg->TargetFlag <= 20))) // Chaos Box -> Chaos Box
	{
		if ((pMsg.result = this->MoveItemToChaosBoxFromChaosBox(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->ChaosBox[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 4 && lpMsg->TargetFlag == 0) // Personal Shop -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromPersonalShop(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 4 && lpMsg->TargetFlag == 4) // Personal Shop -> Personal Shop
	{
		if ((pMsg.result = this->MoveItemToPersonalShopFromPersonalShop(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	if (pMsg.result == 0xFF && lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 0)
	{
		if (lpObj->Inventory[lpMsg->SourceSlot].IsItem() != 0)
		{
			this->GCItemDurSend(lpObj->Index, lpMsg->SourceSlot, (BYTE)lpObj->Inventory[lpMsg->SourceSlot].m_Durability, 0);
		}
	}
}

void CItemManager::CGItemUseRecv(PMSG_ITEM_USE_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	GCLifeSend(aIndex, 0xFD, (BYTE)lpObj->Life);

	if (lpObj->DieRegen != 0)
	{
		return;
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type != INTERFACE_SHOP)
	{
		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		return;
	}

	if (INVENTORY_FULL_RANGE(lpMsg->SourceSlot) == 0)
	{
		return;
	}

	if (lpMsg->SourceSlot == lpMsg->TargetSlot)
	{
		return;
	}

	CItem* lpItem = &lpObj->Inventory[lpMsg->SourceSlot];

	if (lpItem->IsItem() == 0)
	{
		return;
	}

	if ((lpItem->m_Index >= GET_ITEM(12, 7) && lpItem->m_Index <= GET_ITEM(12, 24)) || lpItem->m_Index >= GET_ITEM(15, 0))
	{
		if (gObjectManager.CharacterUseScroll(lpObj, lpItem) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(12, 30)) // Bundle of Jewel of Bless
	{
		if (gObjectManager.CharacterUseJewelOfBless(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
			this->GCItemModifySend(aIndex, lpMsg->TargetSlot);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(13, 15)) // Fruit
	{
		if (lpMsg->type == 0)
		{
			if (gFruit.UseFruitAddPoint(lpObj, ((lpItem->m_Level < 4) ? (3 - lpItem->m_Level) : lpItem->m_Level)) != 0)
			{
				this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
				this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
			}
		}
		else
		{
			if (gFruit.UseFruitSubPoint(lpObj, ((lpItem->m_Level < 4) ? (3 - lpItem->m_Level) : lpItem->m_Level)) != 0)
			{
				this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
				this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
			}
		}
	}
	else if (lpItem->m_Index >= GET_ITEM(14, 0) && lpItem->m_Index <= GET_ITEM(14, 6))
	{
		if (gObjectManager.CharacterUsePotion(lpObj, lpItem) != 0)
		{
			this->DecreaseItemDur(lpObj, lpMsg->SourceSlot, 1);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 7)) // Siege Potion
	{
		if (lpItem->m_Level == 0)
		{
			gEffectManager.AddEffect(lpObj, 0, EFFECT_BLESS_POTION, 120, 0, 0, 0, 0);
		}
		else
		{
			gEffectManager.AddEffect(lpObj, 0, EFFECT_SOUL_POTION, 60, 20, 8, 0, 0);
		}

		this->DecreaseItemDur(lpObj, lpMsg->SourceSlot, 1);
	}
	else if (lpItem->m_Index == GET_ITEM(14, 8)) // Antidote
	{
		gEffectManager.DelEffect(lpObj, EFFECT_POISON);
		gEffectManager.DelEffect(lpObj, EFFECT_ICE);
		this->DecreaseItemDur(lpObj, lpMsg->SourceSlot, 1);
	}
	else if (lpItem->m_Index == GET_ITEM(14, 9)) // Ale
	{
		gObjUseDrink(lpObj, lpItem->m_Level);
		this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
		this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
	}
	else if (lpItem->m_Index == GET_ITEM(14, 10)) // Town Portal Scroll
	{
		if (gObjectManager.CharacterUsePortal(lpObj, lpItem) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 13)) // Jewel of Bless
	{
		if (gObjectManager.CharacterUseJewelOfBless(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
			this->GCItemModifySend(aIndex, lpMsg->TargetSlot);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 14)) // Jewel of Soul
	{
		if (gObjectManager.CharacterUseJewelOfSoul(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
			this->GCItemModifySend(aIndex, lpMsg->TargetSlot);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 16)) // Jewel of Life
	{
		if (gObjectManager.CharacterUseJewelOfLife(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
			this->GCItemModifySend(aIndex, lpMsg->TargetSlot);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 20)) // Remedy of Love
	{
		if (lpItem->m_Level == 0)
		{
			gObjUseDrink(lpObj, 2);
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
		}
	}
	else if (lpItem->m_Index >= GET_ITEM(14, 32) && lpItem->m_Index <= GET_ITEM(14, 37)) // Tintas e Neutralizadores
	{
		if (gObjectManager.CharacterUseInkOrNeutralizer(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);
			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
			this->GCItemModifySend(aIndex, lpMsg->TargetSlot);
		}
	}
	else
	{
		EFFECT_INFO* lpInfo = gEffectManager.GetInfoByItem(lpItem->m_Index);

		if (lpInfo != 0)
		{
			if (gEffectManager.AddEffect(lpObj, 0, lpInfo->Index, 0, 0, 0, 0, 0) != 0)
			{
				this->DecreaseItemDur(lpObj, lpMsg->SourceSlot, 1);
			}
		}
	}
}

void CItemManager::CGItemBuyRecv(PMSG_ITEM_BUY_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_BUY_SEND pMsg;

	pMsg.header.set(0x32, sizeof(pMsg));

	pMsg.result = 0xFF;

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_SHOP)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (SHOP_RANGE(lpObj->TargetShopNumber) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	CItem item;

	if (gShopManager.GetItemByIndex(lpObj->TargetShopNumber, &item, lpMsg->slot) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	int tax = (item.m_BuyMoney * gCastleSiegeSync.GetTaxRateStore(lpObj->Index)) / 100;

	if (((DWORD)lpObj->Money) < (item.m_BuyMoney + tax))
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (item.m_Index == GET_ITEM(13, 4) || item.m_Index == GET_ITEM(13, 5)) //Dark Horse,Dark Reaven
	{
		GDCreateItemSend(aIndex, 0xEB, 0, 0, item.m_Index, (BYTE)item.m_Level, (BYTE)item.m_Durability, item.m_Option1, item.m_Option2, item.m_Option3, -1, item.m_NewOption, item.m_SetOption, 0, 0);

		lpObj->Money -= item.m_BuyMoney + tax;

		GCMoneySend(aIndex, lpObj->Money);

		gCastleSiegeSync.AddTributeMoney(tax);

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (this->InventoryInsertItemStack(lpObj, &item) != 0)
	{
		lpObj->Money -= item.m_BuyMoney + tax;

		GCMoneySend(aIndex, lpObj->Money);

		gCastleSiegeSync.AddTributeMoney(tax);

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	pMsg.result = this->InventoryInsertItem(aIndex, item);

	if (pMsg.result == 0xFF)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	lpObj->Money -= item.m_BuyMoney + tax;

	GCMoneySend(aIndex, lpObj->Money);

	gCastleSiegeSync.AddTributeMoney(tax);

	this->ItemByteConvert(pMsg.ItemInfo, item);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::CGItemSellRecv(PMSG_ITEM_SELL_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_SELL_SEND pMsg;

	pMsg.header.set(0x33, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.money = 0;

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_SHOP)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (SHOP_RANGE(lpObj->TargetShopNumber) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (INVENTORY_FULL_RANGE(lpMsg->slot) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	CItem* lpItem = &lpObj->Inventory[lpMsg->slot];

	if (lpItem->IsItem() == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (gItemMove.CheckItemMoveAllowSell(lpItem->m_Index) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (gServerInfo.m_TradeItemBlockSell != 0 && this->CheckItemMoveToBlock(lpObj, lpItem) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (lpItem->m_Index == GET_ITEM(13, 4) || lpItem->m_Index == GET_ITEM(13, 5)) // Dark Horse,Dark Reaven
	{
		lpItem->PetValue();
	}
	else
	{
		lpItem->Value();
	}

	if (gObjCheckMaxMoney(aIndex, lpItem->m_SellMoney) == 0)
	{
		lpObj->Money = MAX_MONEY;
	}
	else
	{
		lpObj->Money += lpItem->m_SellMoney;
	}

	this->InventoryDelItem(aIndex, lpMsg->slot);

	pMsg.result = 1;
	pMsg.money = lpObj->Money;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	this->UpdateInventoryViewport(aIndex, lpMsg->slot);
}

void CItemManager::CGItemRepairRecv(PMSG_ITEM_REPAIR_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_REPAIR_SEND pMsg;

	pMsg.header.set(0x34, sizeof(pMsg));

	pMsg.money = 0;

	if (lpMsg->type == 1 && lpObj->Level < 50)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (lpMsg->slot == 0xFF)
	{
		int MaxValue = this->GetInventoryMaxValue(lpObj);

		for (int n = 0; n < MaxValue; n++)
		{
			if (lpObj->Inventory[n].IsItem() == 0)
			{
				continue;
			}

			pMsg.money = this->RepairItem(lpObj, &lpObj->Inventory[n], n, lpMsg->type);

			if (pMsg.money != 0)
			{
				DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			}
		}

		gObjectManager.CharacterCalcAttribute(aIndex);
		return;
	}

	if (INVENTORY_FULL_RANGE(lpMsg->slot) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	pMsg.money = this->RepairItem(lpObj, &lpObj->Inventory[lpMsg->slot], lpMsg->slot, lpMsg->type);

	if (pMsg.money != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		gObjectManager.CharacterCalcAttribute(aIndex);
	}
}

void CItemManager::GCItemChangeSend(int aIndex, BYTE slot) // OK
{
	gObjectManager.CharacterMakePreviewCharSet(aIndex);

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_ITEM_CHANGE_SEND pMsg;

	pMsg.header.set(0x25, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[slot]);

	pMsg.ItemInfo[1] = slot * 16;
	pMsg.ItemInfo[1] |= ((lpObj->Inventory[slot].m_Level - 1) / 2) & 0x0F;

	memcpy(pMsg.CharSet, lpObj->CharSet, sizeof(pMsg.CharSet));

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemDeleteSend(int aIndex, BYTE slot, BYTE flag) // OK
{
	PMSG_ITEM_DELETE_SEND pMsg;

	pMsg.header.set(0x28, sizeof(pMsg));

	pMsg.slot = slot;
	pMsg.flag = flag;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemDurSend(int aIndex, BYTE slot, BYTE dur, BYTE flag) // OK
{
	PMSG_ITEM_DUR_SEND pMsg;

	pMsg.header.set(0x2A, sizeof(pMsg));

	pMsg.slot = slot;
	pMsg.dur = dur;
	pMsg.flag = flag;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemListSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	PMSG_ITEM_LIST_SEND pMsg;

	pMsg.header.setE(0xF3, 0x10, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_ITEM_LIST info;

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_ItemExist != 0)
			{
				info.slot = n;

				this->ItemByteConvert(info.ItemInfo, lpObj->Inventory[n]);

				memcpy(&send[size], &info, sizeof(info));
				size += sizeof(info);

				++pMsg.count;
			}
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void CItemManager::GCItemEquipmentSend(int aIndex) // OK
{
	PMSG_ITEM_EQUIPMENT_SEND pMsg;

	pMsg.header.set(0xF3, 0x13, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	memcpy(pMsg.CharSet, gObj[aIndex].CharSet, sizeof(pMsg.CharSet));

	MsgSendV2(&gObj[aIndex], (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemModifySend(int aIndex, BYTE slot) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Inventory[slot].IsItem() == 0)
	{
		return;
	}

	PMSG_ITEM_MODIFY_SEND pMsg;

	pMsg.header.set(0xF3, 0x14, sizeof(pMsg));

	pMsg.slot = slot;

	this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[slot]);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}
