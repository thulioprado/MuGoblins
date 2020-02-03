// CommandManager.cpp: implementation of the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "BonusManager.h"
#include "DSProtocol.h"
#include "JSProtocol.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "Guild.h"
#include "ItemManager.h"
#include "Log.h"
#include "MapServerManager.h"
#include "Message.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Protocol.h"
#include "Quest.h"
#include "QuestReward.h"
#include "ServerInfo.h"
#include "Util.h"

CCommandManager gCommandManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandManager::CCommandManager() : m_CommandLabels() // OK
{

}

CCommandManager::~CCommandManager() // OK
{

}

void CCommandManager::Init() // OK
{
	this->Add(COMMAND_MOVE, gMessage.GetMessage(32));
	this->Add(COMMAND_POST, "/all");
	this->Add(COMMAND_ADD_POINT1, "/f");
	this->Add(COMMAND_ADD_POINT2, "/d");
	this->Add(COMMAND_ADD_POINT3, "/v");
	this->Add(COMMAND_ADD_POINT4, "/e");
	this->Add(COMMAND_ADD_POINT5, "/l");
	this->Add(COMMAND_READD_POINTS, "/readd");
	this->Add(COMMAND_WARE, "/bau");
	this->Add(COMMAND_GM_MOVE, gMessage.GetMessage(44));
	this->Add(COMMAND_GM_POST, gMessage.GetMessage(45));
	this->Add(COMMAND_TRACK, gMessage.GetMessage(46));
	this->Add(COMMAND_TRACE, gMessage.GetMessage(47));
	this->Add(COMMAND_DISCONNECT, gMessage.GetMessage(48));
	this->Add(COMMAND_FIREWORKS, gMessage.GetMessage(49));
	this->Add(COMMAND_MAKE, gMessage.GetMessage(50));
	this->Add(COMMAND_SKIN, gMessage.GetMessage(51));
	this->Add(COMMAND_SET_MONEY, gMessage.GetMessage(52));
	this->Add(COMMAND_NOTICE, gMessage.GetMessage(53));
	this->Add(COMMAND_GUILD_WAR, gMessage.GetMessage(55));
	this->Add(COMMAND_BATTLE_SOCCER, gMessage.GetMessage(56));
	this->Add(COMMAND_REQUEST, gMessage.GetMessage(57));
	this->Add(COMMAND_HIDE, gMessage.GetMessage(58));
}

void CCommandManager::Add(BYTE code, char* label) // OK
{
	this->m_CommandLabels[code].push_back(label);
}

long CCommandManager::GetNumber(char* arg, int pos) // OK
{
	int count = 0, p = 0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for (int n = 0; n < len; n++)
	{
		if (arg[n] == 0x20)
		{
			count++;
		}
		else if (count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	return atoi(buffer);
}

void CCommandManager::GetString(char* arg, char* out, int size, int pos) // OK
{
	int count = 0, p = 0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for (int n = 0; n < len; n++)
	{
		if (arg[n] == 0x20)
		{
			++count;
		}
		else if (count == pos)
		{
			buffer[p] = arg[n];
			++p;
		}
	}

	memcpy(out, buffer, (size - 1));

	out[size - 1] = 0;
}

long CCommandManager::GetCommandCode(char* label) // OK
{
	for (const auto& labels : this->m_CommandLabels)
	{
		for (const auto& command : labels.second)
		{
			if (_stricmp(command.c_str(), label) == 0)
			{
				return labels.first;
			}
		}
	}

	return -1;
}

bool CCommandManager::ManagementCore(LPOBJ lpObj, char* message) // OK
{
	char command[32] = {0};

	memset(command, 0, sizeof(command));

	this->GetString(message, command, sizeof(command), 0);

	int code = this->GetCommandCode(command);

	char* argument = &message[strlen(command)];

	if (argument[0] == 0x20)
	{
		argument++;
	}

	switch (code)
	{
		case COMMAND_MOVE:
			this->CommandMove(lpObj, argument);
			break;
		case COMMAND_POST:
			this->CommandPost(lpObj, argument);
			break;
		case COMMAND_ADD_POINT1:
			this->CommandAddPoint(lpObj, argument, 0);
			break;
		case COMMAND_ADD_POINT2:
			this->CommandAddPoint(lpObj, argument, 1);
			break;
		case COMMAND_ADD_POINT3:
			this->CommandAddPoint(lpObj, argument, 2);
			break;
		case COMMAND_ADD_POINT4:
			this->CommandAddPoint(lpObj, argument, 3);
			break;
		case COMMAND_ADD_POINT5:
			this->CommandAddPoint(lpObj, argument, 4);
			break;
		case COMMAND_READD_POINTS:
			this->CommandReAddPoint(lpObj);
			break;
		case COMMAND_WARE:
			this->CommandWare(lpObj, argument);
			break;
		case COMMAND_GM_MOVE:
			this->CommandGMMove(lpObj, argument);
			break;
		case COMMAND_GM_POST:
			this->CommandGMPost(lpObj, argument);
			break;
		case COMMAND_TRACK:
			this->CommandTrack(lpObj, argument);
			break;
		case COMMAND_TRACE:
			this->CommandTrace(lpObj, argument);
			break;
		case COMMAND_DISCONNECT:
			this->CommandDisconnect(lpObj, argument);
			break;
		case COMMAND_FIREWORKS:
			this->CommandFireworks(lpObj, argument);
			break;
		case COMMAND_MAKE:
			this->CommandMake(lpObj, argument);
			break;
		case COMMAND_SKIN:
			this->CommandSkin(lpObj, argument);
			break;
		case COMMAND_SET_MONEY:
			this->CommandSetMoney(lpObj, argument);
			break;
		case COMMAND_NOTICE:
			this->CommandNotice(lpObj, argument);
			break;
		case COMMAND_GUILD_WAR:
			this->CommandGuildWar(lpObj, argument);
			break;
		case COMMAND_BATTLE_SOCCER:
			this->CommandBattleSoccer(lpObj, argument);
			break;
		case COMMAND_REQUEST:
			this->CommandRequest(lpObj, argument);
			break;
		case COMMAND_HIDE:
			this->CommandHide(lpObj, argument);
			break;
		default:
			return 0;
	}

	return 1;
}

void CCommandManager::CommandMove(LPOBJ lpObj, char* arg) // OK
{
	MOVE_INFO MoveInfo;

	if (gMove.GetInfoByName(arg, &MoveInfo) != 0)
	{
		gMove.Move(lpObj, MoveInfo.Index);
		return;
	}
}

void CCommandManager::CommandPost(LPOBJ lpObj, char* arg) // OK
{
	char mode[10] = {0};

	this->GetString(arg, mode, sizeof(mode), 0);

	if (_stricmp(mode, "on") == 0)
	{
		lpObj->PostOn = true;
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(492));
		return;
	}
	
	if (_stricmp(mode, "off") == 0)
	{
		lpObj->PostOn = false;
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(493));
		return;
	}

	if (!lpObj->PostOn)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(494));
		return;
	}

	WORD Level[MAX_ACCOUNT_LEVEL] = {500, 1};
	BYTE Delay[MAX_ACCOUNT_LEVEL] = {50, 15};

	if (lpObj->Level < Level[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(65), Level[lpObj->AccountLevel]);
		return;
	}

	DWORD tick = (GetTickCount() - lpObj->PostTime) / 1000;

	if (tick < Delay[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(68), (Delay[lpObj->AccountLevel] - tick));
		return;
	}

	lpObj->PostTime = GetTickCount();

	char text[80];
	wsprintf(text, "%s: %s", lpObj->Name, arg);
	GJPostMessageSend(text);
	
	gLog.Output(LOG_COMMAND, "[CommandPost][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);
}

void CCommandManager::CommandAddPoint(LPOBJ lpObj, char* arg, int type) // OK
{
	if (lpObj->Class != CLASS_DL && type == 4)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(71));
		return;
	}
	
	int amount = this->GetNumber(arg, 0);

	if (amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(72));
		return;
	}

	if (gObjectManager.CharacterLevelUpPointAdd(lpObj, type, amount) == 0)
	{
		return;
	}

	GCCharacterUpdate(lpObj->Index);
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(74), amount, lpObj->LevelUpPoint);

	gLog.Output(LOG_COMMAND, "[CommandAddPoint][%s][%s] - (Type: %d, Amount: %d)", lpObj->Account, lpObj->Name, type, amount);
}

void CCommandManager::CommandReAddPoint(LPOBJ lpObj) // OK
{
	if (lpObj->AccountLevel != 1)
	{
		return;
	}

	lpObj->LevelUpPoint += lpObj->Strength;
	lpObj->LevelUpPoint += lpObj->Dexterity;
	lpObj->LevelUpPoint += lpObj->Vitality;
	lpObj->LevelUpPoint += lpObj->Energy;
	lpObj->LevelUpPoint += (lpObj->Class == CLASS_DL) ? lpObj->Leadership : 0;
	lpObj->LevelUpPoint -= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
	lpObj->LevelUpPoint -= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
	lpObj->LevelUpPoint -= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
	lpObj->LevelUpPoint -= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
	lpObj->LevelUpPoint -= (lpObj->Class == CLASS_DL) ? gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership : 0;

	lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
	lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
	lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
	lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
	lpObj->Leadership = (lpObj->Class == CLASS_DL) ? gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership : 0;

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCCharacterUpdate(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(495), lpObj->LevelUpPoint);

	gLog.Output(LOG_COMMAND, "[CommandReAddPoint][%s][%s]", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandWare(LPOBJ lpObj, char* arg) // OK
{
	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->LoadWarehouse != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(86));
		return;
	}

	int number = this->GetNumber(arg, 0);

	if (number < 0 || number >= lpObj->WarehouseTotal)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(87));
		return;
	}

	lpObj->WarehouseMoney = 0;

	memset(lpObj->WarehouseMap, 0xFF, WAREHOUSE_SIZE);

	for (int n = 0; n < WAREHOUSE_SIZE; ++n)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
	lpObj->LoadWarehouse = 0;
	lpObj->WarehouseNumber = number;

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(88), number);

	gLog.Output(LOG_COMMAND, "[CommandWare][%s][%s] - (Number: %d)", lpObj->Account, lpObj->Name, number);
}

void CCommandManager::CommandGMMove(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(96));
		return;
	}

	char name[11] = {0};

	this->GetString(arg, name, sizeof(name), 0);

	int map = this->GetNumber(arg, 1);
	int x = this->GetNumber(arg, 2);
	int y = this->GetNumber(arg, 3);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(97));
		return;
	}

	gObjTeleport(lpTarget->Index, map, x, y);

	gLog.Output(LOG_COMMAND, "[CommandGMMove][%s][%s] - (Name: %s, Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, name, map, x, y);
}

void CCommandManager::CommandGMPost(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(98));
		return;
	}

	gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[%s] %s", lpObj->Name, arg);

	gLog.Output(LOG_COMMAND, "[CommandGMPost][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);
}

void CCommandManager::CommandTrack(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(99));
		return;
	}

	char name[11] = {0};

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(100));
		return;
	}

	gObjTeleport(lpObj->Index, lpTarget->Map, lpTarget->X, lpTarget->Y);

	gLog.Output(LOG_COMMAND, "[CommandTrack][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);
}

void CCommandManager::CommandTrace(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(101));
		return;
	}

	char name[11] = {0};

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(102));
		return;
	}

	gObjTeleport(lpTarget->Index, lpObj->Map, lpObj->X, lpObj->Y);

	gLog.Output(LOG_COMMAND, "[CommandTrace][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);
}

void CCommandManager::CommandDisconnect(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(103));
		return;
	}

	char name[11] = {0};

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(104));
		return;
	}

	gObjUserKill(lpTarget->Index);

	gLog.Output(LOG_COMMAND, "[CommandDisconnect][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);
}

void CCommandManager::CommandFireworks(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(105));
		return;
	}

	for (int n = 0; n < 15; n++)
	{
		GCFireworksSend(lpObj, (lpObj->X + (((GetLargeRand() % 5) * 2) - 4)), (lpObj->Y + (((GetLargeRand() % 5) * 2) - 4)));
	}

	gLog.Output(LOG_COMMAND, "[CommandFireworks][%s][%s] - (Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, lpObj->Map, lpObj->X, lpObj->Y);
}

void CCommandManager::CommandMake(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(106));
		return;
	}

	int section = this->GetNumber(arg, 0);
	int type = this->GetNumber(arg, 1);
	int level = this->GetNumber(arg, 2);
	int skill = this->GetNumber(arg, 3);
	int luck = this->GetNumber(arg, 4);
	int option = this->GetNumber(arg, 5);
	int exc = this->GetNumber(arg, 6);
	int set = this->GetNumber(arg, 7);

	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc, set, 0, 0);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(107));

	gLog.Output(LOG_COMMAND, "[CommandMake][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d)", lpObj->Account, lpObj->Name, section, type, level, skill, luck, option, exc, set);
}

void CCommandManager::CommandSkin(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(108));
		return;
	}

	char name[11] = {0};

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(109));
		return;
	}

	lpTarget->Change = this->GetNumber(arg, 1);

	gObjViewportListProtocolCreate(lpTarget);

	gObjectManager.CharacterUpdateMapEffect(lpTarget);

	gLog.Output(LOG_COMMAND, "[CommandSkin][%s][%s] - (Name: %s, Change: %d)", lpObj->Account, lpObj->Name, name, lpTarget->Change);
}

void CCommandManager::CommandSetMoney(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(110));
		return;
	}

	char name[11] = {0};

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(111));
		return;
	}

	int money = this->GetNumber(arg, 1);

	if (money < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(112));
		return;
	}

	if (money == 0)
	{
		lpTarget->Money = 0;
	}
	else if (gObjCheckMaxMoney(lpTarget->Index, money) == 0)
	{
		lpTarget->Money = MAX_MONEY;
	}
	else
	{
		lpTarget->Money += money;
	}

	GCMoneySend(lpTarget->Index, lpTarget->Money);

	gLog.Output(LOG_COMMAND, "[CommandSetMoney][%s][%s] - (Name: %s, Money: %d)", lpObj->Account, lpObj->Name, name, money);
}

void CCommandManager::CommandNotice(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(113));
		return;
	}

	//GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(), 0, 0, 0, 0, 0, 0, arg);

	gLog.Output(LOG_COMMAND, "[CommandNotice][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);
}

void CCommandManager::CommandGuildWar(LPOBJ lpObj, char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg, GuildName, sizeof(GuildName), 0);

	if (strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName, lpObj->Index, 0);
	}
}

void CCommandManager::CommandBattleSoccer(LPOBJ lpObj, char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg, GuildName, sizeof(GuildName), 0);

	if (strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName, lpObj->Index, 1);
	}
}

void CCommandManager::CommandRequest(LPOBJ lpObj, char* arg) // OK
{
	char mode[5] = {0};

	this->GetString(arg, mode, sizeof(mode), 0);

	if (strcmp(mode, "on") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(126));
	}
	else if (strcmp(mode, "off") == 0)
	{
		lpObj->Option &= ~1;
		lpObj->Option &= ~2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(127));
	}
	else if (strcmp(mode, "auto") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option |= 4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		this->GetString(arg, lpObj->AutoPartyPassword, sizeof(lpObj->AutoPartyPassword), 1);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(128));
	}
}

void CCommandManager::CommandHide(LPOBJ lpObj, char* arg) // OK
{
	if (gGameMaster.CheckGameMasterLevel(lpObj, 0) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(135));
		return;
	}

	if (gEffectManager.CheckEffect(lpObj, EFFECT_INVISIBILITY) == 0)
	{
		gEffectManager.AddEffect(lpObj, 0, EFFECT_INVISIBILITY, 0, 0, 0, 0, 0);

		gObjViewportListProtocolDestroy(lpObj);

		gLog.Output(LOG_COMMAND, "[CommandHide][%s][%s] - (State: %d)", lpObj->Account, lpObj->Name, 1);
	}
	else
	{
		gEffectManager.DelEffect(lpObj, EFFECT_INVISIBILITY);

		gObjViewportListProtocolCreate(lpObj);

		gLog.Output(LOG_COMMAND, "[CommandHide][%s][%s] - (State: %d)", lpObj->Account, lpObj->Name, 0);
	}
}
