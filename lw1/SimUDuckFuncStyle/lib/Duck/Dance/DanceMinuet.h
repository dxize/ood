#pragma once

#include "IDanceBehavior.h"
#include <iostream>

DanceBehavior DanceMenuet()
{
    return { []() {
        std::cout << "Dance Menuet!" << std::endl;
    } };
}