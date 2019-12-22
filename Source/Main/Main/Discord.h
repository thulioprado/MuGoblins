#pragma once

#include "discord/discord.h"

class CDiscord
{
public:
	CDiscord();
	~CDiscord();

public:
	void Load();
	void Run();
	void SetState(const char* State);

private:
	std::thread Thread;
	std::string State;
	bool Update;
	bool Stop;
};

extern CDiscord Discord;