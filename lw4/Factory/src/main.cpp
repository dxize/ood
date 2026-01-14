#include "../include/app/Designer.h"
#include "../include/app/Painter.h"
#include "../include/canvas/ConsoleCanvas.h"
#include "../include/canvas/MultiCanvas.h"
#include "../include/canvas/SvgCanvas.h"
#include "../include/factory/ShapeFactory.h"

#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>

int main()
{

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    try
    {
        std::ofstream svg("output.svg", std::ios::binary);
        if (!svg)
        {
            std::cerr << "Cannot open SVG output file: " << "output.svg" << "\n";
            return 2;
        }

        canvas::ConsoleCanvas consoleCanvas(std::cout);
        canvas::SvgCanvas svgCanvas(svg, 800, 600);
        canvas::MultiCanvas multi({consoleCanvas, svgCanvas});

        factory::ShapeFactory shapeFactory;
        app::Designer designer(shapeFactory);
        app::Painter painter;

        domain::PictureDraft draft = designer.CreateDraft(std::cin);
        painter.DrawPicture(draft, multi);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}//повторить разницу фабрик
