#include "Library.h"
#include "Protocol.h"
#include "Player.h"
#include "Monster.h"
#include "Discord.h"

CProtocol::CProtocol()
{
}

CProtocol::~CProtocol()
{
}

void CProtocol::Load()
{
	Memory::Call(0x4DB63D, this->Core);
}

int CProtocol::Core(DWORD Protocol, BYTE* Data, int Size, int Index)
{
	switch (Protocol)
	{
		case 0x17:
		{
			return ::Protocol.UserDie(Index, (PMSG_USER_DIE_RECV*)(Data));
		}
		case 0x26:
		{
			return ::Protocol.UpdateHP(Index, (PMSG_LIFE_RECV*)(Data));
		}
		case 0x27:
		{
			return ::Protocol.UpdateMP(Index, (PMSG_MANA_RECV*)(Data));
		}
		case 0x9C:
		{
			return ::Protocol.ObtainedExperience(Index, (PMSG_REWARD_EXPERIENCE_RECV*)(Data));
		}
		case 0xA3:
		{
			return ::Protocol.QuestReward(Index, (PMSG_QUEST_REWARD_RECV*)(Data));
		}
		case 0xD7:
		{
			return ::Protocol.SetDamage(Index, (PMSG_DAMAGE_RECV*)(Data));
		}
		case 0xF1:
		{
			switch (GET_SUBHEAD(Data))
			{
				case 0x00:
				{
					return ::Protocol.ClientConnect(Index, (PMSG_CONNECT_CLIENT_RECV*)(Data));
				}
			}

			break;
		}
		case 0xF3:
		{
			switch (GET_SUBHEAD(Data))
			{
				case 0x03:
				{
					return ::Protocol.CharacterInfo(Index, (PMSG_CHARACTER_INFO_RECV*)(Data));
				}
				case 0x04:
				{
					return ::Protocol.CharacterRegen(Index, (PMSG_CHARACTER_REGEN_RECV*)(Data));
				}
				case 0x05:
				{
					return ::Protocol.LevelUp(Index, (PMSG_LEVEL_UP_RECV*)(Data));
				}
				case 0x06:
				{
					return ::Protocol.UpdatePoints(Index, (PMSG_LEVEL_UP_POINT_RECV*)(Data));
				}
				case 0x07:
				{
					return ::Protocol.MonsterSetDamage(Index, (PMSG_MONSTER_DAMAGE_RECV*)(Data));
				}
				case 0xFD:
				{
					return ::Protocol.LockMain((PMSG_LOCK_RECV*)(Data));
				}
				case 0xFE:
				{
					return ::Protocol.MonsterHealth((PMSG_MONSTER_HEALTH_RECV*)(Data));
				}
				case 0xFF:
				{
					return ::Protocol.DiscordUpdate((PMSG_DISCORD_UPDATE_RECV*)(Data));
				}
			}
		}
	}

	return pProtocolCore(Protocol, Data, Size, Index);
}

int CProtocol::UserDie(int Index, PMSG_USER_DIE_RECV* Data)
{
	auto UserIndex = MAKE_NUMBERW(Data->index[0], Data->index[1]);

	if (UserIndex == Player.Index)
	{
		Player.HP = 0;

		Player.Execute(Index, [](PlayerObject* Object) {
			Object->HP = 0;
		});
	}

	return pProtocolCore(0x17, (LPBYTE)(Data), Data->header.size, Index);
}

int CProtocol::UpdateHP(int Index, PMSG_LIFE_RECV* Data)
{
	switch (Data->type)
	{
		case 0xFE:
		{
			Player.MaxHP = Data->life;

			Player.Execute(Index, [](PlayerObject* Object) {
				Object->MaxHP = GET_MAX_WORD_VALUE(Player.MaxHP);
			});

			break;
		}
		case 0xFF:
		{
			Player.HP = ((Player.HP == 0) ? Player.HP : Data->life);

			Player.Execute(Index, [&](PlayerObject* Object) {
				Object->HP = GET_MAX_WORD_VALUE(Player.HP);
			});

			break;
		}
	}

	PMSG_LIFE_RECV2 pMsg;

	pMsg.header.set(0x26, sizeof(pMsg));
	pMsg.type = Data->type;
	pMsg.life[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->life));
	pMsg.life[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->life));
	pMsg.flag = Data->flag;

	return pProtocolCore(0x26, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::UpdateMP(int Index, PMSG_MANA_RECV* Data)
{
	switch (Data->type)
	{
		case 0xFE:
		{
			Player.MaxMP = Data->mana;
			Player.MaxBP = Data->bp;

			Player.Execute(Index, [](PlayerObject* Object) {
				Object->MaxMP = GET_MAX_WORD_VALUE(Player.MaxMP);
				Object->MaxBP = GET_MAX_WORD_VALUE(Player.MaxBP);
			});
		} break;
		case 0xFF:
		{
			Player.MP = Data->mana;
			Player.BP = Data->bp;

			Player.Execute(Index, [](PlayerObject* Object) {
				Object->MP = GET_MAX_WORD_VALUE(Player.MP);
				Object->BP = GET_MAX_WORD_VALUE(Player.BP);
			});
		} break;
	}

	PMSG_MANA_RECV2 pMsg;

	pMsg.header.set(0x27, sizeof(pMsg));
	pMsg.type = Data->type;
	pMsg.mana[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->mana));
	pMsg.mana[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->mana));
	pMsg.bp[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->bp));
	pMsg.bp[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->bp));

	return pProtocolCore(0x27, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ObtainedExperience(int Index, PMSG_REWARD_EXPERIENCE_RECV* Data)
{
	Player.Experience += Data->experience;
	Player.Damage = Data->damage;

	auto Current = double(Player.Experience) - double(Player.PreviousNextExperience);
	auto Total = double(Player.NextExperience) - double(Player.PreviousNextExperience);
	Player.PercentExperience = (DWORD)((Current / Total) * 100);

	if (Player.PercentExperience > 100)
	{
		Player.PercentExperience = 100;
	}

	PMSG_REWARD_EXPERIENCE_RECV2 pMsg;

	pMsg.header.setE(0x9C, sizeof(pMsg));
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	pMsg.experience[0] = SET_NUMBERHW(Data->experience);
	pMsg.experience[1] = SET_NUMBERLW(Data->experience);
	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->damage));
	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->damage));

	return pProtocolCore(0x9C, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::QuestReward(int Index, PMSG_QUEST_REWARD_RECV* Data)
{
	Player.LevelUpPoints = Data->LevelUpPoints;

	PMSG_QUEST_REWARD_RECV2 pMsg;
	pMsg.header.set(0xA3, sizeof(pMsg));
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	pMsg.QuestReward = Data->QuestReward;
	pMsg.QuestAmount = Data->QuestAmount;

	return pProtocolCore(0xA3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::SetDamage(int Index, PMSG_DAMAGE_RECV* Data)
{
	int UserIndex = MAKE_NUMBERW(Data->index[0], Data->index[1]);

	if (UserIndex == Player.Index)
	{
		Player.HP = Data->life;

		Player.Execute(Index, [](PlayerObject* Object) {
			Object->HP = GET_MAX_WORD_VALUE(Player.HP);
		});
	}

	Player.Damage = Data->damage;

	PMSG_DAMAGE_RECV2 pMsg;

	pMsg.header.set(0xD7, sizeof(pMsg));
	pMsg.index[0] = Data->index[0];
	pMsg.index[1] = Data->index[1];
	pMsg.type = Data->type;
	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->damage));
	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->damage));

	return pProtocolCore(0xD7, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::ClientConnect(int Index, PMSG_CONNECT_CLIENT_RECV* Data)
{
	pLockMain = FALSE;

	Player.Index = MAKE_NUMBERW(Data->index[0], Data->index[1]);

	return pProtocolCore(0xF1, (LPBYTE)(Data), Data->header.size, Index);
}

int CProtocol::CharacterInfo(int Index, PMSG_CHARACTER_INFO_RECV* Data)
{
	pLockMain = FALSE;

	Player.Experience = Data->Experience;
	Player.NextExperience = Data->NextExperience;
	Player.PreviousNextExperience = Data->PreviousNextExperience;
	Player.HP = Data->Life;
	Player.MaxHP = Data->MaxLife;
	Player.MP = Data->Mana;
	Player.MaxMP = Data->MaxMana;
	Player.BP = Data->BP;
	Player.MaxBP = Data->MaxBP;
	Player.LevelUpPoints = Data->LevelUpPoint;

	auto Current = double(Player.Experience) - double(Player.PreviousNextExperience);
	auto Total = double(Player.NextExperience) - double(Player.PreviousNextExperience);
	Player.PercentExperience = (DWORD)((Current / Total) * 100);

	if (Player.PercentExperience > 100)
	{
		Player.PercentExperience = 100;
	}

	PMSG_CHARACTER_INFO_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x03, sizeof(pMsg));
	pMsg.X = Data->X;
	pMsg.Y = Data->Y;
	pMsg.Map = Data->Map;
	pMsg.Dir = Data->Dir;
	pMsg.Experience = (DWORD)(Data->Experience);
	pMsg.NextExperience = (DWORD)(Data->NextExperience);
	pMsg.LevelUpPoint = GET_MAX_WORD_VALUE(Data->LevelUpPoint);
	pMsg.Strength = Data->Strength;
	pMsg.Dexterity = Data->Dexterity;
	pMsg.Vitality = Data->Vitality;
	pMsg.Energy = Data->Energy;
	pMsg.Life = GET_MAX_WORD_VALUE(Data->Life);
	pMsg.MaxLife = GET_MAX_WORD_VALUE(Data->MaxLife);
	pMsg.Mana = GET_MAX_WORD_VALUE(Data->Mana);
	pMsg.MaxMana = GET_MAX_WORD_VALUE(Data->MaxMana);
	pMsg.BP = GET_MAX_WORD_VALUE(Data->BP);
	pMsg.MaxBP = GET_MAX_WORD_VALUE(Data->MaxBP);
	pMsg.Money = Data->Money;
	pMsg.PKLevel = Data->PKLevel;
	pMsg.CtlCode = Data->CtlCode;
	pMsg.FruitAddPoint = Data->FruitAddPoint;
	pMsg.MaxFruitAddPoint = Data->MaxFruitAddPoint;
	pMsg.Leadership = Data->Leadership;
	pMsg.FruitSubPoint = Data->FruitSubPoint;
	pMsg.MaxFruitSubPoint = Data->MaxFruitSubPoint;

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::CharacterRegen(int Index, PMSG_CHARACTER_REGEN_RECV* Data)
{
	Player.Experience = Data->Experience;
	Player.HP = Data->Life;
	Player.MP = Data->Mana;
	Player.BP = Data->BP;

	PMSG_CHARACTER_REGEN_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x04, sizeof(pMsg));
	pMsg.X = Data->X;
	pMsg.Y = Data->Y;
	pMsg.Map = Data->Map;
	pMsg.Dir = Data->Dir;
	pMsg.Life = GET_MAX_WORD_VALUE(Data->Life);
	pMsg.Mana = GET_MAX_WORD_VALUE(Data->Mana);
	pMsg.BP = GET_MAX_WORD_VALUE(Data->BP);
	pMsg.Experience = (DWORD)(Data->Experience);
	pMsg.Money = Data->Money;

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::LevelUp(int Index, PMSG_LEVEL_UP_RECV* Data)
{
	Player.MaxHP = Data->MaxHP;
	Player.MaxMP = Data->MaxMP;
	Player.MaxBP = Data->MaxBP;
	Player.LevelUpPoints = Data->LevelUpPoint;
	Player.NextExperience = Data->NextExperience;
	Player.PreviousNextExperience = Data->PreviousNextExperience;

	auto Current = double(Player.Experience) - double(Player.PreviousNextExperience);
	auto Total = double(Player.NextExperience) - double(Player.PreviousNextExperience);
	Player.PercentExperience = (DWORD)((Current / Total) * 100);

	if (Player.PercentExperience > 100)
	{
		Player.PercentExperience = 100;
	}

	PMSG_LEVEL_UP_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x05, sizeof(pMsg));
	pMsg.Level = Data->Level;
	pMsg.LevelUpPoint = GET_MAX_WORD_VALUE(Data->LevelUpPoint);
	pMsg.MaxHP = GET_MAX_WORD_VALUE(Data->MaxHP);
	pMsg.MaxMP = GET_MAX_WORD_VALUE(Data->MaxMP);
	pMsg.MaxBP = GET_MAX_WORD_VALUE(Data->MaxBP);
	pMsg.FruitAddPoint = Data->FruitAddPoint;
	pMsg.MaxFruitAddPoint = Data->MaxFruitAddPoint;
	pMsg.FruitSubPoint = Data->FruitSubPoint;
	pMsg.MaxFruitSubPoint = Data->MaxFruitSubPoint;

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::UpdatePoints(int Index, PMSG_LEVEL_UP_POINT_RECV* Data)
{
	Player.LevelUpPoints = Data->LevelUpPoint;

	if (Data->result > 0)
	{
		Player.MaxHP = Data->MaxLife;
		Player.MaxMP = Data->MaxMana;
		Player.MaxBP = Data->MaxBP;

		Player.Execute(Index, [&](PlayerObject* Object) {
			Object->LevelUpPoint = Data->LevelUpPoint > 0;

			switch (Data->result)
			{
				case 0x10:
				{
					++Object->Strength;
					break;
				}
				case 0x11:
				{
					++Object->Dexterity;
					break;
				}
				case 0x12:
				{
					++Object->Vitality;
					break;
				}
				case 0x13:
				{
					++Object->Energy;
					break;
				}
				case 0x14:
				{
					++Object->Leadership;
					break;
				}
			}
		});
	}

	return 1;
}

int CProtocol::MonsterSetDamage(int Index, PMSG_MONSTER_DAMAGE_RECV* Data)
{
	Player.HP = Data->life;
	Player.Damage = Data->damage;

	PMSG_MONSTER_DAMAGE_RECV2 pMsg;

	pMsg.header.set(0xF3, 0x07, sizeof(pMsg));
	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(Data->damage));
	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(Data->damage));

	return pProtocolCore(0xF3, (LPBYTE)(&pMsg), sizeof(pMsg), Index);
}

int CProtocol::LockMain(PMSG_LOCK_RECV* Data)
{
	pLockMain = Data->lock;
	return 1;
}

int CProtocol::MonsterHealth(PMSG_MONSTER_HEALTH_RECV* Data)
{
	Monster.Reset(Data->count);

	for (BYTE i = 0; i < Data->count; ++i)
	{
		Monster.Add(Data->health[i].index, Data->health[i].percent);
	}

	return 1;
}

int CProtocol::DiscordUpdate(PMSG_DISCORD_UPDATE_RECV* Data)
{
	char text[50];
	wsprintf(text, "Total online: %d", Data->total);
	Discord.SetActivity(text);

	return 1;
}

CProtocol Protocol;