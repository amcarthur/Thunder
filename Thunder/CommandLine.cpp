#include "stdafx.h"
#include "Command.h"
#include "CommandLine.h"
#include "CommandLineExceptions.h"

namespace thunder
{
	CommandLine::CommandLine()
	{

	}

	CommandLine::~CommandLine()
	{

	}

	void CommandLine::AddCommand(const Command& command)
	{
		_commands.push_back(command);
	}

	void CommandLine::Parse(int argc, wchar_t* argv[])
	{
		if (argc < 3)
		{
			throw Exceptions::InvalidSyntaxException();
		}

		std::wstring command(argv[1]);

		auto commandIter = std::find_if(_commands.begin(), _commands.end(), [&command](const Command& c) {
			return c.Name() == command;
		});

		if (commandIter == _commands.end())
		{
			throw Exceptions::CommandNotFoundException(command);
		}

		CommandArguments args;

		bool waitingOnArgument = false;
		std::wstring optionWaiting = L"";
		for (int i = 2; i < argc; ++i)
		{
			std::wstring opt(argv[i]);
			if (IsFlag(opt))
			{
				if (waitingOnArgument)
				{
					throw Exceptions::InvalidSyntaxException();
				}

				optionWaiting = L"";
				args.insert(std::make_pair(opt, L""));
			}
			else if (IsOption(opt))
			{
				if (waitingOnArgument)
				{
					throw Exceptions::InvalidSyntaxException();
				}

				waitingOnArgument = true;
				optionWaiting = opt;
				args.insert(std::make_pair(opt, L""));
			}
			else if (waitingOnArgument)
			{
				waitingOnArgument = false;
				auto argIter = args.find(optionWaiting);
				if (argIter == args.end())
				{
					throw Exceptions::InvalidSyntaxException();
				}

				argIter->second = opt;
			}
			else
			{
				throw Exceptions::InvalidSyntaxException();
			}
		}

		if (waitingOnArgument)
		{
			throw Exceptions::InvalidSyntaxException();
		}

		commandIter->ValidateArgs(args);
		commandIter->CallHandler(args);
	}

	bool CommandLine::IsOption(const std::wstring& str)
	{
		if (str.length() < 2)
			return false;

		if (IsFlag(str))
			return false;

		return str[0] == L'-';
	}

	bool CommandLine::IsFlag(const std::wstring& str)
	{
		if (str.length() < 3)
			return false;

		return str[0] == L'-' && str[1] == L'-';
	}
}