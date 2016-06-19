#include "stdafx.h"
#include "Process.h"
#include "ProcessExceptions.h"
#include "Utils.h"

namespace thunder
{
	Process::Process()
	{
		_process = new blackbone::Process();
		_attached = false;
		_processId = 0;
	}

	Process::Process(blackbone::Process* proc)
	{
		_process = proc;
		_attached = true;
		_processId = _process->pid();
	}

	Process::~Process()
	{
		_process->Detach();
		_attached = false;
		_processId = 0;
		delete _process;
	}

	void Process::Attach(DWORD processId)
	{
		_processId = processId;
		NTSTATUS status = _process->Attach(processId);
		if (!NT_SUCCESS(status))
		{
			throw Exceptions::ProcessAttachException();
		}

		blackbone::Wow64Barrier barrier = _process->core().native()->GetWow64Barrier();
		if (barrier.type == blackbone::wow_32_64 || barrier.type == blackbone::wow_64_32)
		{
			throw Exceptions::WOW64BarrierException();
		}

		status = _process->EnsureInit();
		if (!NT_SUCCESS(status))
		{
			throw Exceptions::ProcessAttachException();
		}

		_attached = true;
	}

	void Process::Attach(const std::wstring& processName)
	{
		std::vector<DWORD> processIds;
		blackbone::Process::EnumByName(processName, processIds);

		if (processIds.empty())
		{
			throw Exceptions::ProcessNotFoundException();
		}

		Attach(processIds.front());
	}

	void Process::Inject(const filesystem::path& dllPath, std::wstring functionName, std::wstring functionArgument)
	{
		if (!FileExists(dllPath))
		{
			throw Exceptions::DllFileNotFoundException();
		}

		if (!_attached)
		{
			Attach(_processId);
		}

		if (!_process->valid())
		{
			throw Exceptions::ProcessNotFoundException();
		}

		NTSTATUS status;
		auto mod = _process->modules().Inject(dllPath.wstring(), &status);
		if (!NT_SUCCESS(status))
		{
			throw Exceptions::InjectionFailedException();
		}

		if (functionName.length() > 0)
		{
			Call(mod->name, functionName, functionArgument);
		}
	}

	void Process::ManuallyMap(const filesystem::path& dllPath, std::wstring functionName, std::wstring functionArgument)
	{
		if (!FileExists(dllPath))
		{
			throw Exceptions::DllFileNotFoundException();
		}

		if (!_attached)
		{
			Attach(_processId);
		}

		if (!_process->valid())
		{
			throw Exceptions::ProcessNotFoundException();
		}

		// TODO: implement all the different options
		auto moduleData = _process->mmap().MapImage(dllPath.wstring());

		if (functionName.length() > 0)
		{
			Call(moduleData->name, functionName, functionArgument);
		}
	}

	void Process::CreateAndAttach(Process& process, const filesystem::path& exePath, const std::wstring& commandLineArguments, bool createSuspended)
	{
		if (!Process::FileExists(exePath))
		{
			throw Exceptions::ExeFileNotFoundException();
		}

		blackbone::Process* proc = new blackbone::Process();
		NTSTATUS status = proc->CreateAndAttach(exePath.wstring(), createSuspended, true, commandLineArguments);
		if (!NT_SUCCESS(status))
		{
			throw Exceptions::ProcessCreateException();
		}

		process._process = proc;
		process._attached = true;
		process._processId = proc->pid();
	}

	void Process::Eject(const std::wstring& moduleName)
	{
		if (!_attached)
		{
			Attach(_processId);
		}

		if (!_process->valid())
		{
			throw Exceptions::ProcessNotFoundException();
		}

		auto module = _process->modules().GetModule(moduleName);
		if (module == nullptr)
		{
			throw Exceptions::ModuleNotFoundException();
		}

		_process->modules().Unload(module);
	}

	void Process::Call(const std::wstring& moduleName, const std::wstring& functionName, std::wstring functionArgument, bool usesArgument)
	{
		if (!_attached)
		{
			Attach(_processId);
		}

		if (!_process->valid())
		{
			throw Exceptions::ProcessNotFoundException();
		}

		auto mod = _process->modules().GetModule(moduleName);

		if (mod == nullptr)
		{
			throw Exceptions::ModuleNotFoundException();
		}

		auto pRemote = _process->modules().GetExport(mod, Utils::wideStringToString(functionName).c_str());

		if (!pRemote.procAddress)
		{
			throw Exceptions::RemoteCallException();
		}

		DWORD ret;
		if (usesArgument)
		{
			auto argMem = _process->memory().Allocate(0x1000, PAGE_READWRITE);
			argMem.Write(0, functionArgument.length() * sizeof(wchar_t) + 2, functionArgument.c_str());
			ret = _process->remote().ExecDirect(pRemote.procAddress, argMem.ptr());
		}
		else
		{
			ret = _process->remote().ExecDirect(pRemote.procAddress, NULL);
		}
		
		if (ret != 0)
		{
			throw Exceptions::RemoteCallException();
		}
	}

	void Process::Resume()
	{
		if (!NT_SUCCESS(_process->Resume()))
		{
			throw Exceptions::ProcessCreateException();
		}
	}

	void Process::InitializeCLR()
	{
		filesystem::path dllPath(L"C:\\Git\\ThunderCLRHost\\x64\\Debug\\ThunderCLRHost.dll");
		Inject(dllPath);
		Call(L"ThunderCLRHost.dll", L"fnThunderCLRHostInit", L"", false);
	}

	void Process::DestroyCLR()
	{
		Call(L"ThunderCLRHost.dll", L"fnThunderCLRHostDestroy", L"", false);
		Eject(L"ThunderCLRHost.dll");
	}

	void Process::ExecuteAssembly(const filesystem::path& assemblyPath, const std::wstring& className, const std::wstring& methodName, const std::wstring& argument)
	{
		typedef int(*fnThunderCLRHostExecute)(const wchar_t* assemblyPath, const wchar_t* className, const wchar_t* methodName, const wchar_t* argument);

		auto mod = _process->modules().GetModule(L"ThunderCLRHost.dll");

		if (mod == nullptr)
		{
			throw Exceptions::ModuleNotFoundException();
		}

		auto pRemote = _process->modules().GetExport(mod, Utils::wideStringToString(L"fnThunderCLRHostExecute").c_str());

		if (!pRemote.procAddress)
		{
			throw Exceptions::RemoteCallException();
		}

		blackbone::RemoteFunction<fnThunderCLRHostExecute> pfn(*_process, (fnThunderCLRHostExecute)pRemote.procAddress, assemblyPath.c_str(), className.c_str(), methodName.c_str(), argument.c_str());

		int ret = 0;
		NTSTATUS status = pfn.Call(ret);
	}

	bool Process::FileExists(const filesystem::path& filePath)
	{
		return filesystem::exists(filePath) && filesystem::is_regular_file(filePath);
	}
}