#include "Library.h"
#include "Patch.h"
#include "Window.h"
#include "Protocol.h"
#include "Player.h"
#include "Discord.h"

HMODULE wzAudio = nullptr;

BOOL APIENTRY DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			wzAudio = LoadLibrary(".\\wza.dll");
			
			if (!Patch())
			{
				return FALSE;
			}

			Window.Attach(Instance);
			Protocol.Load();
			Player.Load();
			Discord.Start();

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			Window.Detach();

			if (wzAudio)
			{
				FreeLibrary(wzAudio);
			}

			break;
		}
	}

	return TRUE;
}

//
// WzAudio
//
extern "C" int __declspec(dllexport) wzAudioCreate(HWND Unk)
{
	static auto Function = (int(*)(HWND))(GetProcAddress(wzAudio, "wzAudioCreate"));
	return Function(Unk);
}

extern "C" int __declspec(dllexport) wzAudioDestroy()
{
	static auto Function = (int(*)())(GetProcAddress(wzAudio, "wzAudioDestroy"));
	return Function();
}

extern "C" int __declspec(dllexport) wzAudioPlay(WPARAM Unk)
{
	static auto Function = (int(*)(WPARAM))(GetProcAddress(wzAudio, "wzAudioPlay"));
	return Function(Unk);
}

extern "C" int __declspec(dllexport) wzAudioPause()
{
	static auto Function = (int(*)())(GetProcAddress(wzAudio, "wzAudioPause"));
	return Function();
}

extern "C" int __declspec(dllexport) wzAudioStop()
{
	static auto Function = (int(*)())(GetProcAddress(wzAudio, "wzAudioStop"));
	return Function();
}

extern "C" unsigned int __declspec(dllexport) wzAudioSetVolume(int Unk)
{
	static auto Function = (unsigned int(*)(int))(GetProcAddress(wzAudio, "wzAudioSetVolume"));
	return Function(Unk);
}

extern "C" unsigned int __declspec(dllexport) wzAudioGetVolume()
{
	static auto Function = (unsigned int(*)())(GetProcAddress(wzAudio, "wzAudioGetVolume"));
	return Function();
}

extern "C" unsigned int __declspec(dllexport) wzAudioVolumeUp()
{
	static auto Function = (unsigned int(*)())(GetProcAddress(wzAudio, "wzAudioVolumeUp"));
	return Function();
}

extern "C" unsigned int __declspec(dllexport) wzAudioVolumeDown()
{
	static auto Function = (unsigned int(*)())(GetProcAddress(wzAudio, "wzAudioVolumeDown"));
	return Function();
}

extern "C" int __declspec(dllexport) wzAudioOpenFile(char* Unk)
{
	static auto Function = (int(*)(char*))(GetProcAddress(wzAudio, "wzAudioOpenFile"));
	return Function(Unk);
}

extern "C" int __declspec(dllexport) wzAudioSeek(WPARAM Unk)
{
	static auto Function = (int(*)(WPARAM))(GetProcAddress(wzAudio, "wzAudioSeek"));
	return Function(Unk);
}

extern "C" int __declspec(dllexport) wzAudioGetStreamOffsetRange()
{
	static auto Function = (int(*)())(GetProcAddress(wzAudio, "wzAudioGetStreamOffsetRange"));
	return Function();
}

extern "C" int __declspec(dllexport) wzAudioGetStreamOffsetSec()
{
	static auto Function = (int(*)())(GetProcAddress(wzAudio, "wzAudioGetStreamOffsetSec"));
	return Function();
}

extern "C" unsigned int __declspec(dllexport) wzAudioSetMixerMode(int Unk)
{
	static auto Function = (unsigned int(*)(int))(GetProcAddress(wzAudio, "wzAudioSetMixerMode"));
	return Function(Unk);
}

extern "C" LPSTR __declspec(dllexport) wzAudioGetStreamInfo(char* Unk1, char* Unk2)
{
	static auto Function = (char* (*)(char*, char*))(GetProcAddress(wzAudio, "wzAudioGetStreamInfo"));
	return Function(Unk1, Unk2);
}

extern "C" int __declspec(dllexport) wzAudioOption(int Unk1, int Unk2)
{
	static auto Function = (int(*)(int, int))(GetProcAddress(wzAudio, "wzAudioOption"));
	return Function(Unk1, Unk2);
}

extern "C" int __declspec(dllexport) wzAudioSetEqualizer(int Unk)
{
	static auto Function = (int(*)(int))(GetProcAddress(wzAudio, "wzAudioSetEqualizer"));
	return Function(Unk);
}