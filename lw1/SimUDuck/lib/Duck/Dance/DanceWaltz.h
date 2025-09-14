#pragma once

#include "IDanceBehavior.h"
#include <iostream>

class DanceWaltz : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dance waltz" << std::endl;
	}
};