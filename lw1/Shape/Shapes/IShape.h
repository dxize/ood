#pragma once
#include <string>
#include <cstdint>
#include "DrawShapes/ICanvas.h"

class IShape
{
public:
    explicit IShape(uint32_t color)
        : m_color(color)
    {}

    virtual ~IShape() = default;

    virtual void Move(double dx, double dy) = 0;
    virtual void Draw(ICanvas& canvas) = 0;

protected:
    uint32_t m_color;
};
