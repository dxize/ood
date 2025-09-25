#ifndef QUACKBEHAVIOR_H
#define QUACKBEHAVIOR_H

#include "IQuakBehavior.h"
#include <iostream>

QuackBehavior QuackDefault()
{
    return { []() {
        std::cout << "Quack Quack!!!" << std::endl;
    } };
}

#endif