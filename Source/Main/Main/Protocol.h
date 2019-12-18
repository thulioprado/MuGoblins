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
	int UserDie(int Index, PMSG_USER_DIE_RECV* Data);
	int UpdateHP(int Index, PMSG_LIFE_RECV* Data);
	int UpdateMP(int Index, PMSG_MANA_RECV* Data);
	int ObtainedExperience(int Index, PMSG_REWARD_EXPERIENCE_RECV* Data);
	int QuestReward(int Index, PMSG_QUEST_REWARD_RECV* Data);
	int SetDamage(int Index, PMSG_DAMAGE_RECV* Data);
	int ClientConnect(int Index, PMSG_CONNECT_CLIENT_RECV* Data);
	int CharacterInfo(int Index, PMSG_CHARACTER_INFO_RECV* Data);
	int CharacterRegen(int Index, PMSG_CHARACTER_REGEN_RECV* Data);
	int LevelUp(int Index, PMSG_LEVEL_UP_RECV* Data);
	int UpdatePoints(int index, PMSG_LEVEL_UP_POINT_RECV* Data);
	int MonsterSetDamage(int Index, PMSG_MONSTER_DAMAGE_RECV* Data);
	int LockMain(PMSG_LOCK_RECV* Data);
	int MonsterHealth(PMSG_MONSTER_HEALTH_RECV* Data);
	int DiscordUpdate(PMSG_DISCORD_UPDATE_RECV* Data);
};

extern CProtocol Protocol;