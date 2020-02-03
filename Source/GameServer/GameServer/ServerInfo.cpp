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
	this->m_DevilSquareMixRate1[0] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL0", 0, path);
	this->m_DevilSquareMixRate1[1] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL1", 0, path);
	this->m_DevilSquareMixRate2[0] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL0", 0, path);
	this->m_DevilSquareMixRate2[1] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL1", 0, path);
	this->m_DevilSquareMixRate3[0] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL0", 0, path);
	this->m_DevilSquareMixRate3[1] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL1", 0, path);
	this->m_DevilSquareMixRate4[0] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL0", 0, path);
	this->m_DevilSquareMixRate4[1] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL1", 0, path);
	this->m_DevilSquareMixRate5[0] = GetPrivateProfileInt(section, "DevilSquareMixRate5_AL0", 0, path);
	this->m_DevilSquareMixRate5[1] = GetPrivateProfileInt(section, "DevilSquareMixRate5_AL1", 0, path);
	this->m_DevilSquareMixRate6[0] = GetPrivateProfileInt(section, "DevilSquareMixRate6_AL0", 0, path);
	this->m_DevilSquareMixRate6[1] = GetPrivateProfileInt(section, "DevilSquareMixRate6_AL1", 0, path);
	this->m_DevilSquareMixRate7[0] = GetPrivateProfileInt(section, "DevilSquareMixRate7_AL0", 0, path);
	this->m_DevilSquareMixRate7[1] = GetPrivateProfileInt(section, "DevilSquareMixRate7_AL1", 0, path);
	this->m_PlusItemLevelMixRate1[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL0", 0, path);
	this->m_PlusItemLevelMixRate1[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL1", 0, path);
	this->m_PlusItemLevelMixRate2[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL0", 0, path);
	this->m_PlusItemLevelMixRate2[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL1", 0, path);
	this->m_PlusItemLevelMixRate3[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate3_AL0", 0, path);
	this->m_PlusItemLevelMixRate3[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate3_AL1", 0, path);
	this->m_PlusItemLevelMixRate4[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate4_AL0", 0, path);
	this->m_PlusItemLevelMixRate4[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate4_AL1", 0, path);
	this->m_DinorantMixRate[0] = GetPrivateProfileInt(section, "DinorantMixRate_AL0", 0, path);
	this->m_DinorantMixRate[1] = GetPrivateProfileInt(section, "DinorantMixRate_AL1", 0, path);
	this->m_FruitMixRate[0] = GetPrivateProfileInt(section, "FruitMixRate_AL0", 0, path);
	this->m_FruitMixRate[1] = GetPrivateProfileInt(section, "FruitMixRate_AL1", 0, path);
	this->m_Wing2MixRate[0] = GetPrivateProfileInt(section, "Wing2MixRate_AL0", 0, path);
	this->m_Wing2MixRate[1] = GetPrivateProfileInt(section, "Wing2MixRate_AL1", 0, path);
	this->m_BloodCastleMixRate1[0] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL0", 0, path);
	this->m_BloodCastleMixRate1[1] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL1", 0, path);
	this->m_BloodCastleMixRate2[0] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL0", 0, path);
	this->m_BloodCastleMixRate2[1] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL1", 0, path);
	this->m_BloodCastleMixRate3[0] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL0", 0, path);
	this->m_BloodCastleMixRate3[1] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL1", 0, path);
	this->m_BloodCastleMixRate4[0] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL0", 0, path);
	this->m_BloodCastleMixRate4[1] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL1", 0, path);
	this->m_BloodCastleMixRate5[0] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL0", 0, path);
	this->m_BloodCastleMixRate5[1] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL1", 0, path);
	this->m_BloodCastleMixRate6[0] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL0", 0, path);
	this->m_BloodCastleMixRate6[1] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL1", 0, path);
	this->m_BloodCastleMixRate7[0] = GetPrivateProfileInt(section, "BloodCastleMixRate7_AL0", 0, path);
	this->m_BloodCastleMixRate7[1] = GetPrivateProfileInt(section, "BloodCastleMixRate7_AL1", 0, path);
	this->m_BloodCastleMixRate8[0] = GetPrivateProfileInt(section, "BloodCastleMixRate8_AL0", 0, path);
	this->m_BloodCastleMixRate8[1] = GetPrivateProfileInt(section, "BloodCastleMixRate8_AL1", 0, path);
	this->m_Wing1MixRate[0] = GetPrivateProfileInt(section, "Wing1MixRate_AL0", 0, path);
	this->m_Wing1MixRate[1] = GetPrivateProfileInt(section, "Wing1MixRate_AL1", 0, path);
	this->m_PetMixRate[0] = GetPrivateProfileInt(section, "PetMixRate_AL0", 0, path);
	this->m_PetMixRate[1] = GetPrivateProfileInt(section, "PetMixRate_AL1", 0, path);
	this->m_PieceOfHornMixRate[0] = GetPrivateProfileInt(section, "PieceOfHornMixRate_AL0", 0, path);
	this->m_PieceOfHornMixRate[1] = GetPrivateProfileInt(section, "PieceOfHornMixRate_AL1", 0, path);
	this->m_BrokenHornMixRate[0] = GetPrivateProfileInt(section, "BrokenHornMixRate_AL0", 0, path);
	this->m_BrokenHornMixRate[1] = GetPrivateProfileInt(section, "BrokenHornMixRate_AL1", 0, path);
	this->m_HornOfFenrirMixRate[0] = GetPrivateProfileInt(section, "HornOfFenrirMixRate_AL0", 0, path);
	this->m_HornOfFenrirMixRate[1] = GetPrivateProfileInt(section, "HornOfFenrirMixRate_AL1", 0, path);
	this->m_HornOfFenrirUpgradeMixRate[0] = GetPrivateProfileInt(section, "HornOfFenrirUpgradeMixRate_AL0", 0, path);
	this->m_HornOfFenrirUpgradeMixRate[1] = GetPrivateProfileInt(section, "HornOfFenrirUpgradeMixRate_AL1", 0, path);
	this->m_ShieldPotionMixRate1[0] = GetPrivateProfileInt(section, "ShieldPotionMixRate1_AL0", 0, path);
	this->m_ShieldPotionMixRate1[1] = GetPrivateProfileInt(section, "ShieldPotionMixRate1_AL1", 0, path);
	this->m_ShieldPotionMixRate2[0] = GetPrivateProfileInt(section, "ShieldPotionMixRate2_AL0", 0, path);
	this->m_ShieldPotionMixRate2[1] = GetPrivateProfileInt(section, "ShieldPotionMixRate2_AL1", 0, path);
	this->m_ShieldPotionMixRate3[0] = GetPrivateProfileInt(section, "ShieldPotionMixRate3_AL0", 0, path);
	this->m_ShieldPotionMixRate3[1] = GetPrivateProfileInt(section, "ShieldPotionMixRate3_AL1", 0, path);
	this->m_JewelOfHarmonyItemPurityMixRate[0] = GetPrivateProfileInt(section, "JewelOfHarmonyItemPurityMixRate_AL0", 0, path);
	this->m_JewelOfHarmonyItemPurityMixRate[1] = GetPrivateProfileInt(section, "JewelOfHarmonyItemPurityMixRate_AL1", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[0] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate1_AL0", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[1] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate1_AL1", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[0] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate2_AL0", 0, path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[1] = GetPrivateProfileInt(section, "JewelOfHarmonyItemSmeltMixRate2_AL1", 0, path);
	this->m_JewelOfHarmonyItemRestoreMixRate[0] = GetPrivateProfileInt(section, "JewelOfHarmonyItemRestoreMixRate_AL0", 0, path);
	this->m_JewelOfHarmonyItemRestoreMixRate[1] = GetPrivateProfileInt(section, "JewelOfHarmonyItemRestoreMixRate_AL1", 0, path);
	this->m_Item380MixRate1[0] = GetPrivateProfileInt(section, "Item380MixRate1_AL0", 0, path);
	this->m_Item380MixRate1[1] = GetPrivateProfileInt(section, "Item380MixRate1_AL1", 0, path);
	this->m_Item380MixRate2[0] = GetPrivateProfileInt(section, "Item380MixRate2_AL0", 0, path);
	this->m_Item380MixRate2[1] = GetPrivateProfileInt(section, "Item380MixRate2_AL1", 0, path);
	this->m_IllusionTempleMixRate1[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate1_AL0", 0, path);
	this->m_IllusionTempleMixRate1[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate1_AL1", 0, path);
	this->m_IllusionTempleMixRate2[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate2_AL0", 0, path);
	this->m_IllusionTempleMixRate2[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate2_AL1", 0, path);
	this->m_IllusionTempleMixRate3[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate3_AL0", 0, path);
	this->m_IllusionTempleMixRate3[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate3_AL1", 0, path);
	this->m_IllusionTempleMixRate4[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate4_AL0", 0, path);
	this->m_IllusionTempleMixRate4[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate4_AL1", 0, path);
	this->m_IllusionTempleMixRate5[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate5_AL0", 0, path);
	this->m_IllusionTempleMixRate5[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate5_AL1", 0, path);
	this->m_IllusionTempleMixRate6[0] = GetPrivateProfileInt(section, "IllusionTempleMixRate6_AL0", 0, path);
	this->m_IllusionTempleMixRate6[1] = GetPrivateProfileInt(section, "IllusionTempleMixRate6_AL1", 0, path);
	this->m_FeatherOfCondorMixRate[0] = GetPrivateProfileInt(section, "FeatherOfCondorMixRate_AL0", 0, path);
	this->m_FeatherOfCondorMixRate[1] = GetPrivateProfileInt(section, "FeatherOfCondorMixRate_AL1", 0, path);
	this->m_Wing3MixRate[0] = GetPrivateProfileInt(section, "Wing3MixRate_AL0", 0, path);
	this->m_Wing3MixRate[1] = GetPrivateProfileInt(section, "Wing3MixRate_AL1", 0, path);
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

	this->m_DamageRatePvM[0] = GetPrivateProfileInt(section, "DWDamageRatePvM", 0, path);

	this->m_DamageRatePvM[1] = GetPrivateProfileInt(section, "DKDamageRatePvM", 0, path);

	this->m_DamageRatePvM[2] = GetPrivateProfileInt(section, "FEDamageRatePvM", 0, path);

	this->m_DamageRatePvM[3] = GetPrivateProfileInt(section, "MGDamageRatePvM", 0, path);

	this->m_DamageRatePvM[4] = GetPrivateProfileInt(section, "DLDamageRatePvM", 0, path);

	this->m_DamageRateTo[0][0] = GetPrivateProfileInt(section, "DWDamageRateToDW", 0, path);

	this->m_DamageRateTo[0][1] = GetPrivateProfileInt(section, "DWDamageRateToDK", 0, path);

	this->m_DamageRateTo[0][2] = GetPrivateProfileInt(section, "DWDamageRateToFE", 0, path);

	this->m_DamageRateTo[0][3] = GetPrivateProfileInt(section, "DWDamageRateToMG", 0, path);

	this->m_DamageRateTo[0][4] = GetPrivateProfileInt(section, "DWDamageRateToDL", 0, path);

	this->m_DamageRateTo[1][0] = GetPrivateProfileInt(section, "DKDamageRateToDW", 0, path);

	this->m_DamageRateTo[1][1] = GetPrivateProfileInt(section, "DKDamageRateToDK", 0, path);

	this->m_DamageRateTo[1][2] = GetPrivateProfileInt(section, "DKDamageRateToFE", 0, path);

	this->m_DamageRateTo[1][3] = GetPrivateProfileInt(section, "DKDamageRateToMG", 0, path);

	this->m_DamageRateTo[1][4] = GetPrivateProfileInt(section, "DKDamageRateToDL", 0, path);

	this->m_DamageRateTo[2][0] = GetPrivateProfileInt(section, "FEDamageRateToDW", 0, path);

	this->m_DamageRateTo[2][1] = GetPrivateProfileInt(section, "FEDamageRateToDK", 0, path);

	this->m_DamageRateTo[2][2] = GetPrivateProfileInt(section, "FEDamageRateToFE", 0, path);

	this->m_DamageRateTo[2][3] = GetPrivateProfileInt(section, "FEDamageRateToMG", 0, path);

	this->m_DamageRateTo[2][4] = GetPrivateProfileInt(section, "FEDamageRateToDL", 0, path);

	this->m_DamageRateTo[3][0] = GetPrivateProfileInt(section, "MGDamageRateToDW", 0, path);

	this->m_DamageRateTo[3][1] = GetPrivateProfileInt(section, "MGDamageRateToDK", 0, path);

	this->m_DamageRateTo[3][2] = GetPrivateProfileInt(section, "MGDamageRateToFE", 0, path);

	this->m_DamageRateTo[3][3] = GetPrivateProfileInt(section, "MGDamageRateToMG", 0, path);

	this->m_DamageRateTo[3][4] = GetPrivateProfileInt(section, "MGDamageRateToDL", 0, path);

	this->m_DamageRateTo[4][0] = GetPrivateProfileInt(section, "DLDamageRateToDW", 0, path);

	this->m_DamageRateTo[4][1] = GetPrivateProfileInt(section, "DLDamageRateToDK", 0, path);

	this->m_DamageRateTo[4][2] = GetPrivateProfileInt(section, "DLDamageRateToFE", 0, path);

	this->m_DamageRateTo[4][3] = GetPrivateProfileInt(section, "DLDamageRateToMG", 0, path);

	this->m_DamageRateTo[4][4] = GetPrivateProfileInt(section, "DLDamageRateToDL", 0, path);

	this->m_DamageStuckRate[0] = GetPrivateProfileInt(section, "DWDamageStuckRate", 0, path);

	this->m_DamageStuckRate[1] = GetPrivateProfileInt(section, "DKDamageStuckRate", 0, path);

	this->m_DamageStuckRate[2] = GetPrivateProfileInt(section, "FEDamageStuckRate", 0, path);

	this->m_DamageStuckRate[3] = GetPrivateProfileInt(section, "MGDamageStuckRate", 0, path);

	this->m_DamageStuckRate[4] = GetPrivateProfileInt(section, "DLDamageStuckRate", 0, path);

	this->m_DKDamageMultiplierConstA = GetPrivateProfileInt(section, "DKDamageMultiplierConstA", 0, path);

	this->m_DLDamageMultiplierConstA = GetPrivateProfileInt(section, "DLDamageMultiplierConstA", 0, path);
	
	this->m_DKDamageMultiplierMaxRate = GetPrivateProfileInt(section, "DKDamageMultiplierMaxRate", 0, path);

	this->m_DLDamageMultiplierMaxRate = GetPrivateProfileInt(section, "DLDamageMultiplierMaxRate", 0, path);

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

	this->m_MPRecoveryRate[0] = GetPrivateProfileInt(section, "DWMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[1] = GetPrivateProfileInt(section, "DKMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[2] = GetPrivateProfileInt(section, "FEMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[3] = GetPrivateProfileInt(section, "MGMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[4] = GetPrivateProfileInt(section, "DLMPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[0] = GetPrivateProfileInt(section, "DWBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[1] = GetPrivateProfileInt(section, "DKBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[2] = GetPrivateProfileInt(section, "FEBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[3] = GetPrivateProfileInt(section, "MGBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[4] = GetPrivateProfileInt(section, "DLBPRecoveryRate", 0, path);

	this->m_SDRecoveryRate[0] = GetPrivateProfileInt(section, "DWSDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[1] = GetPrivateProfileInt(section, "DKSDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[2] = GetPrivateProfileInt(section, "FESDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[3] = GetPrivateProfileInt(section, "MGSDRecoveryRate", 0, path);

	this->m_SDRecoveryRate[4] = GetPrivateProfileInt(section, "DLSDRecoveryRate", 0, path);

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
	this->m_DuelSwitch = GetPrivateProfileInt(section, "DuelSwitch", 0, path);

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
	this->m_ElfBufferMaxReset[0] = GetPrivateProfileInt(section, "ElfBufferMaxReset_AL0", 0, path);
	this->m_ElfBufferMaxReset[1] = GetPrivateProfileInt(section, "ElfBufferMaxReset_AL1", 0, path);

	this->m_ElfBufferDamageConstA = GetPrivateProfileInt(section, "ElfBufferDamageConstA", 0, path);
	this->m_ElfBufferDamageConstB = GetPrivateProfileInt(section, "ElfBufferDamageConstB", 0, path);
	this->m_ElfBufferDefenseConstA = GetPrivateProfileInt(section, "ElfBufferDefenseConstA", 0, path);
	this->m_ElfBufferDefenseConstB = GetPrivateProfileInt(section, "ElfBufferDefenseConstB", 0, path);

	char temp[100];
	GetPrivateProfileString(section, "ExperienceRate", "1", temp, sizeof(temp), path);

	this->m_ExperienceRate = _atoi64(temp);

	this->m_AddExperienceRate[0] = GetPrivateProfileInt(section, "AddExperienceRate_AL0", 0, path);
	this->m_AddExperienceRate[1] = GetPrivateProfileInt(section, "AddExperienceRate_AL1", 0, path);

	this->m_AddEventExperienceRate[0] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL0", 0, path);
	this->m_AddEventExperienceRate[1] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL1", 0, path);
	
	this->m_ItemDropTime = GetPrivateProfileInt(section, "ItemDropTime", 0, path);
	this->m_ItemDropRate[0] = GetPrivateProfileInt(section, "ItemDropRate_AL0", 0, path);
	this->m_ItemDropRate[1] = GetPrivateProfileInt(section, "ItemDropRate_AL1", 0, path);

	this->m_MoneyDropTime = GetPrivateProfileInt(section, "MoneyDropTime", 0, path);
	this->m_MoneyRate = GetPrivateProfileInt(section, "MoneyAmountDropRate", 0, path);
	this->m_MoneyAmountDropRate[0] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL0", 0, path);
	this->m_MoneyAmountDropRate[1] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL1", 0, path);

	this->m_WeaponDurabilityRate = GetPrivateProfileInt(section, "WeaponDurabilityRate", 0, path);
	this->m_ArmorDurabilityRate = GetPrivateProfileInt(section, "ArmorDurabilityRate", 0, path);
	this->m_WingDurabilityRate = GetPrivateProfileInt(section, "WingDurabilityRate", 0, path);
	this->m_GuardianDurabilityRate = GetPrivateProfileInt(section, "GuardianDurabilityRate", 0, path);
	this->m_PendantDurabilityRate = GetPrivateProfileInt(section, "PendantDurabilityRate", 0, path);
	this->m_RingDurabilityRate = GetPrivateProfileInt(section, "RingDurabilityRate", 0, path);
	this->m_PetDurabilityRate = GetPrivateProfileInt(section, "PetDurabilityRate", 0, path);

	this->m_TradeItemBlock = GetPrivateProfileInt(section, "TradeItemBlock", 0, path);
	this->m_TradeItemBlockExc = GetPrivateProfileInt(section, "TradeItemBlockExc", 0, path);
	this->m_TradeItemBlockSet = GetPrivateProfileInt(section, "TradeItemBlockSet", 0, path);
	this->m_TradeItemBlockHarmony = GetPrivateProfileInt(section, "TradeItemBlockHarmony", 0, path);
	this->m_TradeItemBlockSell = GetPrivateProfileInt(section, "TradeItemBlockSell", 0, path);
	this->m_MaxLevelUp = GetPrivateProfileInt(section, "MaxLevelUp", 0, path);
	this->m_MaxLevelUpEvent = GetPrivateProfileInt(section, "MaxLevelUpEvent", 0, path);

	this->m_MaxStatPoint[0] = GetPrivateProfileInt(section, "MaxStatPoint_AL0", 0, path);
	this->m_MaxStatPoint[1] = GetPrivateProfileInt(section, "MaxStatPoint_AL1", 0, path);

	this->m_LevelUpPoint[0] = GetPrivateProfileInt(section, "DWLevelUpPoint", 0, path);
	this->m_LevelUpPoint[1] = GetPrivateProfileInt(section, "DKLevelUpPoint", 0, path);
	this->m_LevelUpPoint[2] = GetPrivateProfileInt(section, "FELevelUpPoint", 0, path);
	this->m_LevelUpPoint[3] = GetPrivateProfileInt(section, "MGLevelUpPoint", 0, path);
	this->m_LevelUpPoint[4] = GetPrivateProfileInt(section, "DLLevelUpPoint", 0, path);

	this->m_PlusStatPoint = GetPrivateProfileInt(section, "PlusStatPoint", 0, path);
	this->m_CharacterCreateSwitch = GetPrivateProfileInt(section, "CharacterCreateSwitch", 0, path);
	this->m_MGCreateType = GetPrivateProfileInt(section, "MGCreateType", 0, path);
	this->m_DLCreateType = GetPrivateProfileInt(section, "DLCreateType", 0, path);

	this->m_MGCreateLevel[0] = GetPrivateProfileInt(section, "MGCreateLevel_AL0", 0, path);
	this->m_MGCreateLevel[1] = GetPrivateProfileInt(section, "MGCreateLevel_AL1", 0, path);

	this->m_DLCreateLevel[0] = GetPrivateProfileInt(section, "DLCreateLevel_AL0", 0, path);
	this->m_DLCreateLevel[1] = GetPrivateProfileInt(section, "DLCreateLevel_AL1", 0, path);

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

	this->m_LifeSuccessRate[0] = GetPrivateProfileInt(section, "LifeSuccessRate_AL0", 0, path);

	this->m_LifeSuccessRate[1] = GetPrivateProfileInt(section, "LifeSuccessRate_AL1", 0, path);

	this->m_HarmonySuccessRate[0] = GetPrivateProfileInt(section, "HarmonySuccessRate_AL0", 0, path);

	this->m_HarmonySuccessRate[1] = GetPrivateProfileInt(section, "HarmonySuccessRate_AL1", 0, path);

	this->m_SmeltStoneSuccessRate1[0] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate1_AL0", 0, path);

	this->m_SmeltStoneSuccessRate1[1] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate1_AL1", 0, path);

	this->m_SmeltStoneSuccessRate2[0] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate2_AL0", 0, path);

	this->m_SmeltStoneSuccessRate2[1] = GetPrivateProfileInt(section, "SmeltStoneSuccessRate2_AL1", 0, path);

	this->m_AddLuckSuccessRate1[0] = GetPrivateProfileInt(section, "AddLuckSuccessRate1_AL0", 0, path);

	this->m_AddLuckSuccessRate1[1] = GetPrivateProfileInt(section, "AddLuckSuccessRate1_AL1", 0, path);

	this->m_AddLuckSuccessRate2[0] = GetPrivateProfileInt(section, "AddLuckSuccessRate2_AL0", 0, path);

	this->m_AddLuckSuccessRate2[1] = GetPrivateProfileInt(section, "AddLuckSuccessRate2_AL1", 0, path);

	this->m_FruitAddPointMin = GetPrivateProfileInt(section, "FruitAddPointMin", 0, path);

	this->m_FruitAddPointMax = GetPrivateProfileInt(section, "FruitAddPointMax", 0, path);

	this->m_FruitAddPointSuccessRate[0] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL0", 0, path);

	this->m_FruitAddPointSuccessRate[1] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL1", 0, path);

	this->m_FruitSubPointMin = GetPrivateProfileInt(section, "FruitSubPointMin", 0, path);

	this->m_FruitSubPointMax = GetPrivateProfileInt(section, "FruitSubPointMax", 0, path);

	this->m_FruitSubPointSuccessRate[0] = GetPrivateProfileInt(section, "FruitSubPointSuccessRate_AL0", 0, path);

	this->m_FruitSubPointSuccessRate[1] = GetPrivateProfileInt(section, "FruitSubPointSuccessRate_AL1", 0, path);
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

	this->m_IllusionTempleEvent = GetPrivateProfileInt(section, "IllusionTempleEvent", 0, path);

	this->m_IllusionTempleMinUser = GetPrivateProfileInt(section, "IllusionTempleMinUser", 0, path);

	this->m_IllusionTempleRewardFenrirRate = GetPrivateProfileInt(section, "IllusionTempleRewardFenrirRate", 0, path);
	
	this->m_InvasionManagerSwitch = GetPrivateProfileInt(section, "InvasionManagerSwitch", 0, path);

	this->m_KanturuEvent = GetPrivateProfileInt(section, "KanturuEvent", 0, path);
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
