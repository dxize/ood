#ifndef LAB01_SIMUDUCK_IFLYBEHAVIOR_H
#define LAB01_SIMUDUCK_IFLYBEHAVIOR_H

#include <functional>


struct FlyBehavior
{
    std::function<void()> Fly;
    std::function<int()> GetCount;
};

#endif