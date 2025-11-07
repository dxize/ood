#include <iostream>
#include <string>
#include "app/PaintPicture.h"

int main()
{
    std::cout << "Should we use new API (y)? ";
    std::string input;
    if (std::getline(std::cin, input) && (input == "y" || input == "Y"))
    {
        app::PaintPictureOnModernGraphicsRenderer();
    }
    else
    {
        app::PaintPictureOnCanvas();
    }
}
