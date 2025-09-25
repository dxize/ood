#pragma once

#include "IDanceBehavior.h"
#include <iostream>

DanceBehavior DanceWaltz()
{
    return { []() {
        std::cout << "Dance Waltz!" << std::endl;
    } };
}