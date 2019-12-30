// ServerInfo.cpp: implementation of the CServerInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerInfo.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "DefaultClassInfo.h"
#include "DevilSquare.h"
#include "EffectManager.h"
#include "Filter.h"
#include "GameMaster.h"
#include "Gate.h"
#include "HackPacketCheck.h"
#include "InvasionManager.h"
#include "ItemBagManager.h"
#include "ItemDrop.h"
#include "ItemMove.h"
#include "ItemOption.h"
#include "ItemOptionRate.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "Message.h"
#include "MonsterAIAutomata.h"
#include "MonsterAIElement.h"
#include "MonsterAIGroup.h"
#include "MonsterAIRule.h"
#include "MonsterAIUnit.h"
#include "MonsterManager.h"
#include "MonsterSetBase.h"
#include "MonsterSkillElement.h"
#include "MonsterSkillManager.h"
#include "MonsterSkillUnit.h"
#include "Move.h"
#include "MoveSummon.h"
#include "Notice.h"
#include "PacketManager.h"
#include "Path.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "QuestReward.h"
#include "SetItemOption.h"
#include "SetItemType.h"
#include "ShopManager.h"
#include "SkillHitBox.h"
#include "SkillManager.h"
#include "Util.h"

CServerInfo gServerInfo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerInfo::CServerInfo() // OK
{

}

CServerInfo::~CServerInfo() // OK
{

}

void CServerInfo::ReadInit() // OK
{
	gPath.SetMainPath(MAIN_PATH);

	this->ReadChaosMixInfo();
	this->ReadCharacterInfo();
	this->ReadCommandInfo();
	this->ReadCommonInfo();
	this->ReadCustomInfo();
	this->ReadEventInfo();
	this->ReadEventItemBagInfo();
	this->ReadHackInfo();
	this->ReadItemInfo();
	this->ReadMapInfo();
	this->ReadMonsterInfo();
	this->ReadMoveInfo();
	this->ReadQuestInfo();
	this->ReadShopInfo();
	this->ReadSkillInfo();
	this->ReadUtilInfo();
}

void CServerInfo::ReadChaosMixInfo() // OK
{
	this->ReadChaosMixInfo("GameServerInfo", ".\\Data\\GameServerInfo - ChaosMix.dat");

	LogAdd(LOG_BLUE, "[ServerInfo] ChaosMix loaded successfully");
}

void CServerInfo::ReadCharacterInfo() // OK
{
	this->ReadCharacterInfo("GameServerInfo", ".\\Data\\GameServerInfo - Character.dat");

	gDefaultClassInfo.Load(gPath.GetFullPath("Character\\DefaultClassInfo.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Character loaded successfully");
}

void CServerInfo::ReadCommandInfo() // OK
{
	this->ReadCommandInfo("GameServerInfo", ".\\Data\\GameServerInfo - Command.dat");

	LogAdd(LOG_BLUE, "[ServerInfo] Command loaded successfully");
}

void CServerInfo::ReadCommonInfo() // OK
{
	this->ReadCommonInfo("GameServerInfo", ".\\Data\\GameServerInfo - Common.dat");

	gEffectManager.Load(gPath.GetFullPath("Effect.txt"));

	gMapManager.Load(gPath.GetFullPath("MapManager.txt"));

	gMapServerManager.Load(gPath.GetFullPath("MapServerInfo.dat"));

	gMessage.Load(gPath.GetFullPath("Message.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Common loaded successfully");
}

void CServerInfo::ReadCustomInfo() // OK
{
	this->ReadCustomInfo("GameServerInfo", ".\\Data\\GameServerInfo - Custom.dat");

	LogAdd(LOG_BLUE, "[ServerInfo] Custom loaded successfully");
}

void CServerInfo::ReadEventInfo() // OK
{
	this->ReadEventInfo("GameServerInfo", ".\\Data\\GameServerInfo - Event.dat");

	gBloodCastle.Load(gPath.GetFullPath("Event\\BloodCastle.dat"));
	gBonusManager.Load(gPath.GetFullPath("Event\\BonusManager.dat"));
	gCastleDeep.Load(gPath.GetFullPath("Event\\CastleDeepEvent.dat"));
	gChaosCastle.Load(gPath.GetFullPath("Event\\ChaosCastle.dat"));
	gDevilSquare.Load(gPath.GetFullPath("Event\\DevilSquare.dat"));
	gInvasionManager.Load(gPath.GetFullPath("Event\\InvasionManager.dat"));

#if(GAMESERVER_TYPE==1)

	if (gCastleSiege.Ready(gMapServerManager.GetMapServerGroup()) != 0)
	{
		if (gCastleSiege.LoadData(gPath.GetFullPath("Event\\MuCastleData.dat")) != 0)
		{
			gCastleSiege.LoadPreFixData(".\\Data\\GameServerInfo - Event.dat");
			gCastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);
		}
	}

#endif

	LogAdd(LOG_BLUE, "[ServerInfo] Event loaded successfully");
}

void CServerInfo::ReadEventItemBagInfo() // OK
{
	gItemBagManager.Load(gPath.GetFullPath("EventItemBagManager.txt"));

	gItemBagManager.LoadEventItemBag();

	LogAdd(LOG_BLUE, "[ServerInfo] EventItemBag loaded successfully");
}

void CServerInfo::ReadHackInfo() // OK
{
	this->ReadHackInfo("GameServerInfo", ".\\Data\\GameServerInfo - Common.dat");

	gHackPacketCheck.Load(gPath.GetFullPath("Hack\\HackPacketCheck.txt"));

	gPacketManager.LoadEncryptionKey(gPath.GetFullPath("Hack\\Enc2.dat"));

	gPacketManager.LoadDecryptionKey(gPath.GetFullPath("Hack\\Dec1.dat"));

	LogAdd(LOG_BLUE, "[ServerInfo] Hack loaded successfully");
}

void CServerInfo::ReadItemInfo() // OK
{
	gItemManager.Load(gPath.GetFullPath("Item\\Item.txt"));
	gItemDrop.Load(gPath.GetFullPath("Item\\ItemDrop.txt"));
	gItemMove.Load(gPath.GetFullPath("Item\\ItemMove.txt"));
	gItemOption.Load(gPath.GetFullPath("Item\\ItemOption.txt"));
	gItemOptionRate.Load(gPath.GetFullPath("Item\\ItemOptionRate.txt"));
	gItemStack.Load(gPath.GetFullPath("Item\\ItemStack.txt"));
	gItemValue.Load(gPath.GetFullPath("Item\\ItemValue.txt"));
	gSetItemOption.Load(gPath.GetFullPath("Item\\SetItemOption.txt"));
	gSetItemType.Load(gPath.GetFullPath("Item\\SetItemType.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Item loaded successfully");
}

void CServerInfo::ReadMapInfo() // OK
{
	FILE* file;

	char path[256] = {0};

	char temp[256] = {0};

	for (int n = 0; n < MAX_MAP; n++)
	{
		wsprintf(temp, "Terrain\\terrain%d.att", (n + 1));

		if (fopen_s(&file, gPath.GetFullPath(temp), "r") == 0)
		{
			strcpy_s(path, temp);
			fclose(file);
		}

		gMap[n].Load(gPath.GetFullPath(path), n);
	}
}

void CServerInfo::ReadMonsterInfo() // OK
{
	CMonsterAIAutomata::LoadData(gPath.GetFullPath("Monster\\MonsterAIAutomata.txt"));
	CMonsterAIElement::LoadData(gPath.GetFullPath("Monster\\MonsterAIElement.txt"));
	CMonsterAIGroup::LoadData(gPath.GetFullPath("Monster\\MonsterAIGroup.txt"));
	CMonsterAIRule::LoadData(gPath.GetFullPath("Monster\\MonsterAIRule.txt"));
	CMonsterAIUnit::LoadData(gPath.GetFullPath("Monster\\MonsterAIUnit.txt"));

	gMonsterManager.Load(gPath.GetFullPath("Monster\\Monster.txt"));

#if(GAMESERVER_TYPE==0)

	gMonsterSetBase.Load(gPath.GetFullPath("Monster\\MonsterSetBase.txt"));

#else

	gMonsterSetBase.Load(gPath.GetFullPath("Monster\\MonsterSetBaseCS.txt"));

#endif

	CMonsterSkillElement::LoadData(gPath.GetFullPath("Monster\\MonsterSkillElement.txt"));
	CMonsterSkillUnit::LoadData(gPath.GetFullPath("Monster\\MonsterSkillUnit.txt"));
	CMonsterSkillManager::LoadData(gPath.GetFullPath("Monster\\MonsterSkill.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Monster loaded successfully");
}

void CServerInfo::ReadMoveInfo() // OK
{
	gGate.Load(gPath.GetFullPath("Move\\Gate.txt"));
	gMove.Load(gPath.GetFullPath("Move\\Move.txt"));
	gMoveSummon.Load(gPath.GetFullPath("Move\\MoveSummon.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Move loaded successfully");
}

void CServerInfo::ReadQuestInfo() // OK
{
	gQuest.Load(gPath.GetFullPath("Quest\\Quest.txt"));
	gQuestObjective.Load(gPath.GetFullPath("Quest\\QuestObjective.txt"));
	gQuestReward.Load(gPath.GetFullPath("Quest\\QuestReward.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Quest loaded successfully");
}

void CServerInfo::ReadShopInfo() // OK
{
	gShopManager.Load(gPath.GetFullPath("ShopManager.txt"));
	gShopManager.LoadShop();
	gShopManager.ReloadShopInterface();

	LogAdd(LOG_BLUE, "[ServerInfo] Shop loaded successfully");
}

void CServerInfo::ReadSkillInfo() // OK
{
	this->ReadSkillInfo("GameServerInfo", ".\\Data\\GameServerInfo - Skill.dat");

	gSkillHitBox.Load(gPath.GetFullPath("Skill\\SkillElect.hit"));
	gSkillManager.Load(gPath.GetFullPath("Skill\\Skill.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Skill loaded successfully");
}

void CServerInfo::ReadUtilInfo() // OK
{
	gFilter.Load(gPath.GetFullPath("Util\\Filter.txt"));
	gGameMaster.Load(gPath.GetFullPath("Util\\GameMaster.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Util loaded successfully");
}

void CServerInfo::ReloadMonsterInfo() // OK
{
	for (int n = 0; n < MAX_OBJECT_MONSTER; n++)
	{
		if (gObj[n].Type != OBJECT_MONSTER && gObj[n].Type != OBJECT_NPC)
		{
			continue;
		}

	#if(GAMESERVER_TYPE==1)

		if (gObj[n].CsNpcType != 0)
		{
			continue;
		}

	#endif

		gObjDel(n);
	}

	gObjMonCount = 0;

	this->ReadMonsterInfo();

	gMonsterManager.SetMonsterData();
}

void CServerInfo::ReadStartupInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	GetPrivateProfileString(section, "CustomerName", "", this->m_CustomerName, sizeof(this->m_CustomerName), path);

	GetPrivateProfileString(section, "CustomerHardwareId", "", this->m_CustomerHardwareId, sizeof(this->m_CustomerHardwareId), path);

	GetPrivateProfileString(section, "ServerName", "", this->m_ServerName, sizeof(this->m_ServerName), path);

	this->m_ServerCode = GetPrivateProfileInt(section, "ServerCode", 0, path);

	this->m_ServerLock = GetPrivateProfileInt(section, "ServerLock", 0, path);

	this->m_ServerPort = GetPrivateProfileInt(section, "ServerPort", 0, path);

	wsprintf(this->m_ServerMutex, "WZ_MU_GS_MUTEX_%d", this->m_ServerCode);

	GetPrivateProfileString(section, "ServerVersion", "", GameServerInfo, sizeof(GameServerInfo), path);

	gServerInfo.m_ServerVersion[0] = GameServerInfo[0];
	gServerInfo.m_ServerVersion[1] = GameServerInfo[2];
	gServerInfo.m_ServerVersion[2] = GameServerInfo[3];
	gServerInfo.m_ServerVersion[3] = GameServerInfo[5];
	gServerInfo.m_ServerVersion[4] = GameServerInfo[6];
	gServerInfo.m_ServerVersion[5] = 0;

	GetPrivateProfileString(section, "ServerSerial", "", this->m_ServerSerial, sizeof(this->m_ServerSerial), path);

	this->m_ServerMaxUserNumber = GetPrivateProfileInt(section, "ServerMaxUserNumber", 0, path);

	this->m_ServerMaxUserNumber = ((this->m_ServerMaxUserNumber > MAX_OBJECT_USER) ? MAX_OBJECT_USER : this->m_ServerMaxUserNumber);

	GetPrivateProfileString(section, "DataServerAddress", "", this->m_DataServerAddress, sizeof(this->m_DataServerAddress), path);

	this->m_DataServerPort = GetPrivateProfileInt(section, "DataServerPort", 0, path);

	GetPrivateProfileString(section, "JoinServerAddress", "", this->m_JoinServerAddress, sizeof(this->m_JoinServerAddress), path);

	this->m_JoinServerPort = GetPrivateProfileInt(section, "JoinServerPort", 0, path);

	GetPrivateProfileString(section, "ConnectServerAddress", "", this->m_ConnectServerAddress, sizeof(this->m_ConnectServerAddress), path);

	this->m_ConnectServerPort = GetPrivateProfileInt(section, "ConnectServerPort", 0, path);

	this->m_WriteChatLog = GetPrivateProfileInt(section, "WriteChatLog", 0, path);

	this->m_WriteCommandLog = GetPrivateProfileInt(section, "WriteCommandLog", 0, path);

	this->m_WriteTradeLog = GetPrivateProfileInt(section, "WriteTradeLog", 0, path);

	this->m_WriteConnectLog = GetPrivateProfileInt(section, "WriteConnectLog", 0, path);

	this->m_WriteHackLog = GetPrivateProfileInt(section, "WriteHackLog", 0, path);

	this->m_WriteCashShopLog = GetPrivateProfileInt(section, "WriteCashShopLog", 0, path);

	this->m_WriteChaosMixLog = GetPrivateProfileInt(section, "WriteChaosMixLog", 0, path);
}

void CServerInfo::ReadChaosMixInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	this->m_ChaosItemMixRate[0] = GetPrivateProfileInt(section, "ChaosItemMixRate_AL0", 0, path);
	this->m_ChaosItemMixRate[1] = GetPrivateProfileInt(section, "ChaosItemMixRate_AL1", 0, path);
	this->m_ChaosItemMixRate[2] = GetPrivateProfileInt(section, "ChaosItemMixRate_AL2", 0, path);
	this->m_ChaosItemMixRate[3] = GetPrivateProfileInt(section, "ChaosItemMixRate_AL3", 0, path);
	this->m_DevilSquareMixRate1[0] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL0", 0, path);
	this->m_DevilSquareMixRate1[1] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL1", 0, path);
	this->m_DevilSquareMixRate1[2] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL2", 0, path);
	this->m_DevilSquareMixRate1[3] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL3", 0, path);
	this->m_DevilSquareMixRate2[0] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL0", 0, path);
	this->m_DevilSquareMixRate2[1] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL1", 0, path);
	this->m_DevilSquareMixRate2[2] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL2", 0, path);
	this->m_DevilSquareMixRate2[3] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL3", 0, path);
	this->m_DevilSquareMixRate3[0] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL0", 0, path);
	this->m_DevilSquareMixRate3[1] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL1", 0, path);
	this->m_DevilSquareMixRate3[2] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL2", 0, path);
	this->m_DevilSquareMixRate3[3] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL3", 0, path);
	this->m_DevilSquareMixRate4[0] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL0", 0, path);
	this->m_DevilSquareMixRate4[1] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL1", 0, path);
	this->m_DevilSquareMixRate4[2] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL2", 0, path);
	this->m_DevilSquareMixRate4[3] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL3", 0, path);
	this->m_DevilSquareMixRate5[0] = GetPrivateProfileInt(section, "DevilSquareMixRate5_AL0", 0, path);
	this->m_DevilSquareMixRate5[1] = GetPrivateProfileInt(section, "DevilSquareMixRate5_AL1", 0, path);
	this->m_DevilSquareMixRate5[2] = GetPrivateProfileInt(section, "DevilSquareMixRate5_AL2", 0, path);
	this->m_DevilSquareMixRate5[3] = GetPrivateProfileInt(section, "DevilSquareMixRate5_AL3", 0, path);
	this->m_DevilSquareMixRate6[0] = GetPrivateProfileInt(section, "DevilSquareMixRate6_AL0", 0, path);
	this->m_DevilSquareMixRate6[1] = GetPrivateProfileInt(section, "DevilSquareMixRate6_AL1", 0, path);
	this->m_DevilSquareMixRate6[2] = GetPrivateProfileInt(section, "DevilSquareMixRate6_AL2", 0, path);
	this->m_DevilSquareMixRate6[3] = GetPrivateProfileInt(section, "DevilSquareMixRate6_AL3", 0, path);
	this->m_DevilSquareMixRate7[0] = GetPrivateProfileInt(section, "DevilSquareMixRate7_AL0", 0, path);
	this->m_DevilSquareMixRate7[1] = GetPrivateProfileInt(section, "DevilSquareMixRate7_AL1", 0, path);
	this->m_DevilSquareMixRate7[2] = GetPrivateProfileInt(section, "DevilSquareMixRate7_AL2", 0, path);
	this->m_DevilSquareMixRate7[3] = GetPrivateProfileInt(section, "DevilSquareMixRate7_AL3", 0, path);
	this->m_PlusItemLevelMixRate1[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL0", 0, path);
	this->m_PlusItemLevelMixRate1[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL1", 0, path);
	this->m_PlusItemLevelMixRate1[2] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL2", 0, path);
	this->m_PlusItemLevelMixRate1[3] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL3", 0, path);
	this->m_PlusItemLevelMixRate2[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL0", 0, path);
	this->m_PlusItemLevelMixRate2[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL1", 0, path);
	this->m_PlusItemLevelMixRate2[2] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL2", 0, path);
	this->m_PlusItemLevelMixRate2[3] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL3", 0, path);
	this->m_PlusItemLevelMixRate3[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate3_AL0", 0, path);
	this->m_PlusItemLevelMixRate3[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate3_AL1", 0, path);
	this->m_PlusItemLevelMixRate3[2] = GetPrivateProfileInt(section, "PlusItemLevelMixRate3_AL2", 0, path);
	this->m_PlusItemLevelMixRate3[3] = GetPrivateProfileInt(section, "PlusItemLevelMixRate3_AL3", 0, path);
	this->m_PlusItemLevelMixRate4[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate4_AL0", 0, path);
	this->m_PlusItemLevelMixRate4[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate4_AL1", 0, path);
	this->m_PlusItemLevelMixRate4[2] = GetPrivateProfileInt(section, "PlusItemLevelMixRate4_AL2", 0, path);
	this->m_PlusItemLevelMixRate4[3] = GetPrivateProfileInt(section, "PlusItemLevelMixRate4_AL3", 0, path);
	this->m_DinorantMixRate[0] = GetPrivateProfileInt(section, "DinorantMixRate_AL0", 0, path);
	this->m_DinorantMixRate[1] = GetPrivateProfileInt(section, "DinorantMixRate_AL1", 0, path);
	this->m_DinorantMixRate[2] = GetPrivateProfileInt(section, "DinorantMixRate_AL2", 0, path);
	this->m_DinorantMixRate[3] = GetPrivateProfileInt(section, "DinorantMixRate_AL3", 0, path);
	this->m_FruitMixRate[0] = GetPrivateProfileInt(section, "FruitMixRate_AL0", 0, path);
	this->m_FruitMixRate[1] = GetPrivateProfileInt(section, "FruitMixRate_AL1", 0, path);
	this->m_FruitMixRate[2] = GetPrivateProfileInt(section, "FruitMixRate_AL2", 0, path);
	this->m_FruitMixRate[3] = GetPrivateProfileInt(section, "FruitMixRate_AL3", 0, path);
	this->m_Wing2MixRate[0] = GetPrivateProfileInt(section, "Wing2MixRate_AL0", 0, path);
	this->m_Wing2MixRate[1] = GetPrivateProfileInt(section, "Wing2MixRate_AL1", 0, path);
	this->m_Wing2MixRate[2] = GetPrivateProfileInt(section, "Wing2MixRate_AL2", 0, path);
	this->m_Wing2MixRate[3] = GetPrivateProfileInt(section, "Wing2MixRate_AL3", 0, path);
	this->m_BloodCastleMixRate1[0] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL0", 0, path);
	this->m_BloodCastleMixRate1[1] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL1", 0, path);
	this->m_BloodCastleMixRate1[2] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL2", 0, path);
	this->m_BloodCastleMixRate1[3] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL3", 0, path);
	this->m_BloodCastleMixRate2[0] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL0", 0, path);
	this->m_BloodCastleMixRate2[1] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL1", 0, path);
	this->m_BloodCastleMixRate2[2] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL2", 0, path);
	this->m_BloodCastleMixRate2[3] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL3", 0, path);
	this->m_BloodCastleMixRate3[0] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL0", 0, path);
	this->m_BloodCastleMixRate3[1] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL1", 0, path);
	this->m_BloodCastleMixRate3[2] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL2", 0, path);
	this->m_BloodCastleMixRate3[3] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL3", 0, path);
	this->m_BloodCastleMixRate4[0] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL0", 0, path);
	this->m_BloodCastleMixRate4[1] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL1", 0, path);
	this->m_BloodCastleMixRate4[2] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL2", 0, path);
	this->m_BloodCastleMixRate4[3] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL3", 0, path);
	this->m_BloodCastleMixRate5[0] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL0", 0, path);
	this->m_BloodCastleMixRate5[1] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL1", 0, path);
	this->m_BloodCastleMixRate5[2] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL2", 0, path);
	this->m_BloodCastleMixRate5[3] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL3", 0, path);
	this->m_BloodCastleMixRate6[0] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL0", 0, path);
	this->m_BloodCastleMixRate6[1] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL1", 0, path);
	this->m_BloodCastleMixRate6[2] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL2", 0, path);
	this->m_BloodCastleMixRate6[3] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL3", 0, path);
	this->m_BloodCastleMixRate7[0] = GetPrivateProfileInt(section, "BloodCastleMixRate7_AL0", 0, path);
	this->m_BloodCastleMixRate7[1] = GetPrivateProfileInt(section, "BloodCastleMixRate7_AL1", 0, path);
	this->m_BloodCastleMixRate7[2] = GetPrivateProfileInt(section, "BloodCastleMixRate7_AL2", 0, path);
	this->m_BloodCastleMixRate7[3] = GetPrivateProfileInt(section, "BloodCastleMixRate7_AL3", 0, path);
	this->m_BloodCastleMixRate8[0] = GetPrivateProfileInt(section, "BloodCastleMixRate8_AL0", 0, path);
	this->m_BloodCastleMixRate8[1] = GetPrivateProfileInt(section, "BloodCastleMixRate8_AL1", 0, path);
	this->m_BloodCastleMixRate8[2] = GetPrivateProfileInt(section, "BloodCastleMixRate8_AL2", 0, path);
	this->m_BloodCastleMixRate8[3] = GetPrivateProfileInt(section, "BloodCastleMixRate8_AL3", 0, path);
	this->m_Wing1MixRate[0] = GetPrivateProfileInt(section, "Wing1MixRate_AL0", 0, path);
	this->m_Wing1MixRate[1] = GetPrivateProfileInt(section, "Wing1MixRate_AL1", 0, path);
	this->m_Wing1MixRate[2] = GetPrivateProfileInt(section, "Wing1MixRate_AL2", 0, path);
	this->m_Wing1MixRate[3] = GetPrivateProfileInt(section, "Wing1MixRate_AL3", 0, path);
	this->m_PetMixRate[0] = GetPrivateProfileInt(section, "PetMixRate_AL0", 0, path);
	this->m_PetMixRate[1] = GetPrivateProfileInt(section, "PetMixRate_AL1", 0, path);
	this->m_PetMixRate[2] = GetPrivateProfileInt(section, "PetMixRate_AL2", 0, path);
	this->m_PetMixRate[3] = GetPrivateProfileInt(section, "PetMixRate_AL3", 0, path);
	this->m_PieceOfHornMixRate[0] = GetPrivateProfileInt(section, "PieceOfHornMixRate_AL0", 0, path);
	this->m_PieceOfHornMixRate[1] = GetPrivateProfileInt(section, "PieceOfHornMixRate_AL1", 0, path);
	this->m_PieceOfHornMixRate[2] = GetPrivateProfileInt(section, "PieceOfHornMixRate_AL2", 0, path);
	this->m_PieceOfHornMixRate[3] = GetPrivateProfileInt(section, "PieceOfHornMixRate_AL3", 0, path);
	this->m_BrokenHornMixRate[0] = GetPrivateProfileInt(section, "BrokenHornMixRate_AL0", 0, path);
	this->m_BrokenHornMixRate[1] = GetPrivateProfileInt(section, "BrokenHornMixRate_AL1", 0, path);
	this->m_BrokenHornMixRate[2] = GetPrivateProfileInt(section, "BrokenHornMixRate_AL2", 0, path);
	this->m_BrokenHornMixRate[3] = GetPrivateProfileInt(section, "BrokenHornMixRate_AL3", 0, path);
	this->m_HornOfFenrirMixRate[0] = GetPrivateProfileInt(section, "HornOfFenrirMixRate_AL0", 0, path);
	this->m_HornOfFenrirMixRate[1] = GetPrivateProfileInt(section, "HornOfFenrirMixRate_AL1", 0, path);
	this->m_HornOfFenrirMixRate[2] = GetPrivateProfileInt(section, "HornOfFenrirMixRate_AL2", 0, path);
	this->m_HornOfFenrirMixRate[3] = GetPrivateProfileInt(section, "HornOfFenrirMixRate_AL3", 0, path);
	this->m_HornOfFenrirUpgradeMixRate[0] = GetPrivateProfileInt(section, "HornOfFenrirUpgradeMixRate_AL0", 0, path);
	this->m_HornOfFenrirUpgradeMixRate[1] = GetPrivateProfileInt(section, "HornOfFenrirUpgradeMixRate_AL1", 0, path);
	this->m_HornOfFenrirUpgradeMixRate[2] = GetPrivateProfileInt(section, "HornOfFenrirUpgradeMixRate_AL2", 0, path);
	this->m_HornOfFenrirUpgradeMixRate[3] = GetPrivateProfileInt(section, "HornOfFenrirUpgradeMixRate_AL3", 0, path);
	this->m_ShieldPotionMixRate1[0] = GetPrivateProfileInt(section, "ShieldPotionMixRate1_AL0", 0, path);
	this->m_ShieldPotionMixRate1[1] = GetPrivateProfileInt(section, "ShieldPotionMixRate1_AL1", 0, path);
	this->m_ShieldPotionMixRate1[2] = GetPrivateProfileInt(section, "ShieldPotionMixRate1_AL2", 0, path);
	this->m_ShieldPotionMixRate1[3] = GetPrivateProfileInt(section, "ShieldPotionMixRate1_AL3", 0, path);
	this->m_ShieldPotionMixRate2[0] = GetPrivateProfileInt(section, "ShieldPotionMixRate2_AL0", 0, path);
	this->m_ShieldPotionMixRate2[1] = GetPrivateProfileInt(section, "ShieldPotionMixRate2_AL1", 0, path);
	this->m_ShieldPotionMixRate2[2] = GetPrivateProfileInt(section, "ShieldPotionMixRate2_AL2", 0, path);
	this->m_ShieldPotionMixRate2[3] = GetPrivateProfileInt(section, "ShieldPotionMixRate2_AL3", 0, path);
	this->m_ShieldPotionMixRate3[0] = GetPrivateProfileInt(section, "ShieldPotionMixRate3_AL0", 0, path);
	this->m_ShieldPotionMixRate3[1] = GetPrivateProfileInt(section, "ShieldPotionMixRate3_AL1", 0, path);
	this->m_ShieldPotionMixRate3[2] = GetPrivateProfileInt(section, "ShieldPotionMixRate3_AL2", 0, path);
	this->m_ShieldPotionMixRate3[3] = GetPrivateProfileInt(section, "ShieldPotionMixRate3_AL3", 0, path);
	this->m_JewelOfHarmonyItemPurityMixRate[0] = GetPrivateProfileInt(section, "JewelOfHarmonyItemPurityMixRate_AL0", 0, path);
	this->m_JewelOfHarmonyItemPurityMixRate[1] = GetPrivateProfileInt(section, "JewelOfHarmonyItemPurityMixRate_AL1", 0, path);
	this->m_JewelOfHarmonyItemPurityMixRate[2] = GetPrivateProfileInt(section, "JewelOfHarmonyItemPurityMixRate_AL2", 0, path);
	this->m_JewelOfHarmonyItemPurityMixRate[3] = GetPrivateProfileInt(section, "JewelOfHarmonyItemPurityMixRate_AL3", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[0] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate1_AL0", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[1] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate1_AL1", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[2] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate1_AL2", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[3] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate1_AL3", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[0] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate2_AL0", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[1] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate2_AL1", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[2] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate2_AL2", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[3] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate2_AL3", 0, path);
	this->m_JewelOfHarmonyItemRestoreMixRate[0] = GetPrivateProfileInt(section, "JewelOfHarmonyItemRestoreMixRate_AL0", 0, path);
	this->m_JewelOfHarmonyItemRestoreMixRate[1] = GetPrivateProfileInt(section, "JewelOfHarmonyItemRestoreMixRate_AL1", 0, path);
	this->m_JewelOfHarmonyItemRestoreMixRate[2] = GetPrivateProfileInt(section, "JewelOfHarmonyItemRestoreMixRate_AL2", 0, path);
	this->m_JewelOfHarmonyItemRestoreMixRate[3] = GetPrivateProfileInt(section, "JewelOfHarmonyItemRestoreMixRate_AL3", 0, path);
	this->m_Item380MixRate1[0] = GetPrivateProfileInt(section, "Item380MixRate1_AL0", 0, path);
	this->m_Item380MixRate1[1] = GetPrivateProfileInt(section, "Item380MixRate1_AL1", 0, path);
	this->m_Item380MixRate1[2] = GetPrivateProfileInt(section, "Item380MixRate1_AL2", 0, path);
	this->m_Item380MixRate1[3] = GetPrivateProfileInt(section, "Item380MixRate1_AL3", 0, path);
	this->m_Item380MixRate2[0] = GetPrivateProfileInt(section, "Item380MixRate2_AL0", 0, path);
	this->m_Item380MixRate2[1] = GetPrivateProfileInt(section, "Item380MixRate2_AL1", 0, path);
	this->m_Item380MixRate2[2] = GetPrivateProfileInt(section, "Item380MixRate2_AL2", 0, path);
	this->m_Item380MixRate2[3] = GetPrivateProfileInt(section, "Item380MixRate2_AL3", 0, path);
	this->m_IllusionTempleMixRate1[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate1_AL0", 0, path);
	this->m_IllusionTempleMixRate1[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate1_AL1", 0, path);
	this->m_IllusionTempleMixRate1[2] = GetPrivateProfileInt(section, "IllusionTempleMixRate1_AL2", 0, path);
	this->m_IllusionTempleMixRate1[3] = GetPrivateProfileInt(section, "IllusionTempleMixRate1_AL3", 0, path);
	this->m_IllusionTempleMixRate2[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate2_AL0", 0, path);
	this->m_IllusionTempleMixRate2[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate2_AL1", 0, path);
	this->m_IllusionTempleMixRate2[2] = GetPrivateProfileInt(section, "IllusionTempleMixRate2_AL2", 0, path);
	this->m_IllusionTempleMixRate2[3] = GetPrivateProfileInt(section, "IllusionTempleMixRate2_AL3", 0, path);
	this->m_IllusionTempleMixRate3[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate3_AL0", 0, path);
	this->m_IllusionTempleMixRate3[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate3_AL1", 0, path);
	this->m_IllusionTempleMixRate3[2] = GetPrivateProfileInt(section, "IllusionTempleMixRate3_AL2", 0, path);
	this->m_IllusionTempleMixRate3[3] = GetPrivateProfileInt(section, "IllusionTempleMixRate3_AL3", 0, path);
	this->m_IllusionTempleMixRate4[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate4_AL0", 0, path);
	this->m_IllusionTempleMixRate4[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate4_AL1", 0, path);
	this->m_IllusionTempleMixRate4[2] = GetPrivateProfileInt(section, "IllusionTempleMixRate4_AL2", 0, path);
	this->m_IllusionTempleMixRate4[3] = GetPrivateProfileInt(section, "IllusionTempleMixRate4_AL3", 0, path);
	this->m_IllusionTempleMixRate5[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate5_AL0", 0, path);
	this->m_IllusionTempleMixRate5[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate5_AL1", 0, path);
	this->m_IllusionTempleMixRate5[2] = GetPrivateProfileInt(section, "IllusionTempleMixRate5_AL2", 0, path);
	this->m_IllusionTempleMixRate5[3] = GetPrivateProfileInt(section, "IllusionTempleMixRate5_AL3", 0, path);
	this->m_IllusionTempleMixRate6[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate6_AL0", 0, path);
	this->m_IllusionTempleMixRate6[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate6_AL1", 0, path);
	this->m_IllusionTempleMixRate6[2] = GetPrivateProfileInt(section, "IllusionTempleMixRate6_AL2", 0, path);
	this->m_IllusionTempleMixRate6[3] = GetPrivateProfileInt(section, "IllusionTempleMixRate6_AL3", 0, path);
	this->m_FeatherOfCondorMixRate[0] = GetPrivateProfileInt(section, "FeatherOfCondorMixRate_AL0", 0, path);
	this->m_FeatherOfCondorMixRate[1] = GetPrivateProfileInt(section, "FeatherOfCondorMixRate_AL1", 0, path);
	this->m_FeatherOfCondorMixRate[2] = GetPrivateProfileInt(section, "FeatherOfCondorMixRate_AL2", 0, path);
	this->m_FeatherOfCondorMixRate[3] = GetPrivateProfileInt(section, "FeatherOfCondorMixRate_AL3", 0, path);
	this->m_Wing3MixRate[0] = GetPrivateProfileInt(section, "Wing3MixRate_AL0", 0, path);
	this->m_Wing3MixRate[1] = GetPrivateProfileInt(section, "Wing3MixRate_AL1", 0, path);
	this->m_Wing3MixRate[2] = GetPrivateProfileInt(section, "Wing3MixRate_AL2", 0, path);
	this->m_Wing3MixRate[3] = GetPrivateProfileInt(section, "Wing3MixRate_AL3", 0, path);
}

void CServerInfo::ReadCharacterInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	this->m_DuelDamageRate = GetPrivateProfileInt(section, "DuelDamageRate", 0, path);
	
	this->m_CustomArenaDamageRate = GetPrivateProfileInt(section, "CustomArenaDamageRate", 0, path);

	this->m_ChaosCastleDamageRate = GetPrivateProfileInt(section, "ChaosCastleDamageRate", 0, path);

	this->m_IllusionTempleDamageRate = GetPrivateProfileInt(section, "IllusionTempleDamageRate", 0, path);

	this->m_CastleSiegeDamageRate1 = GetPrivateProfileInt(section, "CastleSiegeDamageRate1", 0, path);

	this->m_CastleSiegeDamageRate2 = GetPrivateProfileInt(section, "CastleSiegeDamageRate2", 0, path);

	this->m_CastleSiegeDamageRate3 = GetPrivateProfileInt(section, "CastleSiegeDamageRate3", 0, path);

	this->m_GeneralDamageRatePvP = GetPrivateProfileInt(section, "GeneralDamageRatePvP", 0, path);

	this->m_GeneralDamageRatePvM = GetPrivateProfileInt(section, "GeneralDamageRatePvM", 0, path);

	this->m_ReflectDamageRatePvP = GetPrivateProfileInt(section, "ReflectDamageRatePvP", 0, path);

	this->m_ReflectDamageRatePvM = GetPrivateProfileInt(section, "ReflectDamageRatePvM", 0, path);

	this->m_DamageRatePvP[0] = GetPrivateProfileInt(section, "DWDamageRatePvP", 0, path);

	this->m_DamageRatePvP[1] = GetPrivateProfileInt(section, "DKDamageRatePvP", 0, path);

	this->m_DamageRatePvP[2] = GetPrivateProfileInt(section, "FEDamageRatePvP", 0, path);

	this->m_DamageRatePvP[3] = GetPrivateProfileInt(section, "MGDamageRatePvP", 0, path);

	this->m_DamageRatePvP[4] = GetPrivateProfileInt(section, "DLDamageRatePvP", 0, path);

	this->m_DamageRatePvP[5] = GetPrivateProfileInt(section, "SUDamageRatePvP", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRatePvP[6] = GetPrivateProfileInt(section, "RFDamageRatePvP", 0, path);

#endif

	this->m_DamageRatePvM[0] = GetPrivateProfileInt(section, "DWDamageRatePvM", 0, path);

	this->m_DamageRatePvM[1] = GetPrivateProfileInt(section, "DKDamageRatePvM", 0, path);

	this->m_DamageRatePvM[2] = GetPrivateProfileInt(section, "FEDamageRatePvM", 0, path);

	this->m_DamageRatePvM[3] = GetPrivateProfileInt(section, "MGDamageRatePvM", 0, path);

	this->m_DamageRatePvM[4] = GetPrivateProfileInt(section, "DLDamageRatePvM", 0, path);

	this->m_DamageRatePvM[5] = GetPrivateProfileInt(section, "SUDamageRatePvM", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRatePvM[6] = GetPrivateProfileInt(section, "RFDamageRatePvM", 0, path);

#endif

	this->m_DamageRateTo[0][0] = GetPrivateProfileInt(section, "DWDamageRateToDW", 0, path);

	this->m_DamageRateTo[0][1] = GetPrivateProfileInt(section, "DWDamageRateToDK", 0, path);

	this->m_DamageRateTo[0][2] = GetPrivateProfileInt(section, "DWDamageRateToFE", 0, path);

	this->m_DamageRateTo[0][3] = GetPrivateProfileInt(section, "DWDamageRateToMG", 0, path);

	this->m_DamageRateTo[0][4] = GetPrivateProfileInt(section, "DWDamageRateToDL", 0, path);

	this->m_DamageRateTo[0][5] = GetPrivateProfileInt(section, "DWDamageRateToSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRateTo[0][6] = GetPrivateProfileInt(section, "DWDamageRateToRF", 0, path);

#endif

	this->m_DamageRateTo[1][0] = GetPrivateProfileInt(section, "DKDamageRateToDW", 0, path);

	this->m_DamageRateTo[1][1] = GetPrivateProfileInt(section, "DKDamageRateToDK", 0, path);

	this->m_DamageRateTo[1][2] = GetPrivateProfileInt(section, "DKDamageRateToFE", 0, path);

	this->m_DamageRateTo[1][3] = GetPrivateProfileInt(section, "DKDamageRateToMG", 0, path);

	this->m_DamageRateTo[1][4] = GetPrivateProfileInt(section, "DKDamageRateToDL", 0, path);

	this->m_DamageRateTo[1][5] = GetPrivateProfileInt(section, "DKDamageRateToSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRateTo[1][6] = GetPrivateProfileInt(section, "DKDamageRateToRF", 0, path);

#endif

	this->m_DamageRateTo[2][0] = GetPrivateProfileInt(section, "FEDamageRateToDW", 0, path);

	this->m_DamageRateTo[2][1] = GetPrivateProfileInt(section, "FEDamageRateToDK", 0, path);

	this->m_DamageRateTo[2][2] = GetPrivateProfileInt(section, "FEDamageRateToFE", 0, path);

	this->m_DamageRateTo[2][3] = GetPrivateProfileInt(section, "FEDamageRateToMG", 0, path);

	this->m_DamageRateTo[2][4] = GetPrivateProfileInt(section, "FEDamageRateToDL", 0, path);

	this->m_DamageRateTo[2][5] = GetPrivateProfileInt(section, "FEDamageRateToSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRateTo[2][6] = GetPrivateProfileInt(section, "FEDamageRateToRF", 0, path);

#endif

	this->m_DamageRateTo[3][0] = GetPrivateProfileInt(section, "MGDamageRateToDW", 0, path);

	this->m_DamageRateTo[3][1] = GetPrivateProfileInt(section, "MGDamageRateToDK", 0, path);

	this->m_DamageRateTo[3][2] = GetPrivateProfileInt(section, "MGDamageRateToFE", 0, path);

	this->m_DamageRateTo[3][3] = GetPrivateProfileInt(section, "MGDamageRateToMG", 0, path);

	this->m_DamageRateTo[3][4] = GetPrivateProfileInt(section, "MGDamageRateToDL", 0, path);

	this->m_DamageRateTo[3][5] = GetPrivateProfileInt(section, "MGDamageRateToSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRateTo[3][6] = GetPrivateProfileInt(section, "MGDamageRateToRF", 0, path);

#endif

	this->m_DamageRateTo[4][0] = GetPrivateProfileInt(section, "DLDamageRateToDW", 0, path);

	this->m_DamageRateTo[4][1] = GetPrivateProfileInt(section, "DLDamageRateToDK", 0, path);

	this->m_DamageRateTo[4][2] = GetPrivateProfileInt(section, "DLDamageRateToFE", 0, path);

	this->m_DamageRateTo[4][3] = GetPrivateProfileInt(section, "DLDamageRateToMG", 0, path);

	this->m_DamageRateTo[4][4] = GetPrivateProfileInt(section, "DLDamageRateToDL", 0, path);

	this->m_DamageRateTo[4][5] = GetPrivateProfileInt(section, "DLDamageRateToSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRateTo[4][6] = GetPrivateProfileInt(section, "DLDamageRateToRF", 0, path);

#endif

	this->m_DamageRateTo[5][0] = GetPrivateProfileInt(section, "SUDamageRateToDW", 0, path);

	this->m_DamageRateTo[5][1] = GetPrivateProfileInt(section, "SUDamageRateToDK", 0, path);

	this->m_DamageRateTo[5][2] = GetPrivateProfileInt(section, "SUDamageRateToFE", 0, path);

	this->m_DamageRateTo[5][3] = GetPrivateProfileInt(section, "SUDamageRateToMG", 0, path);

	this->m_DamageRateTo[5][4] = GetPrivateProfileInt(section, "SUDamageRateToDL", 0, path);

	this->m_DamageRateTo[5][5] = GetPrivateProfileInt(section, "SUDamageRateToSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRateTo[5][6] = GetPrivateProfileInt(section, "SUDamageRateToRF", 0, path);

#endif

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageRateTo[6][0] = GetPrivateProfileInt(section, "RFDamageRateToDW", 0, path);

	this->m_DamageRateTo[6][1] = GetPrivateProfileInt(section, "RFDamageRateToDK", 0, path);

	this->m_DamageRateTo[6][2] = GetPrivateProfileInt(section, "RFDamageRateToFE", 0, path);

	this->m_DamageRateTo[6][3] = GetPrivateProfileInt(section, "RFDamageRateToMG", 0, path);

	this->m_DamageRateTo[6][4] = GetPrivateProfileInt(section, "RFDamageRateToDL", 0, path);

	this->m_DamageRateTo[6][5] = GetPrivateProfileInt(section, "RFDamageRateToSU", 0, path);

	this->m_DamageRateTo[6][6] = GetPrivateProfileInt(section, "RFDamageRateToRF", 0, path);

#endif

	this->m_DamageStuckRate[0] = GetPrivateProfileInt(section, "DWDamageStuckRate", 0, path);

	this->m_DamageStuckRate[1] = GetPrivateProfileInt(section, "DKDamageStuckRate", 0, path);

	this->m_DamageStuckRate[2] = GetPrivateProfileInt(section, "FEDamageStuckRate", 0, path);

	this->m_DamageStuckRate[3] = GetPrivateProfileInt(section, "MGDamageStuckRate", 0, path);

	this->m_DamageStuckRate[4] = GetPrivateProfileInt(section, "DLDamageStuckRate", 0, path);

	this->m_DamageStuckRate[5] = GetPrivateProfileInt(section, "SUDamageStuckRate", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_DamageStuckRate[6] = GetPrivateProfileInt(section, "RFDamageStuckRate", 0, path);

#endif

	this->m_DKDamageMultiplierConstA = GetPrivateProfileInt(section, "DKDamageMultiplierConstA", 0, path);

	this->m_DLDamageMultiplierConstA = GetPrivateProfileInt(section, "DLDamageMultiplierConstA", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_RFDamageMultiplierConstA = GetPrivateProfileInt(section, "RFDamageMultiplierConstA", 0, path);

	this->m_RFDamageMultiplierConstB = GetPrivateProfileInt(section, "RFDamageMultiplierConstB", 0, path);

	this->m_RFDamageMultiplierConstC = GetPrivateProfileInt(section, "RFDamageMultiplierConstC", 0, path);

#endif

	this->m_DKDamageMultiplierMaxRate = GetPrivateProfileInt(section, "DKDamageMultiplierMaxRate", 0, path);

	this->m_DLDamageMultiplierMaxRate = GetPrivateProfileInt(section, "DLDamageMultiplierMaxRate", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_RFDamageMultiplierMaxRate = GetPrivateProfileInt(section, "RFDamageMultiplierMaxRate", 0, path);

#endif

	this->m_DarkSpiritRangeAttackRate = GetPrivateProfileInt(section, "DarkSpiritRangeAttackRate", 0, path);

	this->m_DarkSpiritCriticalDamageRate = GetPrivateProfileInt(section, "DarkSpiritCriticalDamageRate", 0, path);

	this->m_DarkSpiritExcellentDamageRate = GetPrivateProfileInt(section, "DarkSpiritExcellentDamageRate", 0, path);

	this->m_DarkSpiritAttackDamageMinConstA = GetPrivateProfileInt(section, "DarkSpiritAttackDamageMinConstA", 0, path);

	this->m_DarkSpiritAttackDamageMinConstB = GetPrivateProfileInt(section, "DarkSpiritAttackDamageMinConstB", 0, path);

	this->m_DarkSpiritAttackDamageMinConstC = GetPrivateProfileInt(section, "DarkSpiritAttackDamageMinConstC", 0, path);

	this->m_DarkSpiritAttackDamageMaxConstA = GetPrivateProfileInt(section, "DarkSpiritAttackDamageMaxConstA", 0, path);

	this->m_DarkSpiritAttackDamageMaxConstB = GetPrivateProfileInt(section, "DarkSpiritAttackDamageMaxConstB", 0, path);

	this->m_DarkSpiritAttackDamageMaxConstC = GetPrivateProfileInt(section, "DarkSpiritAttackDamageMaxConstC", 0, path);

	this->m_DarkSpiritAttackSpeedConstA = GetPrivateProfileInt(section, "DarkSpiritAttackSpeedConstA", 0, path);

	this->m_DarkSpiritAttackSpeedConstB = GetPrivateProfileInt(section, "DarkSpiritAttackSpeedConstB", 0, path);

	this->m_DarkSpiritAttackSpeedConstC = GetPrivateProfileInt(section, "DarkSpiritAttackSpeedConstC", 0, path);

	this->m_DarkSpiritAttackSpeedConstD = GetPrivateProfileInt(section, "DarkSpiritAttackSpeedConstD", 0, path);

	this->m_DarkSpiritAttackSuccessRateConstA = GetPrivateProfileInt(section, "DarkSpiritAttackSuccessRateConstA", 0, path);

	this->m_DarkSpiritAttackSuccessRateConstB = GetPrivateProfileInt(section, "DarkSpiritAttackSuccessRateConstB", 0, path);

	this->m_DarkSpiritAttackSuccessRateConstC = GetPrivateProfileInt(section, "DarkSpiritAttackSuccessRateConstC", 0, path);

	this->m_SatanIncDamageConstA = GetPrivateProfileInt(section, "SatanIncDamageConstA", 0, path);

	this->m_DinorantIncDamageConstA = GetPrivateProfileInt(section, "DinorantIncDamageConstA", 0, path);

	this->m_BlackFenrirIncDamageConstA = GetPrivateProfileInt(section, "BlackFenrirIncDamageConstA", 0, path);

	this->m_DemonIncDamageConstA = GetPrivateProfileInt(section, "DemonIncDamageConstA", 0, path);

	this->m_SkeletonIncDamageConstA = GetPrivateProfileInt(section, "SkeletonIncDamageConstA", 0, path);

	this->m_AngelDecDamageConstA = GetPrivateProfileInt(section, "AngelDecDamageConstA", 0, path);

	this->m_DinorantDecDamageConstA = GetPrivateProfileInt(section, "DinorantDecDamageConstA", 0, path);

	this->m_DinorantDecDamageConstB = GetPrivateProfileInt(section, "DinorantDecDamageConstB", 0, path);

	this->m_DarkHorseDecDamageConstA = GetPrivateProfileInt(section, "DarkHorseDecDamageConstA", 0, path);

	this->m_DarkHorseDecDamageConstB = GetPrivateProfileInt(section, "DarkHorseDecDamageConstB", 0, path);

	this->m_BlueFenrirDecDamageConstA = GetPrivateProfileInt(section, "BlueFenrirDecDamageConstA", 0, path);

	this->m_MariaDecDamageConstA = GetPrivateProfileInt(section, "MariaDecDamageConstA", 0, path);

	this->m_ComboDamageConstA = GetPrivateProfileInt(section, "ComboDamageConstA", 0, path);

	this->m_ComboDamageConstB = GetPrivateProfileInt(section, "ComboDamageConstB", 0, path);

	this->m_EarthquakeDamageConstA = GetPrivateProfileInt(section, "EarthquakeDamageConstA", 0, path);

	this->m_EarthquakeDamageConstB = GetPrivateProfileInt(section, "EarthquakeDamageConstB", 0, path);

	this->m_EarthquakeDamageConstC = GetPrivateProfileInt(section, "EarthquakeDamageConstC", 0, path);

	this->m_ElectricSparkDamageConstA = GetPrivateProfileInt(section, "ElectricSparkDamageConstA", 0, path);

	this->m_ElectricSparkDamageConstB = GetPrivateProfileInt(section, "ElectricSparkDamageConstB", 0, path);

	this->m_DLSkillDamageConstA = GetPrivateProfileInt(section, "DLSkillDamageConstA", 0, path);

	this->m_DLSkillDamageConstB = GetPrivateProfileInt(section, "DLSkillDamageConstB", 0, path);

	this->m_NovaDamageConstA = GetPrivateProfileInt(section, "NovaDamageConstA", 0, path);

	this->m_NovaDamageConstB = GetPrivateProfileInt(section, "NovaDamageConstB", 0, path);

	this->m_NovaDamageConstC = GetPrivateProfileInt(section, "NovaDamageConstC", 0, path);

	this->m_HPRecoveryRate[0] = GetPrivateProfileInt(section, "DWHPRecoveryRate", 0, path);

	this->m_HPRecoveryRate[1] = GetPrivateProfileInt(section, "DKHPRecoveryRate", 0, path);

	this->m_HPRecoveryRate[2] = GetPrivateProfileInt(section, "FEHPRecoveryRate", 0, path);

	this->m_HPRecoveryRate[3] = GetPrivateProfileInt(section, "MGHPRecoveryRate", 0, path);

	this->m_HPRecoveryRate[4] = GetPrivateProfileInt(section, "DLHPRecoveryRate", 0, path);

	this->m_HPRecoveryRate[5] = GetPrivateProfileInt(section, "SUHPRecoveryRate", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_HPRecoveryRate[6] = GetPrivateProfileInt(section, "RFHPRecoveryRate", 0, path);

#endif

	this->m_MPRecoveryRate[0] = GetPrivateProfileInt(section, "DWMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[1] = GetPrivateProfileInt(section, "DKMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[2] = GetPrivateProfileInt(section, "FEMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[3] = GetPrivateProfileInt(section, "MGMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[4] = GetPrivateProfileInt(section, "DLMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[5] = GetPrivateProfileInt(section, "SUMPRecoveryRate", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_MPRecoveryRate[6] = GetPrivateProfileInt(section, "RFMPRecoveryRate", 0, path);

#endif

	this->m_BPRecoveryRate[0] = GetPrivateProfileInt(section, "DWBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[1] = GetPrivateProfileInt(section, "DKBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[2] = GetPrivateProfileInt(section, "FEBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[3] = GetPrivateProfileInt(section, "MGBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[4] = GetPrivateProfileInt(section, "DLBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[5] = GetPrivateProfileInt(section, "SUBPRecoveryRate", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_BPRecoveryRate[6] = GetPrivateProfileInt(section, "RFBPRecoveryRate", 0, path);

#endif

	this->m_SDRecoveryRate[0] = GetPrivateProfileInt(section, "DWSDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[1] = GetPrivateProfileInt(section, "DKSDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[2] = GetPrivateProfileInt(section, "FESDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[3] = GetPrivateProfileInt(section, "MGSDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[4] = GetPrivateProfileInt(section, "DLSDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[5] = GetPrivateProfileInt(section, "SUSDRecoveryRate", 0, path);

	this->m_DWPlasmaStormDamageConstA = GetPrivateProfileInt(section, "DWPlasmaStormDamageConstA", 0, path);

	this->m_DWPlasmaStormDamageConstB = GetPrivateProfileInt(section, "DWPlasmaStormDamageConstB", 0, path);

	this->m_DWPlasmaStormDamageConstC = GetPrivateProfileInt(section, "DWPlasmaStormDamageConstC", 0, path);

	this->m_DWPlasmaStormDamageConstD = GetPrivateProfileInt(section, "DWPlasmaStormDamageConstD", 0, path);

	this->m_DKPlasmaStormDamageConstA = GetPrivateProfileInt(section, "DKPlasmaStormDamageConstA", 0, path);

	this->m_DKPlasmaStormDamageConstB = GetPrivateProfileInt(section, "DKPlasmaStormDamageConstB", 0, path);

	this->m_DKPlasmaStormDamageConstC = GetPrivateProfileInt(section, "DKPlasmaStormDamageConstC", 0, path);

	this->m_DKPlasmaStormDamageConstD = GetPrivateProfileInt(section, "DKPlasmaStormDamageConstD", 0, path);

	this->m_FEPlasmaStormDamageConstA = GetPrivateProfileInt(section, "FEPlasmaStormDamageConstA", 0, path);

	this->m_FEPlasmaStormDamageConstB = GetPrivateProfileInt(section, "FEPlasmaStormDamageConstB", 0, path);

	this->m_FEPlasmaStormDamageConstC = GetPrivateProfileInt(section, "FEPlasmaStormDamageConstC", 0, path);

	this->m_FEPlasmaStormDamageConstD = GetPrivateProfileInt(section, "FEPlasmaStormDamageConstD", 0, path);

	this->m_MGPlasmaStormDamageConstA = GetPrivateProfileInt(section, "MGPlasmaStormDamageConstA", 0, path);

	this->m_MGPlasmaStormDamageConstB = GetPrivateProfileInt(section, "MGPlasmaStormDamageConstB", 0, path);

	this->m_MGPlasmaStormDamageConstC = GetPrivateProfileInt(section, "MGPlasmaStormDamageConstC", 0, path);

	this->m_MGPlasmaStormDamageConstD = GetPrivateProfileInt(section, "MGPlasmaStormDamageConstD", 0, path);

	this->m_DLPlasmaStormDamageConstA = GetPrivateProfileInt(section, "DLPlasmaStormDamageConstA", 0, path);

	this->m_DLPlasmaStormDamageConstB = GetPrivateProfileInt(section, "DLPlasmaStormDamageConstB", 0, path);

	this->m_DLPlasmaStormDamageConstC = GetPrivateProfileInt(section, "DLPlasmaStormDamageConstC", 0, path);

	this->m_DLPlasmaStormDamageConstD = GetPrivateProfileInt(section, "DLPlasmaStormDamageConstD", 0, path);

	this->m_DLPlasmaStormDamageConstE = GetPrivateProfileInt(section, "DLPlasmaStormDamageConstE", 0, path);

	this->m_SUPlasmaStormDamageConstA = GetPrivateProfileInt(section, "SUPlasmaStormDamageConstA", 0, path);

	this->m_SUPlasmaStormDamageConstB = GetPrivateProfileInt(section, "SUPlasmaStormDamageConstB", 0, path);

	this->m_SUPlasmaStormDamageConstC = GetPrivateProfileInt(section, "SUPlasmaStormDamageConstC", 0, path);

	this->m_SUPlasmaStormDamageConstD = GetPrivateProfileInt(section, "SUPlasmaStormDamageConstD", 0, path);

	this->m_DWPhysiDamageMinConstA = GetPrivateProfileInt(section, "DWPhysiDamageMinConstA", 0, path);

	this->m_DWPhysiDamageMaxConstA = GetPrivateProfileInt(section, "DWPhysiDamageMaxConstA", 0, path);

	this->m_DWMagicDamageMinConstA = GetPrivateProfileInt(section, "DWMagicDamageMinConstA", 0, path);

	this->m_DWMagicDamageMaxConstA = GetPrivateProfileInt(section, "DWMagicDamageMaxConstA", 0, path);

	this->m_DKPhysiDamageMinConstA = GetPrivateProfileInt(section, "DKPhysiDamageMinConstA", 0, path);

	this->m_DKPhysiDamageMaxConstA = GetPrivateProfileInt(section, "DKPhysiDamageMaxConstA", 0, path);

	this->m_DKMagicDamageMinConstA = GetPrivateProfileInt(section, "DKMagicDamageMinConstA", 0, path);

	this->m_DKMagicDamageMaxConstA = GetPrivateProfileInt(section, "DKMagicDamageMaxConstA", 0, path);

	this->m_FEPhysiDamageMinConstA = GetPrivateProfileInt(section, "FEPhysiDamageMinConstA", 0, path);

	this->m_FEPhysiDamageMaxConstA = GetPrivateProfileInt(section, "FEPhysiDamageMaxConstA", 0, path);

	this->m_FEPhysiDamageMinBowConstA = GetPrivateProfileInt(section, "FEPhysiDamageMinBowConstA", 0, path);

	this->m_FEPhysiDamageMinBowConstB = GetPrivateProfileInt(section, "FEPhysiDamageMinBowConstB", 0, path);

	this->m_FEPhysiDamageMaxBowConstA = GetPrivateProfileInt(section, "FEPhysiDamageMaxBowConstA", 0, path);

	this->m_FEPhysiDamageMaxBowConstB = GetPrivateProfileInt(section, "FEPhysiDamageMaxBowConstB", 0, path);

	this->m_FEMagicDamageMinConstA = GetPrivateProfileInt(section, "FEMagicDamageMinConstA", 0, path);

	this->m_FEMagicDamageMaxConstA = GetPrivateProfileInt(section, "FEMagicDamageMaxConstA", 0, path);

	this->m_MGPhysiDamageMinConstA = GetPrivateProfileInt(section, "MGPhysiDamageMinConstA", 0, path);

	this->m_MGPhysiDamageMinConstB = GetPrivateProfileInt(section, "MGPhysiDamageMinConstB", 0, path);

	this->m_MGPhysiDamageMaxConstA = GetPrivateProfileInt(section, "MGPhysiDamageMaxConstA", 0, path);

	this->m_MGPhysiDamageMaxConstB = GetPrivateProfileInt(section, "MGPhysiDamageMaxConstB", 0, path);

	this->m_MGMagicDamageMinConstA = GetPrivateProfileInt(section, "MGMagicDamageMinConstA", 0, path);

	this->m_MGMagicDamageMaxConstA = GetPrivateProfileInt(section, "MGMagicDamageMaxConstA", 0, path);

	this->m_DLPhysiDamageMinConstA = GetPrivateProfileInt(section, "DLPhysiDamageMinConstA", 0, path);

	this->m_DLPhysiDamageMinConstB = GetPrivateProfileInt(section, "DLPhysiDamageMinConstB", 0, path);

	this->m_DLPhysiDamageMaxConstA = GetPrivateProfileInt(section, "DLPhysiDamageMaxConstA", 0, path);

	this->m_DLPhysiDamageMaxConstB = GetPrivateProfileInt(section, "DLPhysiDamageMaxConstB", 0, path);

	this->m_DLMagicDamageMinConstA = GetPrivateProfileInt(section, "DLMagicDamageMinConstA", 0, path);

	this->m_DLMagicDamageMaxConstA = GetPrivateProfileInt(section, "DLMagicDamageMaxConstA", 0, path);

	this->m_SUPhysiDamageMinConstA = GetPrivateProfileInt(section, "SUPhysiDamageMinConstA", 0, path);

	this->m_SUPhysiDamageMaxConstA = GetPrivateProfileInt(section, "SUPhysiDamageMaxConstA", 0, path);

	this->m_SUMagicDamageMinConstA = GetPrivateProfileInt(section, "SUMagicDamageMinConstA", 0, path);

	this->m_SUMagicDamageMaxConstA = GetPrivateProfileInt(section, "SUMagicDamageMaxConstA", 0, path);

	this->m_DWAttackSuccessRateConstA = GetPrivateProfileInt(section, "DWAttackSuccessRateConstA", 0, path);

	this->m_DWAttackSuccessRateConstB = GetPrivateProfileInt(section, "DWAttackSuccessRateConstB", 0, path);

	this->m_DWAttackSuccessRateConstC = GetPrivateProfileInt(section, "DWAttackSuccessRateConstC", 0, path);

	this->m_DWAttackSuccessRateConstD = GetPrivateProfileInt(section, "DWAttackSuccessRateConstD", 0, path);

	this->m_DKAttackSuccessRateConstA = GetPrivateProfileInt(section, "DKAttackSuccessRateConstA", 0, path);

	this->m_DKAttackSuccessRateConstB = GetPrivateProfileInt(section, "DKAttackSuccessRateConstB", 0, path);

	this->m_DKAttackSuccessRateConstC = GetPrivateProfileInt(section, "DKAttackSuccessRateConstC", 0, path);

	this->m_DKAttackSuccessRateConstD = GetPrivateProfileInt(section, "DKAttackSuccessRateConstD", 0, path);

	this->m_FEAttackSuccessRateConstA = GetPrivateProfileInt(section, "FEAttackSuccessRateConstA", 0, path);

	this->m_FEAttackSuccessRateConstB = GetPrivateProfileInt(section, "FEAttackSuccessRateConstB", 0, path);

	this->m_FEAttackSuccessRateConstC = GetPrivateProfileInt(section, "FEAttackSuccessRateConstC", 0, path);

	this->m_FEAttackSuccessRateConstD = GetPrivateProfileInt(section, "FEAttackSuccessRateConstD", 0, path);

	this->m_MGAttackSuccessRateConstA = GetPrivateProfileInt(section, "MGAttackSuccessRateConstA", 0, path);

	this->m_MGAttackSuccessRateConstB = GetPrivateProfileInt(section, "MGAttackSuccessRateConstB", 0, path);

	this->m_MGAttackSuccessRateConstC = GetPrivateProfileInt(section, "MGAttackSuccessRateConstC", 0, path);

	this->m_MGAttackSuccessRateConstD = GetPrivateProfileInt(section, "MGAttackSuccessRateConstD", 0, path);

	this->m_DLAttackSuccessRateConstA = GetPrivateProfileInt(section, "DLAttackSuccessRateConstA", 0, path);

	this->m_DLAttackSuccessRateConstB = GetPrivateProfileInt(section, "DLAttackSuccessRateConstB", 0, path);

	this->m_DLAttackSuccessRateConstC = GetPrivateProfileInt(section, "DLAttackSuccessRateConstC", 0, path);

	this->m_DLAttackSuccessRateConstD = GetPrivateProfileInt(section, "DLAttackSuccessRateConstD", 0, path);

	this->m_DLAttackSuccessRateConstE = GetPrivateProfileInt(section, "DLAttackSuccessRateConstE", 0, path);

	this->m_SUAttackSuccessRateConstA = GetPrivateProfileInt(section, "SUAttackSuccessRateConstA", 0, path);

	this->m_SUAttackSuccessRateConstB = GetPrivateProfileInt(section, "SUAttackSuccessRateConstB", 0, path);

	this->m_SUAttackSuccessRateConstC = GetPrivateProfileInt(section, "SUAttackSuccessRateConstC", 0, path);

	this->m_SUAttackSuccessRateConstD = GetPrivateProfileInt(section, "SUAttackSuccessRateConstD", 0, path);

	this->m_DWAttackSuccessRatePvPConstA = GetPrivateProfileInt(section, "DWAttackSuccessRatePvPConstA", 0, path);

	this->m_DWAttackSuccessRatePvPConstB = GetPrivateProfileInt(section, "DWAttackSuccessRatePvPConstB", 0, path);

	this->m_DWAttackSuccessRatePvPConstC = GetPrivateProfileInt(section, "DWAttackSuccessRatePvPConstC", 0, path);

	this->m_DWAttackSuccessRatePvPConstD = GetPrivateProfileInt(section, "DWAttackSuccessRatePvPConstD", 0, path);

	this->m_DKAttackSuccessRatePvPConstA = GetPrivateProfileInt(section, "DKAttackSuccessRatePvPConstA", 0, path);

	this->m_DKAttackSuccessRatePvPConstB = GetPrivateProfileInt(section, "DKAttackSuccessRatePvPConstB", 0, path);

	this->m_DKAttackSuccessRatePvPConstC = GetPrivateProfileInt(section, "DKAttackSuccessRatePvPConstC", 0, path);

	this->m_DKAttackSuccessRatePvPConstD = GetPrivateProfileInt(section, "DKAttackSuccessRatePvPConstD", 0, path);

	this->m_FEAttackSuccessRatePvPConstA = GetPrivateProfileInt(section, "FEAttackSuccessRatePvPConstA", 0, path);

	this->m_FEAttackSuccessRatePvPConstB = GetPrivateProfileInt(section, "FEAttackSuccessRatePvPConstB", 0, path);

	this->m_FEAttackSuccessRatePvPConstC = GetPrivateProfileInt(section, "FEAttackSuccessRatePvPConstC", 0, path);

	this->m_FEAttackSuccessRatePvPConstD = GetPrivateProfileInt(section, "FEAttackSuccessRatePvPConstD", 0, path);

	this->m_MGAttackSuccessRatePvPConstA = GetPrivateProfileInt(section, "MGAttackSuccessRatePvPConstA", 0, path);

	this->m_MGAttackSuccessRatePvPConstB = GetPrivateProfileInt(section, "MGAttackSuccessRatePvPConstB", 0, path);

	this->m_MGAttackSuccessRatePvPConstC = GetPrivateProfileInt(section, "MGAttackSuccessRatePvPConstC", 0, path);

	this->m_MGAttackSuccessRatePvPConstD = GetPrivateProfileInt(section, "MGAttackSuccessRatePvPConstD", 0, path);

	this->m_DLAttackSuccessRatePvPConstA = GetPrivateProfileInt(section, "DLAttackSuccessRatePvPConstA", 0, path);

	this->m_DLAttackSuccessRatePvPConstB = GetPrivateProfileInt(section, "DLAttackSuccessRatePvPConstB", 0, path);

	this->m_DLAttackSuccessRatePvPConstC = GetPrivateProfileInt(section, "DLAttackSuccessRatePvPConstC", 0, path);

	this->m_DLAttackSuccessRatePvPConstD = GetPrivateProfileInt(section, "DLAttackSuccessRatePvPConstD", 0, path);

	this->m_SUAttackSuccessRatePvPConstA = GetPrivateProfileInt(section, "SUAttackSuccessRatePvPConstA", 0, path);

	this->m_SUAttackSuccessRatePvPConstB = GetPrivateProfileInt(section, "SUAttackSuccessRatePvPConstB", 0, path);

	this->m_SUAttackSuccessRatePvPConstC = GetPrivateProfileInt(section, "SUAttackSuccessRatePvPConstC", 0, path);

	this->m_SUAttackSuccessRatePvPConstD = GetPrivateProfileInt(section, "SUAttackSuccessRatePvPConstD", 0, path);

	this->m_DWPhysiSpeedConstA = GetPrivateProfileInt(section, "DWPhysiSpeedConstA", 0, path);

	this->m_DWMagicSpeedConstA = GetPrivateProfileInt(section, "DWMagicSpeedConstA", 0, path);

	this->m_DKPhysiSpeedConstA = GetPrivateProfileInt(section, "DKPhysiSpeedConstA", 0, path);

	this->m_DKMagicSpeedConstA = GetPrivateProfileInt(section, "DKMagicSpeedConstA", 0, path);

	this->m_FEPhysiSpeedConstA = GetPrivateProfileInt(section, "FEPhysiSpeedConstA", 0, path);

	this->m_FEMagicSpeedConstA = GetPrivateProfileInt(section, "FEMagicSpeedConstA", 0, path);

	this->m_MGPhysiSpeedConstA = GetPrivateProfileInt(section, "MGPhysiSpeedConstA", 0, path);

	this->m_MGMagicSpeedConstA = GetPrivateProfileInt(section, "MGMagicSpeedConstA", 0, path);

	this->m_DLPhysiSpeedConstA = GetPrivateProfileInt(section, "DLPhysiSpeedConstA", 0, path);

	this->m_DLMagicSpeedConstA = GetPrivateProfileInt(section, "DLMagicSpeedConstA", 0, path);

	this->m_SUPhysiSpeedConstA = GetPrivateProfileInt(section, "SUPhysiSpeedConstA", 0, path);

	this->m_SUMagicSpeedConstA = GetPrivateProfileInt(section, "SUMagicSpeedConstA", 0, path);

	this->m_DWDefenseSuccessRateConstA = GetPrivateProfileInt(section, "DWDefenseSuccessRateConstA", 0, path);

	this->m_DKDefenseSuccessRateConstA = GetPrivateProfileInt(section, "DKDefenseSuccessRateConstA", 0, path);

	this->m_FEDefenseSuccessRateConstA = GetPrivateProfileInt(section, "FEDefenseSuccessRateConstA", 0, path);

	this->m_MGDefenseSuccessRateConstA = GetPrivateProfileInt(section, "MGDefenseSuccessRateConstA", 0, path);

	this->m_DLDefenseSuccessRateConstA = GetPrivateProfileInt(section, "DLDefenseSuccessRateConstA", 0, path);

	this->m_SUDefenseSuccessRateConstA = GetPrivateProfileInt(section, "SUDefenseSuccessRateConstA", 0, path);
	
	this->m_DWDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section, "DWDefenseSuccessRatePvPConstA", 0, path);

	this->m_DWDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section, "DWDefenseSuccessRatePvPConstB", 0, path);

	this->m_DWDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section, "DWDefenseSuccessRatePvPConstC", 0, path);

	this->m_DKDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section, "DKDefenseSuccessRatePvPConstA", 0, path);

	this->m_DKDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section, "DKDefenseSuccessRatePvPConstB", 0, path);

	this->m_DKDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section, "DKDefenseSuccessRatePvPConstC", 0, path);

	this->m_FEDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section, "FEDefenseSuccessRatePvPConstA", 0, path);

	this->m_FEDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section, "FEDefenseSuccessRatePvPConstB", 0, path);

	this->m_FEDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section, "FEDefenseSuccessRatePvPConstC", 0, path);

	this->m_MGDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section, "MGDefenseSuccessRatePvPConstA", 0, path);

	this->m_MGDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section, "MGDefenseSuccessRatePvPConstB", 0, path);

	this->m_MGDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section, "MGDefenseSuccessRatePvPConstC", 0, path);

	this->m_DLDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section, "DLDefenseSuccessRatePvPConstA", 0, path);

	this->m_DLDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section, "DLDefenseSuccessRatePvPConstB", 0, path);

	this->m_DLDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section, "DLDefenseSuccessRatePvPConstC", 0, path);

	this->m_SUDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section, "SUDefenseSuccessRatePvPConstA", 0, path);

	this->m_SUDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section, "SUDefenseSuccessRatePvPConstB", 0, path);

	this->m_SUDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section, "SUDefenseSuccessRatePvPConstC", 0, path);
	
	this->m_DWDefenseConstA = GetPrivateProfileInt(section, "DWDefenseConstA", 0, path);

	this->m_DKDefenseConstA = GetPrivateProfileInt(section, "DKDefenseConstA", 0, path);

	this->m_FEDefenseConstA = GetPrivateProfileInt(section, "FEDefenseConstA", 0, path);

	this->m_MGDefenseConstA = GetPrivateProfileInt(section, "MGDefenseConstA", 0, path);

	this->m_DLDefenseConstA = GetPrivateProfileInt(section, "DLDefenseConstA", 0, path);

	this->m_SUDefenseConstA = GetPrivateProfileInt(section, "SUDefenseConstA", 0, path);
}

void CServerInfo::ReadCommandInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	this->m_CommandPostSwitch = GetPrivateProfileInt(section, "CommandPostSwitch", 0, path);

	this->m_CommandPostType = GetPrivateProfileInt(section, "CommandPostType", 0, path);

	this->m_CommandPostEnable[0] = GetPrivateProfileInt(section, "CommandPostEnable_AL0", 0, path);

	this->m_CommandPostEnable[1] = GetPrivateProfileInt(section, "CommandPostEnable_AL1", 0, path);

	this->m_CommandPostEnable[2] = GetPrivateProfileInt(section, "CommandPostEnable_AL2", 0, path);

	this->m_CommandPostEnable[3] = GetPrivateProfileInt(section, "CommandPostEnable_AL3", 0, path);

	this->m_CommandPostLevel[0] = GetPrivateProfileInt(section, "CommandPostLevel_AL0", 0, path);

	this->m_CommandPostLevel[1] = GetPrivateProfileInt(section, "CommandPostLevel_AL1", 0, path);

	this->m_CommandPostLevel[2] = GetPrivateProfileInt(section, "CommandPostLevel_AL2", 0, path);

	this->m_CommandPostLevel[3] = GetPrivateProfileInt(section, "CommandPostLevel_AL3", 0, path);

	this->m_CommandPostReset[0] = GetPrivateProfileInt(section, "CommandPostReset_AL0", 0, path);

	this->m_CommandPostReset[1] = GetPrivateProfileInt(section, "CommandPostReset_AL1", 0, path);

	this->m_CommandPostReset[2] = GetPrivateProfileInt(section, "CommandPostReset_AL2", 0, path);

	this->m_CommandPostReset[3] = GetPrivateProfileInt(section, "CommandPostReset_AL3", 0, path);

	this->m_CommandPostMoney[0] = GetPrivateProfileInt(section, "CommandPostMoney_AL0", 0, path);

	this->m_CommandPostMoney[1] = GetPrivateProfileInt(section, "CommandPostMoney_AL1", 0, path);

	this->m_CommandPostMoney[2] = GetPrivateProfileInt(section, "CommandPostMoney_AL2", 0, path);

	this->m_CommandPostMoney[3] = GetPrivateProfileInt(section, "CommandPostMoney_AL3", 0, path);

	this->m_CommandPostDelay[0] = GetPrivateProfileInt(section, "CommandPostDelay_AL0", 0, path);

	this->m_CommandPostDelay[1] = GetPrivateProfileInt(section, "CommandPostDelay_AL1", 0, path);

	this->m_CommandPostDelay[2] = GetPrivateProfileInt(section, "CommandPostDelay_AL2", 0, path);

	this->m_CommandPostDelay[3] = GetPrivateProfileInt(section, "CommandPostDelay_AL3", 0, path);

	this->m_CommandAddPointSwitch = GetPrivateProfileInt(section, "CommandAddPointSwitch", 0, path);

	this->m_CommandAddPointEnable[0] = GetPrivateProfileInt(section, "CommandAddPointEnable_AL0", 0, path);

	this->m_CommandAddPointEnable[1] = GetPrivateProfileInt(section, "CommandAddPointEnable_AL1", 0, path);

	this->m_CommandAddPointEnable[2] = GetPrivateProfileInt(section, "CommandAddPointEnable_AL2", 0, path);

	this->m_CommandAddPointEnable[3] = GetPrivateProfileInt(section, "CommandAddPointEnable_AL3", 0, path);

	this->m_CommandAddPointAutoEnable[0] = GetPrivateProfileInt(section, "CommandAddPointAutoEnable_AL0", 0, path);

	this->m_CommandAddPointAutoEnable[1] = GetPrivateProfileInt(section, "CommandAddPointAutoEnable_AL1", 0, path);

	this->m_CommandAddPointAutoEnable[2] = GetPrivateProfileInt(section, "CommandAddPointAutoEnable_AL2", 0, path);

	this->m_CommandAddPointAutoEnable[3] = GetPrivateProfileInt(section, "CommandAddPointAutoEnable_AL3", 0, path);

	this->m_CommandAddPointMoney[0] = GetPrivateProfileInt(section, "CommandAddPointMoney_AL0", 0, path);

	this->m_CommandAddPointMoney[1] = GetPrivateProfileInt(section, "CommandAddPointMoney_AL1", 0, path);

	this->m_CommandAddPointMoney[2] = GetPrivateProfileInt(section, "CommandAddPointMoney_AL2", 0, path);

	this->m_CommandAddPointMoney[3] = GetPrivateProfileInt(section, "CommandAddPointMoney_AL3", 0, path);

	this->m_CommandPKClearSwitch = GetPrivateProfileInt(section, "CommandPKClearSwitch", 0, path);

	this->m_CommandPKClearEnable[0] = GetPrivateProfileInt(section, "CommandPKClearEnable_AL0", 0, path);

	this->m_CommandPKClearEnable[1] = GetPrivateProfileInt(section, "CommandPKClearEnable_AL1", 0, path);

	this->m_CommandPKClearEnable[2] = GetPrivateProfileInt(section, "CommandPKClearEnable_AL2", 0, path);

	this->m_CommandPKClearEnable[3] = GetPrivateProfileInt(section, "CommandPKClearEnable_AL3", 0, path);

	this->m_CommandPKClearMoney[0] = GetPrivateProfileInt(section, "CommandPKClearMoney_AL0", 0, path);

	this->m_CommandPKClearMoney[1] = GetPrivateProfileInt(section, "CommandPKClearMoney_AL1", 0, path);

	this->m_CommandPKClearMoney[2] = GetPrivateProfileInt(section, "CommandPKClearMoney_AL2", 0, path);

	this->m_CommandPKClearMoney[3] = GetPrivateProfileInt(section, "CommandPKClearMoney_AL3", 0, path);

	this->m_CommandMoneySwitch = GetPrivateProfileInt(section, "CommandMoneySwitch", 0, path);

	this->m_CommandMoneyEnable[0] = GetPrivateProfileInt(section, "CommandMoneyEnable_AL0", 0, path);

	this->m_CommandMoneyEnable[1] = GetPrivateProfileInt(section, "CommandMoneyEnable_AL1", 0, path);

	this->m_CommandMoneyEnable[2] = GetPrivateProfileInt(section, "CommandMoneyEnable_AL2", 0, path);

	this->m_CommandMoneyEnable[3] = GetPrivateProfileInt(section, "CommandMoneyEnable_AL3", 0, path);

	this->m_CommandChangeSwitch = GetPrivateProfileInt(section, "CommandChangeSwitch", 0, path);

	this->m_CommandChangeEnable[0] = GetPrivateProfileInt(section, "CommandChangeEnable_AL0", 0, path);

	this->m_CommandChangeEnable[1] = GetPrivateProfileInt(section, "CommandChangeEnable_AL1", 0, path);

	this->m_CommandChangeEnable[2] = GetPrivateProfileInt(section, "CommandChangeEnable_AL2", 0, path);

	this->m_CommandChangeEnable[3] = GetPrivateProfileInt(section, "CommandChangeEnable_AL3", 0, path);

	this->m_CommandChangeMoney[0] = GetPrivateProfileInt(section, "CommandChangeMoney_AL0", 0, path);

	this->m_CommandChangeMoney[1] = GetPrivateProfileInt(section, "CommandChangeMoney_AL1", 0, path);

	this->m_CommandChangeMoney[2] = GetPrivateProfileInt(section, "CommandChangeMoney_AL2", 0, path);

	this->m_CommandChangeMoney[3] = GetPrivateProfileInt(section, "CommandChangeMoney_AL3", 0, path);

	this->m_CommandChangeLimit[0] = GetPrivateProfileInt(section, "CommandChangeLimit_AL0", 0, path);

	this->m_CommandChangeLimit[1] = GetPrivateProfileInt(section, "CommandChangeLimit_AL1", 0, path);

	this->m_CommandChangeLimit[2] = GetPrivateProfileInt(section, "CommandChangeLimit_AL2", 0, path);

	this->m_CommandChangeLimit[3] = GetPrivateProfileInt(section, "CommandChangeLimit_AL3", 0, path);

	this->m_CommandWareSwitch = GetPrivateProfileInt(section, "CommandWareSwitch", 0, path);

	this->m_CommandWareEnable[0] = GetPrivateProfileInt(section, "CommandWareEnable_AL0", 0, path);

	this->m_CommandWareEnable[1] = GetPrivateProfileInt(section, "CommandWareEnable_AL1", 0, path);

	this->m_CommandWareEnable[2] = GetPrivateProfileInt(section, "CommandWareEnable_AL2", 0, path);

	this->m_CommandWareEnable[3] = GetPrivateProfileInt(section, "CommandWareEnable_AL3", 0, path);

	this->m_CommandWareNumber[0] = GetPrivateProfileInt(section, "CommandWareNumber_AL0", 0, path);

	this->m_CommandWareNumber[1] = GetPrivateProfileInt(section, "CommandWareNumber_AL1", 0, path);

	this->m_CommandWareNumber[2] = GetPrivateProfileInt(section, "CommandWareNumber_AL2", 0, path);

	this->m_CommandWareNumber[3] = GetPrivateProfileInt(section, "CommandWareNumber_AL3", 0, path);

	this->m_CommandResetSwitch = GetPrivateProfileInt(section, "CommandResetSwitch", 0, path);

	this->m_CommandResetType = GetPrivateProfileInt(section, "CommandResetType", 0, path);

	this->m_CommandResetEnable[0] = GetPrivateProfileInt(section, "CommandResetEnable_AL0", 0, path);

	this->m_CommandResetEnable[1] = GetPrivateProfileInt(section, "CommandResetEnable_AL1", 0, path);

	this->m_CommandResetEnable[2] = GetPrivateProfileInt(section, "CommandResetEnable_AL2", 0, path);

	this->m_CommandResetEnable[3] = GetPrivateProfileInt(section, "CommandResetEnable_AL3", 0, path);

	this->m_CommandResetAutoEnable[0] = GetPrivateProfileInt(section, "CommandResetAutoEnable_AL0", 0, path);

	this->m_CommandResetAutoEnable[1] = GetPrivateProfileInt(section, "CommandResetAutoEnable_AL1", 0, path);

	this->m_CommandResetAutoEnable[2] = GetPrivateProfileInt(section, "CommandResetAutoEnable_AL2", 0, path);

	this->m_CommandResetAutoEnable[3] = GetPrivateProfileInt(section, "CommandResetAutoEnable_AL3", 0, path);

	this->m_CommandResetCheckItem[0] = GetPrivateProfileInt(section, "CommandResetCheckItem_AL0", 0, path);

	this->m_CommandResetCheckItem[1] = GetPrivateProfileInt(section, "CommandResetCheckItem_AL1", 0, path);

	this->m_CommandResetCheckItem[2] = GetPrivateProfileInt(section, "CommandResetCheckItem_AL2", 0, path);

	this->m_CommandResetCheckItem[3] = GetPrivateProfileInt(section, "CommandResetCheckItem_AL3", 0, path);

	this->m_CommandResetMove[0] = GetPrivateProfileInt(section, "CommandResetMove_AL0", 0, path);

	this->m_CommandResetMove[1] = GetPrivateProfileInt(section, "CommandResetMove_AL1", 0, path);

	this->m_CommandResetMove[2] = GetPrivateProfileInt(section, "CommandResetMove_AL2", 0, path);

	this->m_CommandResetMove[3] = GetPrivateProfileInt(section, "CommandResetMove_AL3", 0, path);

	this->m_CommandResetQuest[0] = GetPrivateProfileInt(section, "CommandResetQuest_AL0", 0, path);

	this->m_CommandResetQuest[1] = GetPrivateProfileInt(section, "CommandResetQuest_AL1", 0, path);

	this->m_CommandResetQuest[2] = GetPrivateProfileInt(section, "CommandResetQuest_AL2", 0, path);

	this->m_CommandResetQuest[3] = GetPrivateProfileInt(section, "CommandResetQuest_AL3", 0, path);

	this->m_CommandResetSkill[0] = GetPrivateProfileInt(section, "CommandResetSkill_AL0", 0, path);

	this->m_CommandResetSkill[1] = GetPrivateProfileInt(section, "CommandResetSkill_AL1", 0, path);

	this->m_CommandResetSkill[2] = GetPrivateProfileInt(section, "CommandResetSkill_AL2", 0, path);

	this->m_CommandResetSkill[3] = GetPrivateProfileInt(section, "CommandResetSkill_AL3", 0, path);

	this->m_CommandResetLevel[0] = GetPrivateProfileInt(section, "CommandResetLevel_AL0", 0, path);

	this->m_CommandResetLevel[1] = GetPrivateProfileInt(section, "CommandResetLevel_AL1", 0, path);

	this->m_CommandResetLevel[2] = GetPrivateProfileInt(section, "CommandResetLevel_AL2", 0, path);

	this->m_CommandResetLevel[3] = GetPrivateProfileInt(section, "CommandResetLevel_AL3", 0, path);

	this->m_CommandResetMoney[0] = GetPrivateProfileInt(section, "CommandResetMoney_AL0", 0, path);

	this->m_CommandResetMoney[1] = GetPrivateProfileInt(section, "CommandResetMoney_AL1", 0, path);

	this->m_CommandResetMoney[2] = GetPrivateProfileInt(section, "CommandResetMoney_AL2", 0, path);

	this->m_CommandResetMoney[3] = GetPrivateProfileInt(section, "CommandResetMoney_AL3", 0, path);

	this->m_CommandResetCount[0] = GetPrivateProfileInt(section, "CommandResetCount_AL0", 0, path);

	this->m_CommandResetCount[1] = GetPrivateProfileInt(section, "CommandResetCount_AL1", 0, path);

	this->m_CommandResetCount[2] = GetPrivateProfileInt(section, "CommandResetCount_AL2", 0, path);

	this->m_CommandResetCount[3] = GetPrivateProfileInt(section, "CommandResetCount_AL3", 0, path);

	this->m_CommandResetLimit[0] = GetPrivateProfileInt(section, "CommandResetLimit_AL0", 0, path);

	this->m_CommandResetLimit[1] = GetPrivateProfileInt(section, "CommandResetLimit_AL1", 0, path);

	this->m_CommandResetLimit[2] = GetPrivateProfileInt(section, "CommandResetLimit_AL2", 0, path);

	this->m_CommandResetLimit[3] = GetPrivateProfileInt(section, "CommandResetLimit_AL3", 0, path);

	this->m_CommandResetLimitDay[0] = GetPrivateProfileInt(section, "CommandResetLimitDay_AL0", 0, path);

	this->m_CommandResetLimitDay[1] = GetPrivateProfileInt(section, "CommandResetLimitDay_AL1", 0, path);

	this->m_CommandResetLimitDay[2] = GetPrivateProfileInt(section, "CommandResetLimitDay_AL2", 0, path);

	this->m_CommandResetLimitDay[3] = GetPrivateProfileInt(section, "CommandResetLimitDay_AL3", 0, path);

	this->m_CommandResetLimitWek[0] = GetPrivateProfileInt(section, "CommandResetLimitWek_AL0", 0, path);

	this->m_CommandResetLimitWek[1] = GetPrivateProfileInt(section, "CommandResetLimitWek_AL1", 0, path);

	this->m_CommandResetLimitWek[2] = GetPrivateProfileInt(section, "CommandResetLimitWek_AL2", 0, path);

	this->m_CommandResetLimitWek[3] = GetPrivateProfileInt(section, "CommandResetLimitWek_AL3", 0, path);

	this->m_CommandResetLimitMon[0] = GetPrivateProfileInt(section, "CommandResetLimitMon_AL0", 0, path);

	this->m_CommandResetLimitMon[1] = GetPrivateProfileInt(section, "CommandResetLimitMon_AL1", 0, path);

	this->m_CommandResetLimitMon[2] = GetPrivateProfileInt(section, "CommandResetLimitMon_AL2", 0, path);

	this->m_CommandResetLimitMon[3] = GetPrivateProfileInt(section, "CommandResetLimitMon_AL3", 0, path);

	this->m_CommandResetStartLevel[0] = GetPrivateProfileInt(section, "CommandResetStartLevel_AL0", 0, path);

	this->m_CommandResetStartLevel[1] = GetPrivateProfileInt(section, "CommandResetStartLevel_AL1", 0, path);

	this->m_CommandResetStartLevel[2] = GetPrivateProfileInt(section, "CommandResetStartLevel_AL2", 0, path);

	this->m_CommandResetStartLevel[3] = GetPrivateProfileInt(section, "CommandResetStartLevel_AL3", 0, path);

	this->m_CommandResetPoint[0] = GetPrivateProfileInt(section, "CommandResetPoint_AL0", 0, path);

	this->m_CommandResetPoint[1] = GetPrivateProfileInt(section, "CommandResetPoint_AL1", 0, path);

	this->m_CommandResetPoint[2] = GetPrivateProfileInt(section, "CommandResetPoint_AL2", 0, path);

	this->m_CommandResetPoint[3] = GetPrivateProfileInt(section, "CommandResetPoint_AL3", 0, path);

	this->m_CommandResetPointRate[0] = GetPrivateProfileInt(section, "CommandResetPointRateDW", 0, path);

	this->m_CommandResetPointRate[1] = GetPrivateProfileInt(section, "CommandResetPointRateDK", 0, path);

	this->m_CommandResetPointRate[2] = GetPrivateProfileInt(section, "CommandResetPointRateFE", 0, path);

	this->m_CommandResetPointRate[3] = GetPrivateProfileInt(section, "CommandResetPointRateMG", 0, path);

	this->m_CommandResetPointRate[4] = GetPrivateProfileInt(section, "CommandResetPointRateDL", 0, path);

	this->m_CommandResetPointRate[5] = GetPrivateProfileInt(section, "CommandResetPointRateSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_CommandResetPointRate[6] = GetPrivateProfileInt(section, "CommandResetPointRateRF", 0, path);

#endif

	this->m_CommandMasterResetSwitch = GetPrivateProfileInt(section, "CommandMasterResetSwitch", 0, path);

	this->m_CommandMasterResetType = GetPrivateProfileInt(section, "CommandMasterResetType", 0, path);

	this->m_CommandMasterResetEnable[0] = GetPrivateProfileInt(section, "CommandMasterResetEnable_AL0", 0, path);

	this->m_CommandMasterResetEnable[1] = GetPrivateProfileInt(section, "CommandMasterResetEnable_AL1", 0, path);

	this->m_CommandMasterResetEnable[2] = GetPrivateProfileInt(section, "CommandMasterResetEnable_AL2", 0, path);

	this->m_CommandMasterResetEnable[3] = GetPrivateProfileInt(section, "CommandMasterResetEnable_AL3", 0, path);

	this->m_CommandMasterResetCheckItem[0] = GetPrivateProfileInt(section, "CommandMasterResetCheckItem_AL0", 0, path);

	this->m_CommandMasterResetCheckItem[1] = GetPrivateProfileInt(section, "CommandMasterResetCheckItem_AL1", 0, path);

	this->m_CommandMasterResetCheckItem[2] = GetPrivateProfileInt(section, "CommandMasterResetCheckItem_AL2", 0, path);

	this->m_CommandMasterResetCheckItem[3] = GetPrivateProfileInt(section, "CommandMasterResetCheckItem_AL3", 0, path);

	this->m_CommandMasterResetMove[0] = GetPrivateProfileInt(section, "CommandMasterResetMove_AL0", 0, path);

	this->m_CommandMasterResetMove[1] = GetPrivateProfileInt(section, "CommandMasterResetMove_AL1", 0, path);

	this->m_CommandMasterResetMove[2] = GetPrivateProfileInt(section, "CommandMasterResetMove_AL2", 0, path);

	this->m_CommandMasterResetMove[3] = GetPrivateProfileInt(section, "CommandMasterResetMove_AL3", 0, path);

	this->m_CommandMasterResetQuest[0] = GetPrivateProfileInt(section, "CommandMasterResetQuest_AL0", 0, path);

	this->m_CommandMasterResetQuest[1] = GetPrivateProfileInt(section, "CommandMasterResetQuest_AL1", 0, path);

	this->m_CommandMasterResetQuest[2] = GetPrivateProfileInt(section, "CommandMasterResetQuest_AL2", 0, path);

	this->m_CommandMasterResetQuest[3] = GetPrivateProfileInt(section, "CommandMasterResetQuest_AL3", 0, path);

	this->m_CommandMasterResetSkill[0] = GetPrivateProfileInt(section, "CommandMasterResetSkill_AL0", 0, path);

	this->m_CommandMasterResetSkill[1] = GetPrivateProfileInt(section, "CommandMasterResetSkill_AL1", 0, path);

	this->m_CommandMasterResetSkill[2] = GetPrivateProfileInt(section, "CommandMasterResetSkill_AL2", 0, path);

	this->m_CommandMasterResetSkill[3] = GetPrivateProfileInt(section, "CommandMasterResetSkill_AL3", 0, path);

	this->m_CommandMasterResetLevel[0] = GetPrivateProfileInt(section, "CommandMasterResetLevel_AL0", 0, path);

	this->m_CommandMasterResetLevel[1] = GetPrivateProfileInt(section, "CommandMasterResetLevel_AL1", 0, path);

	this->m_CommandMasterResetLevel[2] = GetPrivateProfileInt(section, "CommandMasterResetLevel_AL2", 0, path);

	this->m_CommandMasterResetLevel[3] = GetPrivateProfileInt(section, "CommandMasterResetLevel_AL3", 0, path);

	this->m_CommandMasterResetReset[0] = GetPrivateProfileInt(section, "CommandMasterResetReset_AL0", 0, path);

	this->m_CommandMasterResetReset[1] = GetPrivateProfileInt(section, "CommandMasterResetReset_AL1", 0, path);

	this->m_CommandMasterResetReset[2] = GetPrivateProfileInt(section, "CommandMasterResetReset_AL2", 0, path);

	this->m_CommandMasterResetReset[3] = GetPrivateProfileInt(section, "CommandMasterResetReset_AL3", 0, path);

	this->m_CommandMasterResetMoney[0] = GetPrivateProfileInt(section, "CommandMasterResetMoney_AL0", 0, path);

	this->m_CommandMasterResetMoney[1] = GetPrivateProfileInt(section, "CommandMasterResetMoney_AL1", 0, path);

	this->m_CommandMasterResetMoney[2] = GetPrivateProfileInt(section, "CommandMasterResetMoney_AL2", 0, path);

	this->m_CommandMasterResetMoney[3] = GetPrivateProfileInt(section, "CommandMasterResetMoney_AL3", 0, path);

	this->m_CommandMasterResetCount[0] = GetPrivateProfileInt(section, "CommandMasterResetCount_AL0", 0, path);

	this->m_CommandMasterResetCount[1] = GetPrivateProfileInt(section, "CommandMasterResetCount_AL1", 0, path);

	this->m_CommandMasterResetCount[2] = GetPrivateProfileInt(section, "CommandMasterResetCount_AL2", 0, path);

	this->m_CommandMasterResetCount[3] = GetPrivateProfileInt(section, "CommandMasterResetCount_AL3", 0, path);

	this->m_CommandMasterResetLimit[0] = GetPrivateProfileInt(section, "CommandMasterResetLimit_AL0", 0, path);

	this->m_CommandMasterResetLimit[1] = GetPrivateProfileInt(section, "CommandMasterResetLimit_AL1", 0, path);

	this->m_CommandMasterResetLimit[2] = GetPrivateProfileInt(section, "CommandMasterResetLimit_AL2", 0, path);

	this->m_CommandMasterResetLimit[3] = GetPrivateProfileInt(section, "CommandMasterResetLimit_AL3", 0, path);

	this->m_CommandMasterResetLimitDay[0] = GetPrivateProfileInt(section, "CommandMasterResetLimitDay_AL0", 0, path);

	this->m_CommandMasterResetLimitDay[1] = GetPrivateProfileInt(section, "CommandMasterResetLimitDay_AL1", 0, path);

	this->m_CommandMasterResetLimitDay[2] = GetPrivateProfileInt(section, "CommandMasterResetLimitDay_AL2", 0, path);

	this->m_CommandMasterResetLimitDay[3] = GetPrivateProfileInt(section, "CommandMasterResetLimitDay_AL3", 0, path);

	this->m_CommandMasterResetLimitWek[0] = GetPrivateProfileInt(section, "CommandMasterResetLimitWek_AL0", 0, path);

	this->m_CommandMasterResetLimitWek[1] = GetPrivateProfileInt(section, "CommandMasterResetLimitWek_AL1", 0, path);

	this->m_CommandMasterResetLimitWek[2] = GetPrivateProfileInt(section, "CommandMasterResetLimitWek_AL2", 0, path);

	this->m_CommandMasterResetLimitWek[3] = GetPrivateProfileInt(section, "CommandMasterResetLimitWek_AL3", 0, path);

	this->m_CommandMasterResetLimitMon[0] = GetPrivateProfileInt(section, "CommandMasterResetLimitMon_AL0", 0, path);

	this->m_CommandMasterResetLimitMon[1] = GetPrivateProfileInt(section, "CommandMasterResetLimitMon_AL1", 0, path);

	this->m_CommandMasterResetLimitMon[2] = GetPrivateProfileInt(section, "CommandMasterResetLimitMon_AL2", 0, path);

	this->m_CommandMasterResetLimitMon[3] = GetPrivateProfileInt(section, "CommandMasterResetLimitMon_AL3", 0, path);

	this->m_CommandMasterResetStartLevel[0] = GetPrivateProfileInt(section, "CommandMasterResetStartLevel_AL0", 0, path);

	this->m_CommandMasterResetStartLevel[1] = GetPrivateProfileInt(section, "CommandMasterResetStartLevel_AL1", 0, path);

	this->m_CommandMasterResetStartLevel[2] = GetPrivateProfileInt(section, "CommandMasterResetStartLevel_AL2", 0, path);

	this->m_CommandMasterResetStartLevel[3] = GetPrivateProfileInt(section, "CommandMasterResetStartLevel_AL3", 0, path);

	this->m_CommandMasterResetStartReset[0] = GetPrivateProfileInt(section, "CommandMasterResetStartReset_AL0", 0, path);

	this->m_CommandMasterResetStartReset[1] = GetPrivateProfileInt(section, "CommandMasterResetStartReset_AL1", 0, path);

	this->m_CommandMasterResetStartReset[2] = GetPrivateProfileInt(section, "CommandMasterResetStartReset_AL2", 0, path);

	this->m_CommandMasterResetStartReset[3] = GetPrivateProfileInt(section, "CommandMasterResetStartReset_AL3", 0, path);

	this->m_CommandMasterResetPoint[0] = GetPrivateProfileInt(section, "CommandMasterResetPoint_AL0", 0, path);

	this->m_CommandMasterResetPoint[1] = GetPrivateProfileInt(section, "CommandMasterResetPoint_AL1", 0, path);

	this->m_CommandMasterResetPoint[2] = GetPrivateProfileInt(section, "CommandMasterResetPoint_AL2", 0, path);

	this->m_CommandMasterResetPoint[3] = GetPrivateProfileInt(section, "CommandMasterResetPoint_AL3", 0, path);

	this->m_CommandMasterResetPointRate[0] = GetPrivateProfileInt(section, "CommandMasterResetPointRateDW", 0, path);

	this->m_CommandMasterResetPointRate[1] = GetPrivateProfileInt(section, "CommandMasterResetPointRateDK", 0, path);

	this->m_CommandMasterResetPointRate[2] = GetPrivateProfileInt(section, "CommandMasterResetPointRateFE", 0, path);

	this->m_CommandMasterResetPointRate[3] = GetPrivateProfileInt(section, "CommandMasterResetPointRateMG", 0, path);

	this->m_CommandMasterResetPointRate[4] = GetPrivateProfileInt(section, "CommandMasterResetPointRateDL", 0, path);

	this->m_CommandMasterResetPointRate[5] = GetPrivateProfileInt(section, "CommandMasterResetPointRateSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_CommandMasterResetPointRate[6] = GetPrivateProfileInt(section, "CommandMasterResetPointRateRF", 0, path);

#endif

	this->m_CommandGMMoveSwitch = GetPrivateProfileInt(section, "CommandGMMoveSwitch", 0, path);

	this->m_CommandGMMoveGameMasterLevel = GetPrivateProfileInt(section, "CommandGMMoveGameMasterLevel", 0, path);

	this->m_CommandGMPostSwitch = GetPrivateProfileInt(section, "CommandGMPostSwitch", 0, path);

	this->m_CommandGMPostGameMasterLevel = GetPrivateProfileInt(section, "CommandGMPostGameMasterLevel", 0, path);

	this->m_CommandTrackSwitch = GetPrivateProfileInt(section, "CommandTrackSwitch", 0, path);

	this->m_CommandTrackGameMasterLevel = GetPrivateProfileInt(section, "CommandTrackGameMasterLevel", 0, path);

	this->m_CommandTraceSwitch = GetPrivateProfileInt(section, "CommandTraceSwitch", 0, path);

	this->m_CommandTraceGameMasterLevel = GetPrivateProfileInt(section, "CommandTraceGameMasterLevel", 0, path);

	this->m_CommandDisconnectSwitch = GetPrivateProfileInt(section, "CommandDisconnectSwitch", 0, path);

	this->m_CommandDisconnectGameMasterLevel = GetPrivateProfileInt(section, "CommandDisconnectGameMasterLevel", 0, path);

	this->m_CommandFireworksSwitch = GetPrivateProfileInt(section, "CommandFireworksSwitch", 0, path);

	this->m_CommandFireworksGameMasterLevel = GetPrivateProfileInt(section, "CommandFireworksGameMasterLevel", 0, path);

	this->m_CommandMakeSwitch = GetPrivateProfileInt(section, "CommandMakeSwitch", 0, path);

	this->m_CommandMakeGameMasterLevel = GetPrivateProfileInt(section, "CommandMakeGameMasterLevel", 0, path);

	this->m_CommandSkinSwitch = GetPrivateProfileInt(section, "CommandSkinSwitch", 0, path);

	this->m_CommandSkinGameMasterLevel = GetPrivateProfileInt(section, "CommandSkinGameMasterLevel", 0, path);

	this->m_CommandSetMoneySwitch = GetPrivateProfileInt(section, "CommandSetMoneySwitch", 0, path);

	this->m_CommandSetMoneyGameMasterLevel = GetPrivateProfileInt(section, "CommandSetMoneyGameMasterLevel", 0, path);

	this->m_CommandNoticeSwitch = GetPrivateProfileInt(section, "CommandNoticeSwitch", 0, path);

	this->m_CommandNoticeGameMasterLevel = GetPrivateProfileInt(section, "CommandNoticeGameMasterLevel", 0, path);

	this->m_CommandHideSwitch = GetPrivateProfileInt(section, "CommandHideSwitch", 0, path);

	this->m_CommandHideGameMasterLevel = GetPrivateProfileInt(section, "CommandHideGameMasterLevel", 0, path);
}

void CServerInfo::ReadCommonInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	this->m_ServerMaxUserNumber = GetPrivateProfileInt(section, "ServerMaxUserNumber", 0, path);

	this->m_MainChecksum = GetPrivateProfileInt(section, "MainChecksum", 0, path);

	this->m_MaxItemOption = GetPrivateProfileInt(section, "MaxItemOption", 0, path);

	this->m_MaxIpConnection = GetPrivateProfileInt(section, "MaxIpConnection", 0, path);

	this->m_PersonalCodeCheck = GetPrivateProfileInt(section, "PersonalCodeCheck", 0, path);

	this->m_SetItemAcceptHarmonySwitch = GetPrivateProfileInt(section, "SetItemAcceptHarmonySwitch", 0, path);

	this->m_MonsterLifeRate = GetPrivateProfileInt(section, "MonsterLifeRate", 0, path);

#if(GAMESERVER_UPDATE<=603)

	this->m_MonsterHealthBarSwitch = GetPrivateProfileInt(section, "MonsterHealthBarSwitch", 0, path);

#endif

	this->m_NonPK = GetPrivateProfileInt(section, "NonPK", 0, path);

	this->m_PKLimitFree = GetPrivateProfileInt(section, "PKLimitFree", 0, path);

	this->m_PKDownTime1 = GetPrivateProfileInt(section, "PKDownTime1", 0, path);

	this->m_PKDownTime2 = GetPrivateProfileInt(section, "PKDownTime2", 0, path);

	this->m_TradeSwitch = GetPrivateProfileInt(section, "TradeSwitch", 0, path);

	this->m_PersonalShopSwitch = GetPrivateProfileInt(section, "PersonalShopSwitch", 0, path);

#if(GAMESERVER_UPDATE>=802)

	this->m_PersonalShopMoneyCommisionRate = GetPrivateProfileInt(section, "PersonalShopMoneyCommisionRate", 0, path);

	this->m_PersonalShopJewelCommisionRate = GetPrivateProfileInt(section, "PersonalShopJewelCommisionRate", 0, path);

#endif

	this->m_DuelSwitch = GetPrivateProfileInt(section, "DuelSwitch", 0, path);

#if(GAMESERVER_UPDATE>=402)

	this->m_DuelArenaAnnounceSwitch = GetPrivateProfileInt(section, "DuelArenaAnnounceSwitch", 0, path);

#endif

	this->m_GuildCreateSwitch = GetPrivateProfileInt(section, "GuildCreateSwitch", 0, path);

	this->m_GuildDeleteSwitch = GetPrivateProfileInt(section, "GuildDeleteSwitch", 0, path);

	this->m_GuildCreateMinLevel = GetPrivateProfileInt(section, "GuildCreateMinLevel", 0, path);

	this->m_GuildInsertMaxUser1 = GetPrivateProfileInt(section, "GuildInsertMaxUser1", 0, path);

	this->m_GuildInsertMaxUser2 = GetPrivateProfileInt(section, "GuildInsertMaxUser2", 0, path);

	this->m_GuildAllianceMinUser = GetPrivateProfileInt(section, "GuildAllianceMinUser", 0, path);

	this->m_GuildAllianceMaxCount = GetPrivateProfileInt(section, "GuildAllianceMaxCount", 0, path);

	this->m_GuildOwnerDestroyLimit = GetPrivateProfileInt(section, "GuildOwnerDestroyLimit", 0, path);

	this->m_ElfBufferMaxLevel[0] = GetPrivateProfileInt(section, "ElfBufferMaxLevel_AL0", 0, path);

	this->m_ElfBufferMaxLevel[1] = GetPrivateProfileInt(section, "ElfBufferMaxLevel_AL1", 0, path);

	this->m_ElfBufferMaxLevel[2] = GetPrivateProfileInt(section, "ElfBufferMaxLevel_AL2", 0, path);

	this->m_ElfBufferMaxLevel[3] = GetPrivateProfileInt(section, "ElfBufferMaxLevel_AL3", 0, path);

	this->m_ElfBufferMaxReset[0] = GetPrivateProfileInt(section, "ElfBufferMaxReset_AL0", 0, path);

	this->m_ElfBufferMaxReset[1] = GetPrivateProfileInt(section, "ElfBufferMaxReset_AL1", 0, path);

	this->m_ElfBufferMaxReset[2] = GetPrivateProfileInt(section, "ElfBufferMaxReset_AL2", 0, path);

	this->m_ElfBufferMaxReset[3] = GetPrivateProfileInt(section, "ElfBufferMaxReset_AL3", 0, path);

	this->m_ElfBufferDamageConstA = GetPrivateProfileInt(section, "ElfBufferDamageConstA", 0, path);

	this->m_ElfBufferDamageConstB = GetPrivateProfileInt(section, "ElfBufferDamageConstB", 0, path);

	this->m_ElfBufferDefenseConstA = GetPrivateProfileInt(section, "ElfBufferDefenseConstA", 0, path);

	this->m_ElfBufferDefenseConstB = GetPrivateProfileInt(section, "ElfBufferDefenseConstB", 0, path);

	char temp[100];
	GetPrivateProfileString(section, "ExperienceRate", "1", temp, sizeof(temp), path);

	this->m_ExperienceRate = _atoi64(temp);

	this->m_AddExperienceRate[0] = GetPrivateProfileInt(section, "AddExperienceRate_AL0", 0, path);

	this->m_AddExperienceRate[1] = GetPrivateProfileInt(section, "AddExperienceRate_AL1", 0, path);

	this->m_AddExperienceRate[2] = GetPrivateProfileInt(section, "AddExperienceRate_AL2", 0, path);

	this->m_AddExperienceRate[3] = GetPrivateProfileInt(section, "AddExperienceRate_AL3", 0, path);

#if(GAMESERVER_UPDATE>=401)

	this->m_AddMasterExperienceRate[0] = GetPrivateProfileInt(section, "AddMasterExperienceRate_AL0", 0, path);

	this->m_AddMasterExperienceRate[1] = GetPrivateProfileInt(section, "AddMasterExperienceRate_AL1", 0, path);

	this->m_AddMasterExperienceRate[2] = GetPrivateProfileInt(section, "AddMasterExperienceRate_AL2", 0, path);

	this->m_AddMasterExperienceRate[3] = GetPrivateProfileInt(section, "AddMasterExperienceRate_AL3", 0, path);

	this->m_MinMasterExperienceMonsterLevel[0] = GetPrivateProfileInt(section, "MinMasterExperienceMonsterLevel_AL0", 0, path);

	this->m_MinMasterExperienceMonsterLevel[1] = GetPrivateProfileInt(section, "MinMasterExperienceMonsterLevel_AL1", 0, path);

	this->m_MinMasterExperienceMonsterLevel[2] = GetPrivateProfileInt(section, "MinMasterExperienceMonsterLevel_AL2", 0, path);

	this->m_MinMasterExperienceMonsterLevel[3] = GetPrivateProfileInt(section, "MinMasterExperienceMonsterLevel_AL3", 0, path);

#endif

	this->m_AddEventExperienceRate[0] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL0", 0, path);

	this->m_AddEventExperienceRate[1] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL1", 0, path);

	this->m_AddEventExperienceRate[2] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL2", 0, path);

	this->m_AddEventExperienceRate[3] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL3", 0, path);

#if(GAMESERVER_UPDATE>=501)

	this->m_AddQuestExperienceRate[0] = GetPrivateProfileInt(section, "AddQuestExperienceRate_AL0", 0, path);

	this->m_AddQuestExperienceRate[1] = GetPrivateProfileInt(section, "AddQuestExperienceRate_AL1", 0, path);

	this->m_AddQuestExperienceRate[2] = GetPrivateProfileInt(section, "AddQuestExperienceRate_AL2", 0, path);

	this->m_AddQuestExperienceRate[3] = GetPrivateProfileInt(section, "AddQuestExperienceRate_AL3", 0, path);

#endif

	this->m_ItemDropTime = GetPrivateProfileInt(section, "ItemDropTime", 0, path);

	this->m_ItemDropRate[0] = GetPrivateProfileInt(section, "ItemDropRate_AL0", 0, path);

	this->m_ItemDropRate[1] = GetPrivateProfileInt(section, "ItemDropRate_AL1", 0, path);

	this->m_ItemDropRate[2] = GetPrivateProfileInt(section, "ItemDropRate_AL2", 0, path);

	this->m_ItemDropRate[3] = GetPrivateProfileInt(section, "ItemDropRate_AL3", 0, path);

	this->m_MoneyDropTime = GetPrivateProfileInt(section, "MoneyDropTime", 0, path);

	this->m_MoneyRate = GetPrivateProfileInt(section, "MoneyAmountDropRate", 0, path);

	this->m_MoneyAmountDropRate[0] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL0", 0, path);

	this->m_MoneyAmountDropRate[1] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL1", 0, path);

	this->m_MoneyAmountDropRate[2] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL2", 0, path);

	this->m_MoneyAmountDropRate[3] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL3", 0, path);

	this->m_WeaponDurabilityRate = GetPrivateProfileInt(section, "WeaponDurabilityRate", 0, path);

	this->m_ArmorDurabilityRate = GetPrivateProfileInt(section, "ArmorDurabilityRate", 0, path);

	this->m_WingDurabilityRate = GetPrivateProfileInt(section, "WingDurabilityRate", 0, path);

	this->m_GuardianDurabilityRate = GetPrivateProfileInt(section, "GuardianDurabilityRate", 0, path);

	this->m_PendantDurabilityRate = GetPrivateProfileInt(section, "PendantDurabilityRate", 0, path);

	this->m_RingDurabilityRate = GetPrivateProfileInt(section, "RingDurabilityRate", 0, path);

	this->m_PetDurabilityRate = GetPrivateProfileInt(section, "PetDurabilityRate", 0, path);

#if(GAMESERVER_UPDATE>=803)

	this->m_MuunDurabilityRate = GetPrivateProfileInt(section, "MuunDurabilityRate", 0, path);

#endif

	this->m_TradeItemBlock = GetPrivateProfileInt(section, "TradeItemBlock", 0, path);

	this->m_TradeItemBlockExc = GetPrivateProfileInt(section, "TradeItemBlockExc", 0, path);

	this->m_TradeItemBlockSet = GetPrivateProfileInt(section, "TradeItemBlockSet", 0, path);

	this->m_TradeItemBlockHarmony = GetPrivateProfileInt(section, "TradeItemBlockHarmony", 0, path);

	this->m_TradeItemBlockSell = GetPrivateProfileInt(section, "TradeItemBlockSell", 0, path);

	this->m_MaxLevelUp = GetPrivateProfileInt(section, "MaxLevelUp", 0, path);

	this->m_MaxLevelUpEvent = GetPrivateProfileInt(section, "MaxLevelUpEvent", 0, path);

#if(GAMESERVER_UPDATE>=501)

	this->m_MaxLevelUpQuest = GetPrivateProfileInt(section, "MaxLevelUpQuest", 0, path);

#endif

	this->m_MaxStatPoint[0] = GetPrivateProfileInt(section, "MaxStatPoint_AL0", 0, path);

	this->m_MaxStatPoint[1] = GetPrivateProfileInt(section, "MaxStatPoint_AL1", 0, path);

	this->m_MaxStatPoint[2] = GetPrivateProfileInt(section, "MaxStatPoint_AL2", 0, path);

	this->m_MaxStatPoint[3] = GetPrivateProfileInt(section, "MaxStatPoint_AL3", 0, path);

	this->m_LevelUpPoint[0] = GetPrivateProfileInt(section, "DWLevelUpPoint", 0, path);

	this->m_LevelUpPoint[1] = GetPrivateProfileInt(section, "DKLevelUpPoint", 0, path);

	this->m_LevelUpPoint[2] = GetPrivateProfileInt(section, "FELevelUpPoint", 0, path);

	this->m_LevelUpPoint[3] = GetPrivateProfileInt(section, "MGLevelUpPoint", 0, path);

	this->m_LevelUpPoint[4] = GetPrivateProfileInt(section, "DLLevelUpPoint", 0, path);

#if(GAMESERVER_UPDATE>=301)

	this->m_LevelUpPoint[5] = GetPrivateProfileInt(section, "SULevelUpPoint", 0, path);

#endif

#if(GAMESERVER_UPDATE>=601)

	this->m_LevelUpPoint[6] = GetPrivateProfileInt(section, "RFLevelUpPoint", 0, path);

#endif

	this->m_PlusStatPoint = GetPrivateProfileInt(section, "PlusStatPoint", 0, path);

	this->m_CharacterCreateSwitch = GetPrivateProfileInt(section, "CharacterCreateSwitch", 0, path);

	this->m_MGCreateType = GetPrivateProfileInt(section, "MGCreateType", 0, path);

	this->m_DLCreateType = GetPrivateProfileInt(section, "DLCreateType", 0, path);

#if(GAMESERVER_UPDATE>=301)

	this->m_SUCreateType = GetPrivateProfileInt(section, "SUCreateType", 0, path);

#endif

#if(GAMESERVER_UPDATE>=601)

	this->m_RFCreateType = GetPrivateProfileInt(section, "RFCreateType", 0, path);

#endif

	this->m_MGCreateLevel[0] = GetPrivateProfileInt(section, "MGCreateLevel_AL0", 0, path);

	this->m_MGCreateLevel[1] = GetPrivateProfileInt(section, "MGCreateLevel_AL1", 0, path);

	this->m_MGCreateLevel[2] = GetPrivateProfileInt(section, "MGCreateLevel_AL2", 0, path);

	this->m_MGCreateLevel[3] = GetPrivateProfileInt(section, "MGCreateLevel_AL3", 0, path);

	this->m_DLCreateLevel[0] = GetPrivateProfileInt(section, "DLCreateLevel_AL0", 0, path);

	this->m_DLCreateLevel[1] = GetPrivateProfileInt(section, "DLCreateLevel_AL1", 0, path);

	this->m_DLCreateLevel[2] = GetPrivateProfileInt(section, "DLCreateLevel_AL2", 0, path);

	this->m_DLCreateLevel[3] = GetPrivateProfileInt(section, "DLCreateLevel_AL3", 0, path);

#if(GAMESERVER_UPDATE>=301)

	this->m_SUCreateLevel[0] = GetPrivateProfileInt(section, "SUCreateLevel_AL0", 0, path);

	this->m_SUCreateLevel[1] = GetPrivateProfileInt(section, "SUCreateLevel_AL1", 0, path);

	this->m_SUCreateLevel[2] = GetPrivateProfileInt(section, "SUCreateLevel_AL2", 0, path);

	this->m_SUCreateLevel[3] = GetPrivateProfileInt(section, "SUCreateLevel_AL3", 0, path);

#endif

#if(GAMESERVER_UPDATE>=601)

	this->m_RFCreateLevel[0] = GetPrivateProfileInt(section, "RFCreateLevel_AL0", 0, path);

	this->m_RFCreateLevel[1] = GetPrivateProfileInt(section, "RFCreateLevel_AL1", 0, path);

	this->m_RFCreateLevel[2] = GetPrivateProfileInt(section, "RFCreateLevel_AL2", 0, path);

	this->m_RFCreateLevel[3] = GetPrivateProfileInt(section, "RFCreateLevel_AL3", 0, path);

#endif

	this->m_ShieldGaugeRate = GetPrivateProfileInt(section, "ShieldGaugeRate", 0, path);

	this->m_ShieldGaugeConstA = GetPrivateProfileInt(section, "ShieldGaugeConstA", 0, path);

	this->m_ShieldGaugeConstB = GetPrivateProfileInt(section, "ShieldGaugeConstB", 0, path);

	this->m_ShieldGaugeAttackRate = GetPrivateProfileInt(section, "ShieldGaugeAttackRate", 0, path);

	this->m_ShieldGaugeAttackComboMiss = GetPrivateProfileInt(section, "ShieldGaugeAttackComboMiss", 0, path);

	this->m_PartyReconnectTime = GetPrivateProfileInt(section, "PartyReconnectTime", 0, path);

	this->m_PartyGeneralExperience[0] = GetPrivateProfileInt(section, "PartyGeneralExperience1", 0, path);

	this->m_PartyGeneralExperience[1] = GetPrivateProfileInt(section, "PartyGeneralExperience2", 0, path);

	this->m_PartyGeneralExperience[2] = GetPrivateProfileInt(section, "PartyGeneralExperience3", 0, path);

	this->m_PartyGeneralExperience[3] = GetPrivateProfileInt(section, "PartyGeneralExperience4", 0, path);

	this->m_PartyGeneralExperience[4] = GetPrivateProfileInt(section, "PartyGeneralExperience5", 0, path);

	this->m_PartySpecialExperience[0] = GetPrivateProfileInt(section, "PartySpecialExperience1", 0, path);

	this->m_PartySpecialExperience[1] = GetPrivateProfileInt(section, "PartySpecialExperience2", 0, path);

	this->m_PartySpecialExperience[2] = GetPrivateProfileInt(section, "PartySpecialExperience3", 0, path);

	this->m_PartySpecialExperience[3] = GetPrivateProfileInt(section, "PartySpecialExperience4", 0, path);

	this->m_PartySpecialExperience[4] = GetPrivateProfileInt(section, "PartySpecialExperience5", 0, path);

	this->m_ApplePotionRate = GetPrivateProfileInt(section, "ApplePotionRate", 0, path);

	this->m_SmallLifePotionRate = GetPrivateProfileInt(section, "SmallLifePotionRate", 0, path);

	this->m_MidleLifePotionRate = GetPrivateProfileInt(section, "MidleLifePotionRate", 0, path);

	this->m_LargeLifePotionRate = GetPrivateProfileInt(section, "LargeLifePotionRate", 0, path);

	this->m_SmallManaPotionRate = GetPrivateProfileInt(section, "SmallManaPotionRate", 0, path);

	this->m_MidleManaPotionRate = GetPrivateProfileInt(section, "MidleManaPotionRate", 0, path);

	this->m_LargeManaPotionRate = GetPrivateProfileInt(section, "LargeManaPotionRate", 0, path);

	this->m_SmallShieldPotionRate = GetPrivateProfileInt(section, "SmallShieldPotionRate", 0, path);

	this->m_MidleShieldPotionRate = GetPrivateProfileInt(section, "MidleShieldPotionRate", 0, path);

	this->m_LargeShieldPotionRate = GetPrivateProfileInt(section, "LargeShieldPotionRate", 0, path);

	this->m_SmallCompoundPotionRate1 = GetPrivateProfileInt(section, "SmallCompoundPotionRate1", 0, path);

	this->m_SmallCompoundPotionRate2 = GetPrivateProfileInt(section, "SmallCompoundPotionRate2", 0, path);

	this->m_MidleCompoundPotionRate1 = GetPrivateProfileInt(section, "MidleCompoundPotionRate1", 0, path);

	this->m_MidleCompoundPotionRate2 = GetPrivateProfileInt(section, "MidleCompoundPotionRate2", 0, path);

	this->m_LargeCompoundPotionRate1 = GetPrivateProfileInt(section, "LargeCompoundPotionRate1", 0, path);

	this->m_LargeCompoundPotionRate2 = GetPrivateProfileInt(section, "LargeCompoundPotionRate2", 0, path);

	this->m_EliteLifePotionRate = GetPrivateProfileInt(section, "EliteLifePotionRate", 0, path);

	this->m_EliteManaPotionRate = GetPrivateProfileInt(section, "EliteManaPotionRate", 0, path);

	this->m_EliteShieldPotionRate = GetPrivateProfileInt(section, "EliteShieldPotionRate", 0, path);

	char field[100];
	int count = 0;

	for (BYTE i = 0; i < 8; ++i)
	{
		for (BYTE x = 0; x < 16; ++x)
		{
			wsprintf(field, "TransformationRing%02d", ++count);
			this->m_TransformationRing[i][x] = GetPrivateProfileInt(section, field, 0, path);
		}
	}

	this->m_SoulSuccessRate[0] = GetPrivateProfileInt(section, "SoulSuccessRate_AL0", 0, path);

	this->m_SoulSuccessRate[1] = GetPrivateProfileInt(section, "SoulSuccessRate_AL1", 0, path);

	this->m_SoulSuccessRate[2] = GetPrivateProfileInt(section, "SoulSuccessRate_AL2", 0, path);

	this->m_SoulSuccessRate[3] = GetPrivateProfileInt(section, "SoulSuccessRate_AL3", 0, path);

	this->m_LifeSuccessRate[0] = GetPrivateProfileInt(section, "LifeSuccessRate_AL0", 0, path);

	this->m_LifeSuccessRate[1] = GetPrivateProfileInt(section, "LifeSuccessRate_AL1", 0, path);

	this->m_LifeSuccessRate[2] = GetPrivateProfileInt(section, "LifeSuccessRate_AL2", 0, path);

	this->m_LifeSuccessRate[3] = GetPrivateProfileInt(section, "LifeSuccessRate_AL3", 0, path);

	this->m_HarmonySuccessRate[0] = GetPrivateProfileInt(section, "HarmonySuccessRate_AL0", 0, path);

	this->m_HarmonySuccessRate[1] = GetPrivateProfileInt(section, "HarmonySuccessRate_AL1", 0, path);

	this->m_HarmonySuccessRate[2] = GetPrivateProfileInt(section, "HarmonySuccessRate_AL2", 0, path);

	this->m_HarmonySuccessRate[3] = GetPrivateProfileInt(section, "HarmonySuccessRate_AL3", 0, path);

	this->m_SmeltStoneSuccessRate1[0] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate1_AL0", 0, path);

	this->m_SmeltStoneSuccessRate1[1] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate1_AL1", 0, path);

	this->m_SmeltStoneSuccessRate1[2] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate1_AL2", 0, path);

	this->m_SmeltStoneSuccessRate1[3] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate1_AL3", 0, path);

	this->m_SmeltStoneSuccessRate2[0] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate2_AL0", 0, path);

	this->m_SmeltStoneSuccessRate2[1] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate2_AL1", 0, path);

	this->m_SmeltStoneSuccessRate2[2] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate2_AL2", 0, path);

	this->m_SmeltStoneSuccessRate2[3] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate2_AL3", 0, path);

	this->m_AddLuckSuccessRate1[0] = GetPrivateProfileInt(section, "AddLuckSuccessRate1_AL0", 0, path);

	this->m_AddLuckSuccessRate1[1] = GetPrivateProfileInt(section, "AddLuckSuccessRate1_AL1", 0, path);

	this->m_AddLuckSuccessRate1[2] = GetPrivateProfileInt(section, "AddLuckSuccessRate1_AL2", 0, path);

	this->m_AddLuckSuccessRate1[3] = GetPrivateProfileInt(section, "AddLuckSuccessRate1_AL3", 0, path);

	this->m_AddLuckSuccessRate2[0] = GetPrivateProfileInt(section, "AddLuckSuccessRate2_AL0", 0, path);

	this->m_AddLuckSuccessRate2[1] = GetPrivateProfileInt(section, "AddLuckSuccessRate2_AL1", 0, path);

	this->m_AddLuckSuccessRate2[2] = GetPrivateProfileInt(section, "AddLuckSuccessRate2_AL2", 0, path);

	this->m_AddLuckSuccessRate2[3] = GetPrivateProfileInt(section, "AddLuckSuccessRate2_AL3", 0, path);

	this->m_FruitAddPointMin = GetPrivateProfileInt(section, "FruitAddPointMin", 0, path);

	this->m_FruitAddPointMax = GetPrivateProfileInt(section, "FruitAddPointMax", 0, path);

	this->m_FruitAddPointSuccessRate[0] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL0", 0, path);

	this->m_FruitAddPointSuccessRate[1] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL1", 0, path);

	this->m_FruitAddPointSuccessRate[2] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL2", 0, path);

	this->m_FruitAddPointSuccessRate[3] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL3", 0, path);

	this->m_FruitSubPointMin = GetPrivateProfileInt(section, "FruitSubPointMin", 0, path);

	this->m_FruitSubPointMax = GetPrivateProfileInt(section, "FruitSubPointMax", 0, path);

	this->m_FruitSubPointSuccessRate[0] = GetPrivateProfileInt(section, "FruitSubPointSuccessRate_AL0", 0, path);

	this->m_FruitSubPointSuccessRate[1] = GetPrivateProfileInt(section, "FruitSubPointSuccessRate_AL1", 0, path);

	this->m_FruitSubPointSuccessRate[2] = GetPrivateProfileInt(section, "FruitSubPointSuccessRate_AL2", 0, path);

	this->m_FruitSubPointSuccessRate[3] = GetPrivateProfileInt(section, "FruitSubPointSuccessRate_AL3", 0, path);
}

void CServerInfo::ReadCustomInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	this->m_AnnounceChaosMix = GetPrivateProfileInt(section, "AnnounceChaosMix", 0, path);

	GetPrivateProfileString(section, "AnnounceChaosMixText", "", this->m_AnnounceChaosMixText, sizeof(this->m_AnnounceChaosMixText), path);
}

void CServerInfo::ReadEventInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	this->m_BloodCastleEvent = GetPrivateProfileInt(section, "BloodCastleEvent", 0, path);

	this->m_BloodCastleMaxUser = GetPrivateProfileInt(section, "BloodCastleMaxUser", 0, path);

	this->m_BonusManagerSwitch = GetPrivateProfileInt(section, "BonusManagerSwitch", 0, path);

	this->m_CastleDeepEvent = GetPrivateProfileInt(section, "CastleDeepEvent", 0, path);

	this->m_CastleDeepEventTime = GetPrivateProfileInt(section, "CastleDeepEventTime", 0, path);

	this->m_CastleSiegeEvent = GetPrivateProfileInt(section, "CastleSiegeEvent", 0, path);

	this->m_CastleSiegeCycleStartYear = GetPrivateProfileInt(section, "CastleSiegeCycleStartYear", 0, path);

	this->m_CastleSiegeCycleStartMonth = GetPrivateProfileInt(section, "CastleSiegeCycleStartMonth", 0, path);

	this->m_CastleSiegeCycleStartDay = GetPrivateProfileInt(section, "CastleSiegeCycleStartDay", 0, path);

	this->m_CastleSiegeSpecificState = GetPrivateProfileInt(section, "CastleSiegeSpecificState", 0, path);

	this->m_CastleSiegeStateStartYear = GetPrivateProfileInt(section, "CastleSiegeStateStartYear", 0, path);

	this->m_CastleSiegeStateStartMonth = GetPrivateProfileInt(section, "CastleSiegeStateStartMonth", 0, path);

	this->m_CastleSiegeStateStartDay = GetPrivateProfileInt(section, "CastleSiegeStateStartDay", 0, path);

	this->m_CastleSiegeStateStartHour = GetPrivateProfileInt(section, "CastleSiegeStateStartHour", 0, path);

	this->m_CastleSiegeStateStartMinute = GetPrivateProfileInt(section, "CastleSiegeStateStartMinute", 0, path);

	this->m_CastleSiegeOffensiveWeaponDamage = GetPrivateProfileInt(section, "CastleSiegeOffensiveWeaponDamage", 0, path);

	this->m_CastleSiegeDefensiveWeaponDamage = GetPrivateProfileInt(section, "CastleSiegeDefensiveWeaponDamage", 0, path);

	this->m_CastleSiegeDecayAccumulatedTimeValue = GetPrivateProfileInt(section, "CastleSiegeDecayAccumulatedTimeValue", 0, path);

	this->m_CastleSiegeLowerAccumulatedTimeValue = GetPrivateProfileInt(section, "CastleSiegeLowerAccumulatedTimeValue", 0, path);

	this->m_ChaosCastleEvent = GetPrivateProfileInt(section, "ChaosCastleEvent", 0, path);

	this->m_ChaosCastleMinUser = GetPrivateProfileInt(section, "ChaosCastleMinUser", 0, path);

	this->m_ChaosCastleBlowUserRate = GetPrivateProfileInt(section, "ChaosCastleBlowUserRate", 0, path);

	this->m_DevilSquareEvent = GetPrivateProfileInt(section, "DevilSquareEvent", 0, path);

	this->m_DevilSquareMaxUser = GetPrivateProfileInt(section, "DevilSquareMaxUser", 0, path);

#if(GAMESERVER_UPDATE>=501)

	this->m_DoubleGoerEvent = GetPrivateProfileInt(section, "DoubleGoerEvent", 0, path);

	this->m_DoubleGoerDifficultRate = GetPrivateProfileInt(section, "DoubleGoerDifficultRate", 0, path);

#endif

	this->m_IllusionTempleEvent = GetPrivateProfileInt(section, "IllusionTempleEvent", 0, path);

	this->m_IllusionTempleMinUser = GetPrivateProfileInt(section, "IllusionTempleMinUser", 0, path);

	this->m_IllusionTempleRewardFenrirRate = GetPrivateProfileInt(section, "IllusionTempleRewardFenrirRate", 0, path);

#if(GAMESERVER_UPDATE>=501)

	this->m_ImperialGuardianEvent = GetPrivateProfileInt(section, "ImperialGuardianEvent", 0, path);

	this->m_ImperialGuardianDifficultRate = GetPrivateProfileInt(section, "ImperialGuardianDifficultRate", 0, path);

#endif

	this->m_InvasionManagerSwitch = GetPrivateProfileInt(section, "InvasionManagerSwitch", 0, path);

	this->m_KanturuEvent = GetPrivateProfileInt(section, "KanturuEvent", 0, path);

#if(GAMESERVER_UPDATE>=402)

	this->m_MossMerchantEvent = GetPrivateProfileInt(section, "MossMerchantEvent", 0, path);

	this->m_MossMerchantEventTime = GetPrivateProfileInt(section, "MossMerchantEventTime", 0, path);

#endif

#if(GAMESERVER_UPDATE>=401)

	this->m_RaklionEvent = GetPrivateProfileInt(section, "RaklionEvent", 0, path);

#endif
}

void CServerInfo::ReadHackInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	this->m_CheckSpeedHack = GetPrivateProfileInt(section, "CheckSpeedHack", 0, path);

	this->m_CheckSpeedHackTolerance = GetPrivateProfileInt(section, "CheckSpeedHackTolerance", 0, path);

	this->m_CheckLatencyHack = GetPrivateProfileInt(section, "CheckLatencyHack", 0, path);

	this->m_CheckLatencyHackTolerance = GetPrivateProfileInt(section, "CheckLatencyHackTolerance", 0, path);

	this->m_CheckAutoPotionHack = GetPrivateProfileInt(section, "CheckAutoPotionHack", 0, path);

	this->m_CheckAutoPotionHackTolerance = GetPrivateProfileInt(section, "CheckAutoPotionHackTolerance", 0, path);

	this->m_CheckAutoComboHack = GetPrivateProfileInt(section, "CheckAutoComboHack", 0, path);

	this->m_CheckAutoComboHackTolerance = GetPrivateProfileInt(section, "CheckAutoComboHackTolerance", 0, path);
}

void CServerInfo::ReadSkillInfo(char* section, char* path) // OK
{
	char GameServerInfo[100] = {0};

	this->m_ManaShieldConstA = GetPrivateProfileInt(section, "ManaShieldConstA", 0, path);

	this->m_ManaShieldConstB = GetPrivateProfileInt(section, "ManaShieldConstB", 0, path);

	this->m_ManaShieldConstC = GetPrivateProfileInt(section, "ManaShieldConstC", 0, path);

	this->m_ManaShieldRate[0] = GetPrivateProfileInt(section, "ManaShieldRateDW", 0, path);

	this->m_ManaShieldRate[1] = GetPrivateProfileInt(section, "ManaShieldRateDK", 0, path);

	this->m_ManaShieldRate[2] = GetPrivateProfileInt(section, "ManaShieldRateFE", 0, path);

	this->m_ManaShieldRate[3] = GetPrivateProfileInt(section, "ManaShieldRateMG", 0, path);

	this->m_ManaShieldRate[4] = GetPrivateProfileInt(section, "ManaShieldRateDL", 0, path);

	this->m_ManaShieldRate[5] = GetPrivateProfileInt(section, "ManaShieldRateSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_ManaShieldRate[6] = GetPrivateProfileInt(section, "ManaShieldRateRF", 0, path);

#endif

	this->m_ManaShieldTimeConstA = GetPrivateProfileInt(section, "ManaShieldTimeConstA", 0, path);

	this->m_ManaShieldTimeConstB = GetPrivateProfileInt(section, "ManaShieldTimeConstB", 0, path);

	this->m_ManaShieldMaxRate = GetPrivateProfileInt(section, "ManaShieldMaxRate", 0, path);

	this->m_DefenseConstA = GetPrivateProfileInt(section, "DefenseConstA", 0, path);

	this->m_DefenseTimeConstA = GetPrivateProfileInt(section, "DefenseTimeConstA", 0, path);

	this->m_HealConstA = GetPrivateProfileInt(section, "HealConstA", 0, path);

	this->m_HealConstB = GetPrivateProfileInt(section, "HealConstB", 0, path);

	this->m_GreaterDefenseConstA = GetPrivateProfileInt(section, "GreaterDefenseConstA", 0, path);

	this->m_GreaterDefenseConstB = GetPrivateProfileInt(section, "GreaterDefenseConstB", 0, path);

	this->m_GreaterDefenseRate[0] = GetPrivateProfileInt(section, "GreaterDefenseRateDW", 0, path);

	this->m_GreaterDefenseRate[1] = GetPrivateProfileInt(section, "GreaterDefenseRateDK", 0, path);

	this->m_GreaterDefenseRate[2] = GetPrivateProfileInt(section, "GreaterDefenseRateFE", 0, path);

	this->m_GreaterDefenseRate[3] = GetPrivateProfileInt(section, "GreaterDefenseRateMG", 0, path);

	this->m_GreaterDefenseRate[4] = GetPrivateProfileInt(section, "GreaterDefenseRateDL", 0, path);

	this->m_GreaterDefenseRate[5] = GetPrivateProfileInt(section, "GreaterDefenseRateSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_GreaterDefenseRate[6] = GetPrivateProfileInt(section, "GreaterDefenseRateRF", 0, path);

#endif

	this->m_GreaterDefenseTimeConstA = GetPrivateProfileInt(section, "GreaterDefenseTimeConstA", 0, path);

	this->m_GreaterDamageConstA = GetPrivateProfileInt(section, "GreaterDamageConstA", 0, path);

	this->m_GreaterDamageConstB = GetPrivateProfileInt(section, "GreaterDamageConstB", 0, path);

	this->m_GreaterDamageRate[0] = GetPrivateProfileInt(section, "GreaterDamageRateDW", 0, path);

	this->m_GreaterDamageRate[1] = GetPrivateProfileInt(section, "GreaterDamageRateDK", 0, path);

	this->m_GreaterDamageRate[2] = GetPrivateProfileInt(section, "GreaterDamageRateFE", 0, path);

	this->m_GreaterDamageRate[3] = GetPrivateProfileInt(section, "GreaterDamageRateMG", 0, path);

	this->m_GreaterDamageRate[4] = GetPrivateProfileInt(section, "GreaterDamageRateDL", 0, path);

	this->m_GreaterDamageRate[5] = GetPrivateProfileInt(section, "GreaterDamageRateSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_GreaterDamageRate[6] = GetPrivateProfileInt(section, "GreaterDamageRateRF", 0, path);

#endif

	this->m_GreaterDamageTimeConstA = GetPrivateProfileInt(section, "GreaterDamageTimeConstA", 0, path);

	this->m_SummonMonster1 = GetPrivateProfileInt(section, "SummonMonster1", 0, path);

	this->m_SummonMonster2 = GetPrivateProfileInt(section, "SummonMonster2", 0, path);

	this->m_SummonMonster3 = GetPrivateProfileInt(section, "SummonMonster3", 0, path);

	this->m_SummonMonster4 = GetPrivateProfileInt(section, "SummonMonster4", 0, path);

	this->m_SummonMonster5 = GetPrivateProfileInt(section, "SummonMonster5", 0, path);

	this->m_SummonMonster6 = GetPrivateProfileInt(section, "SummonMonster6", 0, path);

	this->m_SummonMonster7 = GetPrivateProfileInt(section, "SummonMonster7", 0, path);

#if(GAMESERVER_UPDATE>=701)

	this->m_SummonMonster8 = GetPrivateProfileInt(section, "SummonMonster8", 0, path);

#endif

	this->m_GreaterLifeConstA = GetPrivateProfileInt(section, "GreaterLifeConstA", 0, path);

	this->m_GreaterLifeConstB = GetPrivateProfileInt(section, "GreaterLifeConstB", 0, path);

	this->m_GreaterLifeConstC = GetPrivateProfileInt(section, "GreaterLifeConstC", 0, path);

	this->m_GreaterLifeRate[0] = GetPrivateProfileInt(section, "GreaterLifeRateDW", 0, path);

	this->m_GreaterLifeRate[1] = GetPrivateProfileInt(section, "GreaterLifeRateDK", 0, path);

	this->m_GreaterLifeRate[2] = GetPrivateProfileInt(section, "GreaterLifeRateFE", 0, path);

	this->m_GreaterLifeRate[3] = GetPrivateProfileInt(section, "GreaterLifeRateMG", 0, path);

	this->m_GreaterLifeRate[4] = GetPrivateProfileInt(section, "GreaterLifeRateDL", 0, path);

	this->m_GreaterLifeRate[5] = GetPrivateProfileInt(section, "GreaterLifeRateSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_GreaterLifeRate[6] = GetPrivateProfileInt(section, "GreaterLifeRateRF", 0, path);

#endif

	this->m_GreaterLifeTimeConstA = GetPrivateProfileInt(section, "GreaterLifeTimeConstA", 0, path);

	this->m_GreaterLifeTimeConstB = GetPrivateProfileInt(section, "GreaterLifeTimeConstB", 0, path);

	this->m_GreaterLifeMaxRate = GetPrivateProfileInt(section, "GreaterLifeMaxRate", 0, path);

	this->m_FireSlashConstA = GetPrivateProfileInt(section, "FireSlashConstA", 0, path);

	this->m_FireSlashConstB = GetPrivateProfileInt(section, "FireSlashConstB", 0, path);

	this->m_FireSlashTimeConstA = GetPrivateProfileInt(section, "FireSlashTimeConstA", 0, path);

	this->m_FireSlashMaxRate = GetPrivateProfileInt(section, "FireSlashMaxRate", 0, path);

	this->m_GreaterCriticalDamageConstA = GetPrivateProfileInt(section, "GreaterCriticalDamageConstA", 0, path);

	this->m_GreaterCriticalDamageConstB = GetPrivateProfileInt(section, "GreaterCriticalDamageConstB", 0, path);

	this->m_GreaterCriticalDamageTimeConstA = GetPrivateProfileInt(section, "GreaterCriticalDamageTimeConstA", 0, path);

	this->m_GreaterCriticalDamageTimeConstB = GetPrivateProfileInt(section, "GreaterCriticalDamageTimeConstB", 0, path);

	this->m_SternTimeConstA = GetPrivateProfileInt(section, "SternTimeConstA", 0, path);

	this->m_GreaterManaConstA = GetPrivateProfileInt(section, "GreaterManaConstA", 0, path);

	this->m_GreaterManaTimeConstA = GetPrivateProfileInt(section, "GreaterManaTimeConstA", 0, path);

	this->m_InvisibilityTimeConstA = GetPrivateProfileInt(section, "InvisibilityTimeConstA", 0, path);

	this->m_BrandTimeConstA = GetPrivateProfileInt(section, "BrandTimeConstA", 0, path);

	this->m_InfinityArrowTimeConstA = GetPrivateProfileInt(section, "InfinityArrowTimeConstA", 0, path);

	this->m_MagicDamageImmunityTimeConstA = GetPrivateProfileInt(section, "MagicDamageImmunityTimeConstA", 0, path);

	this->m_PhysiDamageImmunityTimeConstA = GetPrivateProfileInt(section, "PhysiDamageImmunityTimeConstA", 0, path);

	this->m_DrainLifeConstA = GetPrivateProfileInt(section, "DrainLifeConstA", 0, path);

	this->m_DrainLifeConstB = GetPrivateProfileInt(section, "DrainLifeConstB", 0, path);

	this->m_ReflectDamageConstA = GetPrivateProfileInt(section, "ReflectDamageConstA", 0, path);

	this->m_ReflectDamageConstB = GetPrivateProfileInt(section, "ReflectDamageConstB", 0, path);

	this->m_ReflectDamageRate[0] = GetPrivateProfileInt(section, "ReflectDamageRateDW", 0, path);

	this->m_ReflectDamageRate[1] = GetPrivateProfileInt(section, "ReflectDamageRateDK", 0, path);

	this->m_ReflectDamageRate[2] = GetPrivateProfileInt(section, "ReflectDamageRateFE", 0, path);

	this->m_ReflectDamageRate[3] = GetPrivateProfileInt(section, "ReflectDamageRateMG", 0, path);

	this->m_ReflectDamageRate[4] = GetPrivateProfileInt(section, "ReflectDamageRateDL", 0, path);

	this->m_ReflectDamageRate[5] = GetPrivateProfileInt(section, "ReflectDamageRateSU", 0, path);

#if(GAMESERVER_UPDATE>=601)

	this->m_ReflectDamageRate[6] = GetPrivateProfileInt(section, "ReflectDamageRateRF", 0, path);

#endif

	this->m_ReflectDamageTimeConstA = GetPrivateProfileInt(section, "ReflectDamageTimeConstA", 0, path);

	this->m_ReflectDamageTimeConstB = GetPrivateProfileInt(section, "ReflectDamageTimeConstB", 0, path);

	this->m_ReflectDamageMaxRate = GetPrivateProfileInt(section, "ReflectDamageMaxRate", 0, path);

	this->m_SwordPowerConstA = GetPrivateProfileInt(section, "SwordPowerConstA", 0, path);

	this->m_SwordPowerConstB = GetPrivateProfileInt(section, "SwordPowerConstB", 0, path);

	this->m_SwordPowerTimeConstA = GetPrivateProfileInt(section, "SwordPowerTimeConstA", 0, path);

	this->m_SwordPowerTimeConstB = GetPrivateProfileInt(section, "SwordPowerTimeConstB", 0, path);

	this->m_SwordPowerMaxRate = GetPrivateProfileInt(section, "SwordPowerMaxRate", 0, path);

	this->m_SleepConstA = GetPrivateProfileInt(section, "SleepConstA", 0, path);

	this->m_SleepConstB = GetPrivateProfileInt(section, "SleepConstB", 0, path);

	this->m_SleepTimeConstA = GetPrivateProfileInt(section, "SleepTimeConstA", 0, path);

	this->m_SleepTimeConstB = GetPrivateProfileInt(section, "SleepTimeConstB", 0, path);

	this->m_SleepMaxTime = GetPrivateProfileInt(section, "SleepMaxTime", 0, path);

	this->m_LesserDefenseConstA = GetPrivateProfileInt(section, "LesserDefenseConstA", 0, path);

	this->m_LesserDefenseConstB = GetPrivateProfileInt(section, "LesserDefenseConstB", 0, path);

	this->m_LesserDefenseConstC = GetPrivateProfileInt(section, "LesserDefenseConstC", 0, path);

	this->m_LesserDefenseConstD = GetPrivateProfileInt(section, "LesserDefenseConstD", 0, path);

	this->m_LesserDefenseTimeConstA = GetPrivateProfileInt(section, "LesserDefenseTimeConstA", 0, path);

	this->m_LesserDefenseTimeConstB = GetPrivateProfileInt(section, "LesserDefenseTimeConstB", 0, path);

	this->m_LesserDefenseMaxRate = GetPrivateProfileInt(section, "LesserDefenseMaxRate", 0, path);

	this->m_LesserDamageConstA = GetPrivateProfileInt(section, "LesserDamageConstA", 0, path);

	this->m_LesserDamageConstB = GetPrivateProfileInt(section, "LesserDamageConstB", 0, path);

	this->m_LesserDamageConstC = GetPrivateProfileInt(section, "LesserDamageConstC", 0, path);

	this->m_LesserDamageConstD = GetPrivateProfileInt(section, "LesserDamageConstD", 0, path);

	this->m_LesserDamageTimeConstA = GetPrivateProfileInt(section, "LesserDamageTimeConstA", 0, path);

	this->m_LesserDamageTimeConstB = GetPrivateProfileInt(section, "LesserDamageTimeConstB", 0, path);

	this->m_LesserDamageMaxRate = GetPrivateProfileInt(section, "LesserDamageMaxRate", 0, path);

	this->m_SahamuttConstA = GetPrivateProfileInt(section, "SahamuttConstA", 0, path);

	this->m_NeilConstA = GetPrivateProfileInt(section, "NeilConstA", 0, path);

	this->m_MagicCircleConstA = GetPrivateProfileInt(section, "MagicCircleConstA", 0, path);

	this->m_MagicCircleTimeConstA = GetPrivateProfileInt(section, "MagicCircleTimeConstA", 0, path);

	this->m_ShieldRecoverConstA = GetPrivateProfileInt(section, "ShieldRecoverConstA", 0, path);

	this->m_ShieldRecoverConstB = GetPrivateProfileInt(section, "ShieldRecoverConstB", 0, path);

	this->m_DragonSlayerConstA = GetPrivateProfileInt(section, "DragonSlayerConstA", 0, path);

	this->m_DragonSlayerConstB = GetPrivateProfileInt(section, "DragonSlayerConstB", 0, path);

	this->m_DragonSlayerConstC = GetPrivateProfileInt(section, "DragonSlayerConstC", 0, path);

	this->m_DragonSlayerConstD = GetPrivateProfileInt(section, "DragonSlayerConstD", 0, path);

	this->m_DragonSlayerMaxRate = GetPrivateProfileInt(section, "DragonSlayerMaxRate", 0, path);

	this->m_GreaterIgnoreDefenseRateConstA = GetPrivateProfileInt(section, "GreaterIgnoreDefenseRateConstA", 0, path);

	this->m_GreaterIgnoreDefenseRateConstB = GetPrivateProfileInt(section, "GreaterIgnoreDefenseRateConstB", 0, path);

	this->m_GreaterIgnoreDefenseRateTimeConstA = GetPrivateProfileInt(section, "GreaterIgnoreDefenseRateTimeConstA", 0, path);

	this->m_GreaterIgnoreDefenseRateTimeConstB = GetPrivateProfileInt(section, "GreaterIgnoreDefenseRateTimeConstB", 0, path);

	this->m_FitnessConstA = GetPrivateProfileInt(section, "FitnessConstA", 0, path);

	this->m_FitnessConstB = GetPrivateProfileInt(section, "FitnessConstB", 0, path);

	this->m_FitnessTimeConstA = GetPrivateProfileInt(section, "FitnessTimeConstA", 0, path);

	this->m_FitnessTimeConstB = GetPrivateProfileInt(section, "FitnessTimeConstB", 0, path);

	this->m_GreaterDefenseSuccessRateConstA = GetPrivateProfileInt(section, "GreaterDefenseSuccessRateConstA", 0, path);

	this->m_GreaterDefenseSuccessRateConstB = GetPrivateProfileInt(section, "GreaterDefenseSuccessRateConstB", 0, path);

	this->m_GreaterDefenseSuccessRateTimeConstA = GetPrivateProfileInt(section, "GreaterDefenseSuccessRateTimeConstA", 0, path);

	this->m_GreaterDefenseSuccessRateTimeConstB = GetPrivateProfileInt(section, "GreaterDefenseSuccessRateTimeConstB", 0, path);

#if(GAMESERVER_UPDATE>=701)

	this->m_PartyHealConstA = GetPrivateProfileInt(section, "PartyHealConstA", 0, path);

	this->m_PartyHealConstB = GetPrivateProfileInt(section, "PartyHealConstB", 0, path);

	this->m_BlessConstA = GetPrivateProfileInt(section, "BlessConstA", 0, path);

	this->m_BlessTimeConstA = GetPrivateProfileInt(section, "BlessTimeConstA", 0, path);

	this->m_BlindConstA = GetPrivateProfileInt(section, "BlindConstA", 0, path);

	this->m_BlindConstB = GetPrivateProfileInt(section, "BlindConstB", 0, path);

	this->m_BlindConstC = GetPrivateProfileInt(section, "BlindConstC", 0, path);

	this->m_BlindTimeConstA = GetPrivateProfileInt(section, "BlindTimeConstA", 0, path);

	this->m_EarthPrisonConstA = GetPrivateProfileInt(section, "EarthPrisonConstA", 0, path);

	this->m_IronDefenseConstA = GetPrivateProfileInt(section, "IronDefenseConstA", 0, path);

	this->m_IronDefenseConstB = GetPrivateProfileInt(section, "IronDefenseConstB", 0, path);

	this->m_IronDefenseTimeConstA = GetPrivateProfileInt(section, "IronDefenseTimeConstA", 0, path);

	this->m_BloodHowlingConstA = GetPrivateProfileInt(section, "BloodHowlingConstA", 0, path);

	this->m_BloodHowlingConstB = GetPrivateProfileInt(section, "BloodHowlingConstB", 0, path);

	this->m_BloodHowlingTimeConstA = GetPrivateProfileInt(section, "BloodHowlingTimeConstA", 0, path);

#endif
}
