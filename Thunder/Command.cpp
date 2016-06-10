#include "stdafx.h"
#include "Command.h"
#include "CommandLineExceptions.h"

namespace thunder
{
	Command::Command(const std::wstring& commandName, const CommandHandler& handler, const CommandOptions& options)
		: _commandName(commandName), _handler(handler), _options(options)
	{

	}

	Command::~Command()
	{

	}

	void Command::ValidateArgs(const CommandArguments& args)
	{
		if (args.empty())
		{
			throw Exceptions::InvalidSyntaxException();
		}

		for (const auto& arg : args)
		{
			if (!IsValidOption(arg.first))
			{
				throw Exceptions::InvalidSyntaxException();
			}
		}

		if (IsMissingRequiredOptions(args))
		{
			throw Exceptions::InvalidSyntaxException();
		}
	}

	bool Command::IsValidOption(const std::wstring& option)
	{
		auto optionIter = _options.find(option);
		return optionIter != _options.end();
	}

	bool Command::IsMissingRequiredOptions(const CommandArguments& args)
	{
		for (const auto& opt : _options)
		{
			std::wstring optionName = opt.first;
			bool optionRequired = opt.second;

			auto optionIter = std::find_if(args.begin(), args.end(), [&optionName] (const std::pair<std::wstring, std::wstring>& arg) {
				return arg.first == optionName;
			});

			if (optionIter == args.end() && optionRequired)
			{
				return true;
			}
		}

		return false;
	}
}