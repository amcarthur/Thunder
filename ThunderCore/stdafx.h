// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <stdexcept>
#include <locale>
#include <codecvt>
#include <filesystem>

#include <BlackBone\Process\Process.h>
#include <BlackBone\Process\RPC\RemoteFunction.hpp>
#include <BlackBone\PE\ImageNET.h>

#define BLACKBONE_STATIC

using namespace std::experimental;