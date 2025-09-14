#ifndef LAB01_SIMUDUCK_IFLYBEHAVIOR_H
#define LAB01_SIMUDUCK_IFLYBEHAVIOR_H

struct IFlyBehavior
{
	virtual ~IFlyBehavior() {};
	virtual void Fly() const = 0;
	virtual int GetCountFly() const = 0;
	virtual void SetCountFly() = 0;
};
#endif