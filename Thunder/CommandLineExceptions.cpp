#include "stdafx.h"
#include "CommandLineExceptions.h"
#include "Utils.h"

namespace thunder
{
	namespace Exceptions
	{
		CommandNotFoundException::CommandNotFoundException() 
			: runtime_error("The command entered was invalid.")
		{ 
		}

		CommandNotFoundException::CommandNotFoundException(const std::wstring& command)
			: runtime_error("The command '" + Utils::wideStringToString(command) + "' is invalid.") 
		{
		}

		InvalidSyntaxException::InvalidSyntaxException() 
			: runtime_error("Invalid syntax.")
		{
		}
	}
}