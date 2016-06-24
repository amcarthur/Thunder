#include "stdafx.h"
#include "ThunderProcess.h"
#include "Utils.h"

namespace thunder
{
	ThunderProcess::ThunderProcess()
	{
		_process = new blackbone::Process();
		_attached = false;
		_refcount = 1;
	}

	ThunderProcess::ThunderProcess(blackbone::Process* proc)
	{
		_process = proc;
		_attached = true;
		_refcount = 1;
	}

	ThunderProcess::~ThunderProcess()
	{
		_process->Detach();
		_attached = false;
		delete _process;
	}

	STDMETHODIMP ThunderProcess::QueryInterface(REFIID guid, void **pObj) {
		if (pObj == NULL) {
			return E_POINTER;
		}
		else if (guid == IID_IUnknown) {
			*pObj = this;
			AddRef();
			return S_OK;
		}
		else if (guid == IID_ThunderInterface) {
			*pObj = this;
			AddRef();
			return S_OK;
		}
		else {
			*pObj = NULL;
			return E_NOINTERFACE;
		}
	}

	STDMETHODIMP_(ULONG) ThunderProcess::AddRef() {
		return InterlockedIncrement(&_refcount);
	}

	STDMETHODIMP_(ULONG) ThunderProcess::Release() {
		ULONG result = InterlockedDecrement(&_refcount);
		if (result == 0) delete this;
		return result;
	}

	STDMETHODIMP ThunderProcess::InterfaceSupportsErrorInfo(REFIID riid)
	{
		if (InlineIsEqualGUID(IID_ThunderInterface, riid))
		{
			return S_OK;
		}

		return S_FALSE;
	}

	STDMETHODIMP ThunderProcess::AttachById(DWORD processId)
	{
		NTSTATUS status = _process->Attach(processId);
		if (!NT_SUCCESS(status))
		{
			return E_FAIL;
		}

		blackbone::Wow64Barrier barrier = _process->core().native()->GetWow64Barrier();
		if (barrier.type == blackbone::wow_32_64 || barrier.type == blackbone::wow_64_32)
		{
			return E_NOTIMPL;
		}

		status = _process->EnsureInit();
		if (!NT_SUCCESS(status))
		{
			return E_FAIL;
		}

		_attached = true;
		return S_OK;
	}

	STDMETHODIMP ThunderProcess::AttachByName(LPCWSTR processName)
	{
		std::vector<DWORD> processIds;
		blackbone::Process::EnumByName(processName, processIds);

		if (processIds.empty())
		{
			// Create generic error object.
			ICreateErrorInfo* pCreateErrorInfo;
			CreateErrorInfo(&pCreateErrorInfo);

			// Set rich error information.
			pCreateErrorInfo->SetDescription(
				L"The process was not found.");
			pCreateErrorInfo->SetGUID(IID_ThunderInterface);
			pCreateErrorInfo->SetSource(L"Thunder.ThunderProcess");

			// Exchange ICreateErrorInfo for IErrorInfo.
			IErrorInfo* pErrorInfo;
			pCreateErrorInfo->QueryInterface(IID_IErrorInfo,
				(void**)&pErrorInfo);

			// Make the error information available to the client.
			SetErrorInfo(0, pErrorInfo);

			// Release the interface pointers.
			pErrorInfo->Release();
			pCreateErrorInfo->Release();

			return E_INVALIDARG;
		}

		return AttachById(processIds.front());
	}

	STDMETHODIMP ThunderProcess::Inject(LPCWSTR dllPath, LPCWSTR functionName, LPCWSTR functionArgument)
	{
		if (!FileExists(dllPath))
		{
			return E_INVALIDARG;
		}

		if (!_attached)
		{
			HRESULT attachResult = AttachById(_process->pid());
			if (FAILED(attachResult))
			{
				return attachResult;
			}
		}

		if (!_process->valid())
		{
			return E_HANDLE;
		}

		NTSTATUS status;
		auto mod = _process->modules().Inject(dllPath, &status);
		if (!NT_SUCCESS(status))
		{
			return E_FAIL;
		}

		if (wcslen(functionName) > 0)
		{
			HRESULT callResult = Call(mod->name.c_str(), functionName, TRUE, functionArgument);
			if (FAILED(callResult))
			{
				return callResult;
			}
		}

		return S_OK;
	}

	STDMETHODIMP ThunderProcess::ManuallyMap(LPCWSTR dllPath, LPCWSTR functionName, LPCWSTR functionArgument)
	{
		if (!FileExists(dllPath))
		{
			return E_INVALIDARG;
		}

		if (!_attached)
		{
			HRESULT attachResult = AttachById(_process->pid());
			if (FAILED(attachResult))
			{
				return attachResult;
			}
		}

		if (!_process->valid())
		{
			return E_FAIL;
		}

		// TODO: implement all the different options
		auto moduleData = _process->mmap().MapImage(dllPath);

		if (moduleData == nullptr)
		{
			return E_FAIL;
		}

		if (wcslen(functionName) > 0)
		{
			HRESULT callResult = Call(moduleData->name.c_str(), functionName, true, functionArgument);
			if (FAILED(callResult))
			{
				return E_FAIL;
			}
		}

		return S_OK;
	}

	STDMETHODIMP ThunderProcess::CreateAndAttach(LPCWSTR exePath, LPCWSTR commandLineArguments, BOOL createSuspended)
	{
		if (!ThunderProcess::FileExists(exePath))
		{
			return E_INVALIDARG;
		}

		blackbone::Process* proc = new blackbone::Process();
		NTSTATUS status = proc->CreateAndAttach(exePath, createSuspended, true, commandLineArguments);
		if (!NT_SUCCESS(status))
		{
			return E_FAIL;
		}

		_process = proc;
		_attached = true;

		return S_OK;
	}

	STDMETHODIMP ThunderProcess::Eject(LPCWSTR moduleName)
	{
		if (!_attached)
		{
			HRESULT attachResult = AttachById(_process->pid());
			if (FAILED(attachResult))
			{
				return E_FAIL;
			}
		}

		if (!_process->valid())
		{
			return E_FAIL;
		}

		auto module = _process->modules().GetModule(moduleName);
		if (module == nullptr)
		{
			return E_FAIL;
		}

		if (!_process->modules().Unload(module))
		{
			return E_FAIL;
		}

		return S_OK;
	}

	STDMETHODIMP ThunderProcess::Call(LPCWSTR moduleName, LPCWSTR functionName, BOOL usesArgument, LPCWSTR functionArgument)
	{
		if (!_attached)
		{
			HRESULT attachResult = AttachById(_process->pid());
			if (FAILED(attachResult))
			{
				return E_FAIL;
			}
		}

		if (!_process->valid())
		{
			return E_FAIL;
		}

		auto mod = _process->modules().GetModule(moduleName);

		if (mod == nullptr)
		{
			return E_FAIL;
		}

		auto pRemote = _process->modules().GetExport(mod, Utils::wideStringToString(functionName).c_str());
		
		if (!pRemote.procAddress)
		{
			return E_FAIL;
		}

		DWORD ret;
		if (usesArgument)
		{
			auto argMem = _process->memory().Allocate(0x1000, PAGE_READWRITE);
			if (!argMem.valid())
			{
				return E_FAIL;
			}

			NTSTATUS status = argMem.Write(0, wcslen(functionArgument) * sizeof(wchar_t) + 2, functionArgument);
			if (!NT_SUCCESS(status))
			{
				return E_FAIL;
			}

			ret = _process->remote().ExecDirect(pRemote.procAddress, argMem.ptr());
		}
		else
		{
			ret = _process->remote().ExecDirect(pRemote.procAddress, NULL);
		}

		return S_OK;
	}

	STDMETHODIMP ThunderProcess::Resume()
	{
		if (!NT_SUCCESS(_process->Resume()))
		{
			return E_FAIL;
		}

		return S_OK;
	}

	STDMETHODIMP ThunderProcess::InitializeCLR()
	{
		auto thunderCLRHostDllPath = filesystem::current_path() / THUNDERCLRHOST_DLL;
		HRESULT injectResult = Inject(thunderCLRHostDllPath.c_str());
		if (FAILED(injectResult))
		{
			return injectResult;
		}

		return Call(THUNDERCLRHOST_DLL, THUNDERCLRHOST_INIT);
	}

	STDMETHODIMP ThunderProcess::DestroyCLR()
	{
		HRESULT callResult = Call(THUNDERCLRHOST_DLL, THUNDERCLRHOST_DESTROY);
		if (FAILED(callResult))
		{
			return callResult;
		}
		
		return Eject(THUNDERCLRHOST_DLL);
	}

	STDMETHODIMP ThunderProcess::ExecuteAssembly(LPCWSTR assemblyPath, LPCWSTR className, LPCWSTR methodName, LPCWSTR argument)
	{
		typedef int(*fnThunderCLRHostExecute)(const wchar_t* assemblyPath, const wchar_t* className, const wchar_t* methodName, const wchar_t* argument);

		if (!ThunderProcess::FileExists(assemblyPath))
		{
			return E_INVALIDARG;
		}

		auto mod = _process->modules().GetModule(THUNDERCLRHOST_DLL);

		if (mod == nullptr)
		{
			return E_FAIL;
		}

		auto pRemote = _process->modules().GetExport(mod, Utils::wideStringToString(THUNDERCLRHOST_EXECUTE).c_str());

		if (!pRemote.procAddress)
		{
			return E_FAIL;
		}

		blackbone::RemoteFunction<fnThunderCLRHostExecute> pfn(*_process, (fnThunderCLRHostExecute)pRemote.procAddress, assemblyPath, className, methodName, argument);

		int ret = 0;
		NTSTATUS status = pfn.Call(ret);
		if (!NT_SUCCESS(status))
		{
			return E_FAIL;
		}

		return S_OK;
	}

	bool ThunderProcess::FileExists(LPCWSTR filePath)
	{
		return filesystem::exists(filePath) && filesystem::is_regular_file(filePath);
	}
}