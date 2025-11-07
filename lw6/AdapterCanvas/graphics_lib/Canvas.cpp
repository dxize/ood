#include "ICanvas.h"
#include <iostream>

namespace graphics_lib
{
    class CCanvas : public ICanvas
    {
    public:
        void MoveTo(int x, int y) override
        {
            std::cout << "MoveTo (" << x << ", " << y << ")" << std::endl;
        }
        void LineTo(int x, int y) override
        {
            std::cout << "LineTo (" << x << ", " << y << ")" << std::endl;
        }
    };
}
