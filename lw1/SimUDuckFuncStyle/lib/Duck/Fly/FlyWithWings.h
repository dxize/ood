#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include "IFlyBehavior.h"
#include <iostream>

FlyBehavior FlyWithWings()
{
    auto counter = std::make_shared<int>(0);

    return {
        [counter]() mutable {    
            ++(*counter);
            std::cout << "I'm flying with wings " << *counter << " times!!!\n";
        },
        [counter]() {            
            return *counter;
        }
    };
}

#endif