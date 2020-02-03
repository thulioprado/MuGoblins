#pragma once

#include "Types.hpp"
#include <Windows.h>
#include <stdio.h>
#include <DbgHelp.h>

class MiniDump
{
public:
    MiniDump() :
        ExceptionFilter(SetUnhandledExceptionFilter(this->DumpExceptionFilter))
    {
        SetErrorMode(SEM_FAILCRITICALERRORS);
    }

    ~MiniDump()
    {
	    SetUnhandledExceptionFilter(this->ExceptionFilter);
    }

private:
    static LONG WINAPI DumpExceptionFilter(EXCEPTION_POINTERS* Pointers)
    {
        SYSTEMTIME Now;
        GetLocalTime(&Now);

        char Path[MAX_PATH];
        sprintf_s(Path, ".\\Dumps\\[%02d-%02d-%04d][%02d-%02d-%02d].dmp", Now.wDay, Now.wMonth, Now.wYear, Now.wHour, Now.wMinute, Now.wSecond);

        auto File = CreateFile(Path, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);

        if (File != INVALID_HANDLE_VALUE)
        {
            MINIDUMP_EXCEPTION_INFORMATION MDEI;

            MDEI.ThreadId = GetCurrentThreadId();
            MDEI.ExceptionPointers = Pointers;
            MDEI.ClientPointers = 0;

            if (MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), File, (MINIDUMP_TYPE)(MiniDumpScanMemory + MiniDumpWithIndirectlyReferencedMemory), &MDEI, 0, 0) != 0)
            {
                CloseHandle(File);
                return EXCEPTION_EXECUTE_HANDLER;
            }
            
            CloseHandle(File);
        }

        return EXCEPTION_CONTINUE_SEARCH;
    }

private:
    LPTOP_LEVEL_EXCEPTION_FILTER ExceptionFilter;
};