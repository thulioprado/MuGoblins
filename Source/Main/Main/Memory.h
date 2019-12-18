#pragma once

class Memory
{
public:
	static inline bool Writable(DWORD Offset, DWORD Size)
	{
		DWORD Old;
		return VirtualProtect((void*)(Offset), Size, PAGE_EXECUTE_READWRITE, &Old);
	}

	template<typename T>
	static inline void Change(DWORD Offset, T Value)
	{
		*((T*)(Offset)) = Value;
	}

	template<>
	static inline void Change(DWORD Offset, const char* Value)
	{
		memcpy((void*)(Offset), Value, strlen(Value) + 1);
	}

	template<>
	static inline void Change(DWORD Offset, std::vector<BYTE> Value)
	{
		memcpy((void*)(Offset), Value.data(), Value.size());
	}

	template<DWORD Size = 1>
	static inline void Nop(DWORD Offset)
	{
		for (DWORD i = 0; i < Size; ++i)
		{
			Change<BYTE>(Offset + i, 0x90);
		}
	}

	static inline void Call(DWORD Offset, void* Function)
	{
		Change<BYTE>(Offset, 0xE8);
		Change<DWORD>(Offset + 1, DWORD(Function) - (Offset + 5));
	}

	static inline void Jump(DWORD Offset, void* Function)
	{
		Change<BYTE>(Offset, 0xE9);
		Change<DWORD>(Offset + 1, DWORD(Function) - (Offset + 5));
	}

	static inline void Hook(DWORD Offset, void* Function)
	{
		Change<DWORD>(Offset + 1, DWORD(Function) - (Offset + 5));
	}
};