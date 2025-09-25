#pragma once
#include "ICommand.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Commands.h"

class CCommandParser
{
public:
	CCommandParser() noexcept;

	std::unique_ptr<ICommand> Parse(const std::string& input);
private:
	std::unordered_map<std::string, 
		std::function<std::unique_ptr<ICommand>()>> m_factory;
};