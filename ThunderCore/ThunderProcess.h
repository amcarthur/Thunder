#pragma once

namespace thunder
{
	// {540F822B-E4EF-4A18-9FA4-344460190D71}
	static const GUID IID_ThunderInterface =
	{ 0x540f822b, 0xe4ef, 0x4a18,{ 0x9f, 0xa4, 0x34, 0x44, 0x60, 0x19, 0xd, 0x71 } };

	struct IThunderProcess : public IUnknown
	{
	public:
		STDMETHOD(AttachById)(DWORD processId) = 0;
		STDMETHOD(AttachByName)(LPCWSTR processName) = 0;
		STDMETHOD(CreateAndAttach)(LPCWSTR exePath, LPCWSTR commandLineArguments = L"", BOOL createSuspended = FALSE) = 0;
		STDMETHOD(Resume)() = 0;
		STDMETHOD(Inject)(LPCWSTR dllPath, LPCWSTR functionName = L"", LPCWSTR functionArgument = L"") = 0;
		STDMETHOD(ManuallyMap)(LPCWSTR dllPath, LPCWSTR functionName = L"", LPCWSTR functionArgument = L"") = 0;
		STDMETHOD(Eject)(LPCWSTR moduleName) = 0;
		STDMETHOD(Call)(LPCWSTR moduleName, LPCWSTR functionName, BOOL usesArgument = FALSE, LPCWSTR functionArgument = L"") = 0;
		STDMETHOD(InitializeCLR)() = 0;
		STDMETHOD(DestroyCLR)() = 0;
		STDMETHOD(ExecuteAssembly)(LPCWSTR assemblyPath, LPCWSTR className, LPCWSTR methodName, LPCWSTR methodArgument = L"") = 0;
	};

	class ThunderProcess : public IThunderProcess, public ISupportErrorInfo
	{
	public:
		ThunderProcess();
		ThunderProcess(blackbone::Process* proc);
		~ThunderProcess();

		STDMETHOD(QueryInterface)(REFIID guid, void **pObj);
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();
		STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

		STDMETHOD(AttachById)(DWORD processId);
		STDMETHOD(AttachByName)(LPCWSTR processName);
		STDMETHOD(CreateAndAttach)(LPCWSTR exePath, LPCWSTR commandLineArguments = L"", BOOL createSuspended = FALSE);
		STDMETHOD(Resume)();
		STDMETHOD(Inject)(LPCWSTR dllPath, LPCWSTR functionName = L"", LPCWSTR functionArgument = L"");
		STDMETHOD(ManuallyMap)(LPCWSTR dllPath, LPCWSTR functionName = L"", LPCWSTR functionArgument = L"");
		STDMETHOD(Eject)(LPCWSTR moduleName);
		STDMETHOD(Call)(LPCWSTR moduleName, LPCWSTR functionName, BOOL usesArgument = FALSE, LPCWSTR functionArgument = L"");
		STDMETHOD(InitializeCLR)();
		STDMETHOD(DestroyCLR)();
		STDMETHOD(ExecuteAssembly)(LPCWSTR assemblyPath, LPCWSTR className, LPCWSTR methodName, LPCWSTR methodArgument = L"");
		
		static bool FileExists(LPCWSTR filePath);

	private:
		volatile long _refcount;
		blackbone::Process* _process;
		bool _attached;
		LPCWSTR THUNDERCLRHOST_DLL = L"ThunderCLRHost.dll";
		LPCWSTR THUNDERCLRHOST_INIT = L"fnThunderCLRHostInit";
		LPCWSTR THUNDERCLRHOST_DESTROY = L"fnThunderCLRHostDestroy";
		LPCWSTR THUNDERCLRHOST_EXECUTE = L"fnThunderCLRHostExecute";
	};
}