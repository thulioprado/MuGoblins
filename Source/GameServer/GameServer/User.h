#pragma once

#include "ComboSkill.h"
#include "Effect.h"
#include "Item.h"
#include "MonsterAIAgro.h"
#include "MonsterSkillElementOption.h"
#include "Skill.h"

#if(GAMESERVER_TYPE==0)

#define MAX_OBJECT 10000
#define MAX_OBJECT_MONSTER 8000
#define MAX_OBJECT_USER 1000

#else

#define MAX_OBJECT 4500
#define MAX_OBJECT_MONSTER 2500
#define MAX_OBJECT_USER 1000

#endif

#define OBJECT_START_MONSTER 0
#define OBJECT_START_USER (MAX_OBJECT-MAX_OBJECT_USER)

#define MAX_SELF_DEFENSE 5
#define MAX_HIT_DAMAGE 40
#define MAX_VIEWPORT 75
#define MAX_GUILD_USER 80
#define MAX_MONEY 2000000000
#define MAX_CHECKSUM_KEY 1024
#define MAX_CHARACTER_LEVEL 10000
#define MAX_ACCOUNT_LEVEL 4
#define MAX_DAMAGE_REDUCTION 6
#define MAX_MESSAGE_SIZE 255
#define MAX_CHAT_MESSAGE_SIZE 59
#define MAX_ROAD_PATH_TABLE 16
#define MAX_MONSTER_SEND_MSG 20
#define MAX_MONSTER_SEND_ATTACK_MSG 30

#define OBJECT_RANGE(x) (((x)<0)?0:((x)>=MAX_OBJECT)?0:1)
#define OBJECT_MONSTER_RANGE(x) (((x)<OBJECT_START_MONSTER)?0:((x)>=MAX_OBJECT_MONSTER)?0:1)
#define OBJECT_SUMMON_RANGE(x) (((x)<MAX_OBJECT_MONSTER)?0:((x)>=OBJECT_START_USER)?0:1)
#define OBJECT_USER_RANGE(x) (((x)<OBJECT_START_USER)?0:((x)>=MAX_OBJECT)?0:1)
#define GET_MAX_RESISTANCE(x,y,z) (((((x)>(y))?(x):(y))>(z))?(((x)>(y))?(x):(y)):(z))
#define GET_MAX_WORD_VALUE(x) (((x)>65000)?65000:((WORD)(x)))
#define CHECK_RANGE(x,y) (((x)<0)?0:((x)>=(y))?0:1)

#define CHAR_SET_SIZE  24

enum eObjectConnectState
{
	OBJECT_OFFLINE = 0,
	OBJECT_CONNECTED = 1,
	OBJECT_LOGGED = 2,
	OBJECT_ONLINE = 3,
};

enum eObjectType
{
	OBJECT_NONE = 0,
	OBJECT_USER = 1,
	OBJECT_MONSTER = 2,
	OBJECT_NPC = 3,
	OBJECT_ITEM = 5,
};

enum eObjectState
{
	OBJECT_EMPTY = 0,
	OBJECT_CREATE = 1,
	OBJECT_PLAYING = 2,
	OBJECT_DYING = 4,
	OBJECT_DIECMD = 8,
	OBJECT_DIED = 16,
	OBJECT_DELCMD = 32,
};

enum eInterfaceType
{
	INTERFACE_NONE = 0,
	INTERFACE_TRADE = 1,
	INTERFACE_PARTY = 2,
	INTERFACE_SHOP = 3,
	INTERFACE_GUILD = 4,
	INTERFACE_GUILD_CREATE = 5,
	INTERFACE_WAREHOUSE = 6,
	INTERFACE_CHAOS_BOX = 7,
	INTERFACE_PERSONAL_SHOP = 8,
	INTERFACE_QUEST = 10,
	INTERFACE_TRANSFER = 11,
	INTERFACE_COMMON = 12,
	INTERFACE_TRAINER = 13,
	INTERFACE_CASTLE_SIEGE_WEAPON_OFFENSE = 15,
	INTERFACE_CASTLE_SIEGE_WEAPON_DEFENSE = 16
};

enum eActionType
{
	ACTION_ATTACK1 = 120,
	ACTION_ATTACK2 = 121,
	ACTION_STAND1 = 122,
	ACTION_STAND2 = 123,
	ACTION_MOVE1 = 124,
	ACTION_MOVE2 = 125,
	ACTION_DAMAGE1 = 126,
	ACTION_DIE1 = 127,
	ACTION_SIT1 = 128,
	ACTION_POSE1 = 129,
	ACTION_HEALING1 = 130,
	ACTION_GREETING1 = 131,
	ACTION_GOODBYE1 = 132,
	ACTION_CLAP1 = 133,
	ACTION_GESTURE1 = 134,
	ACTION_DIRECTION1 = 135,
	ACTION_UNKNOWN1 = 136,
	ACTION_CRY1 = 137,
	ACTION_CHEER1 = 138,
	ACTION_AWKWARD1 = 139,
	ACTION_SEE1 = 140,
	ACTION_WIN1 = 141,
	ACTION_SMILE1 = 142,
	ACTION_SLEEP1 = 143,
	ACTION_COLD1 = 144,
	ACTION_AGAIN1 = 145,
	ACTION_RESPECT1 = 146,
	ACTION_SALUTE1 = 147,
	ACTION_RUSH1 = 148,
	ACTION_SCISSORS = 149,
	ACTION_ROCK = 150,
	ACTION_PAPER = 151,
	ACTION_HUSTLE = 152,
	ACTION_PROVOCATION = 153,
	ACTION_LOOK_AROUND = 154,
	ACTION_CHEERS = 155,
	ACTION_HANDCLAP = 156,
	ACTION_POINTDANCE = 157,
	ACTION_JACK1 = 158,
	ACTION_JACK2 = 159,
	ACTION_SANTA1_1 = 160,
	ACTION_SANTA1_2 = 161,
	ACTION_SANTA1_3 = 162,
	ACTION_SANTA2_1 = 163,
	ACTION_SANTA2_2 = 164,
	ACTION_SANTA2_3 = 165,
	ACTION_RAGEBUFF_1 = 166,
	ACTION_RAGEBUFF_2 = 167,
	ACTION_PICKAXE = 168,
};

enum eDamageReductionType
{
	DAMAGE_REDUCTION_EXCELLENT_ITEM = 0,
	DAMAGE_REDUCTION_SET_ITEM = 1,
	DAMAGE_REDUCTION_JOH_ITEM = 2,
	DAMAGE_REDUCTION_380_ITEM = 3,
	DAMAGE_REDUCTION_MASTER_SKILL_TREE = 4,
	DAMAGE_REDUCTION_SOCKET_ITEM = 5,
};

enum eExperienceType
{
	EXPERIENCE_COMMON = 0,
	EXPERIENCE_PARTY = 1,
	EXPERIENCE_QUEST = 2,
	EXPERIENCE_DEVIL_SQUARE = 3,
	EXPERIENCE_BLOOD_CASTLE = 4,
	EXPERIENCE_CHAOS_CASTLE = 5,
	EXPERIENCE_CRYWOLF = 6,
	EXPERIENCE_ILLUSION_TEMPLE = 7,
	EXPERIENCE_IMPERIAL_GUARDIAN = 8,
};

struct MESSAGE_STATE_MACHINE
{
	MESSAGE_STATE_MACHINE() // OK
	{
		this->Clear();
	}

	void Clear() // OK
	{
		this->MsgCode = -1;
		this->SendUser = -1;
		this->MsgTime = 0;
		this->SubCode = 0;
	}

	int MsgCode;
	int SendUser;
	int MsgTime;
	int SubCode;
};

struct MESSAGE_STATE_MACHINE_COMMON
{
	MESSAGE_STATE_MACHINE_COMMON() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_MACHINE;

		for(int n=0;n < MAX_MONSTER_SEND_MSG;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_MACHINE& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_MACHINE* CommonStruct;
	MESSAGE_STATE_MACHINE* ObjectStruct[MAX_MONSTER_SEND_MSG];
};

struct MESSAGE_STATE_MACHINE_HEADER
{
	MESSAGE_STATE_MACHINE_HEADER() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_MACHINE_COMMON;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_MACHINE_COMMON& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_MACHINE_COMMON* CommonStruct;
	MESSAGE_STATE_MACHINE_COMMON* ObjectStruct[MAX_OBJECT];
};

struct MESSAGE_STATE_ATTACK_MACHINE
{
	MESSAGE_STATE_ATTACK_MACHINE() // OK
	{
		this->Clear();
	}

	void Clear() // OK
	{
		this->MsgCode = -1;
		this->SendUser = -1;
		this->MsgTime = 0;
		this->SubCode = 0;
		this->SubCode2 = 0;
	}

	int MsgCode;
	int SendUser;
	int MsgTime;
	int SubCode;
	int SubCode2;
};

struct MESSAGE_STATE_ATTACK_MACHINE_COMMON
{
	MESSAGE_STATE_ATTACK_MACHINE_COMMON() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_ATTACK_MACHINE;

		for(int n=0;n < MAX_MONSTER_SEND_ATTACK_MSG;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_ATTACK_MACHINE& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_ATTACK_MACHINE* CommonStruct;
	MESSAGE_STATE_ATTACK_MACHINE* ObjectStruct[MAX_MONSTER_SEND_ATTACK_MSG];
};

struct MESSAGE_STATE_ATTACK_MACHINE_HEADER
{
	MESSAGE_STATE_ATTACK_MACHINE_HEADER() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_ATTACK_MACHINE_COMMON;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_ATTACK_MACHINE_COMMON& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_ATTACK_MACHINE_COMMON* CommonStruct;
	MESSAGE_STATE_ATTACK_MACHINE_COMMON* ObjectStruct[MAX_OBJECT];
};

struct EFFECT_OPTION
{
	int AddPhysiDamage;
	int AddMagicDamage;
	int AddCurseDamage;
	int AddMinPhysiDamage;
	int AddMaxPhysiDamage;
	int AddMinMagicDamage;
	int AddMaxMagicDamage;
	int AddMinCurseDamage;
	int AddMaxCurseDamage;
	int AddAttackSuccessRate;
	int AddPhysiSpeed;
	int AddMagicSpeed;
	int AddDefense;
	int AddDefenseSuccessRate;
	int AddMaxHP;
	int AddMaxMP;
	int AddMaxBP;
	int AddMaxSD;
	int AddStrength;
	int AddDexterity;
	int AddVitality;
	int AddEnergy;
	int AddLeadership;
	int AddDamageReduction;
	int AddDamageReflect;
	int AddCriticalDamageRate;
	int AddCriticalDamage;
	int AddExcellentDamageRate;
	int AddExcellentDamage;
	int AddExperienceRate;
	int AddMasterExperienceRate;
	int AddVolcanoExperienceRate;
	int AddPartyBonusExperienceRate;
	int AddIgnoreDefenseRate;
	int AddItemDropRate;
	int AddHPRecovery;
	int AddMPRecovery;
	int AddBPRecovery;
	int AddSDRecovery;
	int AddHPRecoveryRate;
	int AddMPRecoveryRate;
	int AddBPRecoveryRate;
	int AddSDRecoveryRate;
	int AddSwordPowerDamageRate;
	int AddSwordPowerDefenseRate;
	int SubDefense;
	int MulPhysiDamage;
	int MulMagicDamage;
	int MulCurseDamage;
	int MulAttackSuccessRate;
	int MulPhysiSpeed;
	int MulMagicSpeed;
	int MulDefense;
	int MulDefenseSuccessRate;
	int MulMaxHP;
	int MulMaxMP;
	int MulMaxBP;
	int MulMaxSD;
	int DivPhysiDamage;
	int DivMagicDamage;
	int DivCurseDamage;
	int DivAttackSuccessRate;
	int DivPhysiSpeed;
	int DivMagicSpeed;
	int DivDefense;
	int DivDefenseSuccessRate;
	int DivMaxHP;
	int DivMaxMP;
	int DivMaxBP;
	int DivMaxSD;
};

struct PENTAGRAM_OPTION
{
	int MulPentagramDamage;
	int MulPentagramDefense;
	int AddElementalCriticalDamageRate;
	int AddElementalDefenseTransferRate;
	int AddElementalAttackTransferRate;
	int MulElementalDefenseSuccessRate;
	int MulElementalAttackSuccessRate;
};

struct PENTAGRAM_JEWEL_OPTION
{
	int AddElementalDamage;
	int AddElementalDamagePvP;
	int AddElementalDamagePvM;
	int AddElementalDamageRange;
	int AddElementalDamageMelee;
	int MulElementalDamagePvP;
	int MulElementalDamagePvM;
	int MulElementalDamageVsFire;
	int MulElementalDamageVsWater;
	int MulElementalDamageVsEarth;
	int MulElementalDamageVsWind;
	int MulElementalDamageVsDark;
	int AddElementalDefense;
	int AddElementalDefensePvP;
	int AddElementalDefensePvM;
	int AddElementalDefenseRange;
	int AddElementalDefenseMelee;
	int MulElementalDefenseVsFire;
	int MulElementalDefenseVsWater;
	int MulElementalDefenseVsEarth;
	int MulElementalDefenseVsWind;
	int MulElementalDefenseVsDark;
	int MulElementalAttackSuccessRate;
	int MulElementalDefenseSuccessRate;
	int AddElementalDamageReductionPvP;
	int AddElementalDamageReductionPvM;
	int AddElementalSlowRate;
	int AddElementalDebuffRate;
	int AddElementalCriticalDamageRatePvP;
	int AddElementalCriticalDamageRatePvM;
	int AddElementalExcellentDamageRatePvP;
	int AddElementalExcellentDamageRatePvM;
};

struct ACTION_STATE
{
	DWORD Rest:1;
	DWORD Attack:1;
	DWORD Move:1;
	DWORD Escape:1;
	DWORD Emotion:4;
	DWORD EmotionCount:8;
};

struct VIEWPORT_STRUCT
{
	#pragma pack(1)
	BYTE state;
	shrt index;
	BYTE type;
	#pragma pack()
};

struct HIT_DAMAGE_STRUCT
{
	int index;
	int damage;
	DWORD time;
};

struct INTERFACE_STATE
{
	DWORD use:2;
	DWORD state:4;
	DWORD type:10;
};

struct SKILL_OPTION_DATA
{
	BYTE SkillKey[10];
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
};

struct OBJECTSTRUCT
{
	int Index;
	int Connected;
	char LoginMessageSend;
	char LoginMessageCount;
	char CloseCount;
	char CloseType;
	BOOL EnableDelCharacter;
	struct PER_SOCKET_CONTEXT* PerSocketContext;
	SOCKET Socket;
	char IpAddr[16];
	BYTE ClassCode;
	BYTE ClassFlag;
	DWORD AutoSaveTime;
	DWORD ConnectTickCount;
	DWORD ClientTickCount;
	DWORD ServerTickCount;
	BYTE CheckTickCount;
	DWORD PostTime;
	DWORD TimeCount;
	DWORD PKTickCount;
	short CheckSumTableNum;
	DWORD CheckSumTime;
	WORD Type;
	BYTE Live;
	char Account[11];
	char Name[11];
	char PersonalCode[14];
	CComboSkill ComboSkill;
	WORD Class;
	BYTE DBClass;
	BYTE ChangeUp;
	short Level;
	int LevelUpPoint;
	WORD FruitAddPoint;
	WORD FruitSubPoint;
	QWORD Experience;
	QWORD NextExperience;
	DWORD Money;
	int Strength;
	int Dexterity;
	int Vitality;
	int Energy;
	int Leadership;
	float Life;
	float MaxLife;
	float ScriptMaxLife;
	float Mana;
	float MaxMana;
	WORD ChatLimitTime;
	BYTE ChatLimitTimeSec;
	int AddStrength;
	int AddDexterity;
	int AddVitality;
	int AddEnergy;
	int AddLeadership;
	int BP;
	int MaxBP;
	int AddBP;
	float VitalityToLife;
	float EnergyToMana;
	char PKCount;
	char PKLevel;
	int PKTime;
	short X;
	short Y;
	BYTE Dir;
	BYTE Map;
	int AddLife;
	int AddMana;
	BYTE StartX;
	BYTE StartY;
	short OldX;
	short OldY;
	short TX;
	short TY;
	short MTX;
	short MTY;
	int PathCount;
	int PathCur;
	char PathStartEnd;
	short PathOri[15];
	short PathX[15];
	short PathY[15];
	char PathDir[15];
	DWORD PathTime;
	DWORD Authority;
	DWORD AuthorityCode;
	DWORD Penalty;
	BYTE AccountItemBlock;
	ACTION_STATE ActionState;
	BYTE ActionNumber;
	DWORD State;
	BYTE Rest;
	BYTE ViewState;
	DWORD ViewSkillState;
	DWORD LastMoveTime;
	DWORD LastAttackTime;
	DWORD TeleportTime;
	char Teleport;
	char KillerType;
	char DieRegen;
	char RegenOk;
	BYTE RegenMapNumber;
	BYTE RegenMapX;
	BYTE RegenMapY;
	DWORD RegenTime;
	DWORD MaxRegenTime;
	short PosNum;
	DWORD CurActionTime;
	DWORD NextActionTime;
	DWORD DelayActionTime;
	char DelayLevel;
	int DrinkSpeed;
	DWORD DrinkLastTime;
	DWORD MonsterDeleteTime;
	char KalimaGateExist;
	int KalimaGateIndex;
	char KalimaGateEnterCount;
	OBJECTSTRUCT* AttackObj;
	bool AttackerKilled;
	int SelfDefense[MAX_SELF_DEFENSE];
	DWORD SelfDefenseTime[MAX_SELF_DEFENSE];
	DWORD MySelfDefenseTime;
	int PartyNumber;
	int PartyTargetUser;
	int GuildNumber;
	struct GUILD_INFO_STRUCT* Guild;
	char GuildName[11];
	int GuildStatus;
	int GuildUnionTimeStamp;
	int SummonIndex;
	int Change;
	short TargetNumber;
	short TargetShopNumber;
	short ShopNumber;
	short LastAttackerID;
	int PhysiDamageMin;
	int PhysiDamageMax;
	int MagicDamageMin;
	int MagicDamageMax;
	int CurseDamageMin;
	int CurseDamageMax;
	int PhysiDamageMaxLeft;
	int PhysiDamageMinLeft;
	int PhysiDamageMaxRight;
	int PhysiDamageMinRight;
	int DKDamageMultiplierRate;
	int DLDamageMultiplierRate;
	int RFDamageMultiplierRate[3];
	int AttackSuccessRate;
	int PhysiSpeed;
	int MagicSpeed;
	int Defense;
	int MagicDefense;
	int DefenseSuccessRate;
	short MoveSpeed;
	short MoveRange;
	short AttackRange;
	short AttackType;
	short ViewRange;
	short Attribute;
	short ItemRate;
	short MoneyRate;
	CSkill* SkillBackup;
	CSkill* Skill;
	WORD MultiSkillIndex;
	WORD MultiSkillCount;
	BYTE CharSet[CHAR_SET_SIZE];
	BYTE Resistance[MAX_RESISTANCE_TYPE];
	BYTE AddResistance[MAX_RESISTANCE_TYPE];
	VIEWPORT_STRUCT* VpPlayer;
	VIEWPORT_STRUCT* VpPlayer2;
	VIEWPORT_STRUCT* VpPlayerItem;
	int VPCount;
	int VPCount2;
	int VPCountItem;
	HIT_DAMAGE_STRUCT* HitDamage;
	short HitDamageCount;
	INTERFACE_STATE Interface;
	DWORD InterfaceTime;
	char Transaction;
	CItem* Inventory;
	CItem* Inventory1;
	CItem* Inventory2;
	BYTE* InventoryMap;
	BYTE* InventoryMap1;
	BYTE* InventoryMap2;
	CItem* Trade;
	BYTE* TradeMap;
	int TradeMoney;
	bool TradeOk;
	CItem* Warehouse;
	BYTE* WarehouseMap;
	char WarehouseCount;
	short WarehousePW;
	BYTE WarehouseLock;
	int WarehouseMoney;
	int WarehouseSave;
	CItem* ChaosBox;
	BYTE* ChaosBoxMap;
	int ChaosMoney;
	int ChaosSuccessRate;
	int ChaosLock;

	DWORD Option;
	int ChaosCastleBlowTime;
	int DuelUserReserved;
	int DuelUserRequested;
	int DuelUser;
	BYTE DuelScore;
	DWORD DuelTickCount;
	bool PShopOpen;
	bool PShopTransaction;
	bool PShopItemChange;
	bool PShopRedrawAbs;
	char PShopText[36];
	bool PShopWantDeal;
	int PShopDealerIndex;
	char PShopDealerName[10];
	int PShopCustom;
	int PShopCustomType;
	int PShopCustomOffline;
	int PShopCustomOfflineTime;
	CRITICAL_SECTION PShopTrade;
	int VpPShopPlayer[MAX_VIEWPORT];
	int VpPShopPlayerCount;
	bool IsInBattleGround;
	bool HaveWeaponInHand;
	bool UseEventServer;
	int LoadWarehouse;
	BYTE Quest[50];
	bool SendQuestInfo;
	int CheckLifeTime;
	BYTE LastTeleportTime;
	BYTE SkillNovaState;
	BYTE SkillNovaCount;
	DWORD SkillNovaTime;
	BYTE ReqWarehouseOpen;
	bool IsFullSetItem;
	WORD SkillSummonPartyTime;
	BYTE SkillSummonPartyMap;
	BYTE SkillSummonPartyX;
	BYTE SkillSummonPartyY;
	bool IsChaosMixCompleted;
	bool SkillLongSpearChange;
	DWORD CharSaveTime;
	int LoadQuestKillCount;
	int QuestKillCountIndex;
	struct QUEST_KILL_COUNT* QuestKillCount;
	CEffect* Effect;
	DWORD* SkillDelay;
	DWORD* HackPacketDelay;
	DWORD* HackPacketCount;
	int ExtInventory;
	int ExtWarehouse;
	int WarehouseNumber;
	int AccountLevel;
	char AccountExpireDate[20];
	char AutoPartyPassword[11];
	int AutoAddPointCount;
	int AutoAddPointStats[5];
	int AutoResetEnable;
	int AutoResetStats[5];
	int AttackCustom;
	int AttackCustomSkill;
	int AttackCustomDelay;
	int AttackCustomZoneX;
	int AttackCustomZoneY;
	int AttackCustomZoneMap;
	int AttackCustomOffline;
	int AttackCustomOfflineTime;
	int AttackCustomOfflineMoneyDelay;
	int CommandManagerTransaction[1];
	DWORD AutoAttackTime;
	DWORD PotionTime;
	DWORD ComboTime;
	DWORD HPAutoRecuperationTime;
	DWORD MPAutoRecuperationTime;
	DWORD BPAutoRecuperationTime;
	int Reset;
	int MasterReset;
	int ChangeSkin;
	EFFECT_OPTION EffectOption;
	int ArmorSetBonus;
	int SkillDamageBonus;
	int DoubleDamageRate;
	int TripleDamageRate;
	int IgnoreDefenseRate;
	int IgnoreShieldGaugeRate;
	int CriticalDamageRate;
	int CriticalDamage;
	int ExcellentDamageRate;
	int ExcellentDamage;
	int ResistDoubleDamageRate;
	int ResistIgnoreDefenseRate;
	int ResistIgnoreShieldGaugeRate;
	int ResistCriticalDamageRate;
	int ResistExcellentDamageRate;
	int ResistStunRate;
	int ExperienceRate;
	int MasterExperienceRate;
	int ItemDropRate;
	int MoneyAmountDropRate;
	int HPRecovery;
	int MPRecovery;
	int BPRecovery;
	int SDRecovery;
	int HPRecoveryRate;
	int MPRecoveryRate;
	int BPRecoveryRate;
	int SDRecoveryRate;
	int SDRecoveryType;
	int HPRecoveryCount;
	int MPRecoveryCount;
	int BPRecoveryCount;
	int MPConsumptionRate;
	int BPConsumptionRate;
	int ShieldGaugeRate;
	int DecreaseShieldGaugeRate;
	int DamagePvP;
	int DefensePvP;
	int AttackSuccessRatePvP;
	int DefenseSuccessRatePvP;
	int ShieldDamageReduction;
	int ShieldDamageReductionTime;
	int DamageReduction[MAX_DAMAGE_REDUCTION];
	int DamageReflect;
	int HuntHP;
	int HuntMP;
	int HuntBP;
	int HuntSD;
	int WeaponDurabilityRate;
	int ArmorDurabilityRate;
	int WingDurabilityRate;
	int GuardianDurabilityRate;
	int PendantDurabilityRate;
	int RingDurabilityRate;
	int PetDurabilityRate;
	int FullDamageReflectRate;
	int DefensiveFullHPRestoreRate;
	int DefensiveFullMPRestoreRate;
	int DefensiveFullSDRestoreRate;
	int DefensiveFullBPRestoreRate;
	int OffensiveFullHPRestoreRate;
	int OffensiveFullMPRestoreRate;
	int OffensiveFullSDRestoreRate;
	int OffensiveFullBPRestoreRate;
	int ResurrectionTalismanActive;
	int ResurrectionTalismanMap;
	int ResurrectionTalismanX;
	int ResurrectionTalismanY;
	int MobilityTalismanActive;
	int MobilityTalismanMap;
	int MobilityTalismanX;
	int MobilityTalismanY;
	bool MapServerMoveQuit;
	bool MapServerMoveRequest;
	DWORD MapServerMoveQuitTickCount;
	short NextServerCode;
	short LastServerCode;
	short DestMap;
	BYTE DestX;
	BYTE DestY;
	#if(GAMESERVER_TYPE==1)
	union
	{
		struct
		{
			BYTE CsNpcExistVal1;
			BYTE CsNpcExistVal2;
			BYTE CsNpcExistVal3;
			BYTE CsNpcExistVal4;
		};

		int CsNpcExistVal;
	};
	BYTE CsNpcType;
	BYTE CsGateOpen;
	int CsGateLeverLinkIndex;
	BYTE CsNpcDfLevel;
	BYTE CsNpcRgLevel;
	BYTE CsJoinSide;
	bool CsGuildInvolved;
	#endif
	bool IsCastleNPCUpgradeCompleted;
	BYTE CsSiegeWeaponState;
	int CsWeaponIndex;
	BYTE KillCount;
	int AccumulatedDamage;
	#if(GAMESERVER_TYPE==1)
	BYTE LifeStoneCount;
	BYTE CreationState;
	int CreatedActivationTime;
	#endif
	int AccumulatedCrownAccessTime;
	CMonsterSkillElementOption MonsterSkillElementOption;
	int BasicAI;
	int CurrentAI;
	int CurrentAIState;
	int LastAIRunTime;
	int GroupNumber;
	int SubGroupNumber;
	int GroupMemberGuid;
	int RegenType;
	CMonsterAIAgro Agro;
	int LastAutomataRuntime;
	int LastAutomataDelay;
	int CrywolfMVPScore;
	DWORD LastCheckTick;
	SKILL_OPTION_DATA SkillOptionData;
};

struct OBJECTSTRUCT_HEADER
{
	OBJECTSTRUCT_HEADER() // OK
	{
		this->CommonStruct = new OBJECTSTRUCT;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	OBJECTSTRUCT& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	OBJECTSTRUCT* CommonStruct;
	OBJECTSTRUCT* ObjectStruct[MAX_OBJECT];
};

typedef OBJECTSTRUCT* LPOBJ;

extern OBJECTSTRUCT_HEADER gObj;

extern MESSAGE_STATE_MACHINE_HEADER gSMMsg;

extern MESSAGE_STATE_ATTACK_MACHINE_HEADER gSMAttackProcMsg;

extern DWORD gCheckSum[MAX_CHECKSUM_KEY];

extern QWORD gLevelExperience[MAX_CHARACTER_LEVEL+1];

//**************************************************************************//
// OBJECT MAIN FUNCTIONS ***************************************************//
//**************************************************************************//
void gObjEventRunProc();
void gObjViewportProc();
void gObjFirstProc();
void gObjCloseProc();
void gObjCountProc();
void gObjAccountLevelProc();
void gObjMathAuthenticatorProc();
//**************************************************************************//
// OBJECT BASE FUNCTIONS ***************************************************//
//**************************************************************************//
void gObjInit();
void gObjAllLogOut();
void gObjAllDisconnect();
void gObjSetExperienceTable();
void gObjCharZeroSet(int aIndex);
void gObjClearPlayerOption(LPOBJ lpObj);
void gObjClearSpecialOption(LPOBJ lpObj);
void gObjCalcExperience(LPOBJ lpObj);
bool gObjGetRandomFreeLocation(int map,int* ox,int* oy,int tx,int ty,int count);
bool gObjAllocData(int aIndex);
void gObjFreeData(int aIndex);
short gObjAddSearch(SOCKET socket,char* IpAddress);
short gObjAdd(SOCKET socket,char* IpAddress,int aIndex);
short gObjDel(int aIndex);
LPOBJ gObjFind(char* name);
int gObjCalcDistance(LPOBJ lpObj,LPOBJ lpTarget);
//**************************************************************************//
// OBJECT CHECK FUNCTIONS **************************************************//
//**************************************************************************//
bool gObjIsConnected(int aIndex);
bool gObjIsConnectedGP(int aIndex);
bool gObjIsConnectedGS(int aIndex);
bool gObjIsNameValid(int aIndex,char* name);
bool gObjIsAccountValid(int aIndex,char* account);
bool gObjIsChangeSkin(int aIndex);
bool gObjCheckMaxMoney(int aIndex,DWORD AddMoney);
bool gObjCheckPersonalCode(int aIndex,char* PersonalCode);
bool gObjCheckResistance(LPOBJ lpObj,int type);
bool gObjCheckTeleportArea(int aIndex,int x,int y);
bool gObjCheckMapTile(LPOBJ lpObj,int type);
//**************************************************************************//
// ITEM TRANSACTION FUNCTIONS **********************************************//
//**************************************************************************//
bool gObjFixInventoryPointer(int aIndex);
void gObjSetInventory1Pointer(LPOBJ lpObj);
void gObjSetInventory2Pointer(LPOBJ lpObj);
bool gObjFixEventInventoryPointer(int aIndex);
void gObjSetEventInventory1Pointer(LPOBJ lpObj);
void gObjSetEventInventory2Pointer(LPOBJ lpObj);
bool gObjInventoryTransaction(int aIndex);
bool gObjInventoryCommit(int aIndex);
bool gObjInventoryRollback(int aIndex);
//**************************************************************************//
// VIEWPORT FUNCTIONS ******************************************************//
//**************************************************************************//
void gObjSetViewport(int aIndex,int state);
void gObjClearViewport(LPOBJ lpObj);
void gObjViewportListProtocolDestroy(LPOBJ lpObj);
void gObjViewportListProtocolCreate(LPOBJ lpObj);
void gObjViewportListProtocol(int aIndex);
void gObjViewportListDestroy(int aIndex);
void gObjViewportListCreate(int aIndex);
//**************************************************************************//
// USER FUNCTIONS **********************************************************//
//**************************************************************************//
void gObjSetKillCount(int aIndex,int type);
void gObjTeleportMagicUse(int aIndex,int x,int y);
void gObjInterfaceCheckTime(LPOBJ lpObj);
void gObjSkillNovaCheckTime(LPOBJ lpObj);
void gObjPKDownCheckTime(LPOBJ lpObj,int TargetLevel);
void gObjUserDie(LPOBJ lpObj,LPOBJ lpTarget);
void gObjPlayerKiller(LPOBJ lpObj,LPOBJ lpTarget);
BOOL gObjMoveGate(int aIndex,int gate);
void gObjTeleport(int aIndex,int map,int x,int y);
void gObjSummonAlly(LPOBJ lpObj,int map,int x,int y);
void gObjSkillUseProc(LPOBJ lpObj);
void gObjUserKill(int aIndex);
bool gObjInventorySearchSerialNumber(LPOBJ lpObj,DWORD serial);
bool gObjWarehouseSearchSerialNumber(LPOBJ lpObj,DWORD serial);
void gObjAddMsgSend(LPOBJ lpObj,int MsgCode,int SendUser,int SubCode);
void gObjAddMsgSendDelay(LPOBJ lpObj,int MsgCode,int SendUser,int MsgTimeDelay,int SubCode);
void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj,int MsgCode,int SendUser,int MsgTimeDelay,int SubCode,int SubCode2);

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

void gObjSecondProc();
void gObjDelayLifeCheck(int aIndex);
BOOL gObjBackSpring(LPOBJ lpObj,LPOBJ lpTarget);
BOOL gObjBackSpring2(LPOBJ lpObj,LPOBJ lpTarget,int count);
bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjTimeCheckSelfDefense(LPOBJ lpObj);
BOOL gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjGuildWarEndSend(LPOBJ lpObj, BYTE Result1, BYTE Result2);
void gObjGuildWarEndSend(GUILD_INFO_STRUCT * lpGuild1, GUILD_INFO_STRUCT * lpGuild2, BYTE Result1, BYTE Result2);
void gObjGuildWarEnd(GUILD_INFO_STRUCT * lpGuild, GUILD_INFO_STRUCT * lpTargetGuild);
BOOL gObjGuildWarProc(GUILD_INFO_STRUCT * lpGuild1, GUILD_INFO_STRUCT * lpGuild2, int score);
BOOL gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
BOOL gObjGuildWarMasterClose(LPOBJ lpObj);
int gObjGuildWarItemGive(GUILD_INFO_STRUCT * lpWinGuild, GUILD_INFO_STRUCT * lpLoseGuild);
void gObjSetPosition(int aIndex, int x, int y);
void gObjAuthorityCodeSet(LPOBJ lpObj);
int gObjGetGuildUnionNumber(LPOBJ lpObj);
void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen);
BOOL gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjNotifyUpdateUnionV1(LPOBJ lpObj);
void gObjNotifyUpdateUnionV2(LPOBJ lpObj);
void gObjUnionUpdateProc(int iIndex);
void gObjUseDrink(LPOBJ lpObj,int level);
void gObjCustomLogPlusChaosMix(LPOBJ lpObj,int type,int index);
bool gObjCheckAutoParty(LPOBJ lpObj,LPOBJ lpTarget);

extern int gObjCount;
extern int gObjMonCount;
extern int gObjCallMonCount;
extern int gObjTotalUser;
extern int gObjTotalMonster;
extern int gCloseMsg;
extern int gCloseMsgTime;
extern int gGameServerLogOut;
extern int gGameServerDisconnect;
