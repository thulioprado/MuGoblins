// SkillManager.h: interface for the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"
#include "Protocol.h"
#include "User.h"

#if(GAMESERVER_UPDATE>=602)
#define MAX_SKILL 618
#else
#define MAX_SKILL 555
#endif
#if(GAMESERVER_UPDATE>=701)
#define MAX_SKILL_LIST 60
#define MAX_MASTER_SKILL_LIST 120
#else
#define MAX_SKILL_LIST 60
#define MAX_MASTER_SKILL_LIST 60
#endif

#define CHECK_SKILL_ATTACK_COUNT(x) (((++x)>=10)?0:1)
#define CHECK_SKILL_ATTACK_EXTENDED_COUNT(x) (((++x)>=20)?0:1)

enum eSkillNumber
{
	SKILL_NONE = 0,
	SKILL_POISON = 1,
	SKILL_METEORITE = 2,
	SKILL_LIGHTNING = 3,
	SKILL_FIRE_BALL = 4,
	SKILL_FLAME = 5,
	SKILL_TELEPORT = 6,
	SKILL_ICE = 7,
	SKILL_TWISTER = 8,
	SKILL_EVIL_SPIRIT = 9,
	SKILL_HELL_FIRE = 10,
	SKILL_POWER_WAVE = 11,
	SKILL_AQUA_BEAM = 12,
	SKILL_BLAST = 13,
	SKILL_INFERNO = 14,
	SKILL_TELEPORT_ALLY = 15,
	SKILL_MANA_SHIELD = 16,
	SKILL_ENERGY_BALL = 17,
	SKILL_DEFENSE = 18,
	SKILL_FALLING_SLASH = 19,
	SKILL_LUNGE = 20,
	SKILL_UPPERCUT = 21,
	SKILL_CYCLONE = 22,
	SKILL_SLASH = 23,
	SKILL_TRIPLE_SHOT = 24,
	SKILL_HEAL = 26,
	SKILL_GREATER_DEFENSE = 27,
	SKILL_GREATER_DAMAGE = 28,
	SKILL_SUMMON1 = 30,
	SKILL_SUMMON2 = 31,
	SKILL_SUMMON3 = 32,
	SKILL_SUMMON4 = 33,
	SKILL_SUMMON5 = 34,
	SKILL_SUMMON6 = 35,
	SKILL_SUMMON7 = 36,
	SKILL_DECAY = 38,
	SKILL_ICE_STORM = 39,
	SKILL_NOVA = 40,
	SKILL_TWISTING_SLASH = 41,
	SKILL_RAGEFUL_BLOW = 42,
	SKILL_DEATH_STAB = 43,
	SKILL_CRESCENT_MOON_SLASH = 44,
	SKILL_MANA_GLAIVE = 45,
	SKILL_STAR_FALL = 46,
	SKILL_IMPALE = 47,
	SKILL_GREATER_LIFE = 48,
	SKILL_FIRE_BREATH = 49,
	SKILL_MONSTER_AREA_ATTACK = 50,
	SKILL_ICE_ARROW = 51,
	SKILL_PENETRATION = 52,
	SKILL_FIRE_SLASH = 55,
	SKILL_POWER_SLASH = 56,
	SKILL_SPIRAL_SLASH = 57,
	SKILL_NOVA_START = 58,
	SKILL_COMBO = 59,
	SKILL_FORCE = 60,
	SKILL_FIRE_BURST = 61,
	SKILL_EARTHQUAKE = 62,
	SKILL_SUMMON_PARTY = 63,
	SKILL_GREATER_CRITICAL_DAMAGE = 64,
	SKILL_ELECTRIC_SPARK = 65,
	SKILL_FORCE_WAVE = 66,
	SKILL_STERN = 67,
	SKILL_REMOVE_STERN = 68,
	SKILL_GREATER_MANA = 69,
	SKILL_INVISIBILITY = 70,
	SKILL_REMOVE_INVISIBILITY = 71,
	SKILL_REMOVE_ALL_EFFECT = 72,
	SKILL_MANA_RAYS = 73,
	SKILL_FIRE_BLAST = 74,
	SKILL_BRAND = 75,
	SKILL_FIRE_SCREAM = 78,
	SKILL_EXPLOSION = 79,
	SKILL_MONSTER_SUMMON = 200,
	SKILL_MAGIC_DAMAGE_IMMUNITY = 201,
	SKILL_PHYSI_DAMAGE_IMMUNITY = 202,
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_MULTI_SKILL_ATTACK_RECV
{
	PBMSG_HEAD header; // C3:[PROTOCOL_CODE4]
	BYTE skill;
	BYTE x;
	BYTE y;
	BYTE serial;
	BYTE count;
};

struct PMSG_MULTI_SKILL_ATTACK
{
	BYTE index[2];
	BYTE MagicKey;
};

struct PMSG_SKILL_ATTACK_RECV
{
	PBMSG_HEAD header; // C3:19
	BYTE skill;
	BYTE index[2];
	BYTE dis;
};

struct PMSG_SKILL_CANCEL_RECV
{
	PBMSG_HEAD header; // C3:1B
	BYTE skill;
	BYTE index[2];
};

struct PMSG_DURATION_SKILL_ATTACK_RECV
{
	PBMSG_HEAD header; // C3:1E
	BYTE skill;
	BYTE x;
	BYTE y;
	BYTE dir;
	BYTE dis;
	BYTE angle;
	BYTE index[2];
	BYTE MagicKey;
};

struct PMSG_SKILL_TELEPORT_ALLY_RECV
{
	PBMSG_HEAD header; // C3:B0
	WORD index;
	BYTE x;
	BYTE y;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_SKILL_ATTACK_SEND
{
	PBMSG_HEAD header; // C3:19
	BYTE skill;
	BYTE index[2];
	BYTE target[2];
};

struct PMSG_SKILL_CANCEL_SEND
{
	PBMSG_HEAD header; // C1:1B
	BYTE skill;
	BYTE index[2];
};

struct PMSG_DURATION_SKILL_ATTACK_SEND
{
	PBMSG_HEAD header; // C3:1E
	BYTE skill;
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE dir;
};

struct PMSG_SKILL_NOVA_SEND
{
	PBMSG_HEAD header; // C1:BA
	BYTE index[2];
	BYTE type;
	BYTE count;
};

struct PMSG_SKILL_LIST_SEND
{
	PSBMSG_HEAD header; // C1:F3:11
	BYTE count;
	BYTE type;
};

struct PMSG_SKILL_LIST
{
	BYTE slot;
	BYTE skill;
	BYTE level;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct SKILL_INFO
{
	char Name[32];
	int Damage;
	int Mana;
	int BP;
	int Range;
	int Radio;
	int Delay;
	int Type;
	int Effect;
	int RequireLevel;
	int RequireEnergy;
	int RequireLeadership;
	int RequireKillCount;
	int RequireGuildStatus;
	int RequireClass[MAX_CLASS];
};

class CSkillManager
{
public:
	CSkillManager();
	virtual ~CSkillManager();
	void Load(char* path);
	bool GetInfo(BYTE index, SKILL_INFO* lpInfo);
	int GetSkillDamage(BYTE index);
	int GetSkillMana(BYTE index);
	int GetSkillBP(BYTE index);
	int GetSkillType(BYTE index);
	int GetSkillEffect(BYTE index);
	int GetSkillAngle(int x, int y, int tx, int ty);
	int GetSkillFrustrum(int* SkillFrustrumX, int* SkillFrustrumY, BYTE angle, int x, int y, float sx, float sy, float tx, float ty);
	int GetSkillNumber(int index, int level);
	bool CheckSkillMana(LPOBJ lpObj, BYTE index);
	bool CheckSkillBP(LPOBJ lpObj, BYTE index);
	bool CheckSkillRange(BYTE index, int x, int y, int tx, int ty);
	bool CheckSkillRadio(BYTE index, int x, int y, int tx, int ty);
	bool CheckSkillFrustrum(int* SkillFrustrumX, int* SkillFrustrumY, int x, int y);
	bool CheckSkillDelay(LPOBJ lpObj, BYTE index);
	bool CheckSkillTarget(LPOBJ lpObj, int aIndex, int bIndex, int type);
	bool CheckSkillRequireLevel(LPOBJ lpObj, BYTE index);
	bool CheckSkillRequireEnergy(LPOBJ lpObj, BYTE index);
	bool CheckSkillRequireLeadership(LPOBJ lpObj, BYTE index);
	bool CheckSkillRequireKillPoint(LPOBJ lpObj, BYTE index);
	bool CheckSkillRequireGuildStatus(LPOBJ lpObj, BYTE index);
	bool CheckSkillRequireClass(LPOBJ lpObj, BYTE index);
	bool CheckSkillRequireWeapon(LPOBJ lpObj, BYTE index);
	int AddSkillWeapon(LPOBJ lpObj, BYTE index, int level);
	int DelSkillWeapon(LPOBJ lpObj, BYTE index, int level);
	int AddSkill(LPOBJ lpObj, BYTE index, int level);
	int DelSkill(LPOBJ lpObj, BYTE index);
	CSkill* GetSkill(LPOBJ lpObj, BYTE index);
	bool ConvertSkillByte(CSkill* lpSkill, BYTE* lpMsg);
	void SkillByteConvert(BYTE* lpMsg, CSkill* lpSkill);
	void UseAttackSkill(int aIndex, int bIndex, CSkill* lpSkill);
	void UseDurationSkillAttack(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y, BYTE dir, BYTE angle);
	bool RunningSkill(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y, BYTE angle, bool combo);
	bool BasicSkillAttack(int aIndex, int bIndex, CSkill* lpSkill);
	bool MultiSkillAttack(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillChangeUse(int aIndex);
	bool SkillEvilSpirit(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillManaShield(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillDefense(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillHeal(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillGreaterDefense(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillGreaterDamage(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillSummon(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillDecay(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y);
	bool SkillIceStorm(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y);
	bool SkillNova(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillTwistingSlash(int aIndex, int bIndex, CSkill* lpSkill, bool combo);
	bool SkillRagefulBlow(int aIndex, int bIndex, CSkill* lpSkill, bool combo);
	bool SkillDeathStab(int aIndex, int bIndex, CSkill* lpSkill, bool combo);
	bool SkillGreaterLife(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillMonsterAreaAttack(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillPowerSlash(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle);
	bool SkillForce(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillFireBurst(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillEarthquake(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillSummonParty(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillGreaterCriticalDamage(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillElectricSpark(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle);
	bool SkillCastleSiege(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillStern(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle);
	bool SkillRemoveStern(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillGreaterMana(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillInvisibility(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillRemoveInvisibility(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillRemoveAllEffect(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillBrand(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillPlasmaStorm(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillInfinityArrow(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillMonsterSummon(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillMagicDamageImmunity(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillPhysiDamageImmunity(int aIndex, int bIndex, CSkill* lpSkill);
	bool SkillSwordPowerGetDefense(int aIndex, int* defense);
	bool SkillSwordPowerGetPhysiDamage(int aIndex, int* DamageMin, int* DamageMax);
	bool SkillSwordPowerGetMagicDamage(int aIndex, int* DamageMin, int* DamageMax);
	bool SkillSwordPowerGetCurseDamage(int aIndex, int* DamageMin, int* DamageMax);
	void ApplyMeteoriteEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplyIceStormEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplyTwistingSlashEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplyRagefulBlowEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplyDeathStabEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplyFireSlashEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplyFireBurstEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplyFireScreamEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplyEarthquakeEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void ApplySwordSlashEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);
	void CGMultiSkillAttackRecv(PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg, int aIndex, bool type);
	void CGSkillAttackRecv(PMSG_SKILL_ATTACK_RECV* lpMsg, int aIndex);
	void CGDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg, int aIndex);
	void CGSkillCancelRecv(PMSG_SKILL_CANCEL_RECV* lpMsg, int aIndex);
	void CGSkillTeleportAllyRecv(PMSG_SKILL_TELEPORT_ALLY_RECV* lpMsg, int aIndex);
	void GCSkillAttackSend(LPOBJ lpObj, BYTE skill, int aIndex, BYTE type);
	void GCSkillCancelSend(LPOBJ lpObj, BYTE skill);
	void GCDurationSkillAttackSend(LPOBJ lpObj, BYTE skill, BYTE x, BYTE y, BYTE dir);
	void GCSkillAddSend(int aIndex, BYTE slot, BYTE skill, BYTE level, BYTE type);
	void GCSkillDelSend(int aIndex, BYTE slot, BYTE skill, BYTE level, BYTE type);
	void GCSkillListSend(LPOBJ lpObj, BYTE type);

private:
	std::unordered_map<BYTE, SKILL_INFO> m_SkillInfo;
};

extern CSkillManager gSkillManager;
