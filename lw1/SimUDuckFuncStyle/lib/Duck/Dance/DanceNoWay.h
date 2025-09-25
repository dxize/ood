#pragma once

#include "IDanceBehavior.h"

DanceBehavior DanceNoWay()
{
    return { []() {} };
}