#ifndef MODELDUCK_H
#define MODELDUCK_H

#include "Duck.h"
#include "Fly/FlyNoWay.h"
#include "Quack/QuackBehavior.h"
#include "Dance/DanceNoWay.h"

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(FlyNoWay(), QuackDefault(), DanceNoWay())
	{
	}

	void Display() const override
	{
		std::cout << "I'm model duck" << std::endl;
	}
};

#endif