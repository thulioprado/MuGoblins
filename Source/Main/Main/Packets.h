#pragma once

#define SET_NUMBERHB(x)						((BYTE)((DWORD)(x)>>(DWORD)8))
#define SET_NUMBERLB(x)						((BYTE)((DWORD)(x)&0xFF))
#define SET_NUMBERHW(x)						((WORD)((DWORD)(x)>>(DWORD)16))
#define SET_NUMBERLW(x)						((WORD)((DWORD)(x)&0xFFFF))
#define SET_NUMBERHDW(x)					((DWORD)((QWORD)(x)>>(QWORD)32))
#define SET_NUMBERLDW(x)					((DWORD)((QWORD)(x)&0xFFFFFFFF))
#define MAKE_NUMBERW(x,y)					((WORD)(((BYTE)((y)&0xFF))|((BYTE)((x)&0xFF)<<8)))
#define MAKE_NUMBERDW(x,y)					((DWORD)(((WORD)((y)&0xFFFF))|((WORD)((x)&0xFFFF)<<16)))
#define GET_SUBHEAD(data)					((data[0] == 0xC1) ? data[3] : data[4])

struct PBMSG_HEAD
{
	void set(BYTE head, BYTE size)
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
	}

	void setE(BYTE head, BYTE size)
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
	}

	BYTE type;
	BYTE size;
	BYTE head;
};

struct PSBMSG_HEAD
{
	void set(BYTE head, BYTE subh, BYTE size)
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head, BYTE subh, BYTE size)
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size;
	BYTE head;
	BYTE subh;
};

struct PWMSG_HEAD
{
	void set(BYTE head, WORD size)
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	void setE(BYTE head, WORD size)
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
};

struct PSWMSG_HEAD
{
	void set(BYTE head, BYTE subh, WORD size)
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head, BYTE subh, WORD size)
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
	BYTE subh;
};

struct PMSG_USER_DIE_RECV
{
	PBMSG_HEAD header;
	BYTE index[2];
	BYTE skill;
	BYTE killer[2];
};

struct PMSG_LIFE_RECV
{
	PBMSG_HEAD header;
	BYTE type;
	DWORD life;
	BYTE flag;
};

struct PMSG_LIFE_RECV2
{
	PBMSG_HEAD header;
	BYTE type;
	BYTE life[2];
	BYTE flag;
};

struct PMSG_MANA_RECV
{
	PBMSG_HEAD header;
	BYTE type;
	DWORD mana;
	DWORD bp;
};

struct PMSG_MANA_RECV2
{
	PBMSG_HEAD header;
	BYTE type;
	BYTE mana[2];
	BYTE bp[2];
};

struct PMSG_REWARD_EXPERIENCE_RECV
{
	PBMSG_HEAD header;
	BYTE index[2];
	QWORD experience;
	DWORD damage;
};

struct PMSG_REWARD_EXPERIENCE_RECV2
{
	PBMSG_HEAD header;
	BYTE index[2];
	WORD experience[2];
	BYTE damage[2];
};

struct PMSG_REWARD_EXPERIENCE_RECV3
{
	PBMSG_HEAD header;
	BYTE index[2];
	BYTE experience[2];
	BYTE damage[2];
};

struct PMSG_QUEST_REWARD_RECV
{
	PBMSG_HEAD header;
	BYTE index[2];
	BYTE QuestReward;
	BYTE QuestAmount;
	DWORD LevelUpPoints;
};

struct PMSG_QUEST_REWARD_RECV2
{
	PBMSG_HEAD header;
	BYTE index[2];
	BYTE QuestReward;
	BYTE QuestAmount;
};

struct PMSG_DAMAGE_RECV
{
	PBMSG_HEAD header;
	BYTE index[2];
	DWORD damage;
	BYTE type;
	DWORD life;
};

struct PMSG_DAMAGE_RECV2
{
	PBMSG_HEAD header;
	BYTE index[2];
	BYTE damage[2];
	BYTE type;
};

struct PMSG_CONNECT_CLIENT_RECV
{
	PSBMSG_HEAD header;
	BYTE result;
	BYTE index[2];
	BYTE ClientVersion[5];
};

struct PMSG_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header;
	BYTE X;
	BYTE Y;
	BYTE Map;
	BYTE Dir;
	QWORD Experience;
	QWORD NextExperience;
	QWORD PreviousNextExperience;
	DWORD LevelUpPoint;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	DWORD Life;
	DWORD MaxLife;
	DWORD Mana;
	DWORD MaxMana;
	DWORD BP;
	DWORD MaxBP;
	DWORD Money;
	BYTE PKLevel;
	BYTE CtlCode;
	WORD FruitAddPoint;
	WORD MaxFruitAddPoint;
	WORD Leadership;
	WORD FruitSubPoint;
	WORD MaxFruitSubPoint;
};

struct PMSG_CHARACTER_INFO_RECV2
{
	PSBMSG_HEAD header;
	BYTE X;
	BYTE Y;
	BYTE Map;
	BYTE Dir;
	DWORD Experience;
	DWORD NextExperience;
	WORD LevelUpPoint;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	WORD BP;
	WORD MaxBP;
	DWORD Money;
	BYTE PKLevel;
	BYTE CtlCode;
	WORD FruitAddPoint;
	WORD MaxFruitAddPoint;
	WORD Leadership;
	WORD FruitSubPoint;
	WORD MaxFruitSubPoint;
};

struct PMSG_CHARACTER_REGEN_RECV
{
	PSBMSG_HEAD header;
	BYTE X;
	BYTE Y;
	BYTE Map;
	BYTE Dir;
	DWORD Life;
	DWORD Mana;
	DWORD BP;
	QWORD Experience;
	DWORD Money;
};

struct PMSG_CHARACTER_REGEN_RECV2
{
	PSBMSG_HEAD header;
	BYTE X;
	BYTE Y;
	BYTE Map;
	BYTE Dir;
	WORD Life;
	WORD Mana;
	WORD BP;
	DWORD Experience;
	DWORD Money;
};

struct PMSG_LEVEL_UP_RECV
{
	PSBMSG_HEAD header;
	WORD Level;
	DWORD LevelUpPoint;
	DWORD MaxHP;
	DWORD MaxMP;
	DWORD MaxBP;
	WORD FruitAddPoint;
	WORD MaxFruitAddPoint;
	WORD FruitSubPoint;
	WORD MaxFruitSubPoint;
	QWORD NextExperience;
	QWORD PreviousNextExperience;
};

struct PMSG_LEVEL_UP_RECV2
{
	PSBMSG_HEAD header;
	WORD Level;
	WORD LevelUpPoint;
	WORD MaxHP;
	WORD MaxMP;
	WORD MaxBP;
	WORD FruitAddPoint;
	WORD MaxFruitAddPoint;
	WORD FruitSubPoint;
	WORD MaxFruitSubPoint;
};

struct PMSG_LEVEL_UP_POINT_RECV
{
	PSBMSG_HEAD header;
	BYTE result;
	DWORD MaxLife;
	DWORD MaxMana;
	DWORD MaxBP;
	DWORD LevelUpPoint;
};

struct PMSG_LEVEL_UP_POINT_RECV2
{
	PSBMSG_HEAD header;
	BYTE result;
	WORD MaxLifeAndMana;
	WORD MaxBP;
};

struct PMSG_MONSTER_DAMAGE_RECV
{
	PSBMSG_HEAD header;
	DWORD damage;
	DWORD life;
};

struct PMSG_MONSTER_DAMAGE_RECV2
{
	PSBMSG_HEAD header;
	BYTE damage[2];
};