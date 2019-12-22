#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>
#include <thread>
#include <combaseapi.h>
#include <unordered_map>
#include <gl/GL.h>

typedef unsigned __int64 QWORD;

#define null		nullptr

#include "detours.h"
#include "Memory.h"
#include "Registry.h"
#include "Defines.h"