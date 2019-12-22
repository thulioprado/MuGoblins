#include "Library.h"
#include "Discord.h"
#include <WinInet.h>

#if _DEBUG
#	pragma comment(lib, "discord/discordd.lib")
#else
#	pragma comment(lib, "discord/discord.lib")
#endif

#pragma comment(lib, "discord/discord_game_sdk.dll.lib")
#pragma comment(lib, "WinInet.lib")

using namespace std::chrono_literals;
using namespace discord;

CDiscord::CDiscord() : Thread(), State(""), Update(false), Stop(false)
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

void CDiscord::Load()
{
	this->Thread = std::thread(&CDiscord::Run, this);
}

void CDiscord::Run()
{
	bool Connect = InternetCheckConnection("https://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0);

	if (!Connect)
	{
		return;
	}

	try
	{
		::Core* Core{};

		if (::Core::Create(652742485365358592, DiscordCreateFlags_Default, &Core) == Result::Ok)
		{
			::Activity Activity;

			Activity.SetType(ActivityType::Playing);
			Activity.GetAssets().SetLargeImage("gob");
			Activity.GetTimestamps().SetStart(time(nullptr));
			Activity.SetState("Iniciando");
			Activity.SetDetails("www.mugoblins.com");
			
			Core->ActivityManager().UpdateActivity(Activity, [](Result result) {});

			do
			{
				if (this->Update)
				{
					Activity.SetState(this->State.c_str());
					Core->ActivityManager().UpdateActivity(Activity, [](Result result) {});

					this->Update = false;
				}

				Core->RunCallbacks();
				std::this_thread::sleep_for(250ms);
			}
			while (!this->Stop);
		}
	}
	catch (...)
	{
	}
}

void CDiscord::SetState(const char* State)
{
	this->State = State;
	this->Update = true;
}

CDiscord Discord;