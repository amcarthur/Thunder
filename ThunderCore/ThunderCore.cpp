// ThunderCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ThunderCore.h"
#include "ThunderCoreFactory.h"
#include "ThunderProcess.h"

THUNDERCORE_API LPUNKNOWN WINAPI CreateInstance()
{
	return static_cast<thunder::IThunderProcess*>(new thunder::ThunderProcess());
}