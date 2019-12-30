#pragma once

#include "Packets.h"

class CProtocol
{
public:
	CProtocol();
	~CProtocol();

public:
	void Load();

private:
	static int Core(DWORD Protocol, BYTE* Data, int Size, int Index);

private:
	int ViewportPlayers(int Index, LPBYTE Data);
	int UserDie(int Index, PMSG_USER_DIE_RECV* Data);
	int ViewportItems(int Index, LPBYTE Data);
	int ItemGet(int Index, PMSG_ITEM_GET_RECV* Data);
	int ItemMove(int Index, PMSG_ITEM_MOVE_RECV* Data);
	int ItemChange(int Index, PMSG_ITEM_CHANGE_RECV* Data);
	int UpdateHP(int Index, PMSG_LIFE_RECV* Data);
	int UpdateMP(int Index, PMSG_MANA_RECV* Data);
	int ShopItemList(int Index, LPBYTE Data);
	int ItemBuy(int Index, PMSG_ITEM_BUY_RECV* Data);
	int TradeItemDel(int Index, PMSG_TRADE_ITEM_DEL_RECV* Data);
	int TradeItemAdd(int Index, PMSG_TRADE_ITEM_ADD_RECV* Data);
	int PersonalShopItemList(int Index, LPBYTE Data);
	int PersonalShopItemBuy(int Index, PMSG_PSHOP_ITEM_BUY_RECV* Data);
	int ViewportChange(int Index, LPBYTE Data);
	int ViewportCastleSiegeWeapon(int Index, LPBYTE Data);
	int ChaosMix(int Index, PMSG_CHAOS_MIX_RECV* Data);
	int ObtainedExperience(int Index, PMSG_REWARD_EXPERIENCE_RECV* Data);
	int QuestReward(int Index, PMSG_QUEST_REWARD_RECV* Data);
	int SetDamage(int Index, PMSG_DAMAGE_RECV* Data);
	int ClientConnect(int Index, PMSG_CONNECT_CLIENT_RECV* Data);
	int CharacterList(int Index, LPBYTE Data);
	int CharacterInfo(int Index, PMSG_CHARACTER_INFO_RECV* Data);
	int CharacterRegen(int Index, PMSG_CHARACTER_REGEN_RECV* Data);
	int LevelUp(int Index, PMSG_LEVEL_UP_RECV* Data);
	int UpdatePoints(int index, PMSG_LEVEL_UP_POINT_RECV* Data);
	int MonsterSetDamage(int Index, PMSG_MONSTER_DAMAGE_RECV* Data);
	int ItemList(int Index, LPBYTE Data);
	int ItemEquipment(int Index, PMSG_ITEM_EQUIPMENT_RECV* Data);
	int ItemModify(int Index, PMSG_ITEM_MODIFY_RECV* Data);
	int PostMessage(PMSG_POST_MESSAGE_RECV* Data);
	int CustomSettings(PMSG_CUSTOM_SETTINGS_RECV* Data);
	int LockMain(PMSG_LOCK_RECV* Data);
	int MonsterHealth(LPBYTE Data);
	int DiscordUpdate(PMSG_DISCORD_UPDATE_RECV* Data);
};

extern CProtocol Protocol;