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

struct PMSG_VIEWPORT_PLAYER
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE CharSet[CHAR_SET_SIZE];
	DWORD ViewSkillState;
	char name[10];
	BYTE tx;
	BYTE ty;
	BYTE DirAndPkLevel;
};

struct PMSG_VIEWPORT_PLAYER2
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE CharSet[CHAR_SET_SIZE_MAIN];
	DWORD ViewSkillState;
	char name[10];
	BYTE tx;
	BYTE ty;
	BYTE DirAndPkLevel;
};

struct PMSG_USER_DIE_RECV
{
	PBMSG_HEAD header;
	BYTE index[2];
	BYTE skill;
	BYTE killer[2];
};

struct PMSG_VIEWPORT_ITEM
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_VIEWPORT_ITEM2
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
};

struct PMSG_VIEWPORT_RECV
{
	PWMSG_HEAD header;
	BYTE count;
};

struct PMSG_ITEM_GET_RECV
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_ITEM_GET_RECV2
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
};

struct PMSG_ITEM_MOVE_RECV
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE SourceFlag;
	BYTE SourceSlot;
	BYTE TargetFlag;
	BYTE TargetSlot;
	BYTE ItemInfo[MAX_ITEM_INFO];
	bool PrismUpdate;
};

struct PMSG_ITEM_MOVE_RECV2
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
};

struct PMSG_ITEM_CHANGE_RECV
{
	PBMSG_HEAD header;
	BYTE index[2];
	BYTE ItemInfo[MAX_ITEM_INFO];
	BYTE CharSet[CHAR_SET_SIZE];
};

struct PMSG_ITEM_CHANGE_RECV2
{
	PBMSG_HEAD header;
	BYTE index[2];
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
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

struct PMSG_SHOP_ITEM_LIST
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_SHOP_ITEM_LIST2
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
};

struct PMSG_SHOP_ITEM_LIST_RECV
{
	PWMSG_HEAD header;
	BYTE type;
	BYTE count;
};

struct PMSG_ITEM_BUY_RECV
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_ITEM_BUY_RECV2
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
};

struct PMSG_TRADE_ITEM_DEL_RECV
{
	PBMSG_HEAD header;
	BYTE slot;
};

struct PMSG_TRADE_ITEM_ADD_RECV
{
	PBMSG_HEAD header;
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_TRADE_ITEM_ADD_RECV2
{
	PBMSG_HEAD header;
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
};

struct PMSG_PSHOP_ITEM_LIST
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
	DWORD value;
};

struct PMSG_PSHOP_ITEM_LIST2
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
	DWORD value;
};

struct PMSG_PSHOP_ITEM_LIST_RECV
{
	PSWMSG_HEAD header;
	BYTE result;
	BYTE index[2];
	char name[10];
	char text[36];
	BYTE count;
};

struct PMSG_PSHOP_ITEM_BUY_RECV
{
	PSBMSG_HEAD header;
	BYTE result;
	BYTE index[2];
	BYTE ItemInfo[MAX_ITEM_INFO];
	BYTE slot;
};

struct PMSG_PSHOP_ITEM_BUY_RECV2
{
	PSBMSG_HEAD header;
	BYTE result;
	BYTE index[2];
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
	BYTE slot;
};

struct PMSG_VIEWPORT_CHANGE
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE skin[2];
	DWORD ViewSkillState;
	char name[10];
	BYTE tx;
	BYTE ty;
	BYTE DirAndPkLevel;
	BYTE CharSet[CHAR_SET_SIZE];
};

struct PMSG_VIEWPORT_CHANGE2
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE skin[2];
	DWORD ViewSkillState;
	char name[10];
	BYTE tx;
	BYTE ty;
	BYTE DirAndPkLevel;
	BYTE CharSet[CHAR_SET_SIZE_MAIN];
};

struct PMSG_VIEWPORT_CASTLE_SIEGE_WEAPON
{
	BYTE type;
	BYTE skin[2];
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE CharSet[CHAR_SET_SIZE - 1];
	DWORD ViewSkillState;
};

struct PMSG_VIEWPORT_CASTLE_SIEGE_WEAPON2
{
	BYTE type;
	BYTE skin[2];
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE CharSet[CHAR_SET_SIZE_MAIN - 1];
	DWORD ViewSkillState;
};

struct PMSG_CHAOS_MIX_RECV
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_CHAOS_MIX_RECV2
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
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

struct PMSG_CHARACTER_LIST_RECV
{
	PSWMSG_HEAD header;
	BYTE ClassCode;
	BYTE MoveCnt;
	BYTE count;
};

struct PMSG_CHARACTER_LIST_RECV2
{
	PSBMSG_HEAD header;
	BYTE ClassCode;
	BYTE MoveCnt;
	BYTE count;
};

struct PMSG_CHARACTER_LIST
{
	BYTE slot;
	char Name[10];
	WORD Level;
	BYTE CtlCode;
	BYTE CharSet[CHAR_SET_SIZE];
	BYTE GuildStatus;
};

struct PMSG_CHARACTER_LIST2
{
	BYTE slot;
	char Name[10];
	WORD Level;
	BYTE CtlCode;
	BYTE CharSet[CHAR_SET_SIZE_MAIN];
	BYTE GuildStatus;
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

struct PMSG_ITEM_LIST
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_ITEM_LIST2
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
};

struct PMSG_ITEM_LIST_RECV
{
	PSWMSG_HEAD header;
	BYTE count;
};

struct PMSG_ITEM_EQUIPMENT_RECV
{
	PSBMSG_HEAD header;
	BYTE index[2];
	BYTE CharSet[CHAR_SET_SIZE];
};

struct PMSG_ITEM_EQUIPMENT_RECV2
{
	PSBMSG_HEAD header;
	BYTE index[2];
	BYTE CharSet[CHAR_SET_SIZE_MAIN];
};

struct PMSG_ITEM_MODIFY_RECV
{
	PSBMSG_HEAD header;
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_ITEM_MODIFY_RECV2
{
	PSBMSG_HEAD header;
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO_MAIN];
};

struct PMSG_CHARACTER_UPDATE_RECV
{
	PSWMSG_HEAD header;
	WORD Level;
	DWORD LevelUpPoint;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Leadership;
	DWORD HP;
	DWORD MaxHP;
	DWORD MP;
	DWORD MaxMP;
	DWORD BP;
	DWORD MaxBP;
	WORD FruitAddPoint;
	WORD MaxFruitAddPoint;
	WORD FruitSubPoint;
	WORD MaxFruitSubPoint;
};

struct PMSG_POST_MESSAGE_RECV
{
	PSBMSG_HEAD header;
	char message[80];
};

struct PMSG_CUSTOM_SETTINGS_RECV
{
	PSWMSG_HEAD header;
	WORD TransformationRings[8][16];
};

struct PMSG_LOCK_RECV
{
	PSBMSG_HEAD header;
	BYTE lock;
};

struct PMSG_MONSTER_HEALTH
{
	WORD index;
	BYTE percent;
};

struct PMSG_MONSTER_HEALTH_RECV
{
	PSWMSG_HEAD header;
	BYTE count;
};

struct PMSG_DISCORD_UPDATE_RECV
{
	PSBMSG_HEAD header;
	int total;
};