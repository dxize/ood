#pragma once

#include "IDanceBehavior.h"
#include <iostream>

class DanceMinuet : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dance minuet" << std::endl;
	}
};