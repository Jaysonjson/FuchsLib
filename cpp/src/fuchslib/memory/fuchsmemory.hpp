#pragma once

#ifdef _WIN32
#include "windows.h"
using FUCHS_HANDLE = HANDLE;
using FUCHS_WORD = DWORD64;
using FUCHS_PID = DWORD;
#endif

#ifdef __linux__
/* Linux doesnt have Handles */
using FUCHS_HANDLE = nullptr;
using FUCHS_WORD = int64;
using FUCHS_PID = pid_t;
#endif