#ifndef MUTEQUAKBEHAVIOR_H
#define MUTEQUAKBEHAVIOR_H

#include "IQuakBehavior.h"

QuackBehavior MuteQuack()
{
    return { []() {} };
}

#endif