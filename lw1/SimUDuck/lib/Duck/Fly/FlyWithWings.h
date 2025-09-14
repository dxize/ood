#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings : public IFlyBehavior
{
public:
	void Fly() const override
	{
		std::cout << "I'm flying with wings " << m_countFly
			<< " times!!!" << std::endl;
	}

	int GetCountFly() const override
	{
		return m_countFly;
	}

	void SetCountFly() override
	{
		m_countFly++;
	}
private:
	int m_countFly = 0;
};
#endif