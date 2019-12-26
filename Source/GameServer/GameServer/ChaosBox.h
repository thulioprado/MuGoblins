// ChaosBox.h: interface for the CChaosMix class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ItemManager.h"
#include "Protocol.h"
#include "User.h"

#define MAX_TALISMAN_OF_LUCK 10

enum eChaosMixNumber
{
	CHAOS_MIX_CHAOS_ITEM = 1,
	CHAOS_MIX_DEVIL_SQUARE = 2,
	CHAOS_MIX_PLUS_ITEM_LEVEL1 = 3,
	CHAOS_MIX_PLUS_ITEM_LEVEL2 = 4,
	CHAOS_MIX_DINORANT = 5,
	CHAOS_MIX_FRUIT = 6,
	CHAOS_MIX_WING1 = 7,
	CHAOS_MIX_BLOOD_CASTLE = 8,
	CHAOS_MIX_WING2 = 11,
	CHAOS_MIX_PET1 = 13,
	CHAOS_MIX_PET2 = 14,
	CHAOS_MIX_SIEGE_POTION1 = 15,
	CHAOS_MIX_SIEGE_POTION2 = 16,
	CHAOS_MIX_LIFE_STONE = 17,
	CHAOS_MIX_SENIOR = 18,
	CHAOS_MIX_PLUS_ITEM_LEVEL3 = 22,
	CHAOS_MIX_PLUS_ITEM_LEVEL4 = 23,
	CHAOS_MIX_PRISM_RING = 24
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CHAOS_MIX_RECV
{
	PBMSG_HEAD header; // C1:86
	BYTE type;
	BYTE info;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CHAOS_MIX_SEND
{
	PBMSG_HEAD header; // C1:86
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CChaosBox
{
public:
	CChaosBox();
	virtual ~CChaosBox();
	void ChaosBoxInit(LPOBJ lpObj);
	void ChaosBoxItemDown(LPOBJ lpObj, int slot);
	void ChaosBoxItemKeep(LPOBJ lpObj, int slot);
	void ChaosBoxItemSave(LPOBJ lpObj);
	void ChaosItemMix(LPOBJ lpObj);
	void DevilSquareMix(LPOBJ lpObj);
	void PlusItemLevelMix(LPOBJ lpObj, int type);
	void DinorantMix(LPOBJ lpObj);
	void FruitMix(LPOBJ lpObj);
	void Wing2Mix(LPOBJ lpObj, int type);
	void BloodCastleMix(LPOBJ lpObj);
	void Wing1Mix(LPOBJ lpObj);
	void PetMix(LPOBJ lpObj, int type);
	void SiegePotionMix(LPOBJ lpObj, int type);
	void LifeStoneMix(LPOBJ lpObj);
	void SeniorMix(LPOBJ lpObj);
	void PrismRing(LPOBJ lpObj);
	void CGChaosMixRecv(PMSG_CHAOS_MIX_RECV* lpMsg, int aIndex);
	void CGChaosMixCloseRecv(int aIndex);
	void GCChaosBoxSend(LPOBJ lpObj, BYTE type);
	void GCChaosMixSend(int aIndex, BYTE result, CItem* lpItem);
public:
	int m_SeniorMixLimitDay;
	int m_SeniorMixLimitMonth;
	int m_SeniorMixLimitYear;
};

extern CChaosBox gChaosBox;
