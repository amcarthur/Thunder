#include "stdafx.h"
#include "ProcessExceptions.h"
#include "Utils.h"

namespace thunder
{
	namespace Exceptions
	{
		ProcessAttachException::ProcessAttachException()
			: runtime_error("Failed to attach to the process.")
		{
		}

		ProcessCreateException::ProcessCreateException()
			: runtime_error("Failed to create the process.")
		{

		}

		DllFileNotFoundException::DllFileNotFoundException()
			: runtime_error("The DLL file could not be found.")
		{

		}

		ExeFileNotFoundException::ExeFileNotFoundException()
			: runtime_error("The executable file could not be found.")
		{

		}

		ProcessNotFoundException::ProcessNotFoundException()
			: runtime_error("The process was not found.")
		{

		}

		WOW64BarrierException::WOW64BarrierException()
			: runtime_error("Cannot communicate through the WOW64 barrier.")
		{

		}

		RemoteCallException::RemoteCallException()
			: runtime_error("Could not call the remote function.")
		{

		}

		InjectionFailedException::InjectionFailedException()
			: runtime_error("Failed to inject.")
		{

		}

		ModuleNotFoundException::ModuleNotFoundException()
			: runtime_error("The module was not found.")
		{

		}

		EjectionFailedException::EjectionFailedException()
			: runtime_error("Failed to eject.")
		{

		}
	}
}