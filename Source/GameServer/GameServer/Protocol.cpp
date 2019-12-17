#include "stdafx.h"
#include "Protocol.h"
#include "Attack.h"
#include "BloodCastle.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "CastleSiegeWeapon.h"
#include "ChaosBox.h"
#include "ChaosCastle.h"
#include "CommandManager.h"
#include "CSProtocol.h"
#include "DarkSpirit.h"
#include "DefaultClassInfo.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "Duel.h"
#include "EffectManager.h"
#include "ESProtocol.h"
#include "Filter.h"
#include "Fruit.h"
#include "Guild.h"
#include "GuildClass.h"
#include "HackPacketCheck.h"
#include "ItemManager.h"
#include "JewelMix.h"
#include "JSProtocol.h"
#include "Log.h"
#include "Map.h"
#include "MapServerManager.h"
#include "Message.h"
#include "MiniMap.h"
#include "Move.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ObjectManager.h"
#include "Party.h"
#include "PersonalShop.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Trade.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"

void ProtocolCore(BYTE head, BYTE* lpMsg, int size, int aIndex, int encrypt, int serial) // OK
{
	if (gObj[aIndex].Type == OBJECT_USER && gHackPacketCheck.CheckPacketHack(aIndex, head, ((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]), encrypt, serial) == 0)
	{
		return;
	}

	switch (head)
	{
		case 0x00:
			CGChatRecv((PMSG_CHAT_RECV*)lpMsg, aIndex);
			break;
		case 0x02:
			CGChatWhisperRecv((PMSG_CHAT_WHISPER_RECV*)lpMsg, aIndex);
			break;
		case 0x03:
			CGMainCheckRecv((PMSG_MAIN_CHECK_RECV*)lpMsg, aIndex);
			break;
		case 0x0E:
			CGLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg, aIndex);
			break;
		case PROTOCOL_CODE2:
			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg, aIndex);
			break;
		case PROTOCOL_CODE3:
			CGPositionRecv((PMSG_POSITION_RECV*)lpMsg, aIndex);
			break;
		case 0x18:
			CGActionRecv((PMSG_ACTION_RECV*)lpMsg, aIndex);
			break;
		case 0x19:
			gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg, aIndex);
			break;
		case 0x1B:
			gSkillManager.CGSkillCancelRecv((PMSG_SKILL_CANCEL_RECV*)lpMsg, aIndex);
			break;
		case 0x1C:
			gMove.CGTeleportRecv((PMSG_TELEPORT_RECV*)lpMsg, aIndex);
			break;
		case 0x1E:
			gSkillManager.CGDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg, aIndex);
			break;
		case 0x22:
			gItemManager.CGItemGetRecv((PMSG_ITEM_GET_RECV*)lpMsg, aIndex);
			break;
		case 0x23:
			gItemManager.CGItemDropRecv((PMSG_ITEM_DROP_RECV*)lpMsg, aIndex);
			break;
		case 0x24:
			gItemManager.CGItemMoveRecv((PMSG_ITEM_MOVE_RECV*)lpMsg, aIndex);
			break;
		case 0x26:
			gItemManager.CGItemUseRecv((PMSG_ITEM_USE_RECV*)lpMsg, aIndex);
			break;
		case 0x30:
			gNpcTalk.CGNpcTalkRecv((PMSG_NPC_TALK_RECV*)lpMsg, aIndex);
			break;
		case 0x31:
			gNpcTalk.CGNpcTalkCloseRecv(aIndex);
			break;
		case 0x32:
			gItemManager.CGItemBuyRecv((PMSG_ITEM_BUY_RECV*)lpMsg, aIndex);
			break;
		case 0x33:
			gItemManager.CGItemSellRecv((PMSG_ITEM_SELL_RECV*)lpMsg, aIndex);
			break;
		case 0x34:
			gItemManager.CGItemRepairRecv((PMSG_ITEM_REPAIR_RECV*)lpMsg, aIndex);
			break;
		case 0x36:
			gTrade.CGTradeRequestRecv((PMSG_TRADE_REQUEST_RECV*)lpMsg, aIndex);
			break;
		case 0x37:
			gTrade.CGTradeResponseRecv((PMSG_TRADE_RESPONSE_RECV*)lpMsg, aIndex);
			break;
		case 0x3A:
			gTrade.CGTradeMoneyRecv((PMSG_TRADE_MONEY_RECV*)lpMsg, aIndex);
			break;
		case 0x3C:
			gTrade.CGTradeOkButtonRecv((PMSG_TRADE_OK_BUTTON_RECV*)lpMsg, aIndex);
			break;
		case 0x3D:
			gTrade.CGTradeCancelButtonRecv(aIndex);
			break;
		case 0x3F:
			switch (lpMsg[3])
			{
				case 0x01:
					gPersonalShop.CGPShopSetItemPriceRecv((PMSG_PSHOP_SET_ITEM_PRICE_RECV*)lpMsg, aIndex);
					break;
				case 0x02:
					gPersonalShop.CGPShopOpenRecv((PMSG_PSHOP_OPEN_RECV*)lpMsg, aIndex);
					break;
				case 0x03:
					gPersonalShop.CGPShopCloseRecv(aIndex);
					break;
				case 0x05:
					gPersonalShop.CGPShopItemListRecv((PMSG_PSHOP_ITEM_LIST_RECV*)lpMsg, aIndex);
					break;
				case 0x06:
					gPersonalShop.CGPShopBuyItemRecv((PMSG_PSHOP_BUY_ITEM_RECV*)lpMsg, aIndex);
					break;
				case 0x07:
					gPersonalShop.CGPShopLeaveRecv((PMSG_PSHOP_LEAVE_RECV*)lpMsg, aIndex);
					break;
			}
			break;
		case 0x40:
			gParty.CGPartyRequestRecv((PMSG_PARTY_REQUEST_RECV*)lpMsg, aIndex);
			break;
		case 0x41:
			gParty.CGPartyRequestResultRecv((PMSG_PARTY_REQUEST_RESULT_RECV*)lpMsg, aIndex);
			break;
		case 0x42:
			gParty.CGPartyListRecv(aIndex);
			break;
		case 0x43:
			gParty.CGPartyDelMemberRecv((PMSG_PARTY_DEL_MEMBER_RECV*)lpMsg, aIndex);
			break;
		case 0x50:
			gGuild.CGGuildRequestRecv((PMSG_GUILD_REQUEST_RECV*)lpMsg, aIndex);
			break;
		case 0x51:
			gGuild.CGGuildResultRecv((PMSG_GUILD_RESULT_RECV*)lpMsg, aIndex);
			break;
		case 0x52:
			gGuild.CGGuildListRecv(aIndex);
			break;
		case 0x53:
			gGuild.CGGuildDeleteRecv((PMSG_GUILD_DELETE_RECV*)lpMsg, aIndex);
			break;
		case 0x54:
			CGGuildMasterAnswerRecv((PMSG_GUILDMASTERANSWER*)lpMsg, aIndex);
			break;
		case 0x55:
			CGGuildMasterInfoSave(aIndex, (PMSG_GUILDINFOSAVE*)lpMsg);
			break;
		case 0x57:
			CGGuildMasterCreateCancel(aIndex);
			break;
		case 0x61:
			GCGuildWarRequestSendRecv((PMSG_GUILDWARSEND_RESULT*)lpMsg, aIndex);
			break;
		case 0x66:
			GCGuildViewportInfo((PMSG_REQ_GUILDVIEWPORT*)lpMsg, aIndex);
			break;
		case 0x81:
			gWarehouse.CGWarehouseMoneyRecv((PMSG_WAREHOUSE_MONEY_RECV*)lpMsg, aIndex);
			break;
		case 0x82:
			gWarehouse.CGWarehouseClose(aIndex);
			break;
		case 0x83:
			gWarehouse.CGWarehousePasswordRecv((PMSG_WAREHOUSE_PASSWORD_RECV*)lpMsg, aIndex);
			break;
		case 0x86:
			gChaosBox.CGChaosMixRecv((PMSG_CHAOS_MIX_RECV*)lpMsg, aIndex);
			break;
		case 0x87:
			gChaosBox.CGChaosMixCloseRecv(aIndex);
			break;
		case 0x90:
			gDevilSquare.CGDevilSquareEnterRecv((PMSG_DEVIL_SQUARE_ENTER_RECV*)lpMsg, aIndex);
			break;
		case 0x91:
			CGEventRemainTimeRecv((PMSG_EVENT_REMAIN_TIME_RECV*)lpMsg, aIndex);
			break;
		case 0x9A:
			gBloodCastle.CGBloodCastleEnterRecv((PMSG_BLOOD_CASTLE_ENTER_RECV*)lpMsg, aIndex);
			break;
		case 0xA0:
			gQuest.CGQuestInfoRecv(aIndex);
			break;
		case 0xA2:
			gQuest.CGQuestStateRecv((PMSG_QUEST_STATE_RECV*)lpMsg, aIndex);
			break;
		case 0xA7:
			CGPetItemCommandRecv((PMSG_PET_ITEM_COMMAND_RECV*)lpMsg, aIndex);
			break;
		case 0xA9:
			CGPetItemInfoRecv((PMSG_PET_ITEM_INFO_RECV*)lpMsg, aIndex);
			break;
		case 0xAA:
			gDuel.CGDuelStartRecv((PMSG_DUEL_START_RECV*)lpMsg, aIndex);
			break;
		case 0xAB:
			gDuel.CGDuelEndRecv(aIndex);
			break;
		case 0xAC:
			gDuel.CGDuelOkRecv((PMSG_DUEL_OK_RECV*)lpMsg, aIndex);
			break;
		case 0xAF:
			switch (lpMsg[3])
			{
				case 0x01:
					gChaosCastle.CGChaosCastleEnterRecv((PMSG_CHAOS_CASTLE_ENTER_RECV*)lpMsg, aIndex);
					break;
				case 0x02:
					gChaosCastle.CGChaosCastlePositionRecv((PMSG_CHAOS_CASTLE_POSITION_RECV*)lpMsg, aIndex);
					break;
			}
			break;
		case 0xB0:
			gSkillManager.CGSkillTeleportAllyRecv((PMSG_SKILL_TELEPORT_ALLY_RECV*)lpMsg, aIndex);
			break;
		case 0xB1:
			switch (lpMsg[3])
			{
				case 0x01:
					CGMapServerMoveAuthRecv((PMSG_MAP_SERVER_MOVE_AUTH_RECV*)lpMsg, aIndex);
					break;
			}
			break;
		case 0xB2:
			switch (lpMsg[3])
			{
				case 0x00:
					CGReqCastleSiegeState((PMSG_REQ_CASTLESIEGESTATE*)lpMsg, aIndex);
					break;
				case 0x01:
					CGReqRegCastleSiege((PMSG_REQ_REGCASTLESIEGE*)lpMsg, aIndex);
					break;
				case 0x02:
					CGReqGiveUpCastleSiege((PMSG_REQ_GIVEUPCASTLESIEGE*)lpMsg, aIndex);
					break;
				case 0x03:
					CGReqGuildRegInfo((PMSG_REQ_GUILDREGINFO*)lpMsg, aIndex);
					break;
				case 0x04:
					CGReqRegGuildMark((PMSG_REQ_REGGUILDMARK*)lpMsg, aIndex);
					break;
				case 0x05:
					CGReqNpcBuy((PMSG_REQ_NPCBUY*)lpMsg, aIndex);
					break;
				case 0x06:
					CGReqNpcRepair((PMSG_REQ_NPCREPAIR*)lpMsg, aIndex);
					break;
				case 0x07:
					CGReqNpcUpgrade((PMSG_REQ_NPCUPGRADE*)lpMsg, aIndex);
					break;
				case 0x08:
					CGReqTaxMoneyInfo((PMSG_REQ_TAXMONEYINFO*)lpMsg, aIndex);
					break;
				case 0x09:
					CGReqTaxRateChange((PMSG_REQ_TAXRATECHANGE*)lpMsg, aIndex);
					break;
				case 0x10:
					CGReqMoneyDrawOut((PMSG_REQ_MONEYDRAWOUT*)lpMsg, aIndex);
					break;
				case 0x12:
					CGReqCsGateOperate((PMSG_REQ_CSGATEOPERATE*)lpMsg, aIndex);
					break;
				case 0x1B:
					CGReqCsMiniMapData((PMSG_REQ_MINIMAPDATA*)lpMsg, aIndex);
					break;
				case 0x1C:
					CGReqStopCsMiniMapData((PMSG_REQ_STOPMINIMAPDATA*)lpMsg, aIndex);
					break;
				case 0x1D:
					CGReqCsSendCommand((PMSG_REQ_CSCOMMAND*)lpMsg, aIndex);
					break;
				case 0x1F:
					CGReqCsSetEnterHuntZone((PMSG_REQ_CSHUNTZONEENTER*)lpMsg, aIndex);
					break;
			}
			break;
		case 0xB3:
			CGReqNpcDbList((PMSG_REQ_NPCDBLIST*)lpMsg, aIndex);
			break;
		case 0xB4:
			CGReqCsRegGuildList((PMSG_REQ_CSREGGUILDLIST*)lpMsg, aIndex);
			break;
		case 0xB5:
			CGReqCsAttkGuildList((PMSG_REQ_CSATTKGUILDLIST*)lpMsg, aIndex);
			break;
		case 0xB7:
			switch (lpMsg[3])
			{
				case 0x01:
					gCastleSiegeWeapon.CGCastleSiegeWeaponUseRecv((PMSG_CASTLE_SIEGE_WEAPON_USE_RECV*)lpMsg, aIndex);
					break;
				case 0x04:
					gCastleSiegeWeapon.CGCastleSiegeWeaponDamageRecv((PMSG_CASTLE_SIEGE_WEAPON_DAMAGE_RECV*)lpMsg, aIndex);
					break;
			}
			break;
		case 0xB9:
			switch (lpMsg[3])
			{
				case 0x02:
					CGReqGuildMarkOfCastleOwner((PMSG_REQ_GUILDMARK_OF_CASTLEOWNER*)lpMsg, aIndex);
					break;
				case 0x05:
					CGReqCastleHuntZoneEntrance((PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE*)lpMsg, aIndex);
					break;
			}
			break;
		case 0xBC:
			switch (lpMsg[3])
			{
				case 0x00:
					gJewelMix.CGJewelMixRecv((PMSG_JEWEL_MIX_RECV*)lpMsg, aIndex);
					break;
				case 0x01:
					gJewelMix.CGJewelUnMixRecv((PMSG_JEWEL_UNMIX_RECV*)lpMsg, aIndex);
					break;
			}
			break;
		case 0xC0:
			FriendListRequest(aIndex);
			break;
		case 0xC1:
			FriendAddRequest((PMSG_FRIEND_ADD_REQ*)lpMsg, aIndex);
			break;
		case 0xC2:
			WaitFriendAddRequest((PMSG_FRIEND_ADD_SIN_RESULT*)lpMsg, aIndex);
			break;
		case 0xC3:
			FriendDelRequest((PMSG_FRIEND_DEL_REQ*)lpMsg, aIndex);
			break;
		case 0xC4:
			FriendStateClientRecv((PMSG_FRIEND_STATE_C*)lpMsg, aIndex);
			break;
		case 0xC5:
			FriendMemoSend((PMSG_FRIEND_MEMO*)lpMsg, aIndex);
			break;
		case 0xC7:
			FriendMemoReadReq((PMSG_FRIEND_READ_MEMO_REQ*)lpMsg, aIndex);
			break;
		case 0xC8:
			FriendMemoDelReq((PMSG_FRIEND_MEMO_DEL_REQ*)lpMsg, aIndex);
			break;
		case 0xC9:
			FriendMemoListReq(aIndex);
			break;
		case 0xCA:
			FriendChatRoomCreateReq((PMSG_FRIEND_ROOMCREATE_REQ*)lpMsg, aIndex);
			break;
		case 0xCB:
			FriendRoomInvitationReq((PMSG_ROOM_INVITATION*)lpMsg, aIndex);
			break;
		case PROTOCOL_CODE1:
			CGMoveRecv((PMSG_MOVE_RECV*)lpMsg, aIndex);
			break;
		case PROTOCOL_CODE4:
			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)lpMsg, aIndex, 0);
			break;
		case 0xE1:
			gGuild.CGGuildAssignStatusRecv((PMSG_GUILD_ASSIGN_STATUS_RECV*)lpMsg, aIndex);
			break;
		case 0xE2:
			CGGuildAssignType((PMSG_GUILD_ASSIGN_TYPE_REQ*)lpMsg, aIndex);
			break;
		case 0xE5:
			CGRelationShipReqJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ*)lpMsg, aIndex);
			break;
		case 0xE6:
			CGRelationShipAnsJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS*)lpMsg, aIndex);
			break;
		case 0xE9:
			CGUnionList((PMSG_UNIONLIST_REQ*)lpMsg, aIndex);
			break;
		case 0xEB:
			switch (lpMsg[3])
			{
				case 0x01:
					CGRelationShipReqKickOutUnionMember((PMSG_KICKOUT_UNIONMEMBER_REQ*)lpMsg, aIndex);
					break;
			}
			break;
		case 0xF1:
			switch (lpMsg[3])
			{
				case 0x01:
					CGConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)lpMsg, aIndex);
					break;
				case 0x02:
					CGCloseClientRecv((PMSG_CLOSE_CLIENT_RECV*)lpMsg, aIndex);
					break;
				case 0x03:
					// ??
					break;
			}
			break;
		case 0xF3:
			switch (lpMsg[3])
			{
				case 0x00:
					CGCharacterListRecv(aIndex);
					break;
				case 0x01:
					CGCharacterCreateRecv((PMSG_CHARACTER_CREATE_RECV*)lpMsg, aIndex);
					break;
				case 0x02:
					CGCharacterDeleteRecv((PMSG_CHARACTER_DELETE_RECV*)lpMsg, aIndex);
					break;
				case 0x03:
					CGCharacterInfoRecv((PMSG_CHARACTER_INFO_RECV*)lpMsg, aIndex);
					break;
				case 0x06:
					CGLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg, aIndex);
					break;
				case 0x12:
					CGCharacterMoveViewportEnableRecv(aIndex);
					break;
				case 0x30:
					CGOptionDataRecv((PMSG_OPTION_DATA_RECV*)lpMsg, aIndex);
					break;
			}
			break;
	}
}

void CGChatRecv(PMSG_CHAT_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char name[11] = {0};

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	if (strcmp(name, lpObj->Name) != 0)
	{
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message) - 1)] = 0;

	gLog.Output(LOG_CHAT, "[General][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, lpMsg->message);

	gFilter.CheckSyntax(lpMsg->message);

	if (lpMsg->message[0] == '/')
	{
		if (gCommandManager.ManagementCore(&gObj[aIndex], lpMsg->message) == 0)
		{
			return;
		}
		else
		{
			return;
		}
	}

	if (lpObj->ChatLimitTime > 0)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(269), lpObj->ChatLimitTime);
		return;
	}

	if ((lpObj->Penalty & 2) != 0)
	{
		return;
	}

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00, sizeof(pMsg));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	if (lpMsg->message[0] == '~')
	{
		if (OBJECT_RANGE(lpObj->PartyNumber) != 0)
		{
			for (int n = 0; n < MAX_PARTY_USER; n++)
			{
				if (OBJECT_RANGE(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n]) != 0)
				{
					DataSend(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n], (BYTE*)&pMsg, pMsg.header.size);
				}
			}
		}
	}
	else if (lpMsg->message[0] == '@')
	{
		if (lpObj->Guild != 0)
		{
			if (lpMsg->message[1] == '>')
			{
				if (strcmp(lpObj->Name, lpObj->Guild->Names[0]) == 0)
				{
					GDGuildNoticeSave(lpObj->Guild->Name, &lpMsg->message[2]);
				}
			}
			else if (lpMsg->message[1] == '@')
			{
				if (lpObj->Guild->GuildUnion != 0)
				{
					GDUnionServerGroupChattingSend(lpObj->Guild->GuildUnion, lpMsg->name, lpMsg->message);
				}
			}
			else
			{
				if (lpObj->Guild->Number != 0)
				{
					GDGuildServerGroupChattingSend(lpObj->Guild->Number, lpMsg->name, lpMsg->message);
				}
			}
		}
	}
	else
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void CGChatWhisperRecv(PMSG_CHAT_WHISPER_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char name[11] = {0};

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	if (lpObj->ChatLimitTime > 0)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(269), lpObj->ChatLimitTime);
		return;
	}

	if ((lpObj->Penalty & 2) != 0)
	{
		return;
	}

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		GDGlobalWhisperSend(aIndex, name, lpMsg->message);
		return;
	}

	if (aIndex == lpTarget->Index)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(270));
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message) - 1)] = 0;

	gLog.Output(LOG_CHAT, "[Whisper][%s][%s] - (Name: %s, Message: %s)", lpObj->Account, lpObj->Name, name, lpMsg->message);

	gFilter.CheckSyntax(lpMsg->message);

	GCChatWhisperSend(lpTarget->Index, lpObj->Name, lpMsg->message);
}

void CGMainCheckRecv(PMSG_MAIN_CHECK_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gServerInfo.m_MainChecksum != 0)
	{
		if (lpObj->CheckSumTableNum < 0 || lpObj->CheckSumTableNum >= MAX_CHECKSUM_KEY)
		{
			gLog.Output(LOG_HACK, "[%s][%s] Main CheckSum error", lpObj->Account, lpObj->Name);
			GCCloseClientSend(aIndex, 0);
			return;
		}

		if (gCheckSum[lpObj->CheckSumTableNum] != lpMsg->key)
		{
			gLog.Output(LOG_HACK, "[%s][%s] Invalid Main CheckSum", lpObj->Account, lpObj->Name);
			GCCloseClientSend(aIndex, 0);
			return;
		}
	}

	lpObj->CheckSumTime = 0;
}

void CGLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED && lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	if (gServerInfo.m_CheckLatencyHack != 0 && abs((int)((GetTickCount() - lpObj->ServerTickCount) - (lpMsg->TickCount - lpObj->ClientTickCount))) > gServerInfo.m_CheckLatencyHackTolerance)
	{
		gLog.Output(LOG_HACK, "[%s][%s] Client Latency Hack Detected [%d][%d]", lpObj->Account, lpObj->Name, (GetTickCount() - lpObj->ServerTickCount), (lpMsg->TickCount - lpObj->ClientTickCount));
		GCCloseClientSend(aIndex, 0);
		return;
	}

	if (lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->PhysiSpeed > lpObj->PhysiSpeed&& abs((int)(lpObj->PhysiSpeed - lpMsg->PhysiSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
	{
		gLog.Output(LOG_HACK, "[%s][%s] Client Speed Hack Detected [%d][%d]", lpObj->Account, lpObj->Name, lpObj->PhysiSpeed, lpMsg->PhysiSpeed);
		GCCloseClientSend(aIndex, 0);
		return;
	}

	if (lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->MagicSpeed > lpObj->MagicSpeed&& abs((int)(lpObj->MagicSpeed - lpMsg->MagicSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
	{
		gLog.Output(LOG_HACK, "[%s][%s] Client Speed Hack Detected [%d][%d]", lpObj->Account, lpObj->Name, lpObj->MagicSpeed, lpMsg->MagicSpeed);
		GCCloseClientSend(aIndex, 0);
		return;
	}

	lpObj->ConnectTickCount = GetTickCount();
}

void CGPositionRecv(PMSG_POSITION_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Teleport != 0)
	{
		return;
	}

	if (gObjCheckMapTile(lpObj, 2) != 0)
	{
		return;
	}

	if (CC_MAP_RANGE(lpObj->Map) != 0 && (GetTickCount() - lpObj->ChaosCastleBlowTime) < 1000)
	{
		return;
	}

	if (lpMsg->x < (lpObj->X - 15) || lpMsg->x >(lpObj->X + 15) || lpMsg->y < (lpObj->Y - 15) || lpMsg->y >(lpObj->Y + 15))
	{
		return;
	}

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);

	lpObj->X = lpMsg->x;
	lpObj->Y = lpMsg->y;
	lpObj->TX = lpMsg->x;
	lpObj->TY = lpMsg->y;
	lpObj->OldX = lpMsg->x;
	lpObj->OldY = lpMsg->y;

	gMap[lpObj->Map].SetStandAttr(lpObj->TX, lpObj->TY);

	PMSG_POSITION_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE3, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.x = (BYTE)lpObj->TX;

	pMsg.y = (BYTE)lpObj->TY;

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if (lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index, (BYTE*)&pMsg, pMsg.header.size);
			}
		}
	}
}

void CGActionRecv(PMSG_ACTION_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->Dir = lpMsg->dir;

	lpObj->ActionNumber = lpMsg->action;

	if (lpObj->ActionNumber == ACTION_SIT1)
	{
		lpObj->ViewState = 2;
		lpObj->Rest = lpObj->ActionNumber;
	}
	else if (lpObj->ActionNumber == ACTION_POSE1)
	{
		lpObj->ViewState = 3;
		lpObj->Rest = lpObj->ActionNumber;
	}
	else if (lpObj->ActionNumber == ACTION_HEALING1)
	{
		lpObj->ViewState = 4;
		lpObj->Rest = lpObj->ActionNumber;
	}

	PMSG_ACTION_SEND pMsg;

	pMsg.header.set(0x18, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.dir = lpMsg->dir;

	pMsg.action = lpMsg->action;

	pMsg.target[0] = lpMsg->index[0];

	pMsg.target[1] = lpMsg->index[1];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if (lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index, (BYTE*)&pMsg, pMsg.header.size);
			}
		}
	}
}

void CGEventRemainTimeRecv(PMSG_EVENT_REMAIN_TIME_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_EVENT_REMAIN_TIME_SEND pMsg;

	pMsg.header.set(0x91, sizeof(pMsg));

	pMsg.EventType = lpMsg->EventType;

	pMsg.RemainTimeH = 0;

	pMsg.EnteredUser = 0;

	pMsg.RemainTimeL = 0;

	if (lpMsg->EventType == 4)
	{
		lpMsg->ItemLevel = gChaosCastle.GetUserAbleLevel(&gObj[aIndex]);
	}
	else
	{
		lpMsg->ItemLevel = ((lpMsg->EventType == 2) ? lpMsg->ItemLevel : (lpMsg->ItemLevel - 1));
	}

	switch (lpMsg->EventType)
	{
		case 1:
			if (DS_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
			{
				if (gDevilSquare.GetState(lpMsg->ItemLevel) == DS_STATE_EMPTY)
				{
					if (gDevilSquare.GetEnterEnabled(lpMsg->ItemLevel) == 0)
					{
						pMsg.RemainTimeH = gDevilSquare.GetRemainTime(lpMsg->ItemLevel);
					}
					else
					{
						pMsg.EnteredUser = gDevilSquare.GetEnteredUserCount(lpMsg->ItemLevel);
					}
				}
				else
				{
					pMsg.RemainTimeH = gDevilSquare.GetRemainTime(lpMsg->ItemLevel);
				}
			}
			break;
		case 2:
			if (BC_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
			{
				if (gBloodCastle.GetState(lpMsg->ItemLevel) == BC_STATE_EMPTY)
				{
					if (gBloodCastle.GetEnterEnabled(lpMsg->ItemLevel) == 0)
					{
						pMsg.RemainTimeH = gBloodCastle.GetRemainTime(lpMsg->ItemLevel);
					}
					else
					{
						pMsg.EnteredUser = gBloodCastle.GetEnteredUserCount(lpMsg->ItemLevel);
					}
				}
				else
				{
					pMsg.RemainTimeH = gBloodCastle.GetRemainTime(lpMsg->ItemLevel);
				}
			}
			break;
		case 4:
			if (CC_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
			{
				if (gChaosCastle.GetState(lpMsg->ItemLevel) == CC_STATE_EMPTY)
				{
					if (gChaosCastle.GetEnterEnabled(lpMsg->ItemLevel) == 0)
					{
						pMsg.RemainTimeL = gChaosCastle.GetRemainTime(lpMsg->ItemLevel);
					}
					else
					{
						pMsg.EnteredUser = gChaosCastle.GetEnteredUserCount(lpMsg->ItemLevel);
					}
				}
				else
				{
					pMsg.RemainTimeL = gChaosCastle.GetRemainTime(lpMsg->ItemLevel);
				}
			}
			break;
	}

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGPetItemCommandRecv(PMSG_PET_ITEM_COMMAND_RECV* lpMsg, int aIndex) // OK
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpMsg->type != 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	gDarkSpirit[aIndex].ChangeCommand(lpMsg->command, bIndex);
}

void CGPetItemInfoRecv(PMSG_PET_ITEM_INFO_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpMsg->type != 0 && lpMsg->type != 1)
	{
		return;
	}

	if (lpMsg->flag == 0) // Inventory
	{
		if (INVENTORY_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex, lpMsg->type, lpMsg->flag, lpMsg->slot, lpObj->Inventory[lpMsg->slot].m_PetItemLevel, lpObj->Inventory[lpMsg->slot].m_PetItemExp, (BYTE)lpObj->Inventory[lpMsg->slot].m_Durability);
	}
	else if (lpMsg->flag == 1) // Warehouse
	{
		if (WAREHOUSE_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		if (lpObj->LoadWarehouse == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex, lpMsg->type, lpMsg->flag, lpMsg->slot, lpObj->Warehouse[lpMsg->slot].m_PetItemLevel, lpObj->Warehouse[lpMsg->slot].m_PetItemExp, (BYTE)lpObj->Warehouse[lpMsg->slot].m_Durability);
	}
	else if (lpMsg->flag == 2) // Trade
	{
		if (TRADE_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex, lpMsg->type, lpMsg->flag, lpMsg->slot, lpObj->Trade[lpMsg->slot].m_PetItemLevel, lpObj->Trade[lpMsg->slot].m_PetItemExp, (BYTE)lpObj->Trade[lpMsg->slot].m_Durability);
	}
	else if (lpMsg->flag == 3) // Target Trade
	{
		if (TRADE_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0)
		{
			return;
		}

		if (OBJECT_RANGE(lpObj->TargetNumber) == 0)
		{
			return;
		}

		LPOBJ lpTarget = &gObj[lpObj->TargetNumber];

		if (lpTarget->Interface.use == 0 || lpTarget->Interface.type != INTERFACE_TRADE || lpTarget->Interface.state == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex, lpMsg->type, lpMsg->flag, lpMsg->slot, lpTarget->Trade[lpMsg->slot].m_PetItemLevel, lpTarget->Trade[lpMsg->slot].m_PetItemExp, (BYTE)lpTarget->Trade[lpMsg->slot].m_Durability);
	}
	else if (lpMsg->flag == 4) // Chaos Box
	{
		if (CHAOS_BOX_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex, lpMsg->type, lpMsg->flag, lpMsg->slot, lpObj->ChaosBox[lpMsg->slot].m_PetItemLevel, lpObj->ChaosBox[lpMsg->slot].m_PetItemExp, (BYTE)lpObj->ChaosBox[lpMsg->slot].m_Durability);
	}
	else if (lpMsg->flag == 5) // Target Personal Shop
	{
		if (INVENTORY_SHOP_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		if (OBJECT_RANGE(lpObj->PShopDealerIndex) == 0)
		{
			return;
		}

		LPOBJ lpTarget = &gObj[lpObj->PShopDealerIndex];

		GCPetItemInfoSend(aIndex, lpMsg->type, lpMsg->flag, lpMsg->slot, lpTarget->Inventory[lpMsg->slot].m_PetItemLevel, lpTarget->Inventory[lpMsg->slot].m_PetItemExp, (BYTE)lpTarget->Inventory[lpMsg->slot].m_Durability);
	}
}

void CGMapServerMoveAuthRecv(PMSG_MAP_SERVER_MOVE_AUTH_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_CONNECTED)
	{
		CloseClient(aIndex);
		return;
	}

	if (memcmp(gServerInfo.m_ServerVersion, lpMsg->ClientVersion, sizeof(lpMsg->ClientVersion)) != 0)
	{
		GCMapServerMoveAuthSend(aIndex, 6);
		return;
	}

	if (memcmp(gServerInfo.m_ServerSerial, lpMsg->ClientSerial, sizeof(lpMsg->ClientSerial)) != 0)
	{
		GCMapServerMoveAuthSend(aIndex, 6);
		return;
	}

	if (lpObj->LoginMessageSend == 0)
	{
		lpObj->LoginMessageSend++;
		lpObj->LoginMessageCount++;
		lpObj->ConnectTickCount = GetTickCount();
		lpObj->ClientTickCount = lpMsg->TickCount;
		lpObj->ServerTickCount = GetTickCount();

		char account[11] = {0};

		PacketArgumentDecrypt(account, lpMsg->account, sizeof(account));

		lpObj->MapServerMoveRequest = 1;

		GJMapServerMoveAuthSend(aIndex, account, lpMsg->name, lpMsg->AuthCode1, lpMsg->AuthCode2, lpMsg->AuthCode3, lpMsg->AuthCode4);
	}
}

void CGMoveRecv(PMSG_MOVE_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->RegenOk > 0)
	{
		return;
	}

	if (lpObj->Teleport != 0)
	{
		return;
	}

	if (gObjCheckMapTile(lpObj, 1) != 0)
	{
		return;
	}

	if ((GetTickCount() - lpObj->LastMoveTime) < 100)
	{
		return;
	}

	if (gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	if (lpObj->SkillSummonPartyTime != 0)
	{
		lpObj->SkillSummonPartyTime = 0;
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(272));
	}

	lpObj->Dir = lpMsg->path[0] >> 4;
	lpObj->Rest = 0;
	lpObj->PathCur = 0;
	lpObj->PathCount = lpMsg->path[0] & 0x0F;
	lpObj->LastMoveTime = GetTickCount();

	memset(lpObj->PathX, 0, sizeof(lpObj->PathX));

	memset(lpObj->PathY, 0, sizeof(lpObj->PathY));

	memset(lpObj->PathOri, 0, sizeof(lpObj->PathOri));

	lpObj->TX = lpMsg->x;
	lpObj->TY = lpMsg->y;
	lpObj->PathCur = ((lpObj->PathCount > 0) ? 1 : 0);
	lpObj->PathCount = ((lpObj->PathCount > 0) ? (lpObj->PathCount + 1) : lpObj->PathCount);
	lpObj->PathStartEnd = 1;
	lpObj->PathX[0] = lpMsg->x;
	lpObj->PathY[0] = lpMsg->y;
	lpObj->PathDir[0] = lpObj->Dir;

	for (int n = 1; n < lpObj->PathCount; n++)
	{
		if ((n % 2) == 0)
		{
			lpObj->TX = lpObj->PathX[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] & 0x0F) * 2) + 0];
			lpObj->TY = lpObj->PathY[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] & 0x0F) * 2) + 1];
			lpObj->PathX[n] = lpObj->PathX[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] & 0x0F) * 2) + 0];
			lpObj->PathY[n] = lpObj->PathY[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] & 0x0F) * 2) + 1];
			lpObj->PathOri[n - 1] = lpMsg->path[((n + 1) / 2)] & 0x0F;
			lpObj->PathDir[n + 0] = lpMsg->path[((n + 1) / 2)] & 0x0F;
		}
		else
		{
			lpObj->TX = lpObj->PathX[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] / 0x10) * 2) + 0];
			lpObj->TY = lpObj->PathY[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] / 0x10) * 2) + 1];
			lpObj->PathX[n] = lpObj->PathX[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] / 0x10) * 2) + 0];
			lpObj->PathY[n] = lpObj->PathY[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] / 0x10) * 2) + 1];
			lpObj->PathOri[n - 1] = lpMsg->path[((n + 1) / 2)] / 0x10;
			lpObj->PathDir[n + 0] = lpMsg->path[((n + 1) / 2)] / 0x10;
		}
	}

	if (lpObj->TX < (lpObj->X - 15) || lpObj->TX >(lpObj->X + 15) || lpObj->TY < (lpObj->Y - 15) || lpObj->TY >(lpObj->Y + 15))
	{
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
		memset(lpObj->PathX, 0, sizeof(lpObj->PathX));
		memset(lpObj->PathY, 0, sizeof(lpObj->PathY));
		memset(lpObj->PathOri, 0, sizeof(lpObj->PathOri));
		gObjSetPosition(lpObj->Index, lpObj->X, lpObj->Y);
		return;
	}

	if (lpObj->PathCount > 0 && (gMap[lpObj->Map].CheckAttr(lpObj->TX, lpObj->TY, 4) != 0 || gMap[lpObj->Map].CheckAttr(lpObj->TX, lpObj->TY, 8) != 0))
	{
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
		memset(lpObj->PathX, 0, sizeof(lpObj->PathX));
		memset(lpObj->PathY, 0, sizeof(lpObj->PathY));
		memset(lpObj->PathOri, 0, sizeof(lpObj->PathOri));
		gObjSetPosition(lpObj->Index, lpObj->X, lpObj->Y);
		return;
	}

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);

	lpObj->X = lpMsg->x;
	lpObj->Y = lpMsg->y;
	lpObj->TX = lpObj->TX;
	lpObj->TY = lpObj->TY;
	lpObj->OldX = lpObj->TX;
	lpObj->OldY = lpObj->TY;
	lpObj->ViewState = 0;

	gMap[lpObj->Map].SetStandAttr(lpObj->TX, lpObj->TY);

	PMSG_MOVE_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE1, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.x = (BYTE)lpObj->TX;

	pMsg.y = (BYTE)lpObj->TY;

	pMsg.dir = lpObj->Dir << 4;

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if (lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index, (BYTE*)&pMsg, pMsg.header.size);
			}
		}
	}
}

void CGConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_CONNECTED)
	{
		CloseClient(aIndex);
		return;
	}

	if (memcmp(gServerInfo.m_ServerVersion, lpMsg->ClientVersion, sizeof(lpMsg->ClientVersion)) != 0)
	{
		GCConnectAccountSend(aIndex, 6);
		return;
	}

	if (memcmp(gServerInfo.m_ServerSerial, lpMsg->ClientSerial, sizeof(lpMsg->ClientSerial)) != 0)
	{
		GCConnectAccountSend(aIndex, 6);
		return;
	}

	if (lpObj->LoginMessageSend == 0)
	{
		lpObj->LoginMessageSend++;
		lpObj->LoginMessageCount++;
		lpObj->ConnectTickCount = GetTickCount();
		lpObj->ClientTickCount = lpMsg->TickCount;
		lpObj->ServerTickCount = GetTickCount();

		char account[11] = {0};

		PacketArgumentDecrypt(account, lpMsg->account, (sizeof(account) - 1));

		char password[11] = {0};

		PacketArgumentDecrypt(password, lpMsg->password, (sizeof(password) - 1));

		lpObj->MapServerMoveRequest = 0;
		lpObj->LastServerCode = -1;
		lpObj->DestMap = -1;
		lpObj->DestX = 0;
		lpObj->DestY = 0;

		GJConnectAccountSend(aIndex, account, password, lpObj->IpAddr);
	}
}

void CGCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED && lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	gObjectManager.CharacterGameCloseSet(aIndex, lpMsg->type);
}

void CGCharacterListRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	GDCharacterListSend(aIndex);
}

void CGCharacterCreateRecv(PMSG_CHARACTER_CREATE_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	PMSG_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0xF3, 0x01, sizeof(pMsg));

	pMsg.result = 0;

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	if (gServerInfo.m_CharacterCreateSwitch == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (lpMsg->Class != DB_CLASS_DW && lpMsg->Class != DB_CLASS_DK && lpMsg->Class != DB_CLASS_FE && lpMsg->Class != DB_CLASS_MG && lpMsg->Class != DB_CLASS_DL)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}
	
	if (lpMsg->Class == DB_CLASS_MG && (lpObj->ClassCode & 4) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	if (lpMsg->Class == DB_CLASS_DL && (lpObj->ClassCode & 2) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	char name[11] = {0};

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	GDCharacterCreateSend(aIndex, name, lpMsg->Class);
}

void CGCharacterDeleteRecv(PMSG_CHARACTER_DELETE_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	if (lpObj->EnableDelCharacter == 0)
	{
		return;
	}

	PMSG_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0xF3, 0x02, sizeof(pMsg));

	pMsg.result = 0;

	if (gObjCheckPersonalCode(aIndex, lpMsg->PersonalCode) == 0)
	{
		pMsg.result = 2;
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
		return;
	}

	char name[11] = {0};

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	GDCharacterDeleteSend(aIndex, name);
}

void CGCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	lpObj->EnableDelCharacter = 0;

	char name[11] = {0};

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	GDCharacterInfoSend(aIndex, name);
}

void CGLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_LEVEL_UP_POINT_SEND pMsg;

	pMsg.header.set(0xF3, 0x06, sizeof(pMsg));

	pMsg.result = 0;

	if (gObjectManager.CharacterLevelUpPointAdd(lpObj, lpMsg->type, 1) != 0)
	{
		pMsg.result = 16 + lpMsg->type;
		pMsg.MaxLife = (DWORD)(lpObj->MaxLife + lpObj->AddLife);
		pMsg.MaxMana = (DWORD)(lpObj->MaxMana + lpObj->AddMana);
		pMsg.MaxBP = (DWORD)(lpObj->MaxBP + lpObj->AddBP);
	}

	pMsg.LevelUpPoints = lpObj->LevelUpPoint;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGCharacterMoveViewportEnableRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->RegenOk = ((lpObj->RegenOk == 1) ? 2 : lpObj->RegenOk);
}

void CGCharacterNameCheckRecv(PMSG_CHARACTER_NAME_CHECK_RECV* lpMsg, int aIndex) // OK
{
#if(GAMESERVER_UPDATE>=401)

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	char name[11] = {0};

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	GDCharacterNameCheckSend(aIndex, name);

#endif
}

void CGCharacterNameChangeRecv(PMSG_CHARACTER_NAME_CHANGE_RECV* lpMsg, int aIndex) // OK
{
#if(GAMESERVER_UPDATE>=401)

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	char OldName[11] = {0};

	char NewName[11] = {0};

	memcpy(OldName, lpMsg->OldName, sizeof(lpMsg->OldName));

	memcpy(NewName, lpMsg->NewName, sizeof(lpMsg->NewName));

	GDCharacterNameChangeSend(aIndex, OldName, NewName);

#endif
}

void CGOptionChangeSkinRecv(PMSG_OPTION_CHANGE_SKIN_RECV* lpMsg, int aIndex) // OK
{
#if(GAMESERVER_UPDATE>=701)

	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	lpObj->ChangeSkin = lpMsg->ChangeSkin;

	gObjViewportListProtocolCreate(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

#endif
}

void CGOptionDataRecv(PMSG_OPTION_DATA_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	memcpy(lpObj->SkillOptionData.SkillKey, lpMsg->SkillKey, sizeof(lpObj->SkillOptionData.SkillKey));

	lpObj->SkillOptionData.GameOption = lpMsg->GameOption;
	lpObj->SkillOptionData.QKey = lpMsg->QKey;
	lpObj->SkillOptionData.WKey = lpMsg->WKey;
	lpObj->SkillOptionData.EKey = lpMsg->EKey;
	lpObj->SkillOptionData.ChatWindow = lpMsg->ChatWindow;
}

void GCChatTargetSend(LPOBJ lpObj, int aIndex, char* message) // OK
{
	int size = strlen(message);

	size = ((size > MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

	PMSG_CHAT_TARGET_SEND pMsg;

	pMsg.header.set(0x01, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	memcpy(pMsg.message, message, size);

	pMsg.message[size] = 0;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void GCChatWhisperSend(int aIndex, char* name, char* message) // OK
{
	int size = strlen(message);

	size = ((size > MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

	PMSG_CHAT_WHISPER_SEND pMsg;

	pMsg.header.set(0x02, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	memcpy(pMsg.message, message, size);

	pMsg.message[size] = 0;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMainCheckSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	lpObj->CheckSumTableNum = GetLargeRand() % MAX_CHECKSUM_KEY;

	lpObj->CheckSumTime = GetTickCount();

	PMSG_MAIN_CHECK_SEND pMsg;

	pMsg.header.set(0x03, sizeof(pMsg));

	pMsg.key = (((GetLargeRand() % 64) * 16) | ((lpObj->CheckSumTableNum & 0x3F0) * 64) | ((lpObj->CheckSumTableNum & 0x0F)) ^ 0xB479);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCEventStateSend(int aIndex, BYTE state, BYTE event) // OK
{
	PMSG_EVENT_STATE_SEND pMsg;

	pMsg.header.set(0x0B, sizeof(pMsg));

	pMsg.state = state;

	pMsg.event = event;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCEventStateSendToAll(BYTE state, BYTE event) // OK
{
	PMSG_EVENT_STATE_SEND pMsg;

	pMsg.header.set(0x0B, sizeof(pMsg));

	pMsg.state = state;

	pMsg.event = event;

	DataSendAll((BYTE*)&pMsg, pMsg.header.size);
}

void GCServerMsgSend(int aIndex, BYTE msg) // OK
{
	PMSG_SERVER_MSG_SEND pMsg;

	pMsg.header.set(0x0C, sizeof(pMsg));

	pMsg.MsgNumber = msg;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCWeatherSend(int aIndex, BYTE weather) // OK
{
	PMSG_WEATHER_SEND pMsg;

	pMsg.header.set(0x0F, sizeof(pMsg));

	pMsg.weather = weather;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCDamageSend(int aIndex, int bIndex, BYTE flag, int damage, int type) // OK
{
	PMSG_DAMAGE_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE2, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(bIndex) | (flag * 0x80);
	pMsg.index[1] = SET_NUMBERLB(bIndex);
	pMsg.damage = damage;
	pMsg.type = type;
	pMsg.life = (DWORD)gObj[bIndex].Life;

	if (gObj[aIndex].Type == OBJECT_USER)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}

	if (gObj[bIndex].Type == OBJECT_USER)
	{
		DataSend(bIndex, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void GCUserDieSend(LPOBJ lpObj, int aIndex, int skill, int bIndex) // OK
{
	PMSG_USER_DIE_SEND pMsg;

	pMsg.header.set(0x17, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);
	pMsg.skill = skill;
	pMsg.killer[0] = SET_NUMBERHB(bIndex);
	pMsg.killer[1] = SET_NUMBERLB(bIndex);

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void GCActionSend(LPOBJ lpObj, BYTE action, int aIndex, int bIndex) // OK
{
	PMSG_ACTION_SEND pMsg;

	pMsg.header.set(0x18, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.dir = lpObj->Dir;

	pMsg.action = action;

	pMsg.target[0] = SET_NUMBERHB(bIndex);

	pMsg.target[1] = SET_NUMBERLB(bIndex);

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMoneySend(int aIndex, DWORD money) // OK
{
	PMSG_ITEM_GET_SEND pMsg;

	pMsg.header.setE(0x22, sizeof(pMsg));

	pMsg.result = 0xFE;

	memset(pMsg.ItemInfo, 0, sizeof(pMsg.ItemInfo));

	pMsg.ItemInfo[0] = SET_NUMBERHB(SET_NUMBERHW(money));
	pMsg.ItemInfo[1] = SET_NUMBERLB(SET_NUMBERHW(money));
	pMsg.ItemInfo[2] = SET_NUMBERHB(SET_NUMBERLW(money));
	pMsg.ItemInfo[3] = SET_NUMBERLB(SET_NUMBERLW(money));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCLifeSend(int aIndex, BYTE type, int life) // OK
{
	PMSG_LIFE_SEND pMsg;

	pMsg.header.set(0x26, sizeof(pMsg));

	pMsg.type = type;
	pMsg.life = life;
	pMsg.flag = 0;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCManaSend(int aIndex, BYTE type, int mana, int bp) // OK
{
	PMSG_MANA_SEND pMsg;

	pMsg.header.set(0x27, sizeof(pMsg));

	pMsg.type = type;
	pMsg.mana = mana;
	pMsg.bp = bp;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCItemUseSpecialTimeSend(int aIndex, BYTE number, int time) // OK
{
	PMSG_ITEM_SPECIAL_TIME_SEND pMsg;

	pMsg.header.setE(0x29, sizeof(pMsg));

	pMsg.number = number;
	pMsg.time = time;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMapAttrSend(int aIndex, BYTE type, BYTE attr, BYTE flag, BYTE count, PMSG_MAP_ATTR* lpInfo) // OK
{
	BYTE send[256];

	PMSG_MAP_ATTR_SEND pMsg;

	pMsg.header.set(0x46, 0);

	int size = sizeof(pMsg);

	pMsg.type = type;

	pMsg.attr = attr;

	pMsg.flag = flag;

	pMsg.count = count;

	PMSG_MAP_ATTR info;

	for (int n = 0; n < count; n++)
	{
		info.x = lpInfo[n].x;
		info.y = lpInfo[n].y;
		info.tx = lpInfo[n].tx;
		info.ty = lpInfo[n].ty;

		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);
	}

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void GCPartyItemInfoSend(int aIndex, CItem* lpItem) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(lpObj->PartyNumber) == 0)
	{
		return;
	}

	PMSG_PARTY_ITEM_INFO_SEND pMsg;

	pMsg.header.set(0x47, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.ItemInfo = lpItem->m_Index & 0x1FFF;

	if (lpItem->m_Option1 != 0)
	{
		pMsg.ItemInfo |= 0x2000;
	}

	if (lpItem->m_Option2 != 0)
	{
		pMsg.ItemInfo |= 0x4000;
	}

	if (lpItem->m_Option3 != 0)
	{
		pMsg.ItemInfo |= 0x8000;
	}

	if (lpItem->IsExcItem() != 0)
	{
		pMsg.ItemInfo |= 0x10000;
	}

	if (lpItem->IsSetItem() != 0)
	{
		pMsg.ItemInfo |= 0x20000;
	}

	pMsg.level = (BYTE)lpItem->m_Level;

	for (int n = 0; n < MAX_PARTY_USER; n++)
	{
		int index = gParty.m_PartyInfo[lpObj->PartyNumber].Index[n];

		if (OBJECT_RANGE(index) != 0)
		{
			DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void GCEffectInfoSend(int aIndex, BYTE effect) // OK
{
	PMSG_EFFECT_INFO_SEND pMsg;

	pMsg.header.set(0x48, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.effect = effect;

	if (effect == 17)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(&gObj[aIndex], (BYTE*)&pMsg, pMsg.header.size);
}

void GCGuildMasterQuestionSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Interface.use != 0)
	{
		return;
	}

	PBMSG_HEAD pMsg;

	pMsg.set(0x54, sizeof(pMsg));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.size);
}

void GCMonsterSkillSend(LPOBJ lpObj, LPOBJ lpTarget, int skill) // OK
{
	PMSG_MONSTER_SKILL_SEND pMsg;

	pMsg.header.set(0x69, sizeof(pMsg));

	pMsg.skill[0] = SET_NUMBERHB(skill);
	pMsg.skill[1] = SET_NUMBERLB(skill);

	pMsg.index = lpObj->Index;

	pMsg.target = lpTarget->Index;

	if (lpObj->Index == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMonsterDieSend(int aIndex, int bIndex, QWORD experience, int damage, BYTE flag) // OK
{
	char exp[100];
	sprintf_s(exp, "Real XP: %llu", experience);
	gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, exp);
	
	PMSG_REWARD_EXPERIENCE_SEND pMsg;

	pMsg.header.setE(0x9C, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(bIndex) | (flag * 0x80);
	pMsg.index[1] = SET_NUMBERLB(bIndex);
	pMsg.experience = experience;
	pMsg.damage = damage;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCRewardExperienceSend(int aIndex, QWORD experience) // OK
{
	PMSG_REWARD_EXPERIENCE_SEND pMsg;

	pMsg.header.setE(0x9C, sizeof(pMsg));

	pMsg.index[0] = 0xFF;
	pMsg.index[1] = 0xFF;
	pMsg.experience = experience;
	pMsg.damage = 0;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCEventEnterCountSend(int aIndex, BYTE EventType, BYTE EnterCount) // OK
{
	PMSG_EVENT_ENTER_COUNT_SEND pMsg;

	pMsg.header.setE(0x9F, sizeof(pMsg));

	pMsg.EventType = EventType;

	pMsg.EnterCount = EnterCount;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCPetItemInfoSend(int aIndex, BYTE type, BYTE flag, BYTE slot, BYTE level, DWORD experience, BYTE durability) // OK
{
	PMSG_PET_ITEM_INFO_SEND pMsg;

	pMsg.header.set(0xA9, sizeof(pMsg));

	pMsg.type = type;
	pMsg.flag = flag;
	pMsg.slot = slot;
	pMsg.level = level;
	pMsg.experience = experience;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMapServerMoveAuthSend(int aIndex, BYTE result) // OK
{
	PMSG_MAP_SERVER_MOVE_AUTH_SEND pMsg;

	pMsg.header.set(0xB1, 0x01, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCTaxInfoSend(int aIndex, BYTE type, BYTE rate) // OK
{
	PMSG_TAX_INFO_SEND pMsg;

	pMsg.header.set(0xB2, 0x1A, sizeof(pMsg));

	pMsg.type = type;

	pMsg.rate = rate;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCCreationStateSend(int aIndex, BYTE state) // OK
{
	PMSG_CREATION_STATE_SEND pMsg;

	pMsg.header.set(0xB9, 0x01, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.state = state;

	MsgSendV2(&gObj[aIndex], (BYTE*)&pMsg, pMsg.header.size);
}

void GCMonsterAreaSkillSend(int aIndex, int MonsterClass, int sx, int sy, int tx, int ty) // OK
{
	PMSG_MONSTER_AREA_SKILL_SEND pMsg;

	pMsg.header.set(0xBD, 0x0C, sizeof(pMsg));

	pMsg.MonsterClass[0] = SET_NUMBERHB(MonsterClass);
	pMsg.MonsterClass[1] = SET_NUMBERLB(MonsterClass);
	pMsg.sx = sx;
	pMsg.sy = sy;
	pMsg.tx = tx;
	pMsg.ty = ty;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCCharacterCreationEnableSend(int aIndex, BYTE flag, BYTE result) // OK
{
	PMSG_CHARACTER_CREATION_ENABLE_SEND pMsg;

	pMsg.header.set(0xDE, sizeof(pMsg));

	pMsg.flag = flag;
	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCConnectClientSend(int aIndex, BYTE result) // OK
{
	PMSG_CONNECT_CLIENT_SEND pMsg;

	pMsg.header.set(0xF1, 0x00, sizeof(pMsg));

	pMsg.result = result;
	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);
	pMsg.ClientVersion[0] = gServerInfo.m_ServerVersion[0];
	pMsg.ClientVersion[1] = gServerInfo.m_ServerVersion[1];
	pMsg.ClientVersion[2] = gServerInfo.m_ServerVersion[2];
	pMsg.ClientVersion[3] = gServerInfo.m_ServerVersion[3];
	pMsg.ClientVersion[4] = gServerInfo.m_ServerVersion[4];

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	gObj[aIndex].ConnectTickCount = GetTickCount();
}

void GCConnectAccountSend(int aIndex, BYTE result) // OK
{
	PMSG_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0xF1, 0x01, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCConnectAccountSend(int aIndex, BYTE result, SOCKET socket) // OK
{
	PMSG_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0xF1, 0x01, sizeof(pMsg));

	pMsg.result = result;

	DataSendSocket(socket, (BYTE*)&pMsg, pMsg.header.size);
}

void GCCloseClientSend(int aIndex, BYTE result) // OK
{
	PMSG_CLOSE_CLIENT_SEND pMsg;

	pMsg.header.setE(0xF1, 0x02, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCCharacterRegenSend(LPOBJ lpObj) // OK
{
	PMSG_CHARACTER_REGEN_SEND pMsg;

	pMsg.header.setE(0xF3, 0x04, sizeof(pMsg));

	pMsg.X = (BYTE)lpObj->X;
	pMsg.Y = (BYTE)lpObj->Y;
	pMsg.Map = lpObj->Map;
	pMsg.Dir = lpObj->Dir;
	pMsg.Life = (DWORD)(lpObj->Life);
	pMsg.Mana = (DWORD)(lpObj->Mana);
	pMsg.BP = (DWORD)(lpObj->BP);
	pMsg.Experience = lpObj->Experience;
	pMsg.Money = lpObj->Money;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void GCLevelUpSend(LPOBJ lpObj) // OK
{
	PMSG_LEVEL_UP_SEND pMsg;

	pMsg.header.set(0xF3, 0x05, sizeof(pMsg));

	pMsg.Level = lpObj->Level;
	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pMsg.MaxLife = (DWORD)(lpObj->MaxLife + lpObj->AddLife);
	pMsg.MaxMana = (DWORD)(lpObj->MaxMana + lpObj->AddMana);
	pMsg.MaxBP = (DWORD)(lpObj->MaxBP + lpObj->AddBP);
	pMsg.FruitAddPoint = lpObj->FruitAddPoint;
	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);
	pMsg.FruitSubPoint = lpObj->FruitSubPoint;
	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);
	pMsg.NextExperience = lpObj->NextExperience;
	pMsg.PreviousNextExperience = gLevelExperience[lpObj->Level - 1];

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

	GCEffectInfoSend(lpObj->Index, 16);
}

void GCMonsterDamageSend(int aIndex, int damage) // OK
{
	PMSG_MONSTER_DAMAGE_SEND pMsg;

	pMsg.header.set(0xF3, 0x07, sizeof(pMsg));

	pMsg.damage = damage;
	pMsg.life = (DWORD)gObj[aIndex].Life;

	if (gObj[aIndex].Type == OBJECT_USER)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void GCPKLevelSend(int aIndex, int PKLevel) // OK
{
	PMSG_PK_LEVEL_SEND pMsg;

	pMsg.header.set(0xF3, 0x08, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.PKLevel = PKLevel;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	MsgSendV2(&gObj[aIndex], (BYTE*)&pMsg, pMsg.header.size);
}

void GCSummonLifeSend(int aIndex, int life, int MaxLife) // OK
{
	if (MaxLife <= 0)
	{
		return;
	}

	PMSG_SUMMON_LIFE_SEND pMsg;

	pMsg.header.set(0xF3, 0x20, sizeof(pMsg));

	pMsg.life = (life * 100) / MaxLife;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCTimeViewSend(int aIndex, int time) // OK
{
	PMSG_TIME_VIEW_SEND pMsg;

	pMsg.header.set(0xF3, 0x22, sizeof(pMsg));

	pMsg.time = time;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMathAuthenticatorSend(int aIndex) // OK
{
#if(GAMESERVER_UPDATE>=701)

	PMSG_MATH_AUTHENTICATOR_SEND pMsg;

	pMsg.header.set(0xF3, 0x32, sizeof(pMsg));

	pMsg.function = GetLargeRand() % 6;

	pMsg.value = (float)(1 + (GetLargeRand() % 9));

	memset(pMsg.result, 0, sizeof(pMsg.result));

	MAPM mapm = pMsg.value;

	switch (pMsg.function)
	{
		case 0:
			mapm = mapm.sin();
			break;
		case 1:
			mapm = mapm.cos();
			break;
		case 2:
			mapm = mapm.tan();
			break;
		case 3:
			mapm = mapm.sqrt();
			break;
		case 4:
			mapm = mapm.log();
			break;
		case 5:
			mapm = mapm.exp();
			break;
	}

	mapm.toString(pMsg.result, 5);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

#endif
}

void GCFireworksSend(LPOBJ lpObj, int x, int y) // OK
{
	PMSG_FIREWORKS_SEND pMsg;

	pMsg.header.set(0xF3, 0x40, sizeof(pMsg));

	pMsg.type = 0;

	pMsg.x = x;

	pMsg.y = y;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void GCServerCommandSend(int aIndex, BYTE type, BYTE cmd1, BYTE cmd2) // OK
{
	PMSG_SERVER_COMMAND_SEND pMsg;

	pMsg.header.set(0xF3, 0x40, sizeof(pMsg));

	pMsg.type = type;

	pMsg.cmd1 = cmd1;

	pMsg.cmd2 = cmd2;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}


//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

void CGReqCastleSiegeState(PMSG_REQ_CASTLESIEGESTATE* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	GS_GDReqOwnerGuildMaster(gMapServerManager.GetMapServerGroup(), iIndex);
#endif
}

void GCAnsCastleSiegeState(int iIndex, int iResult, LPSTR lpszGuildName, LPSTR lpszGuildMaster)
{
#if (GAMESERVER_TYPE==1)
	if ((lpszGuildName == NULL) || (lpszGuildMaster == NULL))
	{
		return;
	}

	PMSG_ANS_CASTLESIEGESTATE pMsgResult;


	pMsgResult.h.set(0xB2, 0x00, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;

	memcpy(pMsgResult.cOwnerGuild, lpszGuildName, sizeof(pMsgResult.cOwnerGuild));
	memcpy(pMsgResult.cOwnerGuildMaster, lpszGuildMaster, sizeof(pMsgResult.cOwnerGuildMaster));

	SYSTEMTIME tmStateStartDate = {0};
	SYSTEMTIME tmStateEndDate = {0};
	SYSTEMTIME tmSiegeStartDate = {0};

	int iCastleState = gCastleSiege.GetCastleState();
	int bRETVAL = gCastleSiege.GetCastleStateTerm(&tmStateStartDate, &tmStateEndDate);
	tmSiegeStartDate = gCastleSiege.GetCastleLeftSiegeDate();

	if (bRETVAL == FALSE)
	{
		iCastleState = -1;
	}

	int iStateLeftSec = gCastleSiege.GetCurRemainSec();


	pMsgResult.cCastleSiegeState = iCastleState;

	pMsgResult.btStartYearH = SET_NUMBERHB(tmStateStartDate.wYear);
	pMsgResult.btStartYearL = SET_NUMBERLB(tmStateStartDate.wYear);
	pMsgResult.btStartMonth = (BYTE)tmStateStartDate.wMonth;
	pMsgResult.btStartDay = (BYTE)tmStateStartDate.wDay;
	pMsgResult.btStartHour = (BYTE)tmStateStartDate.wHour;
	pMsgResult.btStartMinute = (BYTE)tmStateStartDate.wMinute;

	pMsgResult.btEndYearH = SET_NUMBERHB(tmStateEndDate.wYear);
	pMsgResult.btEndYearL = SET_NUMBERLB(tmStateEndDate.wYear);
	pMsgResult.btEndMonth = (BYTE)tmStateEndDate.wMonth;
	pMsgResult.btEndDay = (BYTE)tmStateEndDate.wDay;
	pMsgResult.btEndHour = (BYTE)tmStateEndDate.wHour;
	pMsgResult.btEndMinute = (BYTE)tmStateEndDate.wMinute;

	pMsgResult.btSiegeStartYearH = SET_NUMBERHB(tmSiegeStartDate.wYear);
	pMsgResult.btSiegeStartYearL = SET_NUMBERLB(tmSiegeStartDate.wYear);
	pMsgResult.btSiegeStartMonth = (BYTE)tmSiegeStartDate.wMonth;
	pMsgResult.btSiegeStartDay = (BYTE)tmSiegeStartDate.wDay;
	pMsgResult.btSiegeStartHour = (BYTE)tmSiegeStartDate.wHour;
	pMsgResult.btSiegeStartMinute = (BYTE)tmSiegeStartDate.wMinute;

	pMsgResult.btStateLeftSec1 = SET_NUMBERHB(SET_NUMBERHW(iStateLeftSec));
	pMsgResult.btStateLeftSec2 = SET_NUMBERLB(SET_NUMBERHW(iStateLeftSec));
	pMsgResult.btStateLeftSec3 = SET_NUMBERHB(SET_NUMBERLW(iStateLeftSec));
	pMsgResult.btStateLeftSec4 = SET_NUMBERLB(SET_NUMBERLW(iStateLeftSec));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void CGReqRegCastleSiege(PMSG_REQ_REGCASTLESIEGE* lpMsg, int iIndex)
{
#if (GAMESERVER_TYPE==1)
	if (gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGSIEGE)
	{
		GCAnsRegCastleSiege(iIndex, 7, "");
		return;
	}
	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		GCAnsRegCastleSiege(iIndex, 6, "");
		return;
	}
	if (gCastleSiege.CheckUnionGuildMaster(iIndex) == FALSE)
	{
		GCAnsRegCastleSiege(iIndex, 0, "");
	}
	else if ((gCastleSiege.CheckCastleOwnerMember(iIndex) != FALSE) ||
		(gCastleSiege.CheckCastleOwnerUnionMember(iIndex) != FALSE))
	{
		GCAnsRegCastleSiege(iIndex, 3, "");
	}
	else
	{
		GS_GDReqRegAttackGuild(gMapServerManager.GetMapServerGroup(), iIndex);
	}
#endif
}

void GCAnsRegCastleSiege(int iIndex, int iResult, LPSTR lpszGuildName)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_REGCASTLESIEGE pMsgResult;

	if (lpszGuildName == NULL)
	{
		return;
	}

	if (OBJECT_RANGE(iIndex) == FALSE)
	{
		return;
	}

	pMsgResult.h.set(0xB2, 0x01, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if (::gObjIsConnected(iIndex))
	{
		LogAdd(LOG_BLACK, "[CastleSiege] [%s][%s] Registered Castle Siege (GUILD:%s) - Result:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, iResult);
	}
#endif
}

void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	if ((gCastleSiege.GetCastleState() < CASTLESIEGE_STATE_REGSIEGE) || (gCastleSiege.GetCastleState() > CASTLESIEGE_STATE_REGMARK)) //Fixed
	{
		GCAnsGiveUpCastleSiege(iIndex, 3, 0, 0, "");
		return;
	}

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	if (gCastleSiege.CheckUnionGuildMaster(iIndex) == FALSE)
	{
		return;
	}

	GS_GDReqGuildSetGiveUp(gMapServerManager.GetMapServerGroup(), iIndex, lpMsg->btGiveUp);
#endif
}

void GCAnsGiveUpCastleSiege(int iIndex, int iResult, int bGiveUp, int iMarkCount, LPSTR lpszGuildName)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_GIVEUPCASTLESIEGE pMsgResult;

	if (lpszGuildName == NULL)
	{
		return;
	}

	if (OBJECT_RANGE(iIndex) == FALSE)
	{
		return;
	}

	pMsgResult.h.set(0xB2, 0x02, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIsGiveUp = bGiveUp;

	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	if ((pMsgResult.btResult == 1) && (iMarkCount > 0))
	{
		gObj[iIndex].Money += (iMarkCount * 3000);

		if (gObj[iIndex].Money > MAX_MONEY)
		{
			gObj[iIndex].Money = MAX_MONEY;
		}
		GCMoneySend(iIndex, gObj[iIndex].Money);
	}
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if (gObjIsConnected(iIndex))
	{
		LogAdd(LOG_BLACK, "[CastleSiege] [%s][%s] GiveUp Castle Siege (GUILD:%s) - Result:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, iResult);
	}
#endif
}

void CGReqGuildRegInfo(PMSG_REQ_GUILDREGINFO* lpMsg, int iIndex)
{
#if (GAMESERVER_TYPE==1)
	if (lpMsg == NULL)
	{
		return;
	}

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	GS_GDReqGuildMarkRegInfo(gMapServerManager.GetMapServerGroup(), iIndex);
#endif
}

void GCAnsGuildRegInfo(int iIndex, int iResult, CSP_ANS_GUILDREGINFO* lpMsgResult)
{
#if (GAMESERVER_TYPE==1)
	PMSG_ANS_GUILDREGINFO pMsgResult;

	if (lpMsgResult == NULL)
	{
		return;
	}

	if (OBJECT_RANGE(iIndex) == FALSE)
	{
		return;
	}

	pMsgResult.h.set(0xB2, 3, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;
	pMsgResult.btGuildMark1 = SET_NUMBERHB(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark2 = SET_NUMBERLB(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark3 = SET_NUMBERHB(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark4 = SET_NUMBERLB(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
	pMsgResult.btRegRank = lpMsgResult->btRegRank;
	pMsgResult.btIsGiveUp = lpMsgResult->bIsGiveUp;
	memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK* lpMsg, int iIndex)
{
#if (GAMESERVER_TYPE==1)
	if (lpMsg == NULL)
	{
		return;
	}

	if (OBJECT_RANGE(iIndex) == FALSE)
	{
		return;
	}

	if (gObj[iIndex].UseEventServer == TRUE)
	{
		return;
	}

	gObj[iIndex].UseEventServer = TRUE;

	if (gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGMARK)
	{
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}
	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}

	if (lpMsg->btItemPos < 0 || lpMsg->btItemPos >= 64)
	{
		PMSG_ANS_REGGUILDMARK pMsgResult = {0};
		pMsgResult.h.set(0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = FALSE;
	}
	else
	{
		int iItemPos = (lpMsg->btItemPos + 12);

		if (gObj[iIndex].Inventory[iItemPos].IsItem() == FALSE)
		{
			PMSG_ANS_REGGUILDMARK pMsgResult = {0};
			pMsgResult.h.set(0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = 3;
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = FALSE;
		}
		else
		{
			if (gObj[iIndex].Inventory[iItemPos].m_Index != GET_ITEM(14, 21) ||
				gObj[iIndex].Inventory[iItemPos].m_Level != 3 ||
				gObj[iIndex].Inventory[iItemPos].m_Durability <= 0.0f)
			{
				PMSG_ANS_REGGUILDMARK pMsgResult = {0};
				pMsgResult.h.set(0xB2, 0x04, sizeof(pMsgResult));
				pMsgResult.btResult = 3;
				DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
				gObj[iIndex].UseEventServer = FALSE;
			}
			else
			{
				GS_GDReqRegGuildMark(gMapServerManager.GetMapServerGroup(), iIndex, iItemPos);
			}
		}
	}
#endif
}

void GCAnsRegGuildMark(int iIndex, int iResult, CSP_ANS_GUILDREGMARK* lpMsgResult)
{
#if (GAMESERVER_TYPE==1)
	if (lpMsgResult == NULL)
	{
		return;
	}

	if (OBJECT_RANGE(iIndex) == FALSE)
	{
		return;
	}

	if (gObj[iIndex].Inventory[lpMsgResult->iItemPos].IsItem() == FALSE)
	{
		PMSG_ANS_REGGUILDMARK pMsgResult = {0};
		pMsgResult.h.set(0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = 0;
	}
	else
	{
		if (gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Index != GET_ITEM(14, 21) &&
			gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Level == 3 ||
			gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Durability <= 0.0f)
		{
			PMSG_ANS_REGGUILDMARK pMsgResult = {0};
			pMsgResult.h.set(0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = 3;
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = FALSE;
		}
		else
		{
			if ((gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Durability -= 1.0) > 0.0f)
			{
				gItemManager.GCItemDurSend(iIndex, lpMsgResult->iItemPos, (BYTE)gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Durability, 0);
			}
			else
			{
				gItemManager.InventoryDelItem(iIndex, lpMsgResult->iItemPos);
				gItemManager.GCItemDeleteSend(iIndex, lpMsgResult->iItemPos, 1);
			}

			PMSG_ANS_REGGUILDMARK pMsgResult;
			pMsgResult.h.set(0xB2, 0x04, sizeof(pMsgResult));

			pMsgResult.btResult = iResult;
			pMsgResult.btGuildMark1 = SET_NUMBERHB(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark2 = SET_NUMBERLB(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark3 = SET_NUMBERHB(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark4 = SET_NUMBERLB(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = 0;

		}
	}
#endif
}

void CGReqNpcBuy(PMSG_REQ_NPCBUY* lpMsg, int iIndex)
{
#if (GAMESERVER_TYPE==1)
	if (lpMsg == NULL)
	{
		return;
	}

	if (::gObjIsConnected(iIndex) == FALSE)
	{
		return;
	}

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	if (gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || (gObj[iIndex].GuildStatus != G_MASTER && gObj[iIndex].GuildStatus != G_SUB_MASTER))
	{
		LogAdd(LOG_RED, "[CastleSiege] CGReqNpcBuy() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
	}
	else
	{
		BYTE btResult = 0;

		BOOL bRETVAL = gCastleSiege.CheckAddDbNPC(iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, btResult);

		if (bRETVAL == FALSE)
		{
			GCAnsNpcBuy(iIndex, btResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
			LogAdd(LOG_RED, "[CastleSiege] CGReqNpcBuy() ERROR - CCastleSiege::CheckAddDbNPC() [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
		}
		else
		{
			LogAdd(LOG_RED, "[CastleSiege] CGReqNpcBuy() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
		}
	}
#endif
}

void GCAnsNpcBuy(int iIndex, int iResult, int iNpcNumber, int iNpcIndex)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_NPCBUY pMsgResult;

	pMsgResult.h.set(0xB2, 0x05, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void CGReqNpcRepair(PMSG_REQ_NPCREPAIR* lpMsg, int iIndex)
{
#if (GAMESERVER_TYPE==1)
	if (lpMsg == NULL)
	{
		return;
	}

	if (::gObjIsConnected(iIndex) == FALSE)
	{
		return;
	}

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	if ((gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER))
	{
		LogAdd(LOG_RED, "[CastleSiege] CGReqNpcRepair() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		return;
	}

	_CS_NPC_DATA pNpcData;
	BOOL bRETVAL;
	int iNpcIndex;
	int iRepairCost;

	bRETVAL = gCastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, pNpcData);

	if (bRETVAL == FALSE)
	{
		GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		LogAdd(LOG_RED, "[CastleSiege] CGReqNpcRepair() ERROR - CL Request Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
		return;
	}
	else
	{
		iNpcIndex = pNpcData.m_iNPC_OBJINDEX;

		if (gObjIsConnected(iNpcIndex) == FALSE)
		{
			GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
			LogAdd(LOG_RED, "[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
			return;
		}
		else if (gObj[iNpcIndex].Live == 0)
		{
			GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
			LogAdd(LOG_RED, "[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
			return;
		}
		else
		{
			if ((gObj[iNpcIndex].MaxLife == gObj[iNpcIndex].Life) || (gObj[iNpcIndex].MaxLife < gObj[iNpcIndex].Life))
			{
				gObj[iNpcIndex].Life = gObj[iNpcIndex].MaxLife;
				GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
				return;
			}
			else
			{
				iRepairCost = 0;

				switch (lpMsg->iNpcNumber)
				{
					case 277:
						iRepairCost = (int)(float((gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life) * 0.5f) + float(gObj[iNpcIndex].CsNpcDfLevel * 1000000));
						break;
					case 283:
						iRepairCost = (int)((gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life) * 0.3f + float((gObj[iNpcIndex].CsNpcDfLevel + gObj[iNpcIndex].CsNpcRgLevel) * 1000000));
						break;
					default:
						GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
						return;
				}

				if (gObj[iIndex].Money < ((DWORD)iRepairCost))
				{
					GCAnsNpcRepair(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
					LogAdd(LOG_RED, "[CastleSiege] CGReqNpcRepair() ERROR - Money isn't enough [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
					return;
				}
				else
				{
					LogAdd(LOG_RED, "[CastleSiege] CGReqNpcRepair() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
					GS_GDReqCastleNpcRepair(gMapServerManager.GetMapServerGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, iRepairCost);
					return;
				}
			}
		}
	}
#endif
}

void GCAnsNpcRepair(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcHP, int iNpcMaxHP)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_NPCREPAIR pMsgResult;

	pMsgResult.h.set(0xB2, 0x06, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcHP = iNpcHP;
	pMsgResult.iNpcMaxHP = iNpcMaxHP;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE* lpMsg, int iIndex)
{
#if (GAMESERVER_TYPE==1)

	if (lpMsg == NULL)
	{
		return;
	}

	if (::gObjIsConnected(iIndex) == FALSE)
	{
		return;
	}

	int iNEED_GEMOFDEFEND = 0;
	int	iNEED_MONEY = 0;
	int	iCUR_UPLVL = 0;
	int	iNXT_UPLVL = 0;
	int	iNXT_UPVALUE = 0;
	int	bENABLE_UPGRADE = FALSE;

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
		return;

	if ((gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER))
	{
		LogAdd(LOG_BLACK, "[CastleSiege] CGReqNpcUpgrade() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		return;
	}

	switch (lpMsg->iNpcNumber)
	{
		case 277:
		{
			_CS_NPC_DATA m_NpcData;
			BOOL bResult = gCastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

			if (bResult)
			{
				switch (lpMsg->iNpcUpType)
				{
					case 1:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;

						if ((iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL + 1)))
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = iNXT_UPLVL;
							iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL - 1)][1];
							iNEED_MONEY = g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL - 1)][2];
						}
					}
					break;

					case 3:
					{
						iCUR_UPLVL = 0;

						for (int i = 0; i < 3; i++)
						{
							if (m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CGATE[i])
							{
								iCUR_UPLVL = (i + 1);
							}
						}
						iNXT_UPLVL = 0;

						for (int i = 0; i < 3; i++)
						{
							if (lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CGATE[i])
							{
								iNXT_UPLVL = (i + 1);
							}
						}

						if ((iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL + 1)))
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL - 1)][0];
							iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL - 1)][1];
							iNEED_MONEY = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL - 1)][2];
						}
					}
					break;
					default:
					{
						GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
						return;
					}
					break;
				}
			}
			else
			{
				GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				return;
			}
		}
		break;
		case 283:
		{
			_CS_NPC_DATA m_NpcData;

			BOOL bResult;

			bResult = gCastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

			if (bResult)
			{
				switch (lpMsg->iNpcUpType)
				{
					case 1:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;

						if ((iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL + 1)))
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = iNXT_UPLVL;
							iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL - 1)][1];
							iNEED_MONEY = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL - 1)][2];
						}
					}
					break;
					case 2:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_RG_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;

						if ((iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL + 1)))
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = iNXT_UPLVL;
							iNEED_GEMOFDEFEND = g_iNpcUpRgLevel_CSTATUE[(iNXT_UPLVL - 1)][1];
							iNEED_MONEY = g_iNpcUpRgLevel_CSTATUE[(iNXT_UPLVL - 1)][2];
						}
					}
					break;
					case 3:
					{
						iCUR_UPLVL = 0;

						for (int i = 0; i < 3; i++)
						{
							if (m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CSTATUE[i])
							{
								iCUR_UPLVL = (i + 1);
							}
						}

						iNXT_UPLVL = 0;

						for (int i = 0; i < 3; i++)
						{
							if (lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CSTATUE[i])
							{
								iNXT_UPLVL = (i + 1);
							}
						}

						if ((iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL + 1)))
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL - 1)][0];
							iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL - 1)][1];
							iNEED_MONEY = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL - 1)][2];
						}
					}
					break;
					default:
					{
						GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
						LogAdd(LOG_BLACK, "[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildNumber, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
						return;
					}
					break;
				}
			}
			else
			{
				GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				return;
			}
		}
		break;
	}

	int iGEMOFDEFEND_COUNT;
	int x;
	int bRETVAL;

	if (bENABLE_UPGRADE == FALSE)
	{
		GCAnsNpcUpgrade(iIndex, 6, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
	}
	else
	{
		if (gObj[iIndex].Money < ((DWORD)iNEED_MONEY))
		{
			GCAnsNpcUpgrade(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
			LogAdd(LOG_BLACK, "[CastleSiege] CGReqNpcUpgrade() ERROR - Money is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Money:%d, Need Money:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildNumber, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL, gObj[iIndex].Money, iNEED_MONEY);
		}
		else
		{
			for (iGEMOFDEFEND_COUNT = 0, x = 0; x < INVENTORY_MAIN_SIZE; x++)
			{
				if (gObj[iIndex].Inventory[x].IsItem() == TRUE)
				{
					if (gObj[iIndex].Inventory[x].m_Index == GET_ITEM(14, 31))
					{
						iGEMOFDEFEND_COUNT += 1;
					}
				}
			}

			if (iGEMOFDEFEND_COUNT < iNEED_GEMOFDEFEND)
			{
				GCAnsNpcUpgrade(iIndex, 4, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				LogAdd(LOG_BLACK, "[CastleSiege] CGReqNpcUpgrade() ERROR - Gem is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Gem:%d, Need Gem:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildNumber, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL, iGEMOFDEFEND_COUNT, iNEED_GEMOFDEFEND);
			}
			else
			{
				bRETVAL = gCastleSiege.PayForUpgradeDbNpc(iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, (iNXT_UPLVL - 1));

				if (bRETVAL == FALSE)
				{
				}
				else
				{
					gObj[iIndex].IsCastleNPCUpgradeCompleted = TRUE;
					LogAdd(LOG_BLACK, "[CastleSiege] [0xB2][0x07] CGReqNpcUpgrade() - Pay For Npc Upgrade (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)", lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, iNXT_UPVALUE);
					GS_GDReqCastleNpcUpgrade(gMapServerManager.GetMapServerGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, iNXT_UPVALUE, (iNXT_UPLVL - 1));
					LogAdd(LOG_BLACK, "[CastleSiege] CGReqNpcUpgrade() REQ OK - [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildNumber, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL);
				}
			}
		}

	}
#endif
}

void GCAnsNpcUpgrade(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_NPCUPGRADE pMsgResult;

	pMsgResult.h.set(0xB2, 0x07, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcUpType = iNpcUpType;
	pMsgResult.iNpcUpValue = iNpcUpValue;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void CGReqTaxMoneyInfo(PMSG_REQ_TAXMONEYINFO* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	if (lpMsg == NULL)
	{
		return;
	}

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	if ((gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER))
	{
		LogAdd(LOG_RED, "[CastleSiege] CGReqTaxMoneyInfo() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		GS_GDReqTaxInfo(gMapServerManager.GetMapServerGroup(), iIndex);
		LogAdd(LOG_RED, "[CastleSiege] CGReqTaxMoneyInfo() REQ OK - [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
#endif
}

void GCAnsTaxMoneyInfo(int iIndex, int iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, __int64 i64Money)
{
#if (GAMESERVER_TYPE==1)
	PMSG_ANS_TAXMONEYINFO pMsgResult;

	pMsgResult.h.set(0xB2, 0x08, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxRateChaos = btTaxRateChaos;
	pMsgResult.btTaxRateStore = btTaxRateStore;
	pMsgResult.btMoney1 = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(i64Money)));
	pMsgResult.btMoney2 = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(i64Money)));
	pMsgResult.btMoney3 = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(i64Money)));
	pMsgResult.btMoney4 = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(i64Money)));
	pMsgResult.btMoney5 = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(i64Money)));
	pMsgResult.btMoney6 = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(i64Money)));
	pMsgResult.btMoney7 = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(i64Money)));
	pMsgResult.btMoney8 = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(i64Money)));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)

	int iMaxTaxRate;
	int iTaxRate;

	if (lpMsg == NULL)
	{
		return;
	}

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	if ((gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER))
	{
		LogAdd(LOG_RED, "[CastleSiege] CGReqTaxRateChange() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		iMaxTaxRate = 0;

		switch (lpMsg->btTaxType)
		{
			case 1:
				iMaxTaxRate = 3;
				break;
			case 2:
				iMaxTaxRate = 3;
				break;
			case 3:
				iMaxTaxRate = 300000;
				break;
		}

		iTaxRate = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btTaxRate1, lpMsg->btTaxRate2), MAKE_NUMBERW(lpMsg->btTaxRate3, lpMsg->btTaxRate4));

		if ((iTaxRate < 0) || (iTaxRate > iMaxTaxRate))
		{
			GCAnsTaxRateChange(iIndex, 0, 0, 0);
			LogAdd(LOG_BLACK, "[CastleSiege] CGReqTaxRateChange() ERROR - Tax Rate Out of Range [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
		else
		{
			GS_GDReqTaxRateChange(gMapServerManager.GetMapServerGroup(), iIndex, lpMsg->btTaxType, iTaxRate);
			LogAdd(LOG_BLACK, "[CastleSiege] CGReqTaxRateChange() REQ OK - [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
	}
#endif
}

void GCAnsTaxRateChange(int iIndex, int iResult, BYTE btTaxType, int iTaxRate)
{
#if (GAMESERVER_TYPE==1)
	PMSG_ANS_TAXRATECHANGE pMsgResult;
	pMsgResult.h.set(0xB2, 0x09, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate1 = SET_NUMBERHB(SET_NUMBERHW(iTaxRate));
	pMsgResult.btTaxRate2 = SET_NUMBERLB(SET_NUMBERHW(iTaxRate));
	pMsgResult.btTaxRate3 = SET_NUMBERHB(SET_NUMBERLW(iTaxRate));
	pMsgResult.btTaxRate4 = SET_NUMBERLB(SET_NUMBERLW(iTaxRate));
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	int iMoneyChange;
	BOOL bRETVAL;

	if (lpMsg == NULL)
	{
		return;
	}

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	if ((gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER))
	{
		LogAdd(LOG_RED, "[CastleSiege] CGReqMoneyDrawOut() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		iMoneyChange = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btMoney4, lpMsg->btMoney3), MAKE_NUMBERW(lpMsg->btMoney2, lpMsg->btMoney1));

		if (iMoneyChange <= 0)
		{
			GCAnsMoneyDrawOut(iIndex, 0, 0);
			LogAdd(LOG_BLACK, "[CastleSiege] CGReqMoneyDrawOut() ERROR - Req Money < 0 [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
		}
		else
		{
			bRETVAL = gCastleSiege.CheckCastleHasMoney(iMoneyChange);

			if (bRETVAL == FALSE)
			{
				GCAnsMoneyDrawOut(iIndex, 0, 0);
				LogAdd(LOG_BLACK, "[CastleSiege] CGReqMoneyDrawOut() ERROR - Castle Money is Low [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
			}
			else
			{
				if ((gObj[iIndex].Money + iMoneyChange) > MAX_MONEY)
				{
					GCAnsMoneyDrawOut(iIndex, 0, 0);
				}
				else
				{
					GS_GDReqCastleMoneyChange(gMapServerManager.GetMapServerGroup(), iIndex, -iMoneyChange);
					LogAdd(LOG_BLACK, "[CastleSiege] CGReqMoneyDrawOut() REQ OK - [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
				}
			}
		}
	}
#endif
}

void GCAnsMoneyDrawOut(int iIndex, int iResult, __int64 i64Money)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_MONEYDRAWOUT pMsgResult;
	pMsgResult.h.set(0xB2, 0x10, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btMoney1 = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(i64Money)));
	pMsgResult.btMoney2 = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(i64Money)));
	pMsgResult.btMoney3 = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(i64Money)));
	pMsgResult.btMoney4 = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(i64Money)));
	pMsgResult.btMoney5 = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(i64Money)));
	pMsgResult.btMoney6 = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(i64Money)));
	pMsgResult.btMoney7 = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(i64Money)));
	pMsgResult.btMoney8 = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(i64Money)));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void GCAnsCsGateState(int iIndex, int iResult, int iGateIndex)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_CSGATESTATE pMsgResult;
	pMsgResult.h.set(0xB2, 0x11, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIndex1 = SET_NUMBERHB((iGateIndex & 0xffff));
	pMsgResult.btIndex2 = SET_NUMBERLB((iGateIndex & 0xffff));
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	int iGateIndex;

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	if (gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		if (gObj[iIndex].CsJoinSide != 1)
		{
			LogAdd(LOG_BLACK, "[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
			return;
		}
	}
	else
	{
		if ((gCastleSiege.CheckCastleOwnerMember(iIndex) == FALSE) && (gCastleSiege.CheckCastleOwnerUnionMember(iIndex) == FALSE))
		{
			LogAdd(LOG_BLACK, "[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
			return;
		}
	}

	iGateIndex = MAKE_NUMBERW(lpMsg->btIndex1, lpMsg->btIndex2);

	if (gCastleSiege.CheckCsGateAlive(iGateIndex) == TRUE)
	{
		int iOperateGate = gCastleSiege.OperateGate(iGateIndex, gObj[iGateIndex].CsNpcExistVal, lpMsg->btOperate);

		if (iOperateGate == 0)
		{
			GCAnsCsGateOperate(iIndex, 0, -1, 0);
			LogAdd(LOG_BLACK, "[CastleSiege] CGReqCsGateOperate() ERROR - Operate Gate Failed [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		}
		else
		{
			GCAnsCsGateOperate(iIndex, 1, iGateIndex, lpMsg->btOperate);
			gCastleSiege.SendCsGateStateViewPort(iGateIndex, lpMsg->btOperate);
			LogAdd(LOG_BLACK, "[CastleSiege] CGReqCsGateOperate() OK - [%s][%s], Guild:(%s)(%d), DOOR:(%d)(X:%d,Y:%d)(STATUE:%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildNumber, iGateIndex, gObj[iGateIndex].X, gObj[iGateIndex].Y, gObj[iGateIndex].CsGateOpen);
		}
	}
	else
	{
		GCAnsCsGateOperate(iIndex, 2, -1, 0);
		LogAdd(LOG_BLACK, "[CastleSiege] CGReqCsGateOperate() ERROR - Gate Doesn't Exist [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
#endif
}

void GCAnsCsGateOperate(int iIndex, int iResult, int iGateIndex, int iGateOperate)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_CSGATEOPERATE pMsgResult;

	pMsgResult.h.set(0xB2, 0x12, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERHB(iGateIndex & 0xffff);
	pMsgResult.btIndex2 = SET_NUMBERLB(iGateIndex & 0xffff);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void GCAnsCsGateCurState(int iIndex, int iGateIndex, int iGateOperate)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_CSGATECURSTATE pMsgResult;
	pMsgResult.h.set(0xB2, 0x13, sizeof(pMsgResult));
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERHB(iGateIndex & 0xffff);
	pMsgResult.btIndex2 = SET_NUMBERLB(iGateIndex & 0xffff);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void GCAnsCsAccessSwitchState(int iIndex, int iSwitchIndex, int iSwitchUserIndex, BYTE btSwitchState)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_NOTIFYSWITCHPROC pMsgResult;
	pMsgResult.h.set(0xB2, 0x14, sizeof(pMsgResult));
	pMsgResult.btIndex1 = SET_NUMBERHB(iSwitchIndex & 0xffff);
	pMsgResult.btIndex2 = SET_NUMBERLB(iSwitchIndex & 0xffff);
	pMsgResult.btUserIndex1 = SET_NUMBERHB(iSwitchUserIndex & 0xffff);
	pMsgResult.btUserIndex2 = SET_NUMBERLB(iSwitchUserIndex & 0xffff);
	pMsgResult.btSwitchState = btSwitchState;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void GCAnsCsAccessCrownState(int iIndex, BYTE btCrownState)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_NOTIFYCROWNPROC pMsgResult;
	pMsgResult.h.set(0xB2, 0x15, sizeof(pMsgResult));
	pMsgResult.btCrownState = btCrownState;

	if (btCrownState == 0)
	{
		if (gObj[iIndex].AccumulatedCrownAccessTime < 0 || gObj[iIndex].AccumulatedCrownAccessTime > 61000)
		{
			gObj[iIndex].AccumulatedCrownAccessTime = 0;
		}

		LogAdd(LOG_BLACK, "[CastleSiege] [Reg. Attempt] Accumulated Crown AccessTime : %d [%s](%s)(%s)", gObj[iIndex].AccumulatedCrownAccessTime, gObj[iIndex].GuildName, gObj[iIndex].Account, gObj[iIndex].Name);
	}

	if (btCrownState == 1)
	{
		LogAdd(LOG_BLACK, "[CastleSiege] [Reg. Success] Accumulated Crown AccessTime : %d [%s](%s)(%s)", gObj[iIndex].AccumulatedCrownAccessTime, gObj[iIndex].GuildName, gObj[iIndex].Account, gObj[iIndex].Name);
		gObj[iIndex].AccumulatedCrownAccessTime = 0;
	}

	if (btCrownState == 2)
	{
		gObj[iIndex].AccumulatedCrownAccessTime += GetTickCount() - gCastleSiege.GetCrownAccessTickCount();

		gObj[iIndex].AccumulatedCrownAccessTime -= gServerInfo.m_CastleSiegeLowerAccumulatedTimeValue;

		if (gObj[iIndex].AccumulatedCrownAccessTime < 0 || gObj[iIndex].AccumulatedCrownAccessTime > 61000)
		{
			gObj[iIndex].AccumulatedCrownAccessTime = 0;
		}

		LogAdd(LOG_BLACK, "[CastleSiege] [Reg. Fail] Accumulated Crown AccessTime : %d [%s](%s)(%s)", gObj[iIndex].AccumulatedCrownAccessTime, gObj[iIndex].GuildName, gObj[iIndex].Account, gObj[iIndex].Name);
	}

	pMsgResult.dwAccumulatedCrownAccessTime = gObj[iIndex].AccumulatedCrownAccessTime;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void GCAnsCsNotifyStart(int iIndex, BYTE btStartState)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_NOTIFYCSSTART pMsgResult;
	pMsgResult.h.set(0xB2, 0x17, sizeof(pMsgResult));
	pMsgResult.btStartState = btStartState;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

void GCAnsCsNotifyProgress(int iIndex, BYTE btCastleSiegeState, LPCSTR lpszGuildName)
{
	if (lpszGuildName == NULL)
	{
		return;
	}

	PMSG_ANS_NOTIFYCSPROGRESS pMsgResult;

	pMsgResult.h.set(0xB2, 0x18, sizeof(pMsgResult));
	pMsgResult.btCastleSiegeState = btCastleSiegeState;
	memset(pMsgResult.szGuildName, 0, sizeof(pMsgResult.szGuildName));
	memcpy(pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GCAnsCsMapSvrTaxInfo(int iIndex, BYTE btTaxType, BYTE btTaxRate)
{
	PMSG_ANS_MAPSVRTAXINFO pMsgResult;

	pMsgResult.h.set(0xB2, 0x1A, sizeof(pMsgResult));
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate = btTaxRate;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void CGReqCsMiniMapData(PMSG_REQ_MINIMAPDATA* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	if (gObjIsConnected(iIndex) == FALSE)
	{
		return;
	}

	if ((gObj[iIndex].CsJoinSide <= 0) || (gCastleSiege.CheckUnionGuildMaster(iIndex) == FALSE))
	{
		GCAnsCsMiniMapData(iIndex, 3);
	}
	else if (gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		GCAnsCsMiniMapData(iIndex, 2);
	}
	else
	{
		gCastleSiege.AddMiniMapDataReqUser(iIndex);
		GCAnsCsMiniMapData(iIndex, 1);
	}
#endif
}

void GCAnsCsMiniMapData(int iIndex, BYTE btResult)
{
	PMSG_ANS_MINIMAPDATA pMsgResult;

	pMsgResult.h.set(0xB2, 0x1B, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void CGReqStopCsMiniMapData(PMSG_REQ_STOPMINIMAPDATA* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	if (gObjIsConnected(iIndex) == FALSE)
	{
		return;
	}

	gCastleSiege.DelMiniMapDataReqUser(iIndex);
#endif
}

void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	if (gObjIsConnected(iIndex) == FALSE)
	{
		return;
	}

	if (gObj[iIndex].CsJoinSide <= 0 || gCastleSiege.CheckUnionGuildMaster(iIndex) == FALSE)
	{
		return;
	}

	if (gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		return;
	}

	GCAnsCsSendCommand(gObj[iIndex].CsJoinSide, lpMsg->btTeam, lpMsg->btX, lpMsg->btY, lpMsg->btCommand);
#endif
}

void GCAnsCsSendCommand(int iCsJoinSize, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_CSCOMMAND pMsgResult;

	pMsgResult.h.set(0xB2, 0x1D, sizeof(pMsgResult));

	pMsgResult.btTeam = btTeam;
	pMsgResult.btX = btX;
	pMsgResult.btY = btY;
	pMsgResult.btCommand = btCommand;

	for (int iIndex = OBJECT_START_USER; iIndex < MAX_OBJECT; iIndex++)
	{
		if (gObjIsConnected(iIndex) == FALSE) continue;

		if ((gObj[iIndex].CsJoinSide == iCsJoinSize) && (gObj[iIndex].Map == MAP_CASTLE_SIEGE))
		{
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		}
	}
#endif
}

void GCAnsCsLeftTimeAlarm(BYTE btHour, BYTE btMinute)
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set(0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	for (int iIndex = OBJECT_START_USER; iIndex < MAX_OBJECT; iIndex++)
	{
		if (!gObjIsConnected(iIndex))
		{
			continue;
		}

		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
	}

}

void GCAnsSelfCsLeftTimeAlarm(int iIndex, BYTE btHour, BYTE btMinute)
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set(0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	if (gObjIsConnected(iIndex) == FALSE)
	{
		return;
	}

	if (gCastleSiege.CheckCastleOwnerMember(iIndex) == FALSE)
	{
		GCAnsCsSetEnterHuntZone(iIndex, 2, lpMsg->btHuntZoneEnter);
	}
	else if (gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		GCAnsCsSetEnterHuntZone(iIndex, 0, lpMsg->btHuntZoneEnter);
	}
	else
	{
		gCastleSiege.SetHuntZoneEnter(lpMsg->btHuntZoneEnter);
		GCAnsCsSetEnterHuntZone(iIndex, 1, lpMsg->btHuntZoneEnter);
	}

#endif
}

void GCAnsCsSetEnterHuntZone(int iIndex, BYTE btResult, BYTE btEnterHuntZone)
{
	PMSG_ANS_CSHUNTZONEENTER pMsgResult;

	pMsgResult.h.set(0xB2, 0x1F, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;
	pMsgResult.btHuntZoneEnter = btEnterHuntZone;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void CGReqNpcDbList(PMSG_REQ_NPCDBLIST* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	PMSG_ANS_NPCDBLIST pResult;

	if (lpMsg == NULL)
	{
		return;
	}

	if (strcmp(gObj[iIndex].GuildName, "") == 0)
	{
		return;
	}

	if (gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || ((gObj[iIndex].GuildStatus != 128) && (gObj[iIndex].GuildStatus != 64)))
	{
		LogAdd(LOG_RED, "[CastleSiege] CGReqNpcDbList() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].Account, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		pResult.h.set(0xB3, sizeof(pResult));
		pResult.iCount = 0;
		pResult.btResult = 2;
		DataSend(iIndex, (LPBYTE)&pResult, sizeof(pResult));
	}
	else
	{
		switch (lpMsg->btMonsterCode)
		{
			case 1:
				gCastleSiege.SendNpcStateList(iIndex, 277);
				break;
			case 2:
				gCastleSiege.SendNpcStateList(iIndex, 283);
				break;
		}
	}
#endif
}

void CGReqCsRegGuildList(PMSG_REQ_CSREGGUILDLIST* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	if (lpMsg == NULL) return;

	GS_GDReqAllGuildMarkRegInfo(gMapServerManager.GetMapServerGroup(), iIndex);
#endif
}

void CGReqCsAttkGuildList(PMSG_REQ_CSATTKGUILDLIST* lpMsg, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	char cBUFFER[1625];
	PMSG_ANS_CSATTKGUILDLIST* lpMsgSend;
	PMSG_CSATTKGUILDLIST* lpMsgSendBody;
	int iCount;

	if (lpMsg == NULL)	return;

	lpMsgSend = (PMSG_ANS_CSATTKGUILDLIST*)(cBUFFER);
	lpMsgSendBody = (PMSG_CSATTKGUILDLIST*)(cBUFFER + sizeof(PMSG_ANS_CSATTKGUILDLIST));

	iCount = 0;

	lpMsgSend->btResult = gCastleSiege.GetCsAttkGuildList(lpMsgSendBody, iCount);
	lpMsgSend->iCount = iCount;

	if (lpMsgSend->iCount < 0)	 lpMsgSend->iCount = 0;
	if (lpMsgSend->iCount > 100)lpMsgSend->iCount = 100;

	lpMsgSend->h.set(0xB5, ((lpMsgSend->iCount * sizeof(PMSG_CSATTKGUILDLIST)) + sizeof(PMSG_ANS_CSATTKGUILDLIST)));
	DataSend(iIndex, (LPBYTE)lpMsgSend, ((lpMsgSend->iCount * sizeof(PMSG_CSATTKGUILDLIST)) + sizeof(PMSG_ANS_CSATTKGUILDLIST)));
#endif
}

void CGReqGuildMarkOfCastleOwner(PMSG_REQ_GUILDMARK_OF_CASTLEOWNER* aRecv, int iIndex)
{
	if (!OBJECT_RANGE(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	char szGuildName[9] = {0};

	memcpy(szGuildName, gCastleSiegeSync.GetCastleOwnerGuild(), 8);

	if (szGuildName)
	{
		GUILD_INFO_STRUCT* lpGuild = gGuildClass.SearchGuild(szGuildName);

		if (lpGuild != NULL)
		{
			PMSG_ANS_GUILDMARK_OF_CASTLEOWNER pMsg = {0};

			pMsg.h.set(0xB9, 0x02, sizeof(pMsg));
			memcpy(pMsg.GuildMarkOfCastleOwner, lpGuild->Mark, sizeof(pMsg.GuildMarkOfCastleOwner));

			DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
}

void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE* aRecv, int iIndex)
{
#if(GAMESERVER_TYPE==1)
	if (!OBJECT_RANGE(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE pMsg = {0};

	pMsg.h.set(0xB9, 0x05, sizeof(pMsg));

	pMsg.btResult = 0;

	int iEnterTaxMoney = 0;

	iEnterTaxMoney = gCastleSiegeSync.GetTaxHuntZone(lpObj->Index, TRUE);

	if (iEnterTaxMoney < 0)
	{
		iEnterTaxMoney = 0;
	}

	BOOL bPermission = FALSE;

	if (gCastleSiege.GetHuntZoneEnter())
	{
		bPermission = TRUE;
	}

	if (lpObj->Guild)
	{
		if (gCastleSiege.CheckCastleOwnerMember(lpObj->Index))
		{
			bPermission = TRUE;
		}

		if (gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index))
		{
			bPermission = TRUE;
		}
	}
	else
	{
		if (gCastleSiege.GetHuntZoneEnter())
		{
			bPermission = TRUE;
		}
	}

	if (bPermission == TRUE)
	{
		if (lpObj->Money >= ((DWORD)iEnterTaxMoney))
		{
			if (lpObj->Map == MAP_CASTLE_SIEGE)
			{
				if (gObjMoveGate(lpObj->Index, 95) == TRUE)
				{
					pMsg.btResult = 1;
					int iOldMoney = lpObj->Money;

					lpObj->Money -= iEnterTaxMoney;

					gCastleSiegeSync.AddTributeMoney(iEnterTaxMoney);

					if (lpObj->Money < 0)
					{
						lpObj->Money = 0;
					}

					GCMoneySend(lpObj->Index, lpObj->Money);

					LogAdd(LOG_BLACK, "[Castle HuntZone] [%s][%s] - Entrance TAX : %d - %d = %d", lpObj->Account, lpObj->Name, iOldMoney, iEnterTaxMoney, lpObj->Money);
				}
			}
		}
	}

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
#endif
}
