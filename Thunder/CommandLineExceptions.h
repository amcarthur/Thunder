#pragma once

namespace thunder
{
	namespace Exceptions
	{
		class CommandNotFoundException : public std::runtime_error
		{
		public:
			CommandNotFoundException();
			CommandNotFoundException(const std::wstring& command);
		};

		class InvalidSyntaxException : public std::runtime_error
		{
		public:
			InvalidSyntaxException();

		};
	}
}