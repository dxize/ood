#pragma once

class IDanceBehavior
{
public:
	virtual ~IDanceBehavior() {};
	virtual void Dance() const = 0;
};