#pragma once

#include <string>

#include "../domain/Color.h"

namespace util
{
	domain::Color ParseColor(const std::string& token);

	const char* ToSvgColor(domain::Color c);
	const char* ToTextColor(domain::Color c);
} 
