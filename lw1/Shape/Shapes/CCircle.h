#pragma once
#include "ISolidShape.h"
#include "CPoint.h"
#include <string>
#include <cstdint>

class CCircle : public ISolidShape
{
public:
    CCircle(const CPoint& center, double radius,
        uint32_t outlineColor, uint32_t fillColor)
        : m_center(center)
        , m_radius(radius)
        , ISolidShape(outlineColor, fillColor)
    {}

    double GetArea() const override;
    double GetPerimeter() const override;
    std::string ToString() const override;
    uint32_t GetOutlineColor() const override;
    uint32_t GetFillColor() const override;

    void Draw(ICanvas& canvas) const override;

    CPoint GetCenter() const;
    double GetRadius() const;

private:
    CPoint   m_center;
    double   m_radius;
};
