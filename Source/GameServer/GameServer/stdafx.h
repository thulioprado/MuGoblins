#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define GAMESERVER_VERSION "PREMIUM"

#ifndef GAMESERVER_TYPE
#define GAMESERVER_TYPE 0
#endif

#ifndef GAMESERVER_UPDATE
#define GAMESERVER_UPDATE 101
#endif

#ifndef GAMESERVER_LANGUAGE
#define GAMESERVER_LANGUAGE 0
#endif

#ifndef PROTECT_STATE
#define PROTECT_STATE 0
#endif

#ifndef ENCRYPT_STATE
#define ENCRYPT_STATE 0
#endif

// System Include
#include <windows.h>
#include <winsock2.h>
#include <mswSock.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include <random>
#include <Rpc.h>
#include <algorithm>
#include <string>
#include <atltime.h>
#include <dbghelp.h>
#include <Psapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "Psapi.lib")

typedef char chr;
typedef float flt;
typedef short shrt;
typedef unsigned __int64 QWORD;
