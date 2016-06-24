#include "stdafx.h"
#include "ThunderCoreFactory.h"
#include "ThunderCore.h"
#include "ThunderProcess.h"

namespace thunder
{
	ULONG g_lockCount = 0;
	ULONG g_objCount = 0;

	ThunderCoreFactory::ThunderCoreFactory()
	{
		m_refCount = 0;
		g_objCount++;
	}

	ThunderCoreFactory::~ThunderCoreFactory()
	{
		g_objCount--;
	}

	ULONG __stdcall ThunderCoreFactory::AddRef()
	{
		return ++m_refCount;
	}

	ULONG __stdcall ThunderCoreFactory::Release()
	{
		if (--m_refCount == 0)
		{
			delete this;
			return 0;
		}

		return m_refCount;
	}

	HRESULT __stdcall ThunderCoreFactory::QueryInterface(REFIID riid, void ** ppAny)
	{
		// IID_IUnknown is the REFIID of standard interface IUnknown
		if (riid == IID_IUnknown)
		{
			*ppAny = (IUnknown *)this;
		}
		else if (riid == IID_IClassFactory)
		{
			*ppAny = (IClassFactory *)this;
		}
		else
		{
			*ppAny = NULL;
			return E_NOINTERFACE;
		}

		((IUnknown *)(*ppAny))->AddRef();
		return S_OK;
	}

	HRESULT __stdcall ThunderCoreFactory::CreateInstance
		(LPUNKNOWN pUnkOuter, REFIID riid, void ** ppAny)
	{
		if (pUnkOuter != NULL)
		{
			return CLASS_E_NOAGGREGATION;
		}

		thunder::ThunderProcess* pThunderProcess = new thunder::ThunderProcess();
		HRESULT hr = pThunderProcess->QueryInterface(riid, ppAny);
		if (FAILED(hr)) delete pThunderProcess;
		return hr;
	}

	HRESULT __stdcall ThunderCoreFactory::LockServer(BOOL fLock)
	{
		if (fLock)
			g_lockCount++;
		else
			g_lockCount--;

		return S_OK;
	}
}