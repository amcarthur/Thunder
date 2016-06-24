#ifdef THUNDERCORE_EXPORTS
#define THUNDERCORE_API extern "C" __declspec(dllexport)
#else
#define THUNDERCORE_API __declspec(dllimport)
#endif

THUNDERCORE_API LPUNKNOWN WINAPI CreateInstance();