// CommandManager.cpp: implementation of the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "BonusManager.h"
#include "DSProtocol.h"
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
#include "ResetTable.h"
#include "ServerInfo.h"
#include "Util.h"

CCommandManager gCommandManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandManager::CCommandManager() // OK
{

}

CCommandManager::~CCommandManager() // OK
{

}

void CCommandManager::Init() // OK
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		this->m_CommandInfo[n].code = -1;
	}

	this->Add(gMessage.GetMessage(32),COMMAND_MOVE);
	this->Add(gMessage.GetMessage(33),COMMAND_POST);
	this->Add(gMessage.GetMessage(34),COMMAND_ADD_POINT1);
	this->Add(gMessage.GetMessage(35),COMMAND_ADD_POINT2);
	this->Add(gMessage.GetMessage(36),COMMAND_ADD_POINT3);
	this->Add(gMessage.GetMessage(37),COMMAND_ADD_POINT4);
	this->Add(gMessage.GetMessage(38),COMMAND_ADD_POINT5);
	this->Add(gMessage.GetMessage(39),COMMAND_PK_CLEAR);
	this->Add(gMessage.GetMessage(40),COMMAND_MONEY);
	this->Add(gMessage.GetMessage(41),COMMAND_CHANGE);
	this->Add(gMessage.GetMessage(42),COMMAND_WARE);
	this->Add(gMessage.GetMessage(43),COMMAND_RESET);
	this->Add(gMessage.GetMessage(44),COMMAND_GM_MOVE);
	this->Add(gMessage.GetMessage(45),COMMAND_GM_POST);
	this->Add(gMessage.GetMessage(46),COMMAND_TRACK);
	this->Add(gMessage.GetMessage(47),COMMAND_TRACE);
	this->Add(gMessage.GetMessage(48),COMMAND_DISCONNECT);
	this->Add(gMessage.GetMessage(49),COMMAND_FIREWORKS);
	this->Add(gMessage.GetMessage(50),COMMAND_MAKE);
	this->Add(gMessage.GetMessage(51),COMMAND_SKIN);
	this->Add(gMessage.GetMessage(52),COMMAND_SET_MONEY);
	this->Add(gMessage.GetMessage(53),COMMAND_NOTICE);
	this->Add(gMessage.GetMessage(54),COMMAND_MASTER_RESET);
	this->Add(gMessage.GetMessage(55),COMMAND_GUILD_WAR);
	this->Add(gMessage.GetMessage(56),COMMAND_BATTLE_SOCCER);
	this->Add(gMessage.GetMessage(57),COMMAND_REQUEST);
	this->Add(gMessage.GetMessage(58),COMMAND_HIDE);
}

void CCommandManager::MainProc() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		if(gObj[n].AutoAddPointCount > 0)
		{
			this->CommandAddPointAutoProc(&gObj[n]);
		}

		if(gObj[n].AutoResetEnable != 0)
		{
			this->CommandResetAutoProc(&gObj[n]);
		}
	}
}

void CCommandManager::Add(char* label,int code) // OK
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		if(this->m_CommandInfo[n].code != -1)
		{
			continue;
		}

		this->m_CommandInfo[n].code = code;
		strcpy_s(this->m_CommandInfo[n].label,label);
		break;
	}
}

long CCommandManager::GetNumber(char* arg,int pos) // OK
{
	int count=0,p=0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len>=sizeof(buffer))?(sizeof(buffer)-1):len);

	for(int n=0;n < len;n++)
	{
		if(arg[n] == 0x20)
		{
			count++;
		}
		else if(count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	return atoi(buffer);
}

void CCommandManager::GetString(char* arg,char* out,int size,int pos) // OK
{
	int count=0,p=0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len>=sizeof(buffer))?(sizeof(buffer)-1):len);

	for(int n=0;n < len;n++)
	{
		if(arg[n] == 0x20)
		{
			count++;
		}
		else if(count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	memcpy(out,buffer,(size-1));
}

long CCommandManager::GetCommandCode(char* label) // OK
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		if(this->m_CommandInfo[n].code == -1)
		{
			continue;
		}

		if(_stricmp(label,this->m_CommandInfo[n].label) == 0)
		{
			return this->m_CommandInfo[n].code;
		}
	}

	return 0;
}

bool CCommandManager::ManagementCore(LPOBJ lpObj,char* message) // OK
{
	char command[32] = {0};

	memset(command,0,sizeof(command));

	this->GetString(message,command,sizeof(command),0);

	int code = this->GetCommandCode(command);

	char* argument = &message[strlen(command)];

	if(argument[0] == 0x20)
	{
		argument++;
	}

	switch(code)
	{
		case COMMAND_MOVE:
			this->CommandMove(lpObj,argument);
			break;
		case COMMAND_POST:
			this->CommandPost(lpObj,argument);
			break;
		case COMMAND_ADD_POINT1:
			this->CommandAddPoint(lpObj,argument,0);
			break;
		case COMMAND_ADD_POINT2:
			this->CommandAddPoint(lpObj,argument,1);
			break;
		case COMMAND_ADD_POINT3:
			this->CommandAddPoint(lpObj,argument,2);
			break;
		case COMMAND_ADD_POINT4:
			this->CommandAddPoint(lpObj,argument,3);
			break;
		case COMMAND_ADD_POINT5:
			this->CommandAddPoint(lpObj,argument,4);
			break;
		case COMMAND_PK_CLEAR:
			this->CommandPKClear(lpObj,argument);
			break;
		case COMMAND_MONEY:
			this->CommandMoney(lpObj,argument);
			break;
		case COMMAND_CHANGE:
			this->CommandChange(lpObj,argument);
			break;
		case COMMAND_WARE:
			this->CommandWare(lpObj,argument);
			break;
		case COMMAND_RESET:
			this->CommandReset(lpObj,argument);
			break;
		case COMMAND_GM_MOVE:
			this->CommandGMMove(lpObj,argument);
			break;
		case COMMAND_GM_POST:
			this->CommandGMPost(lpObj,argument);
			break;
		case COMMAND_TRACK:
			this->CommandTrack(lpObj,argument);
			break;
		case COMMAND_TRACE:
			this->CommandTrace(lpObj,argument);
			break;
		case COMMAND_DISCONNECT:
			this->CommandDisconnect(lpObj,argument);
			break;
		case COMMAND_FIREWORKS:
			this->CommandFireworks(lpObj,argument);
			break;
		case COMMAND_MAKE:
			this->CommandMake(lpObj,argument);
			break;
		case COMMAND_SKIN:
			this->CommandSkin(lpObj,argument);
			break;
		case COMMAND_SET_MONEY:
			this->CommandSetMoney(lpObj,argument);
			break;
		case COMMAND_NOTICE:
			this->CommandNotice(lpObj,argument);
			break;
		case COMMAND_MASTER_RESET:
			this->CommandMasterReset(lpObj,argument);
			break;
		case COMMAND_GUILD_WAR:
			this->CommandGuildWar(lpObj,argument);
			break;
		case COMMAND_BATTLE_SOCCER:
			this->CommandBattleSoccer(lpObj,argument);
			break;
		case COMMAND_REQUEST:
			this->CommandRequest(lpObj,argument);
			break;
		case COMMAND_HIDE:
			this->CommandHide(lpObj,argument);
			break;
		default:
			return 0;
	}

	return 1;
}

void CCommandManager::CommandMove(LPOBJ lpObj,char* arg) // OK
{
	MOVE_INFO MoveInfo;

	if(gMove.GetInfoByName(arg,&MoveInfo) != 0)
	{
		gMove.Move(lpObj,MoveInfo.Index);
		return;
	}
}

void CCommandManager::CommandPost(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandPostSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandPostEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(64));
		return;
	}

	if(lpObj->Level < gServerInfo.m_CommandPostLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(65),gServerInfo.m_CommandPostLevel[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Reset < gServerInfo.m_CommandPostReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(66),gServerInfo.m_CommandPostReset[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandPostMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(67),gServerInfo.m_CommandPostMoney[lpObj->AccountLevel]);
		return;
	}

	DWORD tick = (GetTickCount()-lpObj->PostTime)/1000;

	if(tick < ((DWORD)gServerInfo.m_CommandPostDelay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(68),(gServerInfo.m_CommandPostDelay[lpObj->AccountLevel]-tick));
		return;
	}

	lpObj->PostTime = GetTickCount();

	lpObj->Money -= gServerInfo.m_CommandPostMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	if(gServerInfo.m_CommandPostType == 0)
	{
		PostMessage1(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 1)
	{
		PostMessage2(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 2)
	{
		PostMessage3(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 3)
	{
		PostMessage4(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 4)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),0,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 5)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),1,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 6)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),2,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 7)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),3,lpObj->Name,arg);
	}

	gLog.Output(LOG_COMMAND,"[CommandPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);
}

void CCommandManager::CommandAddPoint(LPOBJ lpObj,char* arg,int type) // OK
{
	if(gServerInfo.m_CommandAddPointSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandAddPointEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(70));
		return;
	}

	if(lpObj->Class != CLASS_DL && type == 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(71));
		return;
	}

	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandAddPointAuto(lpObj,arg,type);
		return;
	}

	int amount = this->GetNumber(arg,0);

	if(amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(72));
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(73),gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel]);
		return;
	}

	if(gObjectManager.CharacterLevelUpPointAdd(lpObj,type,amount) == 0)
	{
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(74),amount,lpObj->LevelUpPoint);

	gLog.Output(LOG_COMMAND,"[CommandAddPoint][%s][%s] - (Type: %d, Amount: %d)",lpObj->Account,lpObj->Name,type,amount);
}

void CCommandManager::CommandAddPointAuto(LPOBJ lpObj,char* arg,int type) // OK
{
	if(gServerInfo.m_CommandAddPointAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(70));
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(73),gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel]);
		return;
	}

	int amount = this->GetNumber(arg,1);

	if(amount > 0)
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = amount;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->Money -= gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel];

		GCMoneySend(lpObj->Index,lpObj->Money);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(114),amount);

		gLog.Output(LOG_COMMAND,"[CommandAddPointAuto][%s][%s] - (Type: %d, Amount: %d)",lpObj->Account,lpObj->Name,type,amount);
	}
	else
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = 0;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(115));
	}
}

void CCommandManager::CommandPKClear(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandPKClearSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandPKClearEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(75));
		return;
	}

	if(lpObj->PKLevel <= 3)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(76));
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandPKClearMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(77),gServerInfo.m_CommandPKClearMoney[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandPKClearMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->PKLevel = 3;

	GCPKLevelSend(lpObj->Index,lpObj->PKLevel);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(78));

	gLog.Output(LOG_COMMAND,"[CommandPKClear][%s][%s] - (PKLevel: %d)",lpObj->Account,lpObj->Name,lpObj->PKLevel);
}

void CCommandManager::CommandMoney(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandMoneySwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandMoneyEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(79));
		return;
	}

	int money = this->GetNumber(arg,0);

	if(money < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(80));
		return;
	}

	if(money == 0)
	{
		lpObj->Money = 0;
	}
	else if(gObjCheckMaxMoney(lpObj->Index,money) == 0)
	{
		lpObj->Money = MAX_MONEY;
	}
	else
	{
		lpObj->Money += money;
	}

	GCMoneySend(lpObj->Index,lpObj->Money);

	gLog.Output(LOG_COMMAND,"[CommandMoney][%s][%s] - (Money: %d)",lpObj->Account,lpObj->Name,money);
}

void CCommandManager::CommandChange(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandChangeSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandChangeEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(81));
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandChangeMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(82),gServerInfo.m_CommandChangeMoney[lpObj->AccountLevel]);
		return;
	}

	int ChangeUp = lpObj->ChangeUp;

	if(lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL)
	{
		ChangeUp++;
	}

	if(ChangeUp >= gServerInfo.m_CommandChangeLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(83));
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandChangeMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	for(int n=0;n <= ((ChangeUp==0)?3:6);n++)
	{
		if(gQuest.CheckQuestListState(lpObj,n,QUEST_FINISH) == 0)
		{
			if(n != 3 || lpObj->Class == CLASS_DK)
			{
				gQuest.AddQuestList(lpObj,n,QUEST_ACCEPT);
				gQuestReward.InsertQuestReward(lpObj,n);
				gQuest.AddQuestList(lpObj,n,QUEST_FINISH);
			}
		}
	}

	lpObj->SendQuestInfo = 0;

	gQuest.GCQuestInfoSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(84));

	gLog.Output(LOG_COMMAND,"[CommandChange][%s][%s] - (ChangeUp: %d)",lpObj->Account,lpObj->Name,ChangeUp);
}

void CCommandManager::CommandWare(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandWareSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandWareEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(85));
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->LoadWarehouse != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(86));
		return;
	}

	int number = this->GetNumber(arg,0);

	if(number < 0 || number >= gServerInfo.m_CommandWareNumber[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(87));
		return;
	}

	lpObj->WarehouseMoney = 0;

	memset(lpObj->WarehouseMap,0xFF,WAREHOUSE_SIZE);

	for(int n=0;n < WAREHOUSE_SIZE;n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->LoadWarehouse = 0;

	lpObj->WarehouseNumber = number;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(88),number);

	gLog.Output(LOG_COMMAND,"[CommandWare][%s][%s] - (Number: %d)",lpObj->Account,lpObj->Name,number);
}

void CCommandManager::CommandReset(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandResetSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandResetEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(89));
		return;
	}

	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandResetAuto(lpObj,arg);
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(90));
		return;
	}

	if(lpObj->Level < gResetTable.GetResetLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(91),gResetTable.GetResetLevel(lpObj));
		return;
	}

	if(lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(92),gResetTable.GetResetMoney(lpObj));
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(93),gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);
		return;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(94));
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::CommandResetAuto(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(89));
		return;
	}

	if(lpObj->AutoResetEnable == 0)
	{
		lpObj->AutoResetEnable = 1;

		lpObj->AutoResetStats[0] = this->GetNumber(arg,1);

		lpObj->AutoResetStats[1] = this->GetNumber(arg,2);

		lpObj->AutoResetStats[2] = this->GetNumber(arg,3);

		lpObj->AutoResetStats[3] = this->GetNumber(arg,4);

		lpObj->AutoResetStats[4] = ((lpObj->Class==CLASS_DL)?this->GetNumber(arg,5):0);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(116));
	}
	else
	{
		lpObj->AutoResetEnable = 0;

		lpObj->AutoResetStats[0] = 0;

		lpObj->AutoResetStats[1] = 0;

		lpObj->AutoResetStats[2] = 0;

		lpObj->AutoResetStats[3] = 0;

		lpObj->AutoResetStats[4] = 0;

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(117));
	}
}

void CCommandManager::CommandGMMove(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandGMMoveSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandGMMoveGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(96));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	int map = this->GetNumber(arg,1);

	int x = this->GetNumber(arg,2);

	int y = this->GetNumber(arg,3);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(97));
		return;
	}

	gObjTeleport(lpTarget->Index,map,x,y);

	gLog.Output(LOG_COMMAND,"[CommandGMMove][%s][%s] - (Name: %s, Map: %d, X: %d, Y: %d)",lpObj->Account,lpObj->Name,name,map,x,y);
}

void CCommandManager::CommandGMPost(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandGMPostSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandGMPostGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(98));
		return;
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[%s] %s",lpObj->Name,arg);

	gLog.Output(LOG_COMMAND,"[CommandGMPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);
}

void CCommandManager::CommandTrack(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandTrackSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandTrackGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(99));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return;
	}

	gObjTeleport(lpObj->Index,lpTarget->Map,lpTarget->X,lpTarget->Y);

	gLog.Output(LOG_COMMAND,"[CommandTrack][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandTrace(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandTraceSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandTraceGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(101));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(102));
		return;
	}

	gObjTeleport(lpTarget->Index,lpObj->Map,lpObj->X,lpObj->Y);

	gLog.Output(LOG_COMMAND,"[CommandTrace][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandDisconnect(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandDisconnectSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandDisconnectGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(103));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(104));
		return;
	}

	gObjUserKill(lpTarget->Index);

	gLog.Output(LOG_COMMAND,"[CommandDisconnect][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandFireworks(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandFireworksSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandFireworksGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(105));
		return;
	}

	for(int n=0;n < 15;n++)
	{
		GCFireworksSend(lpObj,(lpObj->X+(((GetLargeRand()%5)*2)-4)),(lpObj->Y+(((GetLargeRand()%5)*2)-4)));
	}

	gLog.Output(LOG_COMMAND,"[CommandFireworks][%s][%s] - (Map: %d, X: %d, Y: %d)",lpObj->Account,lpObj->Name,lpObj->Map,lpObj->X,lpObj->Y);
}

void CCommandManager::CommandMake(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandMakeSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandMakeGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(106));
		return;
	}

	int section = this->GetNumber(arg,0);
	int type = this->GetNumber(arg,1);
	int level = this->GetNumber(arg,2);
	int skill = this->GetNumber(arg,3);
	int luck = this->GetNumber(arg,4);
	int option = this->GetNumber(arg,5);
	int exc = this->GetNumber(arg,6);
	int set = this->GetNumber(arg,7);

	GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(section,type),level,0,skill,luck,option,-1,exc,set,0);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(107));

	gLog.Output(LOG_COMMAND,"[CommandMake][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d)",lpObj->Account,lpObj->Name,section,type,level,skill,luck,option,exc,set);
}

void CCommandManager::CommandSkin(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandSkinSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandSkinGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(108));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(109));
		return;
	}
	
	lpTarget->Change = this->GetNumber(arg,1);

	gObjViewportListProtocolCreate(lpTarget);

	gObjectManager.CharacterUpdateMapEffect(lpTarget);

	gLog.Output(LOG_COMMAND,"[CommandSkin][%s][%s] - (Name: %s, Change: %d)",lpObj->Account,lpObj->Name,name,lpTarget->Change);
}

void CCommandManager::CommandSetMoney(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandSetMoneySwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandSetMoneyGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(110));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(111));
		return;
	}

	int money = this->GetNumber(arg,1);

	if(money < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(112));
		return;
	}

	if(money == 0)
	{
		lpTarget->Money = 0;
	}
	else if(gObjCheckMaxMoney(lpTarget->Index,money) == 0)
	{
		lpTarget->Money = MAX_MONEY;
	}
	else
	{
		lpTarget->Money += money;
	}

	GCMoneySend(lpTarget->Index,lpTarget->Money);

	gLog.Output(LOG_COMMAND,"[CommandSetMoney][%s][%s] - (Name: %s, Money: %d)",lpObj->Account,lpObj->Name,name,money);
}

void CCommandManager::CommandNotice(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandNoticeSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandNoticeGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(113));
		return;
	}

	GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,arg);

	gLog.Output(LOG_COMMAND,"[CommandNotice][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);
}

void CCommandManager::CommandMasterReset(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandMasterResetSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandMasterResetEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(118));
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(119));
		return;
	}

	if(lpObj->Level < gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(120),gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Reset < gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(121),gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(122),gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->MasterReset >= gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(123),gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel]);
		return;
	}

	if(gServerInfo.m_CommandMasterResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(124));
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_MASTER_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x01,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::CommandGuildWar(LPOBJ lpObj,char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg,GuildName,sizeof(GuildName),0);

	if(strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName,lpObj->Index,0);
	}
}

void CCommandManager::CommandBattleSoccer(LPOBJ lpObj,char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg,GuildName,sizeof(GuildName),0);

	if(strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName,lpObj->Index,1);
	}
}

void CCommandManager::CommandRequest(LPOBJ lpObj,char* arg) // OK
{
	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"on") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(126));
	}
	else if(strcmp(mode,"off") == 0)
	{
		lpObj->Option &= ~1;
		lpObj->Option &= ~2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(127));
	}
	else if(strcmp(mode,"auto") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option |= 4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		this->GetString(arg,lpObj->AutoPartyPassword,sizeof(lpObj->AutoPartyPassword),1);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(128));
	}
}

void CCommandManager::CommandHide(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandHideSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandHideGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(135));
		return;
	}

	if(gEffectManager.CheckEffect(lpObj,EFFECT_INVISIBILITY) == 0)
	{
		gEffectManager.AddEffect(lpObj,0,EFFECT_INVISIBILITY,0,0,0,0,0);

		gObjViewportListProtocolDestroy(lpObj);

		gLog.Output(LOG_COMMAND,"[CommandHide][%s][%s] - (State: %d)",lpObj->Account,lpObj->Name,1);
	}
	else
	{
		gEffectManager.DelEffect(lpObj,EFFECT_INVISIBILITY);

		gObjViewportListProtocolCreate(lpObj);

		gLog.Output(LOG_COMMAND,"[CommandHide][%s][%s] - (State: %d)",lpObj->Account,lpObj->Name,0);
	}
}

void CCommandManager::CommandAddPointAutoProc(LPOBJ lpObj) // OK
{
	if(lpObj->LevelUpPoint == 0 || lpObj->AutoAddPointCount == 0)
	{
		return;
	}

	int AddStatCount = lpObj->AutoAddPointCount;

	int* stat[5] = {&lpObj->Strength,&lpObj->Dexterity,&lpObj->Vitality,&lpObj->Energy,&lpObj->Leadership};

	for(int n=0;n < 5;n++)
	{
		if(lpObj->AutoAddPointStats[n] > 0)
		{
			if(AddStatCount == 0)
			{
				break;
			}

			int AddStat = lpObj->LevelUpPoint/AddStatCount;

			AddStat = (((lpObj->LevelUpPoint%AddStatCount)>0)?(AddStat+1):AddStat);

			AddStat = ((AddStat>lpObj->AutoAddPointStats[n])?lpObj->AutoAddPointStats[n]:AddStat);

			AddStat = ((((*stat[n])+AddStat)>gServerInfo.m_MaxStatPoint[lpObj->AccountLevel])?(gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]-(*stat[n])):AddStat);

			AddStatCount--;

			(*stat[n]) += AddStat;

			lpObj->LevelUpPoint -= AddStat;

			lpObj->AutoAddPointStats[n] -= (((*stat[n])>=gServerInfo.m_MaxStatPoint[lpObj->AccountLevel])?lpObj->AutoAddPointStats[n]:AddStat);

			lpObj->AutoAddPointCount = 0;

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);
}

void CCommandManager::CommandResetAutoProc(LPOBJ lpObj) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	if(lpObj->Level < gResetTable.GetResetLevel(lpObj))
	{
		return;
	}

	if(lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		return;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if(lpMsg->ResetDay >= ((DWORD)gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(129),gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->ResetWek >= ((DWORD)gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(130),gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->ResetMon >= ((DWORD)gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(131),gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]==-1)?(lpObj->Level-gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]):gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level-1];

	lpObj->Reset += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetDay += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetWek += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetMon += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpObj->AutoAddPointCount = 0;

	lpObj->AutoAddPointStats[0] = 0;

	lpObj->AutoAddPointStats[1] = 0;

	lpObj->AutoAddPointStats[2] = 0;

	lpObj->AutoAddPointStats[3] = 0;

	lpObj->AutoAddPointStats[4] = 0;

	if(gServerInfo.m_CommandResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass/16)*16);
		lpObj->ChangeUp = lpObj->DBClass%16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp*16);
		Class -= (Class/32);
		Class += (lpObj->Class*32);

		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);

		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if(gServerInfo.m_CommandResetSkill[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}

	if(gServerInfo.m_CommandResetType == 1)
	{
		int point = gResetTable.GetResetPoint(lpObj);

		point = (point*gServerInfo.m_CommandResetPointRate[lpObj->Class])/100;

		point += (lpObj->Level-1)*gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH)==0)?0:((lpObj->Level>220)?((lpObj->Level-220)*gServerInfo.m_PlusStatPoint):0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	if(gServerInfo.m_CommandMasterResetSwitch != 0 && gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset;

		point = (point*gServerInfo.m_CommandMasterResetPointRate[lpObj->Class])/100;

		lpObj->LevelUpPoint += point;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDResetInfoSaveSend(lpObj->Index,lpMsg->ResetDay,lpMsg->ResetWek,lpMsg->ResetMon);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(95),lpObj->Reset);

	if(gServerInfo.m_CommandResetMove[lpObj->AccountLevel] != 0)
	{
		switch(lpObj->Class)
		{
			case CLASS_DW:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DK:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_FE:
				gObjMoveGate(lpObj->Index,27);
				break;
			case CLASS_MG:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DL:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_SU:
				gObjMoveGate(lpObj->Index,267);
				break;
			case CLASS_RF:
				gObjMoveGate(lpObj->Index,17);
				break;
		}
	}

	lpObj->AutoAddPointStats[0] = lpObj->AutoResetStats[0];

	lpObj->AutoAddPointStats[1] = lpObj->AutoResetStats[1];

	lpObj->AutoAddPointStats[2] = lpObj->AutoResetStats[2];

	lpObj->AutoAddPointStats[3] = lpObj->AutoResetStats[3];

	lpObj->AutoAddPointStats[4] = lpObj->AutoResetStats[4];

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	this->CommandAddPointAutoProc(lpObj);

	gLog.Output(LOG_COMMAND,"[CommandReset][%s][%s] - (Reset: %d)",lpObj->Account,lpObj->Name,lpObj->Reset);
}

void CCommandManager::DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandMasterResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if(lpMsg->MasterResetDay >= ((DWORD)gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(132),gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->MasterResetWek >= ((DWORD)gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(133),gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->MasterResetMon >= ((DWORD)gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(134),gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]==-1)?(lpObj->Level-gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]):gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level-1];

	lpObj->Reset = ((gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]==-1)?(lpObj->Reset-gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]):gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]);

	lpObj->MasterReset += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->MasterResetDay += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->MasterResetWek += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->MasterResetMon += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	if(gServerInfo.m_CommandMasterResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass/16)*16);
		lpObj->ChangeUp = lpObj->DBClass%16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp*16);
		Class -= (Class/32);
		Class += (lpObj->Class*32);

		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);

		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if(gServerInfo.m_CommandMasterResetSkill[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}

	if(gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset;

		point = (point*gServerInfo.m_CommandMasterResetPointRate[lpObj->Class])/100;

		point += (lpObj->Level-1)*gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH)==0)?0:((lpObj->Level>220)?((lpObj->Level-220)*gServerInfo.m_PlusStatPoint):0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	if(gServerInfo.m_CommandResetSwitch != 0 && gServerInfo.m_CommandResetType == 1)
	{
		int point = gResetTable.GetResetPoint(lpObj);

		point = (point*gServerInfo.m_CommandResetPointRate[lpObj->Class])/100;

		lpObj->LevelUpPoint += point;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDMasterResetInfoSaveSend(lpObj->Index,lpMsg->MasterResetDay,lpMsg->MasterResetWek,lpMsg->MasterResetMon);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(125),lpObj->MasterReset);

	if(gServerInfo.m_CommandMasterResetMove[lpObj->AccountLevel] != 0)
	{
		switch(lpObj->Class)
		{
			case CLASS_DW:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DK:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_FE:
				gObjMoveGate(lpObj->Index,27);
				break;
			case CLASS_MG:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DL:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_SU:
				gObjMoveGate(lpObj->Index,267);
				break;
			case CLASS_RF:
				gObjMoveGate(lpObj->Index,17);
				break;
		}
	}

	gLog.Output(LOG_COMMAND,"[CommandMasterReset][%s][%s] - (MasterReset: %d)",lpObj->Account,lpObj->Name,lpObj->MasterReset);
}
