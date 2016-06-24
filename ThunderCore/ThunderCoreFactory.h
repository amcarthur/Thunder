#pragma once

namespace thunder
{
	extern ULONG g_lockCount;
	extern ULONG g_objCount;

	class ThunderCoreFactory : public IClassFactory {
	public:
		ThunderCoreFactory();
		virtual ~ThunderCoreFactory();

		HRESULT __stdcall QueryInterface(REFIID riid, void ** ppAny);
		ULONG __stdcall AddRef();
		ULONG __stdcall Release();

		HRESULT __stdcall CreateInstance
			(LPUNKNOWN pUnkOuter, REFIID riid, void ** ppAny);
		HRESULT __stdcall LockServer(BOOL fLock);

	private:
		ULONG m_refCount;
	};
}