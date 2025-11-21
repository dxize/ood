#pragma once
#include "Icanvas.h"
#include <iostream>
#include <cstddef>  // for size_t

class StdoutCanvas : public ICanvas 
{
public:
    void SetLineColor(RGBAColor color) override 
    {
        std::cout << "SetLineColor(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ", " << (int)color.a << ")" << std::endl;
    }

    void SetLineWidth(double width) override 
    {
        std::cout << "SetLineWidth(" << width << ")" << std::endl;
    }

    void DrawLine(const Point& from, const Point& to) override 
    {
        std::cout << "DrawLine((" << from.x << ", " << from.y << "), (" << to.x << ", " << to.y << "))" << std::endl;
    }

    void SetFillColor(RGBAColor color) override 
    {
        std::cout << "SetFillColor(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ", " << (int)color.a << ")" << std::endl;
    }

    void FillPolygon(const std::vector<Point>& points) override 
    {
        std::cout << "FillPolygon([";
        for (size_t i = 0; i < points.size(); ++i) 
        {
            std::cout << "(" << points[i].x << ", " << points[i].y << ")";
            if (i < points.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "])" << std::endl;
    }

    void DrawEllipse(const Point& center, double rx, double ry) override 
    {
        std::cout << "DrawEllipse((" << center.x << ", " << center.y << "), " << rx << ", " << ry << ")" << std::endl;
    }

    void FillEllipse(const Point& center, double rx, double ry) override 
    {
        std::cout << "FillEllipse((" << center.x << ", " << center.y << "), " << rx << ", " << ry << ")" << std::endl;
    }
};