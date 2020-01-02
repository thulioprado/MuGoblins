#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "MemScript.h"

#define MAX_CLASS					5
#define MAX_RESISTANCE_TYPE			7

#define GET_ITEM(group, index)		(group * 512) + index

void ErrorMessageBox(const char* message, ...);
void XorConvert(LPBYTE buff, int size);