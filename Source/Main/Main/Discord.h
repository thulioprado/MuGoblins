#pragma once

#include "discord/discord.h"

class CDiscord
{
public:
	CDiscord();
	~CDiscord();

public:
	void Start();
	void Update();
	void SetActivity(const char* state);

private:    
	std::unique_ptr<discord::Core> Core;
	discord::Activity Activity;
	std::thread Thread;
	bool Stop;
};

extern CDiscord Discord;