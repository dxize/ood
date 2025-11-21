#pragma once
#include "Types.h"
#include <vector>

class ICanvas 
{
public:
    virtual ~ICanvas() = default;
    virtual void SetLineColor(RGBAColor color) = 0;
    virtual void SetLineWidth(double width) = 0;
    virtual void DrawLine(const Point& from, const Point& to) = 0;
    virtual void SetFillColor(RGBAColor color) = 0;
    virtual void FillPolygon(const std::vector<Point>& points) = 0;
    virtual void DrawEllipse(const Point& center, double rx, double ry) = 0;
    virtual void FillEllipse(const Point& center, double rx, double ry) = 0;
};