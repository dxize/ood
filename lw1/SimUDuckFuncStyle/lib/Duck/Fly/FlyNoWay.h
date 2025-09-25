#ifndef FLYNOWAY_H
#define FLYNOWAY_H

#include "IFlyBehavior.h"

FlyBehavior FlyNoWay()
{
    return {
        []() {},                 
        []() { return 0; }      
    };
}


#endif