#ifndef FLYNOWAY_H
#define FLYNOWAY_H

#include "IFlyBehavior.h"

class FlyNoWay : public IFlyBehavior
{
public:
	void Fly() const override {}

	int GetCountFly() const override
	{
		return 0;
	}

	void SetCountFly() override {}
};

#endif