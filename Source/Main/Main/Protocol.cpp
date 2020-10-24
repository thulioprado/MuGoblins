#include "Library.h"
#include "Protocol.h"
#include "Player.h"
#include "Monster.h"
#include "Discord.h"
#include "Item.h"
#include "Viewport.h"
#include "Camera.h"
#include "Prism.h"

CProtocol::CProtocol()
{
}

CProtocol::~CProtocol()
{
}

void CProtocol::Load()
{
	Memory::Call(0x4DB63D, this->Core);
}

int CProtocol::Core(DWORD Protocol, BYTE* Data, int Size, int Index)
{
	switch (Protocol)
	{
		case 0x12:
		{
			return ::Protocol.ViewportPlayers(Index, Data);
		}
		case 0x17:
		{
			return ::Protocol.UserDie(Index, (PMSG_USER_DIE_RECV*)(Data));
		}
		case 0x20:
		{
			return ::Protocol.ViewportItems(Index, Data);
		}
		case 0x22:
		{
			return ::Protocol.ItemGet(Index, (PMSG_ITEM_GET_RECV*)(Data));
		}
		case 0x24:
		{
			return ::Protocol.ItemMove(Index, (PMSG_ITEM_MOVE_RECV*)(Data));
		}
		case 0x25:
		{
			return ::Protocol.ItemChange(Index, (PMSG_ITEM_CHANGE_RECV*)(Data));
		}
		case 0x26:
		{
			return ::Protocol.UpdateHP(Index, (PMSG_LIFE_RECV*)(Data));
		}
		case 0x27:
		{
			return ::Protocol.UpdateMP(Index, (PMSG_MANA_RECV*)(Data));
		}
		case 0x31:
		{
			return ::Protocol.ShopItemList(Index, Data);
		}
		case 0x32:
		{
			return ::Protocol.ItemBuy(Index, (PMSG_ITEM_BUY_RECV*)(Data));
		}
		case 0x38:
		{
			return ::Protocol.TradeItemDel(Index, (PMSG_TRADE_ITEM_DEL_RECV*)(Data));
		}
		case 0x39:
		{
			return ::Protocol.TradeItemAdd(Index, (PMSG_TRADE_ITEM_ADD_RECV*)(Data));
		}
		case 0x3F:
		{
			switch (GET_SUBHEAD(Data))
			{
				case 0x05:
				case 0x13:
				{
					return ::Protocol.PersonalShopItemList(Index, Data);
				}
				case 0x06:
				{
					return ::Protocol.PersonalShopItemBuy(Index, (PMSG_PSHOP_ITEM_BUY_RECV*)(Data));
				}
			}

			break;
		}
		case 0x45:
		{
			return ::Protocol.ViewportChange(Index, Data);
		}
		case 0x68:
		{
			return ::Protocol.ViewportCastleSiegeWeapon(Index, Data);
		}
		case 0x86:
		{
			return ::Protocol.ChaosMix(Index, (PMSG_CHAOS_MIX_RECV*)(Data));
		}
		case 0x9C:
		{
			return ::Protocol.ObtainedExperience(Index, (PMSG_REWARD_EXPERIENCE_RECV*)(Data));
		}
		case 0xA3:
		{
			return ::Protocol.QuestReward(Index, (PMSG_QUEST_REWARD_RECV*)(Data));
		}
		case 0xD7:
		{
			return ::Protocol.SetDamage(Index, (PMSG_DAMAGE_RECV*)(Data));
		}
		case 0xF1:
		{
			switch (GET_SUBHEAD(Data))
			{
				case 0x00:
				{
					return ::Protocol.ClientConnect(Index, (PMSG_CONNECT_CLIENT_RECV*)(Data));
				}
			}

			break;
		}
		case 0xF3:
		{
			switch (GET_SUBHEAD(Data))
			{
				case 0x00:
				{
					return ::Protocol.CharacterList(Index, Data);
				}
				case 0x03:
				{
					return ::Protocol.CharacterInfo(Index, (PMSG_CHARACTER_INFO_RECV*)(Data));
				}
				case 0x04:
				{
					return ::Protocol.CharacterRegen(Index, (PMSG_CHARACTER_REGEN_RECV*)(Data));
				}
				case 0x05:
				{
					return ::Protocol.LevelUp(Index, (PMSG_LEVEL_UP_RECV*)(Data));
				}
				case 0x06:
				{
					return ::Protocol.UpdatePoints(Index, (PMSG_LEVEL_UP_POINT_RECV*)(Data));
				}
				case 0x07:
				{
					return ::Protocol.MonsterSetDamage(Index, (PMSG_MONSTER_DAMAGE_RECV*)(Data));
				}
				case 0x10:
				{
					return ::Protocol.ItemList(Index, Data);
				}
				case 0x13:
				{
					return ::Protocol.ItemEquipment(Index, (PMSG_ITEM_EQUIPMENT_RECV*)(Data));
				}
				case 0x14:
				{
					return ::Protocol.ItemModify(Index, (PMSG_ITEM_MODIFY_RECV*)(Data));
				}
				case 0xFA:
				{
					return ::Protocol.CharacterUpdate(Index, (PMSG_CHARACTER_UPDATE_RECV*)(Data));
				}
				case 0xFB:
				{
					return ::Protocol.PostMessage((PMSG_POST_MESSAGE_RECV*)(Data));
				}
				case 0xFC:
				{
					return ::Protocol.CustomSettings((PMSG_CUSTOM_SETTINGS_RECV*)(Data));
				}
				case 0xFD:
				{
					return ::Protocol.LockMain((PMSG_LOCK_RECV*)(Data));
				}
				case 0xFE:
				{
					return ::Protocol.MonsterHealth(Data);
				}
				case 0xFF:
				{
					return ::Protocol.DiscordUpdate((PMSG_DISCORD_UPDATE_RECV*)(Data));
				}
			}

			break;
		}
	}

	return pProtocolCore(Protocol, Data, Size, Index);
}

int CProtocol::ViewportPlayers(int Index, LPBYTE Data)
{
	BYTE Buffer[5120];

	PMSG_VIEWPORT_RECV* Info = (PMSG_VIEWPORT_RECV*)(Data);
	PMSG_VIEWPORT_PLAYER* Player;

	int Size = sizeof(PMSG_VIEWPORT_RECV);

	PMSG_VIEWPORT_RECV pMsg;
	PMSG_VIEWPORT_PLAYER2 pPlayer;

	pMsg.header.set(0x12, 0);
	pMsg.count = Info->count;

	int NewSize = sizeof(pMsg);
	WORD PlayerIndex;

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Player = (PMSG_VIEWPORT_PLAYER*)(&Data[Size]);

		pPlayer.index[0] = Player->index[0];
		pPlayer.index[1] = Player->index[1];
		pPlayer.x = Player->x;
		pPlayer.y = Player->y;
		pPlayer.ViewSkillState = Player->ViewSkillState;
		pPlayer.tx = Player->tx;
		pPlayer.ty = Player->ty;
		pPlayer.DirAndPkLevel = Player->DirAndPkLevel;

		memcpy(pPlayer.name, Player->name, sizeof(pPlayer.name));
		memcpy(pPlayer.CharSet, Player->CharSet, sizeof(pPlayer.CharSet));
		memcpy(&Buffer[NewSize], &pPlayer, sizeof(pPlayer));

		Size += sizeof(PMSG_VIEWPORT_PLAYER);
		NewSize += sizeof(pPlayer);

		PlayerIndex = MAKE_NUMBERW((Player->index[0] & ~0x80), Player->index[1]);
		Viewport.AddCustom(PlayerIndex, Player->CharSet);
	}

	pMsg.header.size[0] = SET_NUMBERHB(NewSize);
	pMsg.header.size[1] = SET_NUMBERLB(NewSize);

	memcpy(Buffer, &pMsg, sizeof(pMsg));

	return pProtocolCore(0x12, Buffer, NewSize, Index);
}

int CProtocol::UserDie(int Index, PMSG_USER_DIE_RECV* Data)
{
	auto UserIndex = MAKE_NUMBERW(Data->index[0], Data->index[1]);

	if (UserIndex == Player.Index)
	{
		Player.HP = 0;

		Player.Execute(Index, [](PlayerObject* Object) {
			Object->HP = 0;
		});
	}

	return pProtocolCore(0x17, (LPBYTE)(Data), Data->header.size, Index);
}

int CProtocol::ViewportItems(int Index, LPBYTE Data)
{
	BYTE Buffer[4096];

	PMSG_VIEWPORT_RECV* Info = (PMSG_VIEWPORT_RECV*)(Data);
	PMSG_VIEWPORT_ITEM* Item;

	int Size = sizeof(PMSG_VIEWPORT_RECV);

	PMSG_VIEWPORT_RECV pMsg;
	PMSG_VIEWPORT_ITEM2 pItem;

	pMsg.header.set(0x20, 0);
	pMsg.count = Info->count;

	int NewSize = sizeof(pMsg);

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Item = (PMSG_VIEWPORT_ITEM*)(&Data[Size]);

		pItem.index[0] = Item->index[0];
		pItem.index[1] = Item->index[1];
		pItem.x = Item->x;
		pItem.y = Item->y;

		memcpy(pItem.ItemInfo, Item->ItemInfo, sizeof(pItem.ItemInfo));
		memcpy(&Buffer[NewSize], &pItem, sizeof(pItem));

		Size += sizeof(PMSG_VIEWPORT_ITEM);
		NewSize += sizeof(pItem);
	}

	pMsg.header.size[0] = SET_NUMBERHB(NewSize);
	pMsg.header.size[1] = SET_NUMBERLB(NewSize);

	memcpy(Buffer, &pMsg, sizeof(pMsg));

	return pProtocolCore(0x20, Buffer, NewSize, Index);
}

int CProtocol::ItemGet(int Index, PMSG_ITEM_GET_RECV* Data)
{
	if (Data->result < 0xFD)
	{
		Player.SetInventory(Data->result, Data->ItemInfo);
	}

	PMSG_ITEM_GET_RECV2 pMsg;
	pMsg.header.setE(0x22, sizeof(pMsg));
	pMsg.result = Data->result;
	memcpy(pMsg.ItemInfo, Data->ItemInfo, sizeof(pMsg.ItemInfo));

	return pProtocolCore(0x22, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ItemMove(int Index, PMSG_ITEM_MOVE_RECV* Data)
{
	if (Data->result != 0xFF)
	{
		switch (Data->SourceFlag)
		{
			case TargetFlag::Inventory:
			case TargetFlag::PersonalShop:
			{
				Player.SetInventory(Data->SourceSlot, null);

				if (Data->PrismUpdate && (Data->SourceSlot == 10 || Data->SourceSlot == 11))	// Movendo do slot de anel
				{
					auto ItemIndex = Data->ItemInfo[0] + ((Data->ItemInfo[3] & 0x80) * 2) + ((Data->ItemInfo[5] & 0xF0) * 32);

					switch (ItemIndex)
					{
						case GET_ITEM(13, 39):	// Anel de Prisma [Armadura]
						{
							Prism.GetValue(Player.PrismArmor.Color, 0, 0, 0);
							break;
						}
						case GET_ITEM(13, 40):	// Anel de Prisma [Arma]
						{
							Prism.GetValue(Player.PrismWeapon.Color, 0, 0, 0);
							break;
						}
					}
				}
				else if (!Data->PrismUpdate && (Data->SourceSlot == 10 || Data->SourceSlot == 11))
				{
					auto PrismInfo = Player.Inventory[((Data->SourceSlot == 10) ? 11 : 10)].Prism;
					auto ItemIndex = Data->ItemInfo[0] + ((Data->ItemInfo[3] & 0x80) * 2) + ((Data->ItemInfo[5] & 0xF0) * 32);

					switch (ItemIndex)
					{
						case GET_ITEM(13, 39):	// Anel de Prisma [Armadura]
						{
							Prism.GetValue(Player.PrismArmor.Color, SET_BYTE(PrismInfo.Color[0].Red, PrismInfo.Color[1].Red), SET_BYTE(PrismInfo.Color[0].Green, PrismInfo.Color[1].Green), SET_BYTE(PrismInfo.Color[0].Blue, PrismInfo.Color[1].Blue));
							break;
						}
						case GET_ITEM(13, 40):	// Anel de Prisma [Arma]
						{
							Prism.GetValue(Player.PrismWeapon.Color, SET_BYTE(PrismInfo.Color[0].Red, PrismInfo.Color[1].Red), SET_BYTE(PrismInfo.Color[0].Green, PrismInfo.Color[1].Green), SET_BYTE(PrismInfo.Color[0].Blue, PrismInfo.Color[1].Blue));
							break;
						}
					}
				}
				else if (Data->SourceSlot == 9)
				{
					Player.PrismArmor.Speed = 0;
					Player.PrismWeapon.Speed = 0;
				}

				break;
			}
			case TargetFlag::Trade:
			case TargetFlag::Warehouse:
			case TargetFlag::ChaosMachine:
			case TargetFlag::Trainer:
			{
				Player.SetTempSource(Data->SourceSlot, null);
				break;
			}
		}

		switch (Data->TargetFlag)
		{
			case TargetFlag::Inventory:
			case TargetFlag::PersonalShop:
			{
				Player.SetInventory(Data->TargetSlot, Data->ItemInfo);

				if (Data->PrismUpdate && (Data->TargetSlot == 10 || Data->TargetSlot == 11))	// Movendo para slot de anel
				{
					auto ItemIndex = Data->ItemInfo[0] + ((Data->ItemInfo[3] & 0x80) * 2) + ((Data->ItemInfo[5] & 0xF0) * 32);

					switch (ItemIndex)
					{
						case GET_ITEM(13, 39):	// Anel de Prisma [Armadura]
						{
							Prism.GetValue(Player.PrismArmor.Color, Data->ItemInfo[6], Data->ItemInfo[7], Data->ItemInfo[8]);
							break;
						}
						case GET_ITEM(13, 40):	// Anel de Prisma [Arma]
						{
							Prism.GetValue(Player.PrismWeapon.Color, Data->ItemInfo[6], Data->ItemInfo[7], Data->ItemInfo[8]);
							break;
						}
					}
				}
				else if (Data->TargetSlot == 9)
				{
					auto ItemIndex = Data->ItemInfo[0] + ((Data->ItemInfo[3] & 0x80) * 2) + ((Data->ItemInfo[5] & 0xF0) * 32);

					switch (ItemIndex)
					{
						case GET_ITEM(13, 41):	// Colar de Prisma
						{
							Player.PrismArmor.Speed = ((char)(Data->ItemInfo[6]) * -1) * 33;
							Player.PrismWeapon.Speed = ((char)(Data->ItemInfo[7]) * -1) * 33;
							break;
						}
					}
				}

				break;
			}
			case TargetFlag::Trade:
			case TargetFlag::Warehouse:
			case TargetFlag::ChaosMachine:
			case TargetFlag::Trainer:
			{
				Player.SetTempSource(Data->TargetSlot, Data->ItemInfo);
				break;
			}
		}
	}

	PMSG_ITEM_MOVE_RECV2 pMsg;
	pMsg.header.setE(0x24, sizeof(pMsg));
	pMsg.result = Data->result;
	pMsg.slot = Data->TargetSlot;
	memcpy(pMsg.ItemInfo, Data->ItemInfo, sizeof(pMsg.ItemInfo));

	return pProtocolCore(0x24, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ItemChange(int Index, PMSG_ITEM_CHANGE_RECV* Data)
{
	PMSG_ITEM_CHANGE_RECV2 pMsg;
	pMsg.header.set(0x25, sizeof(pMsg));
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	memcpy(pMsg.ItemInfo, Data->ItemInfo, sizeof(pMsg.ItemInfo));

	WORD PlayerIndex = MAKE_NUMBERW(Data->index[0], Data->index[1]);
	Viewport.AddCustom(PlayerIndex, Data->CharSet);

	return pProtocolCore(0x25, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::UpdateHP(int Index, PMSG_LIFE_RECV* Data)
{
	switch (Data->type)
	{
		case 0xFE:
		{
			Player.MaxHP = Data->life;

			Player.Execute(Index, [](PlayerObject* Object) {
				Object->MaxHP = GET_MAX_WORD_VALUE(Player.MaxHP);
			});

			break;
		}
		case 0xFF:
		{
			Player.HP = ((Player.HP == 0) ? Player.HP : Data->life);

			Player.Execute(Index, [&](PlayerObject* Object) {
				Object->HP = GET_MAX_WORD_VALUE(Player.HP);
			});

			break;
		}
	}

	PMSG_LIFE_RECV2 pMsg;

	pMsg.header.set(0x26, sizeof(pMsg));
	pMsg.type = Data->type;
	pMsg.life[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->life));
	pMsg.life[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->life));
	pMsg.flag = Data->flag;

	return pProtocolCore(0x26, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::UpdateMP(int Index, PMSG_MANA_RECV* Data)
{
	switch (Data->type)
	{
		case 0xFE:
		{
			Player.MaxMP = Data->mana;
			Player.MaxBP = Data->bp;

			Player.Execute(Index, [](PlayerObject* Object) {
				Object->MaxMP = GET_MAX_WORD_VALUE(Player.MaxMP);
				Object->MaxBP = GET_MAX_WORD_VALUE(Player.MaxBP);
			});
		} break;
		case 0xFF:
		{
			Player.MP = Data->mana;
			Player.BP = Data->bp;

			Player.Execute(Index, [](PlayerObject* Object) {
				Object->MP = GET_MAX_WORD_VALUE(Player.MP);
				Object->BP = GET_MAX_WORD_VALUE(Player.BP);
			});
		} break;
	}

	PMSG_MANA_RECV2 pMsg;

	pMsg.header.set(0x27, sizeof(pMsg));
	pMsg.type = Data->type;
	pMsg.mana[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->mana));
	pMsg.mana[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->mana));
	pMsg.bp[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->bp));
	pMsg.bp[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->bp));

	return pProtocolCore(0x27, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ShopItemList(int Index, LPBYTE Data)
{
	BYTE Buffer[4096];

	PMSG_SHOP_ITEM_LIST_RECV* Info = (PMSG_SHOP_ITEM_LIST_RECV*)(Data);
	PMSG_SHOP_ITEM_LIST* Item;

	int Size = sizeof(PMSG_SHOP_ITEM_LIST_RECV);

	PMSG_SHOP_ITEM_LIST_RECV pMsg;
	PMSG_SHOP_ITEM_LIST2 pItem;

	pMsg.header.set(0x31, 0);
	pMsg.count = Info->count;
	pMsg.type = Info->type;

	int NewSize = sizeof(pMsg);

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Item = (PMSG_SHOP_ITEM_LIST*)(&Data[Size]);

		Player.SetTempSource(Item->slot, Item->ItemInfo);

		pItem.slot = Item->slot;

		memcpy(pItem.ItemInfo, Item->ItemInfo, sizeof(pItem.ItemInfo));
		memcpy(&Buffer[NewSize], &pItem, sizeof(pItem));

		Size += sizeof(PMSG_SHOP_ITEM_LIST);
		NewSize += sizeof(pItem);
	}

	pMsg.header.size[0] = SET_NUMBERHB(NewSize);
	pMsg.header.size[1] = SET_NUMBERLB(NewSize);

	memcpy(Buffer, &pMsg, sizeof(pMsg));

	return pProtocolCore(0x31, Buffer, NewSize, Index);
}

int CProtocol::ItemBuy(int Index, PMSG_ITEM_BUY_RECV* Data)
{
	Player.SetInventory(Data->result, Data->ItemInfo);

	PMSG_ITEM_BUY_RECV2 pMsg;
	pMsg.header.set(0x32, sizeof(pMsg));
	pMsg.result = Data->result;
	memcpy(pMsg.ItemInfo, Data->ItemInfo, sizeof(pMsg.ItemInfo));

	return pProtocolCore(0x32, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::TradeItemDel(int Index, PMSG_TRADE_ITEM_DEL_RECV* Data)
{
	Player.SetTempTarget(Data->slot, null);

	return pProtocolCore(0x38, (LPBYTE)(Data), Data->header.size, Index);
}

int CProtocol::TradeItemAdd(int Index, PMSG_TRADE_ITEM_ADD_RECV* Data)
{
	PMSG_TRADE_ITEM_ADD_RECV2 pMsg;
	pMsg.header.set(0x39, sizeof(pMsg));
	pMsg.slot = Data->slot;
	memcpy(pMsg.ItemInfo, Data->ItemInfo, sizeof(pMsg.ItemInfo));

	Player.SetTempTarget(Data->slot, Data->ItemInfo);

	return pProtocolCore(0x39, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::PersonalShopItemList(int Index, LPBYTE Data)
{
	BYTE Buffer[4096];

	PMSG_PSHOP_ITEM_LIST_RECV* Info = (PMSG_PSHOP_ITEM_LIST_RECV*)(Data);
	PMSG_PSHOP_ITEM_LIST* Item;

	int Size = sizeof(PMSG_PSHOP_ITEM_LIST_RECV);

	PMSG_PSHOP_ITEM_LIST_RECV pMsg;
	PMSG_PSHOP_ITEM_LIST2 pItem;

	pMsg.header.set(0x3F, Info->header.subh, 0);
	pMsg.result = Info->result;
	pMsg.index[0] = Info->index[0];
	pMsg.index[1] = Info->index[1];
	pMsg.count = Info->count;

	memcpy(pMsg.name, Info->name, sizeof(pMsg.name));
	memcpy(pMsg.text, Info->text, sizeof(pMsg.text));

	int NewSize = sizeof(pMsg);

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Item = (PMSG_PSHOP_ITEM_LIST*)(&Data[Size]);

		Player.SetTempTarget(Item->slot, Item->ItemInfo);

		pItem.slot = Item->slot;
		pItem.value = Item->value;

		memcpy(pItem.ItemInfo, Item->ItemInfo, sizeof(pItem.ItemInfo));
		memcpy(&Buffer[NewSize], &pItem, sizeof(pItem));

		Size += sizeof(PMSG_PSHOP_ITEM_LIST);
		NewSize += sizeof(pItem);
	}

	pMsg.header.size[0] = SET_NUMBERHB(NewSize);
	pMsg.header.size[1] = SET_NUMBERLB(NewSize);

	memcpy(Buffer, &pMsg, sizeof(pMsg));

	return pProtocolCore(0x3F, Buffer, NewSize, Index);
}

int CProtocol::PersonalShopItemBuy(int Index, PMSG_PSHOP_ITEM_BUY_RECV* Data)
{
	if (Data->slot < INVENTORY_SIZE)
	{
		Player.SetInventory(Data->slot, Data->ItemInfo);
	}

	PMSG_PSHOP_ITEM_BUY_RECV2 pMsg;
	pMsg.header.set(0x3F, 0x06, sizeof(pMsg));
	pMsg.result = Data->result;
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	pMsg.slot = Data->slot;
	memcpy(pMsg.ItemInfo, Data->ItemInfo, sizeof(pMsg.ItemInfo));
	
	return pProtocolCore(0x3F, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ViewportChange(int Index, LPBYTE Data)
{
	BYTE Buffer[5120];

	PMSG_VIEWPORT_RECV* Info = (PMSG_VIEWPORT_RECV*)(Data);
	PMSG_VIEWPORT_CHANGE* Change;

	int Size = sizeof(PMSG_VIEWPORT_RECV);

	PMSG_VIEWPORT_RECV pMsg;
	PMSG_VIEWPORT_CHANGE2 pChange;

	pMsg.header.set(0x45, 0);
	pMsg.count = Info->count;

	int NewSize = sizeof(pMsg);
	WORD PlayerIndex;

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Change = (PMSG_VIEWPORT_CHANGE*)(&Data[Size]);

		pChange.index[0] = Change->index[0];
		pChange.index[1] = Change->index[1];
		pChange.skin[0] = Change->skin[0];
		pChange.skin[1] = Change->skin[1];
		pChange.x = Change->x;
		pChange.y = Change->y;
		pChange.tx = Change->tx;
		pChange.ty = Change->ty;
		pChange.ViewSkillState = Change->ViewSkillState;
		pChange.DirAndPkLevel = Change->DirAndPkLevel;

		memcpy(pChange.name, Change->name, sizeof(pChange.name));
		memcpy(pChange.CharSet, Change->CharSet, sizeof(pChange.CharSet));
		memcpy(&Buffer[NewSize], &pChange, sizeof(pChange));

		Size += sizeof(PMSG_VIEWPORT_CHANGE);
		NewSize += sizeof(pChange);

		PlayerIndex = MAKE_NUMBERW((Change->index[0] & ~0x80), Change->index[1]);
		Viewport.AddCustom(PlayerIndex, Change->CharSet);
	}

	pMsg.header.size[0] = SET_NUMBERHB(NewSize);
	pMsg.header.size[1] = SET_NUMBERLB(NewSize);

	memcpy(Buffer, &pMsg, sizeof(pMsg));

	return pProtocolCore(0x45, Buffer, NewSize, Index);
}

int CProtocol::ViewportCastleSiegeWeapon(int Index, LPBYTE Data)
{
	BYTE Buffer[5120];

	PMSG_VIEWPORT_RECV* Info = (PMSG_VIEWPORT_RECV*)(Data);
	PMSG_VIEWPORT_CASTLE_SIEGE_WEAPON* Weapon;

	int Size = sizeof(PMSG_VIEWPORT_RECV);

	PMSG_VIEWPORT_RECV pMsg;
	PMSG_VIEWPORT_CASTLE_SIEGE_WEAPON2 pWeapon;

	pMsg.header.set(0x68, 0);
	pMsg.count = Info->count;

	int NewSize = sizeof(pMsg);

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Weapon = (PMSG_VIEWPORT_CASTLE_SIEGE_WEAPON*)(&Data[Size]);

		pWeapon.type = Weapon->type;
		pWeapon.index[0] = Weapon->index[0];
		pWeapon.index[1] = Weapon->index[1];
		pWeapon.skin[0] = Weapon->skin[0];
		pWeapon.skin[1] = Weapon->skin[1];
		pWeapon.x = Weapon->x;
		pWeapon.y = Weapon->y;
		pWeapon.ViewSkillState = Weapon->ViewSkillState;

		memcpy(pWeapon.CharSet, Weapon->CharSet, sizeof(pWeapon.CharSet));
		memcpy(&Buffer[NewSize], &pWeapon, sizeof(pWeapon));

		Size += sizeof(PMSG_VIEWPORT_CASTLE_SIEGE_WEAPON);
		NewSize += sizeof(pWeapon);
	}

	pMsg.header.size[0] = SET_NUMBERHB(NewSize);
	pMsg.header.size[1] = SET_NUMBERLB(NewSize);

	memcpy(Buffer, &pMsg, sizeof(pMsg));

	return pProtocolCore(0x68, Buffer, NewSize, Index);
}

int CProtocol::ChaosMix(int Index, PMSG_CHAOS_MIX_RECV* Data)
{
	PMSG_CHAOS_MIX_RECV2 pMsg;
	pMsg.header.set(0x86, sizeof(pMsg));
	pMsg.result = Data->result;
	memcpy(pMsg.ItemInfo, Data->ItemInfo, sizeof(pMsg.ItemInfo));

	if (Data->result == 1 || Data->result == 100)
	{
		Player.SetTempSource(0, Data->ItemInfo);
	}

	return pProtocolCore(0x86, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ObtainedExperience(int Index, PMSG_REWARD_EXPERIENCE_RECV* Data)
{
	Player.Experience += Data->experience;
	Player.Damage = Data->damage;

	auto Current = double(Player.Experience) - double(Player.PreviousNextExperience);
	auto Total = double(Player.NextExperience) - double(Player.PreviousNextExperience);
	Player.PercentExperience = (DWORD)((Current / Total) * 100);

	if (Player.PercentExperience > 100)
	{
		Player.PercentExperience = 100;
	}

	PMSG_REWARD_EXPERIENCE_RECV2 pMsg;

	pMsg.header.setE(0x9C, sizeof(pMsg));
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	pMsg.experience[0] = SET_NUMBERHW(Data->experience);
	pMsg.experience[1] = SET_NUMBERLW(Data->experience);
	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->damage));
	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->damage));

	return pProtocolCore(0x9C, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::QuestReward(int Index, PMSG_QUEST_REWARD_RECV* Data)
{
	Player.LevelUpPoints = Data->LevelUpPoints;

	PMSG_QUEST_REWARD_RECV2 pMsg;
	pMsg.header.set(0xA3, sizeof(pMsg));
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	pMsg.QuestReward = Data->QuestReward;
	pMsg.QuestAmount = Data->QuestAmount;

	return pProtocolCore(0xA3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::SetDamage(int Index, PMSG_DAMAGE_RECV* Data)
{
	int UserIndex = MAKE_NUMBERW(Data->index[0], Data->index[1]);

	if (UserIndex == Player.Index)
	{
		Player.HP = Data->life;

		Player.Execute(Index, [](PlayerObject* Object) {
			Object->HP = GET_MAX_WORD_VALUE(Player.HP);
		});
	}

	Player.Damage = Data->damage;

	PMSG_DAMAGE_RECV2 pMsg;

	pMsg.header.set(0xD7, sizeof(pMsg));
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	pMsg.type = Data->type;
	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->damage));
	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->damage));

	return pProtocolCore(0xD7, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ClientConnect(int Index, PMSG_CONNECT_CLIENT_RECV* Data)
{
	pLockMain = FALSE;

	Player.Index = MAKE_NUMBERW(Data->index[0], Data->index[1]);

	return pProtocolCore(0xF1, (LPBYTE)(Data), Data->header.size, Index);
}

int CProtocol::CharacterList(int Index, LPBYTE Data)
{
	BYTE Buffer[256];

	PMSG_CHARACTER_LIST_RECV* Info = (PMSG_CHARACTER_LIST_RECV*)(Data);
	PMSG_CHARACTER_LIST* Character;

	int Size = sizeof(PMSG_CHARACTER_LIST_RECV);

	PMSG_CHARACTER_LIST_RECV2 pMsg;
	PMSG_CHARACTER_LIST2 pCharacter;

	pMsg.header.set(0xF3, 0x00, 0);
	pMsg.count = Info->count;
	pMsg.ClassCode = Info->ClassCode;
	pMsg.MoveCnt = Info->MoveCnt;

	int NewSize = sizeof(pMsg);

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Character = (PMSG_CHARACTER_LIST*)(&Data[Size]);

		pCharacter.slot = Character->slot;
		pCharacter.Level = Character->Level;
		pCharacter.CtlCode = Character->CtlCode;
		pCharacter.GuildStatus = Character->GuildStatus;

		memcpy(pCharacter.Name, Character->Name, sizeof(pCharacter.Name));
		memcpy(pCharacter.CharSet, Character->CharSet, sizeof(pCharacter.CharSet));
		memcpy(&Buffer[NewSize], &pCharacter, sizeof(pCharacter));

		Size += sizeof(PMSG_CHARACTER_LIST);
		NewSize += sizeof(pCharacter);

		Prism.GetValue(Player.CharacterList[i].PrismArmor.Color, Character->CharSet[18], Character->CharSet[19], Character->CharSet[20]);
		Prism.GetValue(Player.CharacterList[i].PrismWeapon.Color, Character->CharSet[21], Character->CharSet[22], Character->CharSet[23]);

		Player.CharacterList[i].PrismArmor.Speed = ((char)(Character->CharSet[24]) * -1) * 33;
		Player.CharacterList[i].PrismWeapon.Speed = ((char)(Character->CharSet[25]) * -1) * 33;
	}

	pMsg.header.size = (BYTE)(NewSize);

	memcpy(Buffer, &pMsg, sizeof(pMsg));

	return pProtocolCore(0xF3, Buffer, NewSize, Index);
}

int CProtocol::CharacterInfo(int Index, PMSG_CHARACTER_INFO_RECV* Data)
{
	Camera.Clip();

	pLockMain = FALSE;

	Player.Experience = Data->Experience;
	Player.NextExperience = Data->NextExperience;
	Player.PreviousNextExperience = Data->PreviousNextExperience;
	Player.HP = Data->Life;
	Player.MaxHP = Data->MaxLife;
	Player.MP = Data->Mana;
	Player.MaxMP = Data->MaxMana;
	Player.BP = Data->BP;
	Player.MaxBP = Data->MaxBP;
	Player.LevelUpPoints = Data->LevelUpPoint;

	Prism.GetValue(Player.PrismArmor.Color, 0, 0, 0);
	Prism.GetValue(Player.PrismWeapon.Color, 0, 0, 0);

	auto Current = double(Player.Experience) - double(Player.PreviousNextExperience);
	auto Total = double(Player.NextExperience) - double(Player.PreviousNextExperience);
	Player.PercentExperience = (DWORD)((Current / Total) * 100);

	if (Player.PercentExperience > 100)
	{
		Player.PercentExperience = 100;
	}

	PMSG_CHARACTER_INFO_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x03, sizeof(pMsg));
	pMsg.X = Data->X;
	pMsg.Y = Data->Y;
	pMsg.Map = Data->Map;
	pMsg.Dir = Data->Dir;
	pMsg.Experience = (DWORD)(Data->Experience);
	pMsg.NextExperience = (DWORD)(Data->NextExperience);
	pMsg.LevelUpPoint = GET_MAX_WORD_VALUE(Data->LevelUpPoint);
	pMsg.Strength = Data->Strength;
	pMsg.Dexterity = Data->Dexterity;
	pMsg.Vitality = Data->Vitality;
	pMsg.Energy = Data->Energy;
	pMsg.Life = GET_MAX_WORD_VALUE(Data->Life);
	pMsg.MaxLife = GET_MAX_WORD_VALUE(Data->MaxLife);
	pMsg.Mana = GET_MAX_WORD_VALUE(Data->Mana);
	pMsg.MaxMana = GET_MAX_WORD_VALUE(Data->MaxMana);
	pMsg.BP = GET_MAX_WORD_VALUE(Data->BP);
	pMsg.MaxBP = GET_MAX_WORD_VALUE(Data->MaxBP);
	pMsg.Money = Data->Money;
	pMsg.PKLevel = Data->PKLevel;
	pMsg.CtlCode = Data->CtlCode;
	pMsg.FruitAddPoint = Data->FruitAddPoint;
	pMsg.MaxFruitAddPoint = Data->MaxFruitAddPoint;
	pMsg.Leadership = Data->Leadership;
	pMsg.FruitSubPoint = Data->FruitSubPoint;
	pMsg.MaxFruitSubPoint = Data->MaxFruitSubPoint;

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::CharacterRegen(int Index, PMSG_CHARACTER_REGEN_RECV* Data)
{
	Player.Experience = Data->Experience;
	Player.HP = Data->Life;
	Player.MP = Data->Mana;
	Player.BP = Data->BP;

	PMSG_CHARACTER_REGEN_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x04, sizeof(pMsg));
	pMsg.X = Data->X;
	pMsg.Y = Data->Y;
	pMsg.Map = Data->Map;
	pMsg.Dir = Data->Dir;
	pMsg.Life = GET_MAX_WORD_VALUE(Data->Life);
	pMsg.Mana = GET_MAX_WORD_VALUE(Data->Mana);
	pMsg.BP = GET_MAX_WORD_VALUE(Data->BP);
	pMsg.Experience = (DWORD)(Data->Experience);
	pMsg.Money = Data->Money;

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::LevelUp(int Index, PMSG_LEVEL_UP_RECV* Data)
{
	Player.MaxHP = Data->MaxHP;
	Player.MaxMP = Data->MaxMP;
	Player.MaxBP = Data->MaxBP;
	Player.LevelUpPoints = Data->LevelUpPoint;
	Player.NextExperience = Data->NextExperience;
	Player.PreviousNextExperience = Data->PreviousNextExperience;

	auto Current = double(Player.Experience) - double(Player.PreviousNextExperience);
	auto Total = double(Player.NextExperience) - double(Player.PreviousNextExperience);
	Player.PercentExperience = (DWORD)((Current / Total) * 100);

	if (Player.PercentExperience > 100)
	{
		Player.PercentExperience = 100;
	}

	PMSG_LEVEL_UP_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x05, sizeof(pMsg));
	pMsg.Level = Data->Level;
	pMsg.LevelUpPoint = GET_MAX_WORD_VALUE(Data->LevelUpPoint);
	pMsg.MaxHP = GET_MAX_WORD_VALUE(Data->MaxHP);
	pMsg.MaxMP = GET_MAX_WORD_VALUE(Data->MaxMP);
	pMsg.MaxBP = GET_MAX_WORD_VALUE(Data->MaxBP);
	pMsg.FruitAddPoint = Data->FruitAddPoint;
	pMsg.MaxFruitAddPoint = Data->MaxFruitAddPoint;
	pMsg.FruitSubPoint = Data->FruitSubPoint;
	pMsg.MaxFruitSubPoint = Data->MaxFruitSubPoint;

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::UpdatePoints(int Index, PMSG_LEVEL_UP_POINT_RECV* Data)
{
	Player.LevelUpPoints = Data->LevelUpPoint;

	if (Data->result > 0)
	{
		Player.MaxHP = Data->MaxLife;
		Player.MaxMP = Data->MaxMana;
		Player.MaxBP = Data->MaxBP;

		Player.Execute(Index, [&](PlayerObject* Object) {
			Object->LevelUpPoint = Data->LevelUpPoint > 0;

			switch (Data->result)
			{
				case 0x10:
				{
					++Object->Strength;
					break;
				}
				case 0x11:
				{
					++Object->Dexterity;
					break;
				}
				case 0x12:
				{
					++Object->Vitality;
					break;
				}
				case 0x13:
				{
					++Object->Energy;
					break;
				}
				case 0x14:
				{
					++Object->Leadership;
					break;
				}
			}
		});
	}

	return 1;
}

int CProtocol::MonsterSetDamage(int Index, PMSG_MONSTER_DAMAGE_RECV* Data)
{
	Player.HP = Data->life;
	Player.Damage = Data->damage;

	PMSG_MONSTER_DAMAGE_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x07, sizeof(pMsg));
	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->damage));
	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->damage));

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ItemList(int Index, LPBYTE Data)
{
	BYTE Buffer[4096];

	PMSG_ITEM_LIST_RECV* Info = (PMSG_ITEM_LIST_RECV*)(Data);
	PMSG_ITEM_LIST* Item;

	int Size = sizeof(PMSG_ITEM_LIST_RECV);

	PMSG_ITEM_LIST_RECV pMsg;
	PMSG_ITEM_LIST2 pItem;

	pMsg.header.setE(0xF3, 0x10, 0);
	pMsg.count = Info->count;

	int NewSize = sizeof(pMsg);
	int ItemIndex;

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Item = (PMSG_ITEM_LIST*)(&Data[Size]);

		Player.SetInventory(Item->slot, Item->ItemInfo);

		pItem.slot = Item->slot;

		switch (Item->slot)
		{
			case 9:		// Colar
			{
				ItemIndex = Item->ItemInfo[0] + ((Item->ItemInfo[3] & 0x80) * 2) + ((Item->ItemInfo[5] & 0xF0) * 32);

				switch (ItemIndex)
				{
					case GET_ITEM(13, 41):	// Colar de Prisma
					{
						Player.PrismArmor.Speed = ((char)(Item->ItemInfo[6]) * -1) * 33;
						Player.PrismWeapon.Speed = ((char)(Item->ItemInfo[7]) * -1) * 33;
						break;
					}
				}

				break;
			}
			case 10:	// Aneis
			case 11:
			{
				ItemIndex = Item->ItemInfo[0] + ((Item->ItemInfo[3] & 0x80) * 2) + ((Item->ItemInfo[5] & 0xF0) * 32);

				switch (ItemIndex)
				{
					case GET_ITEM(13, 39):	// Anel de Prisma [Armadura]
					{
						Prism.GetValue(Player.PrismArmor.Color, Item->ItemInfo[6], Item->ItemInfo[7], Item->ItemInfo[8]);
						break;
					}
					case GET_ITEM(13, 40):	// Anel de Prisma [Arma]
					{
						Prism.GetValue(Player.PrismWeapon.Color, Item->ItemInfo[6], Item->ItemInfo[7], Item->ItemInfo[8]);
						break;
					}
				}

				break;
			}
		}

		memcpy(pItem.ItemInfo, Item->ItemInfo, sizeof(pItem.ItemInfo));
		memcpy(&Buffer[NewSize], &pItem, sizeof(pItem));
		
		Size += sizeof(PMSG_ITEM_LIST);
		NewSize += sizeof(pItem);
	}

	pMsg.header.size[0] = SET_NUMBERHB(NewSize);
	pMsg.header.size[1] = SET_NUMBERLB(NewSize);

	memcpy(Buffer, &pMsg, sizeof(pMsg));

	return pProtocolCore(0xF3, Buffer, NewSize, Index);
}

int CProtocol::ItemEquipment(int Index, PMSG_ITEM_EQUIPMENT_RECV* Data)
{
	PMSG_ITEM_EQUIPMENT_RECV2 pMsg;
	pMsg.header.set(0xF3, 0x13, sizeof(pMsg));
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	memcpy(pMsg.CharSet, Data->CharSet, sizeof(pMsg.CharSet));

	WORD PlayerIndex = MAKE_NUMBERW(Data->index[0], Data->index[1]);
	Viewport.AddCustom(PlayerIndex, Data->CharSet);

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ItemModify(int Index, PMSG_ITEM_MODIFY_RECV* Data)
{
	PMSG_ITEM_MODIFY_RECV2 pMsg;
	pMsg.header.set(0xF3, 0x14, sizeof(pMsg));
	pMsg.slot = Data->slot;
	memcpy(pMsg.ItemInfo, Data->ItemInfo, sizeof(pMsg.ItemInfo));

	Player.SetInventory(Data->slot, Data->ItemInfo);

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::CharacterUpdate(int Index, PMSG_CHARACTER_UPDATE_RECV* Data)
{
	Player.LevelUpPoints = Data->LevelUpPoint;
	Player.HP = Data->HP;
	Player.MaxHP = Data->MaxHP;
	Player.MP = Data->MP;
	Player.MaxMP = Data->MaxMP;
	Player.BP = Data->BP;
	Player.MaxBP = Data->MaxBP;

	Player.Execute(Index, [&](PlayerObject* Player) {
		Player->Level = Data->Level;
		Player->LevelUpPoint = Data->LevelUpPoint > 0;
		Player->Strength = Data->Strength;
		Player->Dexterity = Data->Dexterity;
		Player->Vitality = Data->Vitality;
		Player->Energy = Data->Energy;
		Player->Leadership = Data->Leadership;
	});
	
	return 1;
}

int CProtocol::PostMessage(PMSG_POST_MESSAGE_RECV* Data)
{
	pShowMessage("", Data->message, 97);
	return 1;
}

int CProtocol::CustomSettings(PMSG_CUSTOM_SETTINGS_RECV* Data)
{
	memcpy(Item.TransformationRings, Data->TransformationRings, sizeof(Item.TransformationRings));
	return 1;
}

int CProtocol::LockMain(PMSG_LOCK_RECV* Data)
{
	pLockMain = Data->lock;
	return 1;
}

int CProtocol::MonsterHealth(LPBYTE Data)
{
	PMSG_MONSTER_HEALTH_RECV* Info = (PMSG_MONSTER_HEALTH_RECV*)(Data);
	PMSG_MONSTER_HEALTH* Health;

	int Size = sizeof(PMSG_MONSTER_HEALTH_RECV);

	Monster.ResetHealth();

	for (BYTE i = 0; i < Info->count; ++i)
	{
		Health = (PMSG_MONSTER_HEALTH*)(&Data[Size]);

		Monster.AddHealth(Health->index, Health->percent);

		Size += sizeof(PMSG_MONSTER_HEALTH);
	}

	return 1;
}

int CProtocol::DiscordUpdate(PMSG_DISCORD_UPDATE_RECV* Data)
{
	char text[50];
	wsprintf(text, "Total online: %d", Data->total);
	Discord.SetState(text);

	return 1;
}

CProtocol Protocol;