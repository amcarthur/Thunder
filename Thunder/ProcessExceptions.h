#pragma once

namespace thunder
{
	namespace Exceptions
	{
		class ProcessAttachException : public std::runtime_error
		{
		public:
			ProcessAttachException();
		};

		class ProcessCreateException : public std::runtime_error
		{
		public:
			ProcessCreateException();
		};

		class DllFileNotFoundException : public std::runtime_error
		{
		public:
			DllFileNotFoundException();
		};

		class ExeFileNotFoundException : public std::runtime_error
		{
		public:
			ExeFileNotFoundException();
		};

		class ProcessNotFoundException : public std::runtime_error
		{
		public:
			ProcessNotFoundException();
		};

		class WOW64BarrierException : public std::runtime_error
		{
		public:
			WOW64BarrierException();
		};

		class RemoteCallException : public std::runtime_error
		{
		public:
			RemoteCallException();
		};

		class InjectionFailedException : public std::runtime_error
		{
		public:
			InjectionFailedException();
		};

		class ModuleNotFoundException : public std::runtime_error
		{
		public:
			ModuleNotFoundException();
		};

		class EjectionFailedException : public std::runtime_error
		{
		public:
			EjectionFailedException();
		};
	}
}