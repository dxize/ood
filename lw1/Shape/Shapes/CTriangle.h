#pragma once
#include "ISolidShape.h"
#include "CPoint.h"
#include <string>
#include <cstdint>

class CTriangle : public ISolidShape
{
public:
    CTriangle(const CPoint& vertex1, const CPoint& vertex2, const CPoint& vertex3,
        uint32_t outlineColor, uint32_t fillColor) 
        : m_vertex1(vertex1)
        , m_vertex2(vertex2)
        , m_vertex3(vertex3)
        , ISolidShape(outlineColor, fillColor) 
    {};

    double GetArea() const override;
    double GetPerimeter() const override;
    std::string ToString() const override;
    uint32_t GetOutlineColor() const override;
    uint32_t GetFillColor() const override;

    void Draw(ICanvas& canvas) const override;

    CPoint GetVertex1() const;
    CPoint GetVertex2() const;
    CPoint GetVertex3() const;

private:
    CPoint m_vertex1;
    CPoint m_vertex2;
    CPoint m_vertex3;
};
