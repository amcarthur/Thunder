#pragma once

namespace thunder
{
	namespace Utils
	{
		std::wstring stringToWideString(const std::string& str);
		std::string wideStringToString(const std::wstring& wstr);
	}
}