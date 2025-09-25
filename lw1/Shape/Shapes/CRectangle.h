#pragma once
#include "ISolidShape.h"
#include "CPoint.h"
#include <string>
#include <cstdint>

class CRectangle : public ISolidShape
{
public:
    CRectangle(const CPoint& topLeft, double width, double height)
        : m_leftTop(topLeft)
        , m_width(width)
        , m_height(height)
        , ISolidShape(outlineColor, fillColor)
    {}

    uint32_t GetOutlineColor() const override;
    uint32_t GetFillColor() const override;

    void Draw(ICanvas& canvas) const override;

private:
    CPoint m_leftTop;
    double m_width, m_height;
};
