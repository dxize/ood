#pragma once

#include <string>

namespace util
{
	std::string Trim(const std::string& s);
	bool IsBlankOrComment(const std::string& line);
	std::string ToLowerAscii(std::string s);
} 
