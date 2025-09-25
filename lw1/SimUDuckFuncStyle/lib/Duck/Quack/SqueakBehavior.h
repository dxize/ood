#ifndef SQUEAKBEHAVIOR_H
#define SQUEAKBEHAVIOR_H

#include "IQuakBehavior.h"
#include <iostream>

QuackBehavior Squeak()
{
    return { []() {
        std::cout << "Squeek!!!" << std::endl;
    } };
}

#endif