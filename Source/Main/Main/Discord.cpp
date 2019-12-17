#include "Library.h"
#include "Discord.h"

#if _DEBUG
#	pragma comment(lib, "discord/discordd.lib")
#else
#	pragma comment(lib, "discord/discord.lib")
#endif

#pragma comment(lib, "discord/discord_game_sdk.dll.lib")

using namespace std::chrono_literals;
using namespace discord;

CDiscord::CDiscord() : Core(), Activity{}, Thread(), Stop(false)
{
}

CDiscord::~CDiscord()
{
	if (this->Thread.joinable())
	{
		this->Stop = true;
		this->Thread.join();
	}
}

void CDiscord::Start()
{
	try
	{
		::Core* Core{};

		if (::Core::Create(652742485365358592, DiscordCreateFlags_Default, &Core) == Result::Ok)
		{
			this->Core.reset(Core);

			if (this->Core)
			{
				this->Activity.SetType(ActivityType::Playing);
				this->Activity.GetAssets().SetLargeImage("gob");
				this->Activity.GetTimestamps().SetStart(time(nullptr));
				this->Activity.SetState("Iniciando");
				this->Activity.SetDetails("www.mugoblins.com");

				this->Core->ActivityManager().UpdateActivity(this->Activity, [](Result result) {});

				this->Thread = std::thread(&CDiscord::Update, this);
			}
		}
	}
	catch (...)
	{
	}
}

void CDiscord::Update()
{
	do
	{
		try
		{
			this->Core->RunCallbacks();
		}
		catch (...)
		{
		}

		std::this_thread::sleep_for(250ms);
	}
	while (!this->Stop);
}

void CDiscord::SetActivity(const char* state)
{
	if (this->Core)
	{
		try
		{
			this->Activity.SetState(state);

			this->Core->ActivityManager().UpdateActivity(this->Activity, [](Result result) {});
		}
		catch (...)
		{
		}
	}
}

CDiscord Discord;