#include "stdafx.h"
#include "DataServerProtocol.h"
#include "BadSyntax.h"
#include "CastleDBSet.h"
#include "CharacterManager.h"
#include "CommandManager.h"
#include "CSProtocol.h"
#include "ESProtocol.h"
#include "GuildManager.h"
#include "QueryManager.h"
#include "Quest.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "Util.h"
#include "Warehouse.h"

void DataServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size) // OK
{
	gServerManager[index].m_PacketTime = GetTickCount();

	switch (head)
	{
		case 0x00:
			GDServerInfoRecv((SDHP_SERVER_INFO_RECV*)lpMsg, index);
			break;
		case 0x01:
			GDCharacterListRecv((SDHP_CHARACTER_LIST_RECV*)lpMsg, index);
			break;
		case 0x02:
			GDCharacterCreateRecv((SDHP_CHARACTER_CREATE_RECV*)lpMsg, index);
			break;
		case 0x03:
			GDCharacterDeleteRecv((SDHP_CHARACTER_DELETE_RECV*)lpMsg, index);
			break;
		case 0x04:
			GDCharacterInfoRecv((SDHP_CHARACTER_INFO_RECV*)lpMsg, index);
			break;
		case 0x05:
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
					gWarehouse.GDWarehouseItemRecv((SDHP_WAREHOUSE_ITEM_RECV*)lpMsg, index);
					break;
				case 0x30:
					gWarehouse.GDWarehouseItemSaveRecv((SDHP_WAREHOUSE_ITEM_SAVE_RECV*)lpMsg);
					break;
			}
			break;
		case 0x07:
			GDCreateItemRecv((SDHP_CREATE_ITEM_RECV*)lpMsg, index);
			break;
		case 0x09:
			GDPetItemInfoRecv((SDHP_PET_ITEM_INFO_RECV*)lpMsg, index);
			break;
		case 0x0C:
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
					gQuest.GDQuestKillCountRecv((SDHP_QUEST_KILL_COUNT_RECV*)lpMsg, index);
					break;
				case 0x30:
					gQuest.GDQuestKillCountSaveRecv((SDHP_QUEST_KILL_COUNT_SAVE_RECV*)lpMsg);
					break;
			}
			break;
		case 0x0F:
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
					gCommandManager.GDCommandResetRecv((SDHP_COMMAND_RESET_RECV*)lpMsg, index);
					break;
				case 0x01:
					gCommandManager.GDCommandMasterResetRecv((SDHP_COMMAND_MASTER_RESET_RECV*)lpMsg, index);
					break;
			}
			break;
		case 0x1E:
			GDCrywolfSyncRecv((SDHP_CRYWOLF_SYNC_RECV*)lpMsg, index);
			break;
		case 0x1F:
			GDCrywolfInfoRecv((SDHP_CRYWOLF_INFO_RECV*)lpMsg, index);
			break;
		case 0x20:
			GDGlobalPostRecv((SDHP_GLOBAL_POST_RECV*)lpMsg, index);
			break;
		case 0x21:
			GDGlobalNoticeRecv((SDHP_GLOBAL_NOTICE_RECV*)lpMsg, index);
			break;
		case 0x30:
			GDCharacterInfoSaveRecv((SDHP_CHARACTER_INFO_SAVE_RECV*)lpMsg);
			break;
		case 0x31:
			GDInventoryItemSaveRecv((SDHP_INVENTORY_ITEM_SAVE_RECV*)lpMsg);
			break;
		case 0x34:
			GDPetItemInfoSaveRecv((SDHP_PET_ITEM_INFO_SAVE_RECV*)lpMsg);
			break;
		case 0x39:
			GDResetInfoSaveRecv((SDHP_RESET_INFO_SAVE_RECV*)lpMsg);
			break;
		case 0x3A:
			GDMasterResetInfoSaveRecv((SDHP_MASTER_RESET_INFO_SAVE_RECV*)lpMsg);
			break;
		case 0x3C:
			GDRankingDuelSaveRecv((SDHP_RANKING_DUEL_SAVE_RECV*)lpMsg);
			break;
		case 0x3D:
			GDRankingBloodCastleSaveRecv((SDHP_RANKING_BLOOD_CASTLE_SAVE_RECV*)lpMsg);
			break;
		case 0x3E:
			GDRankingChaosCastleSaveRecv((SDHP_RANKING_CHAOS_CASTLE_SAVE_RECV*)lpMsg);
			break;
		case 0x3F:
			GDRankingDevilSquareSaveRecv((SDHP_RANKING_DEVIL_SQUARE_SAVE_RECV*)lpMsg);
			break;
		case 0x40:
			GDRankingIllusionTempleSaveRecv((SDHP_RANKING_ILLUSION_TEMPLE_SAVE_RECV*)lpMsg);
			break;
		case 0x42:
			GDCreationCardSaveRecv((SDHP_CREATION_CARD_SAVE_RECV*)lpMsg);
			break;
		case 0x49:
			GDCrywolfInfoSaveRecv((SDHP_CRYWOLF_INFO_SAVE_RECV*)lpMsg);
			break;
		case 0x52:
			GDCustomMonsterRewardSaveRecv((SDHP_CUSTOM_MONSTER_REWARD_SAVE_RECV*)lpMsg);
			break;
		case 0x55:
			GDRankingCustomArenaSaveRecv((SDHP_RANKING_CUSTOM_ARENA_SAVE_RECV*)lpMsg);
			break;
		case 0x70:
			GDConnectCharacterRecv((SDHP_CONNECT_CHARACTER_RECV*)lpMsg, index);
			break;
		case 0x71:
			GDDisconnectCharacterRecv((SDHP_DISCONNECT_CHARACTER_RECV*)lpMsg, index);
			break;
		case 0x72:
			GDGlobalWhisperRecv((SDHP_GLOBAL_WHISPER_RECV*)lpMsg, index);
			break;
		case 0x80:
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
					DS_GDReqCastleTotalInfo(lpMsg, index);
					break;
				case 0x01:
					DS_GDReqOwnerGuildMaster(lpMsg, index);
					break;
				case 0x03:
					DS_GDReqCastleNpcBuy(lpMsg, index);
					break;
				case 0x04:
					DS_GDReqCastleNpcRepair(lpMsg, index);
					break;
				case 0x05:
					DS_GDReqCastleNpcUpgrade(lpMsg, index);
					break;
				case 0x06:
					DS_GDReqTaxInfo(lpMsg, index);
					break;
				case 0x07:
					DS_GDReqTaxRateChange(lpMsg, index);
					break;
				case 0x08:
					DS_GDReqCastleMoneyChange(lpMsg, index);
					break;
				case 0x09:
					DS_GDReqSiegeDateChange(lpMsg, index);
					break;
				case 0x0A:
					DS_GDReqGuildMarkRegInfo(lpMsg, index);
					break;
				case 0x0B:
					DS_GDReqSiegeEndedChange(lpMsg, index);
					break;
				case 0x0C:
					DS_GDReqCastleOwnerChange(lpMsg, index);
					break;
				case 0x0D:
					DS_GDReqRegAttackGuild(lpMsg, index);
					break;
				case 0x0E:
					DS_GDReqRestartCastleState(lpMsg, index);
					break;
				case 0x0F:
					DS_GDReqMapSvrMsgMultiCast(lpMsg, index);
					break;
				case 0x10:
					DS_GDReqRegGuildMark(lpMsg, index);
					break;
				case 0x11:
					DS_GDReqGuildMarkReset(lpMsg, index);
					break;
				case 0x12:
					DS_GDReqGuildSetGiveUp(lpMsg, index);
					break;
				case 0x16:
					DS_GDReqCastleNpcRemove(lpMsg, index);
					break;
				case 0x17:
					DS_GDReqCastleStateSync(lpMsg, index);
					break;
				case 0x18:
					DS_GDReqCastleTributeMoney(lpMsg, index);
					break;
				case 0x19:
					DS_GDReqResetCastleTaxInfo(lpMsg, index);
					break;
				case 0x1A:
					DS_GDReqResetSiegeGuildInfo(lpMsg, index);
					break;
				case 0x1B:
					DS_GDReqResetRegSiegeInfo(lpMsg, index);
					break;
			}
			break;
		case 0x81:
			DS_GDReqCastleInitData(lpMsg, index);
			break;
		case 0x82:
			DS_GDReqCastleNpcInfo(lpMsg, index);
			break;
		case 0x83:
			DS_GDReqAllGuildMarkRegInfo(lpMsg, index);
			break;
		case 0x84:
			DS_GDReqFirstCreateNPC(lpMsg, index);
			break;
		case 0x85:
			DS_GDReqCalcRegGuildList(lpMsg, index);
			break;
		case 0x86:
			DS_GDReqCsGuildUnionInfo(lpMsg, index);
			break;
		case 0x87:
			DS_GDReqCsSaveTotalGuildInfo(lpMsg, index);
			break;
		case 0x88:
			DS_GDReqCsLoadTotalGuildInfo(lpMsg, index);
			break;
		case 0x89:
			DS_GDReqCastleNpcUpdate(lpMsg, index);
			break;
		case 0xE0:
			ESDataRecv(index, head, lpMsg, size);
			break;
		case 0xE1:
			CSDataRecv(index, head, lpMsg, size);
			break;
	}
}

void GDServerInfoRecv(SDHP_SERVER_INFO_RECV* lpMsg, int index) // OK
{
	SDHP_SERVER_INFO_SEND pMsg;

	pMsg.header.set(0x00, sizeof(pMsg));

	pMsg.result = 1;

	pMsg.ItemCount = 0;

	if (gQueryManager.ExecQuery("SELECT ItemCount FROM GameServerInfo WHERE Number=0") == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO GameServerInfo (Number,ItemCount,ZenCount,AceItemCount) VALUES (0,0,0,0)");
		gQueryManager.Close();
	}
	else
	{
		pMsg.ItemCount = gQueryManager.GetAsInteger("ItemCount");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

	gServerManager[index].SetServerInfo(lpMsg->ServerName, lpMsg->ServerPort, lpMsg->ServerCode);
}

void GDCharacterListRecv(SDHP_CHARACTER_LIST_RECV* lpMsg, int index) // OK
{
	BYTE send[2048];

	SDHP_CHARACTER_LIST_SEND pMsg;

	pMsg.header.set(0x01, 0);

	int size = sizeof(pMsg);

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	if (gQueryManager.ExecQuery("SELECT Id FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO AccountCharacter (Id) VALUES ('%s')", lpMsg->account);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
	}

	char CharacterName[5][11];

	memset(CharacterName, 0, sizeof(CharacterName));

	gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account);

	gQueryManager.Fetch();

	pMsg.MoveCnt = (BYTE)gQueryManager.GetAsInteger("MoveCnt");

	pMsg.ExtClass = (BYTE)gQueryManager.GetAsInteger("ExtClass");

	gQueryManager.GetAsString("GameID1", CharacterName[0], sizeof(CharacterName[0]));

	gQueryManager.GetAsString("GameID2", CharacterName[1], sizeof(CharacterName[1]));

	gQueryManager.GetAsString("GameID3", CharacterName[2], sizeof(CharacterName[2]));

	gQueryManager.GetAsString("GameID4", CharacterName[3], sizeof(CharacterName[3]));

	gQueryManager.GetAsString("GameID5", CharacterName[4], sizeof(CharacterName[4]));

	gQueryManager.Close();

	pMsg.count = 0;

	SDHP_CHARACTER_LIST info;

	for (int n = 0; n < 5; n++)
	{
		if (CharacterName[n][0] == 0)
		{
			continue;
		}

		if (gQueryManager.ExecQuery("SELECT cLevel,Class,Inventory,CtlCode FROM Character WHERE AccountID='%s' AND Name='%s'", lpMsg->account, CharacterName[n]) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
		}
		else
		{
			info.slot = n;

			memcpy(info.name, CharacterName[n], sizeof(info.name));

			info.level = (WORD)gQueryManager.GetAsInteger("cLevel");

			info.Class = (BYTE)gQueryManager.GetAsInteger("Class");

			BYTE Inventory[12][16];

			gQueryManager.GetAsBinary("Inventory", Inventory[0], sizeof(Inventory));

			info.CtlCode = (BYTE)gQueryManager.GetAsInteger("CtlCode");

			gQueryManager.Close();

			memset(info.Inventory, 0xFF, sizeof(info.Inventory));

			for (int i = 0; i < 12; i++)
			{
				if (Inventory[i][0] == 0xFF && (Inventory[i][7] & 0x80) == 0x80 && (Inventory[i][9] & 0xF0) == 0xF0)
				{
					info.Inventory[0 + (i * 8)] = 0xFF;
					info.Inventory[1 + (i * 8)] = 0xFF;
					info.Inventory[2 + (i * 8)] = 0xFF;
					info.Inventory[3 + (i * 8)] = 0xFF;
					info.Inventory[4 + (i * 8)] = 0xFF;
					info.Inventory[5 + (i * 8)] = 0;
					info.Inventory[6 + (i * 8)] = 0;
					info.Inventory[7 + (i * 8)] = 0;
				}
				else
				{
					info.Inventory[0 + (i * 8)] = Inventory[i][0];
					info.Inventory[1 + (i * 8)] = Inventory[i][1];
					info.Inventory[2 + (i * 8)] = Inventory[i][7];
					info.Inventory[3 + (i * 8)] = Inventory[i][8];
					info.Inventory[4 + (i * 8)] = Inventory[i][9];
					info.Inventory[5 + (i * 8)] = Inventory[i][10];
					info.Inventory[6 + (i * 8)] = Inventory[i][11];
					info.Inventory[7 + (i * 8)] = Inventory[i][12];
				}
			}

			if (gQueryManager.ExecQuery("SELECT G_Status FROM GuildMember WHERE Name='%s'", CharacterName[n]) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				gQueryManager.Close();

				info.GuildStatus = 0xFF;
			}
			else
			{
				info.GuildStatus = (BYTE)gQueryManager.GetAsInteger("G_Status");

				gQueryManager.Close();
			}

			memcpy(&send[size], &info, sizeof(info));
			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	gSocketManager.DataSend(index, send, size);
}

void GDCharacterCreateRecv(SDHP_CHARACTER_CREATE_RECV* lpMsg, int index) // OK
{
	SDHP_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0x02, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	if (CheckTextSyntax(lpMsg->name, sizeof(lpMsg->name)) == 0 || gBadSyntax.CheckSyntax(lpMsg->name) == 0)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;
	}

	pMsg.slot = 0;

	pMsg.Class = lpMsg->Class;

	memset(pMsg.equipment, 0xFF, sizeof(pMsg.equipment));

	pMsg.level = 1;

	char CharacterName[5][11] = {0};

	if (pMsg.result == 0 || gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		gQueryManager.GetAsString("GameID1", CharacterName[0], sizeof(CharacterName[0]));
		gQueryManager.GetAsString("GameID2", CharacterName[1], sizeof(CharacterName[1]));
		gQueryManager.GetAsString("GameID3", CharacterName[2], sizeof(CharacterName[2]));
		gQueryManager.GetAsString("GameID4", CharacterName[3], sizeof(CharacterName[3]));
		gQueryManager.GetAsString("GameID5", CharacterName[4], sizeof(CharacterName[4]));
		gQueryManager.Close();

		if (GetCharacterSlot(CharacterName, "", &pMsg.slot) == 0)
		{
			pMsg.result = 2;
		}
		else
		{
			if (gQueryManager.ExecQuery("EXEC WZ_CreateCharacter '%s','%s','%d'", lpMsg->account, lpMsg->name, lpMsg->Class) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				gQueryManager.Close();

				pMsg.result = 0;
			}
			else
			{
				pMsg.result = gQueryManager.GetResult(0);

				gQueryManager.Close();
			}

			if (pMsg.result == 1)
			{
				gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameID%d='%s' WHERE Id='%s'", (pMsg.slot + 1), lpMsg->name, lpMsg->account);
				gQueryManager.Close();
			}
		}
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg, int index) // OK
{
	SDHP_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0x03, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	if (CheckTextSyntax(lpMsg->name, sizeof(lpMsg->name)) == 0)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;
	}

	if (pMsg.result == 0 || gQueryManager.ExecQuery("EXEC WZ_DeleteCharacter '%s','%s'", lpMsg->account, lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();

		if (pMsg.result == 1)
		{
			char CharacterName[5][11] = {0};

			if (gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				gQueryManager.Close();

				pMsg.result = 1;
			}
			else
			{
				gQueryManager.GetAsString("GameID1", CharacterName[0], sizeof(CharacterName[0]));
				gQueryManager.GetAsString("GameID2", CharacterName[1], sizeof(CharacterName[1]));
				gQueryManager.GetAsString("GameID3", CharacterName[2], sizeof(CharacterName[2]));
				gQueryManager.GetAsString("GameID4", CharacterName[3], sizeof(CharacterName[3]));
				gQueryManager.GetAsString("GameID5", CharacterName[4], sizeof(CharacterName[4]));
				gQueryManager.Close();

				BYTE slot;

				if (GetCharacterSlot(CharacterName, lpMsg->name, &slot) != 0)
				{
					gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameID%d=NULL WHERE Id='%s'", (slot + 1), lpMsg->account);
					gQueryManager.Close();
				}
			}
		}
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg, int index) // OK
{
	SDHP_CHARACTER_INFO_SEND pMsg;

	pMsg.header.set(0x04, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	pMsg.result = ((CheckTextSyntax(lpMsg->name, sizeof(lpMsg->name)) == 0) ? 0 : 1);

	if (pMsg.result == 0 || gQueryManager.ExecQuery("SELECT * FROM Character WHERE AccountID='%s' AND Name='%s'", lpMsg->account, lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		pMsg.Level = (WORD)gQueryManager.GetAsInteger("cLevel");
		pMsg.Class = (BYTE)gQueryManager.GetAsInteger("Class");
		pMsg.LevelUpPoint = gQueryManager.GetAsInteger("LevelUpPoint");
		pMsg.Experience = gQueryManager.GetAsInteger64("Experience");
		pMsg.Strength = gQueryManager.GetAsInteger("Strength");
		pMsg.Dexterity = gQueryManager.GetAsInteger("Dexterity");
		pMsg.Vitality = gQueryManager.GetAsInteger("Vitality");
		pMsg.Energy = gQueryManager.GetAsInteger("Energy");
		pMsg.Leadership = gQueryManager.GetAsInteger("Leadership");
		pMsg.Money = (DWORD)gQueryManager.GetAsInteger64("Money");
		pMsg.Life = (DWORD)gQueryManager.GetAsFloat("Life");
		pMsg.MaxLife = (DWORD)gQueryManager.GetAsFloat("MaxLife");
		pMsg.Mana = (DWORD)gQueryManager.GetAsFloat("Mana");
		pMsg.MaxMana = (DWORD)gQueryManager.GetAsFloat("MaxMana");
		pMsg.BP = (DWORD)gQueryManager.GetAsFloat("BP");
		pMsg.MaxBP = (DWORD)gQueryManager.GetAsFloat("MaxBP");
		pMsg.Map = (BYTE)gQueryManager.GetAsInteger("MapNumber");
		pMsg.X = (BYTE)gQueryManager.GetAsInteger("MapPosX");
		pMsg.Y = (BYTE)gQueryManager.GetAsInteger("MapPosY");
		pMsg.Dir = (BYTE)gQueryManager.GetAsInteger("MapDir");
		pMsg.PKCount = gQueryManager.GetAsInteger("PkCount");
		pMsg.PKLevel = gQueryManager.GetAsInteger("PkLevel");
		pMsg.PKTime = gQueryManager.GetAsInteger("PkTime");
		pMsg.CtlCode = (BYTE)gQueryManager.GetAsInteger("CtlCode");
		pMsg.FruitAddPoint = (WORD)gQueryManager.GetAsInteger("FruitAddPoint");
		pMsg.FruitSubPoint = (WORD)gQueryManager.GetAsInteger("FruitSubPoint");
		pMsg.SkillOptionData.GameOption = (BYTE)gQueryManager.GetAsInteger("GameOption");
		pMsg.SkillOptionData.QKey = (BYTE)gQueryManager.GetAsInteger("Qkey");
		pMsg.SkillOptionData.WKey = (BYTE)gQueryManager.GetAsInteger("Wkey");
		pMsg.SkillOptionData.EKey = (BYTE)gQueryManager.GetAsInteger("Ekey");
		pMsg.SkillOptionData.ChatWindow = (BYTE)gQueryManager.GetAsInteger("ChatWindow");

		gQueryManager.GetAsBinary("Inventory", pMsg.Inventory[0], sizeof(pMsg.Inventory));
		gQueryManager.GetAsBinary("MagicList", pMsg.Skill[0], sizeof(pMsg.Skill));
		gQueryManager.GetAsBinary("Quest", pMsg.Quest, sizeof(pMsg.Quest));
		gQueryManager.GetAsBinary("EffectList", pMsg.Effect[0], sizeof(pMsg.Effect));
		gQueryManager.GetAsBinary("SkillKey", pMsg.SkillOptionData.SkillKey, sizeof(pMsg.SkillOptionData.SkillKey));

		gQueryManager.Close();

		gQueryManager.ExecQuery("EXEC WZ_GetResetInfo '%s','%s'", lpMsg->account, lpMsg->name);
		gQueryManager.Fetch();

		pMsg.Reset = gQueryManager.GetAsInteger("Reset");

		gQueryManager.Close();

		gQueryManager.ExecQuery("EXEC WZ_GetMasterResetInfo '%s','%s'", lpMsg->account, lpMsg->name);
		gQueryManager.Fetch();

		pMsg.MasterReset = gQueryManager.GetAsInteger("MasterReset");

		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameIDC='%s' WHERE Id='%s'", lpMsg->name, lpMsg->account);
		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}

void GDCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg, int index) // OK
{
	SDHP_CREATE_ITEM_SEND pMsg;

	pMsg.header.set(0x07, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	pMsg.X = lpMsg->X;
	pMsg.Y = lpMsg->Y;
	pMsg.Map = lpMsg->Map;

	if (gQueryManager.ExecQuery("EXEC WZ_GetItemSerial") == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.Serial = 0;
	}
	else
	{
		pMsg.Serial = gQueryManager.GetResult(0);

		gQueryManager.Close();
	}

	if (lpMsg->ItemIndex == 0x1A04 || lpMsg->ItemIndex == 0x1A05)
	{
		if (gQueryManager.ExecQuery("SELECT ItemSerial FROM T_PetItem_Info WHERE ItemSerial=%d", pMsg.Serial) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
			gQueryManager.ExecQuery("INSERT INTO T_PetItem_Info (ItemSerial,Pet_Level,Pet_Exp) VALUES (%d,%d,%d)", pMsg.Serial, 1, 0);
			gQueryManager.Close();
		}
		else
		{
			gQueryManager.Close();
		}
	}

	pMsg.ItemIndex = lpMsg->ItemIndex;
	pMsg.Level = lpMsg->Level;
	pMsg.Dur = lpMsg->Dur;
	pMsg.Option1 = lpMsg->Option1;
	pMsg.Option2 = lpMsg->Option2;
	pMsg.Option3 = lpMsg->Option3;
	pMsg.NewOption = lpMsg->NewOption;
	pMsg.LootIndex = lpMsg->LootIndex;
	pMsg.SetOption = lpMsg->SetOption;
	pMsg.Duration = lpMsg->Duration;

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDPetItemInfoRecv(SDHP_PET_ITEM_INFO_RECV* lpMsg, int index) // OK
{
	BYTE send[4096];

	SDHP_PET_ITEM_INFO_SEND pMsg;

	pMsg.header.set(0x09, 0);

	int size = sizeof(pMsg);

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	pMsg.type = lpMsg->type;

	pMsg.count = 0;

	SDHP_PET_ITEM_INFO2 info;

	for (int n = 0; n < lpMsg->count; n++)
	{
		SDHP_PET_ITEM_INFO1* lpInfo = (SDHP_PET_ITEM_INFO1*)(((BYTE*)lpMsg) + sizeof(SDHP_PET_ITEM_INFO_RECV) + (sizeof(SDHP_PET_ITEM_INFO1) * n));

		if (gQueryManager.ExecQuery("SELECT Pet_Level,Pet_Exp FROM T_PetItem_Info WHERE ItemSerial=%d", lpInfo->serial) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
			gQueryManager.ExecQuery("INSERT INTO T_PetItem_Info (ItemSerial,Pet_Level,Pet_Exp) VALUES (%d,%d,%d)", lpInfo->serial, 1, 0);
			gQueryManager.Close();

			info.slot = lpInfo->slot;
			info.serial = lpInfo->serial;
			info.level = 1;
			info.experience = 0;
		}
		else
		{
			info.slot = lpInfo->slot;
			info.serial = lpInfo->serial;
			info.level = gQueryManager.GetAsInteger("Pet_Level");
			info.experience = gQueryManager.GetAsInteger("Pet_Exp");

			gQueryManager.Close();
		}

		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	gSocketManager.DataSend(index, send, size);
}

void GDCrywolfSyncRecv(SDHP_CRYWOLF_SYNC_RECV* lpMsg, int index) // OK
{
	SDHP_CRYWOLF_SYNC_SEND pMsg;

	pMsg.header.set(0x1E, sizeof(pMsg));

	pMsg.MapServerGroup = lpMsg->MapServerGroup;

	pMsg.CrywolfState = lpMsg->CrywolfState;

	pMsg.OccupationState = lpMsg->OccupationState;

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != 0)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void GDCrywolfInfoRecv(SDHP_CRYWOLF_INFO_RECV* lpMsg, int index) // OK
{
	SDHP_CRYWOLF_INFO_SEND pMsg;

	pMsg.header.set(0x1F, sizeof(pMsg));

	pMsg.MapServerGroup = lpMsg->MapServerGroup;

	if (gQueryManager.ExecQuery("EXEC WZ_CW_InfoLoad '%d'", lpMsg->MapServerGroup) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.CrywolfState = 0;
		pMsg.OccupationState = 0;
	}
	else
	{
		pMsg.CrywolfState = gQueryManager.GetAsInteger("CRYWOLF_STATE");
		pMsg.OccupationState = gQueryManager.GetAsInteger("CRYWOLF_OCCUFY");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}

void GDGlobalPostRecv(SDHP_GLOBAL_POST_RECV* lpMsg, int index) // OK
{
	SDHP_GLOBAL_POST_SEND pMsg;

	pMsg.header.set(0x20, sizeof(pMsg));

	pMsg.MapServerGroup = lpMsg->MapServerGroup;

	pMsg.type = lpMsg->type;

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != 0)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void GDGlobalNoticeRecv(SDHP_GLOBAL_NOTICE_RECV* lpMsg, int index) // OK
{
	SDHP_GLOBAL_NOTICE_SEND pMsg;

	pMsg.header.set(0x21, sizeof(pMsg));

	pMsg.MapServerGroup = lpMsg->MapServerGroup;
	pMsg.type = lpMsg->type;
	pMsg.count = lpMsg->count;
	pMsg.opacity = lpMsg->opacity;
	pMsg.delay = lpMsg->delay;
	pMsg.color = lpMsg->color;
	pMsg.speed = lpMsg->speed;

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != 0)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void GDCharacterInfoSaveRecv(SDHP_CHARACTER_INFO_SAVE_RECV* lpMsg) // OK
{
	char query[4096];
	sprintf_s(query, "UPDATE Character SET cLevel=%d,Class=%d,LevelUpPoint=%d,Experience=%llu,Strength=%d,Dexterity=%d,Vitality=%d,Energy=%d,Leadership=%d,Inventory=?,MagicList=?,Money=%llu,Life=%f,MaxLife=%f,Mana=%f,MaxMana=%f,BP=%f,MaxBP=%f,MapNumber=%d,MapPosX=%d,MapPosY=%d,MapDir=%d,PkCount=%d,PkLevel=%d,PkTime=%d,Quest=?,EffectList=?,FruitAddPoint=%d,FruitSubPoint=%d,SkillKey=?,GameOption=%d,Qkey=%d,Wkey=%d,Ekey=%d,ChatWindow=%d WHERE AccountID='%s' AND Name='%s'", lpMsg->Level, lpMsg->Class, lpMsg->LevelUpPoint, lpMsg->Experience, lpMsg->Strength, lpMsg->Dexterity, lpMsg->Vitality, lpMsg->Energy, lpMsg->Leadership, (QWORD)lpMsg->Money, (float)lpMsg->Life, (float)lpMsg->MaxLife, (float)lpMsg->Mana, (float)lpMsg->MaxMana, (float)lpMsg->BP, (float)lpMsg->MaxBP, lpMsg->Map, lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->PKCount, lpMsg->PKLevel, lpMsg->PKTime, lpMsg->FruitAddPoint, lpMsg->FruitSubPoint, lpMsg->SkillOptionData.GameOption, lpMsg->SkillOptionData.QKey, lpMsg->SkillOptionData.WKey, lpMsg->SkillOptionData.EKey, lpMsg->SkillOptionData.ChatWindow, lpMsg->account, lpMsg->name);

	gQueryManager.BindParameterAsBinary(1, lpMsg->Inventory[0], sizeof(lpMsg->Inventory));
	gQueryManager.BindParameterAsBinary(2, lpMsg->Skill[0], sizeof(lpMsg->Skill));
	gQueryManager.BindParameterAsBinary(3, lpMsg->Quest, sizeof(lpMsg->Quest));
	gQueryManager.BindParameterAsBinary(4, lpMsg->Effect[0], sizeof(lpMsg->Effect));
	gQueryManager.BindParameterAsBinary(5, lpMsg->SkillOptionData.SkillKey, sizeof(lpMsg->SkillOptionData.SkillKey));
	gQueryManager.Query(query);
	gQueryManager.Close();
}

void GDInventoryItemSaveRecv(SDHP_INVENTORY_ITEM_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.BindParameterAsBinary(1, lpMsg->Inventory[0], sizeof(lpMsg->Inventory));
	gQueryManager.ExecQuery("UPDATE Character SET Inventory=? WHERE AccountID='%s' AND Name='%s'", lpMsg->account, lpMsg->name);
	gQueryManager.Close();
}

void GDPetItemInfoSaveRecv(SDHP_PET_ITEM_INFO_SAVE_RECV* lpMsg) // OK
{
	for (int n = 0; n < lpMsg->count; n++)
	{
		SDHP_PET_ITEM_INFO_SAVE* lpInfo = (SDHP_PET_ITEM_INFO_SAVE*)(((BYTE*)lpMsg) + sizeof(SDHP_PET_ITEM_INFO_SAVE_RECV) + (sizeof(SDHP_PET_ITEM_INFO_SAVE) * n));

		if (gQueryManager.ExecQuery("SELECT ItemSerial FROM T_PetItem_Info WHERE ItemSerial=%d", lpInfo->serial) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
			gQueryManager.ExecQuery("INSERT INTO T_PetItem_Info (ItemSerial,Pet_Level,Pet_Exp) VALUES (%d,%d,%d)", lpInfo->serial, lpInfo->level, lpInfo->experience);
			gQueryManager.Close();
		}
		else
		{
			gQueryManager.Close();
			gQueryManager.ExecQuery("UPDATE T_PetItem_Info SET Pet_Level=%d,Pet_Exp=%d WHERE ItemSerial=%d", lpInfo->level, lpInfo->experience, lpInfo->serial);
			gQueryManager.Close();
		}
	}
}

void GDResetInfoSaveRecv(SDHP_RESET_INFO_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("EXEC WZ_SetResetInfo '%s','%s','%d','%d','%d','%d'", lpMsg->account, lpMsg->name, lpMsg->Reset, lpMsg->ResetDay, lpMsg->ResetWek, lpMsg->ResetMon);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GDMasterResetInfoSaveRecv(SDHP_MASTER_RESET_INFO_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("EXEC WZ_SetMasterResetInfo '%s','%s','%d','%d','%d','%d','%d'", lpMsg->account, lpMsg->name, lpMsg->Reset, lpMsg->MasterReset, lpMsg->MasterResetDay, lpMsg->MasterResetWek, lpMsg->MasterResetMon);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GDRankingDuelSaveRecv(SDHP_RANKING_DUEL_SAVE_RECV* lpMsg) // OK
{
	if (gQueryManager.ExecQuery("SELECT Name FROM RankingDuel WHERE Name='%s'", lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingDuel (Name,WinScore,LoseScore) VALUES ('%s',%d,%d)", lpMsg->name, lpMsg->WinScore, lpMsg->LoseScore);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingDuel SET WinScore=WinScore+%d,LoseScore=LoseScore+%d WHERE Name='%s'", lpMsg->WinScore, lpMsg->LoseScore, lpMsg->name);
		gQueryManager.Close();
	}
}

void GDRankingBloodCastleSaveRecv(SDHP_RANKING_BLOOD_CASTLE_SAVE_RECV* lpMsg) // OK
{
	if (gQueryManager.ExecQuery("SELECT Name FROM RankingBloodCastle WHERE Name='%s'", lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingBloodCastle (Name,Score) VALUES ('%s',%d)", lpMsg->name, lpMsg->score);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingBloodCastle SET Score=Score+%d WHERE Name='%s'", lpMsg->score, lpMsg->name);
		gQueryManager.Close();
	}
}

void GDRankingChaosCastleSaveRecv(SDHP_RANKING_CHAOS_CASTLE_SAVE_RECV* lpMsg) // OK
{
	if (gQueryManager.ExecQuery("SELECT Name FROM RankingChaosCastle WHERE Name='%s'", lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingChaosCastle (Name,Score) VALUES ('%s',%d)", lpMsg->name, lpMsg->score);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingChaosCastle SET Score=Score+%d WHERE Name='%s'", lpMsg->score, lpMsg->name);
		gQueryManager.Close();
	}
}

void GDRankingDevilSquareSaveRecv(SDHP_RANKING_DEVIL_SQUARE_SAVE_RECV* lpMsg) // OK
{
	if (gQueryManager.ExecQuery("SELECT Name FROM RankingDevilSquare WHERE Name='%s'", lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingDevilSquare (Name,Score) VALUES ('%s',%d)", lpMsg->name, lpMsg->score);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingDevilSquare SET Score=Score+%d WHERE Name='%s'", lpMsg->score, lpMsg->name);
		gQueryManager.Close();
	}
}

void GDRankingIllusionTempleSaveRecv(SDHP_RANKING_ILLUSION_TEMPLE_SAVE_RECV* lpMsg) // OK
{
	if (gQueryManager.ExecQuery("SELECT Name FROM RankingIllusionTemple WHERE Name='%s'", lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingIllusionTemple (Name,Score) VALUES ('%s',%d)", lpMsg->name, lpMsg->score);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingIllusionTemple SET Score=Score+%d WHERE Name='%s'", lpMsg->score, lpMsg->name);
		gQueryManager.Close();
	}
}

void GDCreationCardSaveRecv(SDHP_CREATION_CARD_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("UPDATE AccountCharacter SET ExtClass=%d WHERE Id='%s'", lpMsg->ExtClass, lpMsg->account);
	gQueryManager.Close();
}

void GDCrywolfInfoSaveRecv(SDHP_CRYWOLF_INFO_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("EXEC WZ_CW_InfoSave '%d','%d','%d'", lpMsg->MapServerGroup, lpMsg->CrywolfState, lpMsg->OccupationState);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GDCustomMonsterRewardSaveRecv(SDHP_CUSTOM_MONSTER_REWARD_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("EXEC WZ_CustomMonsterReward '%s','%s','%d','%d','%d','%d'", lpMsg->account, lpMsg->name, lpMsg->MonsterClass, lpMsg->MapNumber, lpMsg->RewardValue1, lpMsg->RewardValue2);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GDRankingCustomArenaSaveRecv(SDHP_RANKING_CUSTOM_ARENA_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("EXEC WZ_CustomArenaRanking '%s','%s','%d','%d','%d'", lpMsg->account, lpMsg->name, lpMsg->number, lpMsg->score, lpMsg->rank);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GDConnectCharacterRecv(SDHP_CONNECT_CHARACTER_RECV* lpMsg, int index) // OK
{
	CHARACTER_INFO CharacterInfo;

	if (gCharacterManager.GetCharacterInfo(&CharacterInfo, lpMsg->name) != 0)
	{
		return;
	}

	strcpy_s(CharacterInfo.Name, lpMsg->name);

	strcpy_s(CharacterInfo.Account, lpMsg->account);

	CharacterInfo.UserIndex = lpMsg->index;

	CharacterInfo.GameServerCode = gServerManager[index].m_ServerCode;

	gCharacterManager.InsertCharacterInfo(CharacterInfo);

	FriendStateRecv(lpMsg->name, 0);
}

void GDDisconnectCharacterRecv(SDHP_DISCONNECT_CHARACTER_RECV* lpMsg, int index) // OK
{
	CHARACTER_INFO CharacterInfo;

	if (gCharacterManager.GetCharacterInfo(&CharacterInfo, lpMsg->name) == 0)
	{
		return;
	}

	if (CharacterInfo.UserIndex != lpMsg->index)
	{
		return;
	}

	if (CharacterInfo.GameServerCode != gServerManager[index].m_ServerCode)
	{
		return;
	}

	gCharacterManager.RemoveCharacterInfo(CharacterInfo);

	FriendStateRecv(lpMsg->name, 1);
}

void GDGlobalWhisperRecv(SDHP_GLOBAL_WHISPER_RECV* lpMsg, int index) // OK
{
	SDHP_GLOBAL_WHISPER_SEND pMsg;

	pMsg.header.set(0x72, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	CHARACTER_INFO CharacterInfo;

	if (gCharacterManager.GetCharacterInfo(&CharacterInfo, lpMsg->TargetName) == 0)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;
		DGGlobalWhisperEchoSend(CharacterInfo.GameServerCode, CharacterInfo.UserIndex, CharacterInfo.Account, CharacterInfo.Name, lpMsg->name, lpMsg->message);
	}

	memcpy(pMsg.TargetName, lpMsg->TargetName, sizeof(pMsg.TargetName));

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void DGGlobalWhisperEchoSend(WORD ServerCode, WORD index, char* account, char* name, char* SourceName, char* message) // OK
{
	SDHP_GLOBAL_WHISPER_ECHO_SEND pMsg;

	pMsg.header.set(0x73, sizeof(pMsg));

	pMsg.index = index;

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	memcpy(pMsg.SourceName, SourceName, sizeof(pMsg.SourceName));

	memcpy(pMsg.message, message, sizeof(pMsg.message));

	CServerManager* lpServerManager = FindServerByCode(ServerCode);

	if (lpServerManager != 0)
	{
		gSocketManager.DataSend(lpServerManager->m_index, (BYTE*)&pMsg, pMsg.header.size);
	}
}

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

void DS_GDReqCastleTotalInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CASTLEDATA* lpMsg = (CSP_REQ_CASTLEDATA*)lpRecv;
	CASTLE_DATA pCastleData;
	CSP_ANS_CASTLEDATA pMsgSend;

	pMsgSend.h.set(0x80, 0x00, sizeof(CSP_ANS_CASTLEDATA));

	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iRES = gCastleDBSet.DSDB_QueryCastleTotalInfo(lpMsg->wMapSvrNum, lpMsg->iCastleEventCycle, &pCastleData);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CASTLEDATA));
	}
	else
	{
		pMsgSend.iResult = 1;
		pMsgSend.wStartYear = pCastleData.wStartYear;
		pMsgSend.btStartMonth = pCastleData.btStartMonth;
		pMsgSend.btStartDay = pCastleData.btStartDay;
		pMsgSend.wEndYear = pCastleData.wEndYear;
		pMsgSend.btEndMonth = pCastleData.btEndMonth;
		pMsgSend.btEndDay = pCastleData.btEndDay;
		pMsgSend.btIsSiegeGuildList = pCastleData.btIsSiegeGuildList;
		pMsgSend.btIsSiegeEnded = pCastleData.btIsSiegeEnded;
		pMsgSend.btIsCastleOccupied = pCastleData.btIsCastleOccupied;
		pMsgSend.i64CastleMoney = pCastleData.i64CastleMoney;
		pMsgSend.iTaxRateChaos = pCastleData.iTaxRateChaos;
		pMsgSend.iTaxRateStore = pCastleData.iTaxRateStore;
		pMsgSend.iTaxHuntZone = pCastleData.iTaxHuntZone;
		pMsgSend.iFirstCreate = pCastleData.iFirstCreate;

		memset(pMsgSend.szCastleOwnGuild, 0, 8);
		memcpy(pMsgSend.szCastleOwnGuild, pCastleData.szCastleOwnGuild, 8);

		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CASTLEDATA));
	}
}

void DS_GDReqOwnerGuildMaster(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_OWNERGUILDMASTER* lpMsg = (CSP_REQ_OWNERGUILDMASTER*)lpRecv;
	CSP_ANS_OWNERGUILDMASTER pMsgSend;

	pMsgSend.h.set(0x80, 0x01, sizeof(CSP_ANS_OWNERGUILDMASTER));

	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;

	int iRES = gCastleDBSet.DSDB_QueryOwnerGuildMaster(lpMsg->wMapSvrNum, &pMsgSend);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_OWNERGUILDMASTER));
}

void DS_GDReqCastleNpcBuy(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCBUY* lpMsg = (CSP_REQ_NPCBUY*)lpRecv;
	CSP_ANS_NPCBUY pMsgSend;

	pMsgSend.h.set(0x80, 0x03, sizeof(CSP_ANS_NPCBUY));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	pMsgSend.iIndex = lpMsg->iIndex;
	pMsgSend.iNpcNumber = lpMsg->iNpcNumber;
	pMsgSend.iNpcIndex = lpMsg->iNpcIndex;
	pMsgSend.iBuyCost = lpMsg->iBuyCost;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryCastleNpcBuy(lpMsg->wMapSvrNum, lpMsg, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_NPCBUY));
}

void DS_GDReqCastleNpcRepair(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCREPAIR* lpMsg = (CSP_REQ_NPCREPAIR*)lpRecv;
	CSP_ANS_NPCREPAIR pMsgSend;

	pMsgSend.h.set(0x80, 0x04, sizeof(CSP_ANS_NPCREPAIR));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;
	pMsgSend.iNpcNumber = lpMsg->iNpcNumber;
	pMsgSend.iNpcIndex = lpMsg->iNpcIndex;
	pMsgSend.iRepairCost = lpMsg->iRepairCost;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryCastleNpcRepair(lpMsg->wMapSvrNum, lpMsg, &pMsgSend, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_NPCREPAIR));
}

void DS_GDReqCastleNpcUpgrade(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCUPGRADE* lpMsg = (CSP_REQ_NPCUPGRADE*)lpRecv;
	CSP_ANS_NPCUPGRADE pMsgSend;

	pMsgSend.h.set(0x80, 0x05, sizeof(CSP_ANS_NPCUPGRADE));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;
	pMsgSend.iNpcNumber = lpMsg->iNpcNumber;
	pMsgSend.iNpcIndex = lpMsg->iNpcIndex;
	pMsgSend.iNpcUpType = lpMsg->iNpcUpType;
	pMsgSend.iNpcUpValue = lpMsg->iNpcUpValue;
	pMsgSend.iNpcUpIndex = lpMsg->iNpcUpIndex;

	int iRES = gCastleDBSet.DSDB_QueryCastleNpcUpgrade(lpMsg->wMapSvrNum, lpMsg);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = 1;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_NPCUPGRADE));
}

void DS_GDReqTaxInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}
	CSP_REQ_TAXINFO* lpMsg = (CSP_REQ_TAXINFO*)lpRecv;
	CSP_ANS_TAXINFO pMsgSend;
	pMsgSend.h.set(0x80, 0x06, sizeof(CSP_ANS_TAXINFO));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;

	int iRES = gCastleDBSet.DSDB_QueryTaxInfo(lpMsg->wMapSvrNum, &pMsgSend);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = 1;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_TAXINFO));
}

void DS_GDReqTaxRateChange(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_TAXRATECHANGE* lpMsg = (CSP_REQ_TAXRATECHANGE*)lpRecv;
	CSP_ANS_TAXRATECHANGE pMsgSend;
	pMsgSend.h.set(0x80, 0x07, sizeof(CSP_ANS_TAXRATECHANGE));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryTaxRateChange(lpMsg->wMapSvrNum, lpMsg->iTaxKind, lpMsg->iTaxRate, &pMsgSend, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_TAXRATECHANGE));
}

void DS_GDReqCastleMoneyChange(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_MONEYCHANGE* lpMsg = (CSP_REQ_MONEYCHANGE*)lpRecv;
	CSP_ANS_MONEYCHANGE pMsgSend;

	pMsgSend.h.set(0x80, 0x08, sizeof(CSP_ANS_MONEYCHANGE));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;
	pMsgSend.iMoneyChanged = lpMsg->iMoneyChanged;

	int iQueryResult = 0;
	__int64 i64MoneyResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryCastleMoneyChange(lpMsg->wMapSvrNum, lpMsg->iMoneyChanged, &i64MoneyResult, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_MONEYCHANGE));
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
		pMsgSend.i64CastleMoney = i64MoneyResult;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_MONEYCHANGE));
	}
}

void DS_GDReqSiegeDateChange(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}
	CSP_REQ_SDEDCHANGE* lpMsg = (CSP_REQ_SDEDCHANGE*)lpRecv;
	CSP_ANS_SDEDCHANGE pMsgSend;
	pMsgSend.h.set(0x80, 0x09, sizeof(CSP_ANS_SDEDCHANGE));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QuerySiegeDateChange(lpMsg->wMapSvrNum, lpMsg, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_SDEDCHANGE));
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
		pMsgSend.wStartYear = lpMsg->wStartYear;
		pMsgSend.btStartMonth = lpMsg->btStartMonth;
		pMsgSend.btStartDay = lpMsg->btStartDay;
		pMsgSend.wEndYear = lpMsg->wEndYear;
		pMsgSend.btEndMonth = lpMsg->btEndMonth;
		pMsgSend.btEndDay = lpMsg->btEndDay;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_SDEDCHANGE));
	}
}

void DS_GDReqGuildMarkRegInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_GUILDREGINFO* lpMsg = (CSP_REQ_GUILDREGINFO*)lpRecv;

	int iQueryResult = 0;

	CSP_ANS_GUILDREGINFO pMsgSend;

	pMsgSend.h.set(0x80, 0xA, sizeof(CSP_ANS_GUILDREGINFO));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;

	char szGuildName[9] = {'\0'};
	memcpy(szGuildName, lpMsg->szGuildName, 8);

	int iRES = gCastleDBSet.DSDB_QueryGuildMarkRegInfo(lpMsg->wMapSvrNum, szGuildName, &pMsgSend, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_GUILDREGINFO));
}

void DS_GDReqSiegeEndedChange(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_SIEGEENDCHANGE* lpMsg = (CSP_REQ_SIEGEENDCHANGE*)lpRecv;
	CSP_ANS_SIEGEENDCHANGE pMsgSend;

	pMsgSend.h.set(0x80, 0x0B, sizeof(CSP_ANS_SIEGEENDCHANGE));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QuerySiegeEndedChange(lpMsg->wMapSvrNum, lpMsg->bIsSiegeEnded, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_SIEGEENDCHANGE));
}

void DS_GDReqCastleOwnerChange(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CASTLEOWNERCHANGE* lpMsg = (CSP_REQ_CASTLEOWNERCHANGE*)lpRecv;
	CSP_ANS_CASTLEOWNERCHANGE pMsgSend;

	pMsgSend.h.set(0x80, 0x0C, sizeof(CSP_ANS_CASTLEOWNERCHANGE));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryCastleOwnerChange(lpMsg->wMapSvrNum, lpMsg, &pMsgSend, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CASTLEOWNERCHANGE));
}

void DS_GDReqRegAttackGuild(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_REGATTACKGUILD* lpMsg = (CSP_REQ_REGATTACKGUILD*)lpRecv;
	CSP_ANS_REGATTACKGUILD pMsgSend;

	pMsgSend.h.set(0x80, 0x0D, sizeof(CSP_ANS_REGATTACKGUILD));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryRegAttackGuild(lpMsg->wMapSvrNum, lpMsg, &pMsgSend, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_REGATTACKGUILD));
}

void DS_GDReqRestartCastleState(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CASTLESIEGEEND* lpMsg = (CSP_REQ_CASTLESIEGEEND*)lpRecv;
	CSP_ANS_CASTLESIEGEEND pMsgSend;

	pMsgSend.h.set(0x80, 0x0E, sizeof(CSP_ANS_CASTLESIEGEEND));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryRestartCastleState(lpMsg->wMapSvrNum, lpMsg, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CASTLESIEGEEND));
}

void DS_GDReqMapSvrMsgMultiCast(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_MAPSVRMULTICAST* lpMsg = (CSP_REQ_MAPSVRMULTICAST*)lpRecv;
	CSP_ANS_MAPSVRMULTICAST pMsgSend;

	pMsgSend.h.set(0x80, 0x0F, sizeof(CSP_ANS_MAPSVRMULTICAST));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	memcpy(pMsgSend.szMsgText, lpMsg->szMsgText, sizeof(lpMsg->szMsgText));

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != 0)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsgSend, sizeof(CSP_ANS_MAPSVRMULTICAST));
		}
	}
}

void DS_GDReqRegGuildMark(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_GUILDREGMARK* lpMsg = (CSP_REQ_GUILDREGMARK*)lpRecv;
	CSP_ANS_GUILDREGMARK pMsgSend;

	pMsgSend.h.set(0x80, 0x10, sizeof(CSP_ANS_GUILDREGMARK));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;
	pMsgSend.iItemPos = lpMsg->iItemPos;

	char szGuildName[9] = {'\0'};
	memcpy(szGuildName, lpMsg->szGuildName, 8);

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryGuildMarkRegMark(lpMsg->wMapSvrNum, szGuildName, &pMsgSend, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_GUILDREGMARK));
}

void DS_GDReqGuildMarkReset(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_GUILDRESETMARK* lpMsg = (CSP_REQ_GUILDRESETMARK*)lpRecv;
	CSP_ANS_GUILDRESETMARK pMsgSend;

	pMsgSend.h.set(0x80, 0x11, sizeof(CSP_ANS_GUILDRESETMARK));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	char szGuildName[9] = {'\0'};
	memcpy(szGuildName, lpMsg->szGuildName, 8);

	int iRES = gCastleDBSet.DSDB_QueryGuildMarkReset(lpMsg->wMapSvrNum, szGuildName, &pMsgSend);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_GUILDRESETMARK));
}

void DS_GDReqGuildSetGiveUp(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_GUILDSETGIVEUP* lpMsg = (CSP_REQ_GUILDSETGIVEUP*)lpRecv;
	CSP_ANS_GUILDSETGIVEUP pMsgSend;

	pMsgSend.h.set(0x80, 0x12, sizeof(CSP_ANS_GUILDSETGIVEUP));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iIndex = lpMsg->iIndex;

	char szGuildName[9] = {'\0'};
	memcpy(szGuildName, lpMsg->szGuildName, 8);

	int iRES = gCastleDBSet.DSDB_QueryGuildSetGiveUp(lpMsg->wMapSvrNum, szGuildName, lpMsg->bIsGiveUp, &pMsgSend);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_GUILDSETGIVEUP));
}

void DS_GDReqCastleNpcRemove(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCREMOVE* lpMsg = (CSP_REQ_NPCREMOVE*)lpRecv;
	CSP_ANS_NPCREMOVE pMsgSend;

	pMsgSend.h.set(0x80, 0x16, sizeof(CSP_ANS_NPCREMOVE));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iNpcNumber = lpMsg->iNpcNumber;
	pMsgSend.iNpcIndex = lpMsg->iNpcIndex;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryCastleNpcRemove(lpMsg->wMapSvrNum, lpMsg, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_NPCREMOVE));
}

void DS_GDReqCastleStateSync(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CASTLESTATESYNC* lpMsg = (CSP_REQ_CASTLESTATESYNC*)lpRecv;
	CSP_ANS_CASTLESTATESYNC pMsgSend;

	pMsgSend.h.set(0x80, 0x17, sizeof(CSP_ANS_CASTLESTATESYNC));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;
	pMsgSend.iCastleState = lpMsg->iCastleState;
	pMsgSend.iTaxRateChaos = lpMsg->iTaxRateChaos;
	pMsgSend.iTaxRateStore = lpMsg->iTaxRateStore;
	pMsgSend.iTaxHuntZone = lpMsg->iTaxHuntZone;
	memcpy(pMsgSend.szOwnerGuildName, lpMsg->szOwnerGuildName, 8);

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != 0)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CASTLESTATESYNC));
		}
	}
}

void DS_GDReqCastleTributeMoney(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CASTLETRIBUTEMONEY* lpMsg = (CSP_REQ_CASTLETRIBUTEMONEY*)lpRecv;
	CSP_ANS_CASTLETRIBUTEMONEY pMsgSend;

	pMsgSend.h.set(0x80, 0x18, sizeof(CSP_ANS_CASTLETRIBUTEMONEY));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iQueryResult = 0;
	__int64 i64MoneyResult = 0;

	if (lpMsg->iCastleTributeMoney < 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		BOOL bResult = gCastleDBSet.DSDB_QueryCastleMoneyChange(lpMsg->wMapSvrNum, lpMsg->iCastleTributeMoney, &i64MoneyResult, &iQueryResult);

		if (bResult != 0)
		{
			pMsgSend.iResult = 0;
			gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CASTLETRIBUTEMONEY));
			return;
		}

		pMsgSend.iResult = iQueryResult;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CASTLETRIBUTEMONEY));
}

void DS_GDReqResetCastleTaxInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_RESETCASTLETAXINFO* lpMsg = (CSP_REQ_RESETCASTLETAXINFO*)lpRecv;
	CSP_ANS_RESETCASTLETAXINFO pMsgSend;

	pMsgSend.h.set(0x80, 0x19, sizeof(CSP_ANS_RESETCASTLETAXINFO));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryResetCastleTaxInfo(lpMsg->wMapSvrNum, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_RESETCASTLETAXINFO));
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_RESETCASTLETAXINFO));
	}
}

void DS_GDReqResetSiegeGuildInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_RESETSIEGEGUILDINFO* lpMsg = (CSP_REQ_RESETSIEGEGUILDINFO*)lpRecv;
	CSP_ANS_RESETSIEGEGUILDINFO pMsgSend;

	pMsgSend.h.set(0x80, 0x1A, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryResetSiegeGuildInfo(lpMsg->wMapSvrNum, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	}
}

void DS_GDReqResetRegSiegeInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_RESETSIEGEGUILDINFO* lpMsg = (CSP_REQ_RESETSIEGEGUILDINFO*)lpRecv;
	CSP_ANS_RESETSIEGEGUILDINFO pMsgSend;

	pMsgSend.h.set(0x80, 0x1B, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iQueryResult = 0;

	int iRES = gCastleDBSet.DSDB_QueryResetRegSiegeInfo(lpMsg->wMapSvrNum, &iQueryResult);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	}
	else
	{
		pMsgSend.iResult = iQueryResult;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_RESETSIEGEGUILDINFO));
	}
}

void DS_GDReqCastleInitData(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CSINITDATA* lpMsg = (CSP_REQ_CSINITDATA*)lpRecv;
	CASTLE_DATA pCastleData;

	char cBUFFER[5920];

	CSP_ANS_CSINITDATA* lpMsgSend = (CSP_ANS_CSINITDATA*)cBUFFER;
	CSP_NPCDATA* lpMsgSendBody = (CSP_NPCDATA*)&cBUFFER[64];

	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;

	int iDataCount = 200;

	lpMsgSend->iCount = 0;

	int iRES = gCastleDBSet.DSDB_QueryCastleTotalInfo(lpMsg->wMapSvrNum, lpMsg->iCastleEventCycle, &pCastleData);

	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set(0x81, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
	}
	else
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->wStartYear = pCastleData.wStartYear;
		lpMsgSend->btStartMonth = pCastleData.btStartMonth;
		lpMsgSend->btStartDay = pCastleData.btStartDay;
		lpMsgSend->wEndYear = pCastleData.wEndYear;
		lpMsgSend->btEndMonth = pCastleData.btEndMonth;
		lpMsgSend->btEndDay = pCastleData.btEndDay;
		lpMsgSend->btIsSiegeGuildList = pCastleData.btIsSiegeGuildList;
		lpMsgSend->btIsSiegeEnded = pCastleData.btIsSiegeEnded;
		lpMsgSend->btIsCastleOccupied = pCastleData.btIsCastleOccupied;
		lpMsgSend->i64CastleMoney = pCastleData.i64CastleMoney;
		lpMsgSend->iTaxRateChaos = pCastleData.iTaxRateChaos;
		lpMsgSend->iTaxRateStore = pCastleData.iTaxRateStore;
		lpMsgSend->iTaxHuntZone = pCastleData.iTaxHuntZone;
		lpMsgSend->iFirstCreate = pCastleData.iFirstCreate;

		memset(lpMsgSend->szCastleOwnGuild, 0, 8);
		memcpy(lpMsgSend->szCastleOwnGuild, pCastleData.szCastleOwnGuild, 8);

		iRES = gCastleDBSet.DSDB_QueryCastleNpcInfo(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);

		if (iRES != 0)
		{
			lpMsgSend->iResult = 0;
			lpMsgSend->h.set(0x81, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
			gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
		}
		else
		{
			lpMsgSend->iResult = 1;
			lpMsgSend->iCount = iDataCount;
			lpMsgSend->h.set(0x81, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
			gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSINITDATA));
		}
	}
}

void DS_GDReqCastleNpcInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCDATA* lpMsg = (CSP_REQ_NPCDATA*)lpRecv;

	char cBUFFER[5876];

	CSP_ANS_NPCDATA* lpMsgSend = (CSP_ANS_NPCDATA*)cBUFFER;
	CSP_NPCDATA* lpMsgSendBody = (CSP_NPCDATA*)&cBUFFER[20];

	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;

	int iDataCount = 200;

	lpMsgSend->iCount = 0;

	int iRES = gCastleDBSet.DSDB_QueryCastleNpcInfo(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);

	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set(0x82, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_NPCDATA));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_NPCDATA));
	}
	else
	{
		lpMsgSend->iResult = 1;
		lpMsgSend->iCount = iDataCount;
		lpMsgSend->h.set(0x82, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_NPCDATA));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_NPCDATA) * lpMsgSend->iCount) + sizeof(CSP_ANS_NPCDATA));
	}
}

void DS_GDReqAllGuildMarkRegInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_ALLGUILDREGINFO* lpMsg = (CSP_REQ_ALLGUILDREGINFO*)lpRecv;

	char cBUFFER[1876];

	CSP_ANS_ALLGUILDREGINFO* lpMsgSend = (CSP_ANS_ALLGUILDREGINFO*)cBUFFER;
	CSP_GUILDREGINFO* lpMsgSendBody = (CSP_GUILDREGINFO*)&cBUFFER[20];

	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;
	lpMsgSend->iIndex = lpMsg->iIndex;

	int iDataCount = 100;

	lpMsgSend->iCount = 0;

	int iRES = gCastleDBSet.DSDB_QueryAllGuildMarkRegInfo(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);

	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set(0x83, (sizeof(CSP_GUILDREGINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_ALLGUILDREGINFO));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_GUILDREGINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_ALLGUILDREGINFO));
	}
	else
	{
		lpMsgSend->iResult = 1;
		lpMsgSend->iCount = iDataCount;
		lpMsgSend->h.set(0x83, (sizeof(CSP_GUILDREGINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_ALLGUILDREGINFO));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_GUILDREGINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_ALLGUILDREGINFO));
	}
}

void DS_GDReqFirstCreateNPC(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCSAVEDATA* lpMsg = (CSP_REQ_NPCSAVEDATA*)lpRecv;
	CSP_NPCSAVEDATA* lpMsgBody = (CSP_NPCSAVEDATA*)&lpRecv[12];

	CSP_ANS_NPCSAVEDATA pMsgSend;
	pMsgSend.h.set(0x84, sizeof(CSP_ANS_NPCSAVEDATA));
	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iRES = gCastleDBSet.DSDB_QueryFirstCreateNPC(lpMsg->wMapSvrNum, lpMsg);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_NPCSAVEDATA));
	}
	else
	{
		pMsgSend.iResult = 1;
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_NPCSAVEDATA));
	}
}

void DS_GDReqCalcRegGuildList(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CALCREGGUILDLIST* lpMsg = (CSP_REQ_CALCREGGUILDLIST*)lpRecv;

	char cBUFFER[2672];

	CSP_ANS_CALCREGGUILDLIST* lpMsgSend = (CSP_ANS_CALCREGGUILDLIST*)cBUFFER;
	CSP_CALCREGGUILDLIST* lpMsgSendBody = (CSP_CALCREGGUILDLIST*)&cBUFFER[16];

	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;

	int iDataCount = 100;

	lpMsgSend->iCount = 0;

	int iRES = gCastleDBSet.DSDB_QueryCalcRegGuildList(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);

	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set(0x85, (sizeof(CSP_CALCREGGUILDLIST) * lpMsgSend->iCount) + sizeof(CSP_ANS_CALCREGGUILDLIST));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_CALCREGGUILDLIST) * lpMsgSend->iCount) + sizeof(CSP_ANS_CALCREGGUILDLIST));
	}
	else
	{
		lpMsgSend->iResult = 1;
		lpMsgSend->iCount = iDataCount;
		lpMsgSend->h.set(0x85, (sizeof(CSP_CALCREGGUILDLIST) * lpMsgSend->iCount) + sizeof(CSP_ANS_CALCREGGUILDLIST));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_CALCREGGUILDLIST) * lpMsgSend->iCount) + sizeof(CSP_ANS_CALCREGGUILDLIST));
	}
}

void DS_GDReqCsGuildUnionInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CSGUILDUNIONINFO* lpMsg = (CSP_REQ_CSGUILDUNIONINFO*)lpRecv;
	CSP_CSGUILDUNIONINFO* lpMsgBody = (CSP_CSGUILDUNIONINFO*)&lpRecv[12];

	char cBUFFER[1472];

	CSP_ANS_CSGUILDUNIONINFO* lpMsgSend = (CSP_ANS_CSGUILDUNIONINFO*)cBUFFER;
	CSP_CSGUILDUNIONINFO* lpMsgSendBody = (CSP_CSGUILDUNIONINFO*)&cBUFFER[16];

	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;

	if (lpMsg->iCount < 0)
	{
		lpMsg->iCount = 0;
	}

	int iRET_COUNT = 0;

	for (int iGCNT = 0; iGCNT < lpMsg->iCount; iGCNT++)
	{
		char szGuildName[9] = {'\0'};
		memcpy(szGuildName, lpMsgBody[iGCNT].szGuildName, 8);

		int iRES = gCastleDBSet.DSDB_QueryCsGuildUnionInfo(lpMsg->wMapSvrNum, szGuildName, lpMsgBody[iGCNT].iCsGuildID, lpMsgSendBody, &iRET_COUNT);

		if (iRES != 0)
		{
			lpMsgSend->iResult = 0;
			lpMsgSend->iCount = 0;
			lpMsgSend->h.set(0x86, (sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSGUILDUNIONINFO));
			gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSGUILDUNIONINFO));
			return;
		}
	}

	lpMsgSend->iResult = 1;
	lpMsgSend->iCount = iRET_COUNT;
	lpMsgSend->h.set(0x86, (sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSGUILDUNIONINFO));
	gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSGUILDUNIONINFO));
}

void DS_GDReqCsSaveTotalGuildInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CSSAVETOTALGUILDINFO* lpMsg = (CSP_REQ_CSSAVETOTALGUILDINFO*)lpRecv;
	CSP_CSSAVETOTALGUILDINFO* lpMsgBody = (CSP_CSSAVETOTALGUILDINFO*)&lpRecv[12];

	CSP_ANS_CSSAVETOTALGUILDINFO pMsgSend;

	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iRES = gCastleDBSet.DSDB_QueryCsClearTotalGuildInfo(lpMsg->wMapSvrNum);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
		pMsgSend.h.set(0x87, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
	}
	else
	{
		for (int iGCNT = 0; iGCNT < lpMsg->iCount; iGCNT++)
		{
			char szGuildName[9] = {'\0'};
			memcpy(szGuildName, lpMsgBody[iGCNT].szGuildName, 8);

			int iRES = gCastleDBSet.DSDB_QueryCsSaveTotalGuildInfo(lpMsg->wMapSvrNum, szGuildName, lpMsgBody[iGCNT].iCsGuildID, lpMsgBody[iGCNT].iGuildInvolved, lpMsgBody[iGCNT].iGuildScore);

			if (iRES != 0)
			{
				pMsgSend.iResult = 0;
				pMsgSend.h.set(0x87, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
				gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
				return;
			}
		}

		int iQueryResult = 0;

		iRES = gCastleDBSet.DSDB_QueryCsSaveTotalGuildOK(lpMsg->wMapSvrNum, &iQueryResult);

		if (iRES != 0)
		{
			pMsgSend.iResult = 0;
			pMsgSend.h.set(0x87, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
			gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
		}
		else
		{
			pMsgSend.iResult = 1;
			pMsgSend.iResult = 1; //??
			pMsgSend.h.set(0x87, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
			gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_CSSAVETOTALGUILDINFO));
		}
	}
}

void DS_GDReqCsLoadTotalGuildInfo(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_CSLOADTOTALGUILDINFO* lpMsg = (CSP_REQ_CSLOADTOTALGUILDINFO*)lpRecv;

	char cBUFFER[2272];

	CSP_ANS_CSLOADTOTALGUILDINFO* lpMsgSend = (CSP_ANS_CSLOADTOTALGUILDINFO*)cBUFFER;
	CSP_CSLOADTOTALGUILDINFO* lpMsgSendBody = (CSP_CSLOADTOTALGUILDINFO*)&cBUFFER[16];

	lpMsgSend->wMapSvrNum = lpMsg->wMapSvrNum;

	int iDataCount = 100;

	lpMsgSend->iCount = 0;

	int iRES = gCastleDBSet.DSDB_QueryCsLoadTotalGuildInfo(lpMsg->wMapSvrNum, lpMsgSendBody, &iDataCount);

	if (iRES != 0)
	{
		lpMsgSend->iResult = 0;
		lpMsgSend->h.set(0x88, (sizeof(CSP_CSLOADTOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_CSLOADTOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
	}
	else
	{
		lpMsgSend->iResult = 1;
		lpMsgSend->iCount = iDataCount;
		lpMsgSend->h.set(0x88, (sizeof(CSP_CSLOADTOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
		gSocketManager.DataSend(aIndex, (BYTE*)lpMsgSend, (sizeof(CSP_CSLOADTOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
	}
}

void DS_GDReqCastleNpcUpdate(BYTE* lpRecv, int aIndex)
{
	if (lpRecv == NULL)
	{
		return;
	}

	CSP_REQ_NPCUPDATEDATA* lpMsg = (CSP_REQ_NPCUPDATEDATA*)lpRecv;
	CSP_NPCUPDATEDATA* lpMsgBody = (CSP_NPCUPDATEDATA*)&lpRecv[12];

	CSP_ANS_NPCSAVEDATA pMsgSend;
	pMsgSend.h.set(0x89, sizeof(CSP_ANS_NPCSAVEDATA));

	pMsgSend.wMapSvrNum = lpMsg->wMapSvrNum;

	int iRES = gCastleDBSet.DSDB_QueryCastleNpcUpdate(lpMsg->wMapSvrNum, lpMsg);

	if (iRES != 0)
	{
		pMsgSend.iResult = 0;
	}
	else
	{
		pMsgSend.iResult = 1;
	}

	gSocketManager.DataSend(aIndex, (BYTE*)&pMsgSend, sizeof(CSP_ANS_NPCSAVEDATA));
}
