#include "stdafx.h"
#include "ThunderProcessFactory.h"
#include "ThunderProcess.h"

namespace thunder
{
	ULONG g_lockCount = 0;
	ULONG g_objCount = 0;

	ThunderProcessFactory::ThunderProcessFactory()
	{
		m_refCount = 0;
		g_objCount++;
	}

	ThunderProcessFactory::~ThunderProcessFactory()
	{
		g_objCount--;
	}

	ULONG __stdcall ThunderProcessFactory::AddRef()
	{
		return ++m_refCount;
	}

	ULONG __stdcall ThunderProcessFactory::Release()
	{
		if (--m_refCount == 0)
		{
			delete this;
			return 0;
		}

		return m_refCount;
	}

	HRESULT __stdcall ThunderProcessFactory::QueryInterface(REFIID riid, void ** ppAny)
	{
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

	HRESULT __stdcall ThunderProcessFactory::CreateInstance
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

	HRESULT __stdcall ThunderProcessFactory::LockServer(BOOL fLock)
	{
		if (fLock)
			g_lockCount++;
		else
			g_lockCount--;

		return S_OK;
	}
}