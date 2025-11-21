#pragma once
#include <cstdint>
#include <algorithm>

struct RGBAColor 
{
    uint8_t r, g, b, a;
    bool operator==(const RGBAColor& other) const 
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    bool operator!=(const RGBAColor& other) const 
    {
        return !(*this == other);
    }
};

struct Point 
{
    double x, y;
};

struct Rect 
{
    Rect() : x(0), y(0), width(0), height(0) 
    {
    }

    Rect(double x_, double y_, double w_, double h_) 
        : x(x_), y(y_), width(w_), height(h_) 
    {
    }

    bool IsEmpty() const 
    { 
        return width <= 0 || height <= 0; 
    }

    double Right() const 
    { 
        return x + width; 
    }

    double Bottom() const 
    {
        return y + height; 
    }

    double x, y, width, height;
};

inline Rect Union(const Rect& a, const Rect& b) //захват всех фигур (фрейм)
{
    if (a.IsEmpty())
    {
        return b;
    }
    if (b.IsEmpty())
    {
        return a;
    }
    double minX = std::min(a.x, b.x);
    double minY = std::min(a.y, b.y);
    double maxX = std::max(a.Right(), b.Right());
    double maxY = std::max(a.Bottom(), b.Bottom());
    return Rect(minX, minY, maxX - minX, maxY - minY);
}