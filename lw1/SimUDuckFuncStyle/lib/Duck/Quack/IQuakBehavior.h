#ifndef IQUAKBEHAVIOR_H
#define IQUAKBEHAVIOR_H

#include <functional>

struct QuackBehavior
{
    std::function<void()> Quack;
};

#endif