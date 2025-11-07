#pragma once
#include <iostream>

namespace graphics_lib
{
    class ICanvas
    {
    public:
        virtual void MoveTo(int x, int y) = 0;
        virtual void LineTo(int x, int y) = 0;
        virtual ~ICanvas() = default;
    };
}
