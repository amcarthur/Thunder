#pragma once

namespace thunder
{
	class Process
	{
	public:
		Process();
		Process(blackbone::Process* proc);
		~Process();

		void Attach(DWORD processId);
		void Attach(const std::wstring& processName);
		void Inject(const filesystem::path& dllPath, std::wstring functionName = L"", std::wstring functionArgument = L"");
		void ManuallyMap(const filesystem::path& dllPath, std::wstring functionName = L"", std::wstring functionArgument = L"");
		static void CreateAndAttach(Process& process, const filesystem::path& exePath, const std::wstring& commandLineArguments = L"", bool createSuspended = false);
		void Eject(const std::wstring& moduleName);
		void Call(const std::wstring& moduleName, const std::wstring& functionName, std::wstring functionArgument = L"", bool usesArgument = true);
		void Resume();
		void InitializeCLR();
		void DestroyCLR();
		void ExecuteAssembly(const filesystem::path& assemblyPath, const std::wstring& className, const std::wstring& methodName, const std::wstring& argument);
		
		static bool FileExists(const filesystem::path& filePath);

	private:
		blackbone::Process* _process;
		DWORD _processId;
		bool _attached;
	};
}