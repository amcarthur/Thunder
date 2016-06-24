// Thunder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Command.h"
#include "CommandLine.h"
#include "CommandLineExceptions.h"
#include "Utils.h"
#include "../ThunderCore\ThunderProcess.h"

using namespace thunder;

void injectHandler(const CommandArguments& arguments)
{
	auto dllArgIter = arguments.find(L"-D");
	filesystem::path dllPath(dllArgIter->second);

	auto processNameIter = arguments.find(L"-P");
	auto processIdIter = arguments.find(L"-PID");

	DWORD processId = 0;
	std::wstring processName = L"";
	bool useProcessId = false;

	if (processIdIter != arguments.end())
	{
		useProcessId = true;
		processId = std::stol(processIdIter->second);
	}
	else if (processNameIter != arguments.end())
	{
		processName = processNameIter->second;
	}
	else
	{
		throw Exceptions::InvalidSyntaxException();
	}

	auto functionIter = arguments.find(L"-F");
	std::wstring functionName = L"";
	if (functionIter != arguments.end())
	{
		functionName = functionIter->second;
	}

	auto functionArgumentIter = arguments.find(L"-A");
	std::wstring functionArgument = L"";
	if (functionArgumentIter != arguments.end())
	{
		if (functionName.length() == 0)
		{
			throw Exceptions::InvalidSyntaxException();
		}
		functionArgument = functionArgumentIter->second;
	}

	auto manualMapIter = arguments.find(L"--MM");
	bool useManualMap = false;
	if (manualMapIter != arguments.end())
	{
		useManualMap = true;
	}

	ThunderProcess proc;

	if (useProcessId)
	{
		proc.AttachById(processId);
	}
	else
	{
		proc.AttachByName(processName.c_str());
	}

	if (useManualMap)
	{
		proc.ManuallyMap(dllPath.c_str(), functionName.c_str(), functionArgument.c_str());
	}
	else
	{
		proc.Inject(dllPath.c_str(), functionName.c_str(), functionArgument.c_str());
	}
}

void createInjectHandler(const CommandArguments& arguments)
{
	auto exeArgIter = arguments.find(L"-E");
	filesystem::path exePath(exeArgIter->second);

	std::wstring commandLineArgs = L"";
	auto cmdLineArgIter = arguments.find(L"-CL");
	if (cmdLineArgIter != arguments.end())
	{
		commandLineArgs = cmdLineArgIter->second;
	}

	auto dllArgIter = arguments.find(L"-D");
	filesystem::path dllPath(dllArgIter->second);

	if (!ThunderProcess::FileExists(dllPath.c_str()))
	{
		return;
	}

	auto functionIter = arguments.find(L"-F");
	std::wstring functionName = L"";
	if (functionIter != arguments.end())
	{
		functionName = functionIter->second;
	}

	auto functionArgumentIter = arguments.find(L"-A");
	std::wstring functionArgument = L"";
	if (functionArgumentIter != arguments.end())
	{
		if (functionName.length() == 0)
		{
			throw Exceptions::InvalidSyntaxException();
		}
		functionArgument = functionArgumentIter->second;
	}

	auto manualMapIter = arguments.find(L"--MM");
	bool useManualMap = false;
	if (manualMapIter != arguments.end())
	{
		useManualMap = true;
	}

	auto createSuspendedIter = arguments.find(L"--SUS");
	bool createSuspended = false;
	if (createSuspendedIter != arguments.end())
	{
		createSuspended = true;
	}

	ThunderProcess proc;
	proc.CreateAndAttach(exePath.c_str(), commandLineArgs.c_str(), createSuspended);

	if (useManualMap)
	{
		proc.ManuallyMap(dllPath.c_str(), functionName.c_str(), functionArgument.c_str());
	}
	else
	{
		proc.Inject(dllPath.c_str(), functionName.c_str(), functionArgument.c_str());
	}

	if (createSuspended)
	{
		proc.Resume();
	}
}

void callHandler(const CommandArguments& arguments)
{
	auto processNameIter = arguments.find(L"-P");
	auto processIdIter = arguments.find(L"-PID");

	DWORD processId = 0;
	std::wstring processName = L"";
	bool useProcessId = false;

	if (processIdIter != arguments.end())
	{
		useProcessId = true;
		processId = std::stol(processIdIter->second);
	}
	else if (processNameIter != arguments.end())
	{
		processName = processNameIter->second;
	}
	else
	{
		throw Exceptions::InvalidSyntaxException();
	}

	auto moduleIter = arguments.find(L"-M");
	std::wstring moduleName = moduleIter->second;

	auto functionIter = arguments.find(L"-F");
	std::wstring functionName = functionIter->second;

	auto functionArgumentIter = arguments.find(L"-A");
	std::wstring functionArgument = L"";
	if (functionArgumentIter != arguments.end())
	{
		functionArgument = functionArgumentIter->second;
	}

	ThunderProcess proc;

	if (useProcessId)
	{
		proc.AttachById(processId);
	}
	else
	{
		proc.AttachByName(processName.c_str());
	}

	if (functionArgument.length() > 0)
	{
		proc.Call(moduleName.c_str(), functionName.c_str(), true, functionArgument.c_str());
	}
	else
	{
		proc.Call(moduleName.c_str(), functionName.c_str());
	}
}

void ejectHandler(const CommandArguments& arguments)
{
	auto processNameIter = arguments.find(L"-P");
	auto processIdIter = arguments.find(L"-PID");

	DWORD processId = 0;
	std::wstring processName = L"";
	bool useProcessId = false;

	if (processIdIter != arguments.end())
	{
		useProcessId = true;
		processId = std::stol(processIdIter->second);
	}
	else if (processNameIter != arguments.end())
	{
		processName = processNameIter->second;
	}
	else
	{
		throw Exceptions::InvalidSyntaxException();
	}

	auto moduleIter = arguments.find(L"-M");
	std::wstring moduleName = moduleIter->second;

	ThunderProcess proc;

	if (useProcessId)
	{
		proc.AttachById(processId);
	}
	else
	{
		proc.AttachByName(processName.c_str());
	}

	proc.Eject(moduleName.c_str());
}

void hostClrHandler(const CommandArguments& arguments)
{
	auto processNameIter = arguments.find(L"-P");
	auto processIdIter = arguments.find(L"-PID");

	DWORD processId = 0;
	std::wstring processName = L"";
	bool useProcessId = false;

	if (processIdIter != arguments.end())
	{
		useProcessId = true;
		processId = std::stol(processIdIter->second);
	}
	else if (processNameIter != arguments.end())
	{
		processName = processNameIter->second;
	}
	else
	{
		throw Exceptions::InvalidSyntaxException();
	}

	ThunderProcess proc;

	if (useProcessId)
	{
		proc.AttachById(processId);
	}
	else
	{
		proc.AttachByName(processName.c_str());
	}

	proc.InitializeCLR();
}

void stopClrHandler(const CommandArguments& arguments)
{
	auto processNameIter = arguments.find(L"-P");
	auto processIdIter = arguments.find(L"-PID");

	DWORD processId = 0;
	std::wstring processName = L"";
	bool useProcessId = false;

	if (processIdIter != arguments.end())
	{
		useProcessId = true;
		processId = std::stol(processIdIter->second);
	}
	else if (processNameIter != arguments.end())
	{
		processName = processNameIter->second;
	}
	else
	{
		throw Exceptions::InvalidSyntaxException();
	}

	ThunderProcess proc;

	if (useProcessId)
	{
		proc.AttachById(processId);
	}
	else
	{
		proc.AttachByName(processName.c_str());
	}

	proc.DestroyCLR();
}

void invokeClrHandler(const CommandArguments& arguments)
{
	auto processNameIter = arguments.find(L"-P");
	auto processIdIter = arguments.find(L"-PID");

	DWORD processId = 0;
	std::wstring processName = L"";
	bool useProcessId = false;

	if (processIdIter != arguments.end())
	{
		useProcessId = true;
		processId = std::stol(processIdIter->second);
	}
	else if (processNameIter != arguments.end())
	{
		processName = processNameIter->second;
	}
	else
	{
		throw Exceptions::InvalidSyntaxException();
	}

	// thunder invokeclr [-P <process name>] [-PID <process id>] -ASS <assembly path> -C <class> -M <method>] [-A <argument>]

	auto assemblyIter = arguments.find(L"-ASS");
	std::wstring assembly = assemblyIter->second;

	auto classIter = arguments.find(L"-C");
	std::wstring className = classIter->second;

	auto methodIter = arguments.find(L"-M");
	std::wstring methodName = methodIter->second;
	
	auto argumentIter = arguments.find(L"-A");
	std::wstring argument = L"";
	if (argumentIter != arguments.end())
	{
		argument = argumentIter->second;
	}

	ThunderProcess proc;

	if (useProcessId)
	{
		proc.AttachById(processId);
	}
	else
	{
		proc.AttachByName(processName.c_str());
	}

	proc.ExecuteAssembly(assembly.c_str(), className.c_str(), methodName.c_str(), argument.c_str());
}

int wmain(int argc, wchar_t* argv[])
{
	CommandLine commandLine;

	CommandOptions injectOptions;
	injectOptions.insert(std::make_pair(L"-D", true));
	injectOptions.insert(std::make_pair(L"-P", false));
	injectOptions.insert(std::make_pair(L"-PID", false));
	injectOptions.insert(std::make_pair(L"-F", false));
	injectOptions.insert(std::make_pair(L"-A", false));
	injectOptions.insert(std::make_pair(L"--MM", false));
	Command injectCommand(L"inject", injectHandler, injectOptions);
	commandLine.AddCommand(injectCommand);

	CommandOptions createInjectOptions;
	createInjectOptions.insert(std::make_pair(L"-E", true));
	createInjectOptions.insert(std::make_pair(L"-CL", false));
	createInjectOptions.insert(std::make_pair(L"-D", true));
	createInjectOptions.insert(std::make_pair(L"-P", false));
	createInjectOptions.insert(std::make_pair(L"-PID", false));
	createInjectOptions.insert(std::make_pair(L"-F", false));
	createInjectOptions.insert(std::make_pair(L"-A", false));
	createInjectOptions.insert(std::make_pair(L"--MM", false));
	createInjectOptions.insert(std::make_pair(L"--SUS", false));
	Command createInjectCommand(L"createinject", createInjectHandler, createInjectOptions);
	commandLine.AddCommand(createInjectCommand);

	CommandOptions callOptions;
	callOptions.insert(std::make_pair(L"-P", false));
	callOptions.insert(std::make_pair(L"-PID", false));
	callOptions.insert(std::make_pair(L"-M", true));
	callOptions.insert(std::make_pair(L"-F", true));
	callOptions.insert(std::make_pair(L"-A", false));
	Command callCommand(L"call", callHandler, callOptions);
	commandLine.AddCommand(callCommand);

	CommandOptions ejectOptions;
	ejectOptions.insert(std::make_pair(L"-P", false));
	ejectOptions.insert(std::make_pair(L"-PID", false));
	ejectOptions.insert(std::make_pair(L"-M", true));
	Command ejectCommand(L"eject", ejectHandler, ejectOptions);
	commandLine.AddCommand(ejectCommand);

	CommandOptions hostClrOptions;
	hostClrOptions.insert(std::make_pair(L"-P", false));
	hostClrOptions.insert(std::make_pair(L"-PID", false));
	Command hostClrCommand(L"hostclr", hostClrHandler, hostClrOptions);
	commandLine.AddCommand(hostClrCommand);

	CommandOptions stopClrOptions;
	stopClrOptions.insert(std::make_pair(L"-P", false));
	stopClrOptions.insert(std::make_pair(L"-PID", false));
	Command stopClrCommand(L"stopclr", stopClrHandler, stopClrOptions);
	commandLine.AddCommand(stopClrCommand);

	CommandOptions invokeClrOptions;
	invokeClrOptions.insert(std::make_pair(L"-P", false));
	invokeClrOptions.insert(std::make_pair(L"-PID", false));
	invokeClrOptions.insert(std::make_pair(L"-ASS", true));
	invokeClrOptions.insert(std::make_pair(L"-C", true));
	invokeClrOptions.insert(std::make_pair(L"-M", true));
	invokeClrOptions.insert(std::make_pair(L"-A", false));
	Command invokeClrCommand(L"invokeclr", invokeClrHandler, invokeClrOptions);
	commandLine.AddCommand(invokeClrCommand);

	try
	{
		commandLine.Parse(argc, argv);
	}
	catch (const Exceptions::CommandNotFoundException& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const Exceptions::InvalidSyntaxException& e)
	{
		std::cerr << e.what() << std::endl;
	}

    return 0;
}

