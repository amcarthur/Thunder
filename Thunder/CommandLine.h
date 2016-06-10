#pragma once

namespace thunder
{
	class CommandLine
	{
	public:
		CommandLine();
		~CommandLine();

		void AddCommand(const Command& command);

		void Parse(int argc, wchar_t* commandLine[]);

	private:
		bool IsOption(const std::wstring& str);
		bool IsFlag(const std::wstring& str);

	private:
		std::vector<Command> _commands;
	};
}