#pragma once

namespace thunder
{
	typedef std::map<std::wstring, bool> CommandOptions;
	typedef std::map<std::wstring, std::wstring> CommandArguments;
	typedef std::function<void(const CommandArguments&)> CommandHandler;

	class Command
	{
	public:
		Command(const std::wstring& commandName, const CommandHandler& handler, const CommandOptions& options);
		~Command();

		const std::wstring& Name() const
		{
			return _commandName;
		}

		void ValidateArgs(const CommandArguments& args);

		void CallHandler(const CommandArguments& options)
		{
			_handler(options);
		}

	private:
		bool IsValidOption(const std::wstring& option);
		bool IsMissingRequiredOptions(const CommandArguments& args);

	private:
		std::wstring _commandName;
		CommandHandler _handler;
		CommandOptions _options;

	};
}