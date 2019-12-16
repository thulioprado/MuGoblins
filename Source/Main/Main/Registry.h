#pragma once

class Registry
{
public:
	template<typename T>
	static inline T Read(const char* field)
	{
		T Value = T();
		HKEY Key;

		if (RegOpenKey(HKEY_CURRENT_USER, "Software\\Webzen\\MU\\Config", &Key) == 0)
		{
			DWORD Size = sizeof(T);

			RegQueryValueEx(Key, field, null, null, LPBYTE(&Value), &Size);
			RegCloseKey(Key);
		}

		return Value;
	}

	template<>
	static inline bool Read(const char* field)
	{
		int Value = 0;
		HKEY Key;

		if (RegOpenKey(HKEY_CURRENT_USER, "Software\\Webzen\\MU\\Config", &Key) == 0)
		{
			DWORD Size = sizeof(int);

			RegQueryValueEx(Key, field, null, null, LPBYTE(&Value), &Size);
			RegCloseKey(Key);
		}

		return (Value > 0);
	}
};