#include "CTriangle.h"
#include <cmath>
#include <sstream>
#include <iomanip>

static double Distance(const CPoint& a, const CPoint& b)
{
    double x = b.x - a.x;
    double y = b.y - a.y;
    return std::hypot(x, y); //sqrt(x*x + y*y)
}

double CTriangle::GetArea() const
{
    double a = Distance(m_vertex1, m_vertex2);
    double b = Distance(m_vertex2, m_vertex3);
    double c = Distance(m_vertex3, m_vertex1);
    double p = (a + b + c) / 2.0;
    return std::sqrt(p * (p - a) * (p - b) * (p - c));
}

double CTriangle::GetPerimeter() const
{
    return Distance(m_vertex1, m_vertex2)
        + Distance(m_vertex2, m_vertex3)
        + Distance(m_vertex3, m_vertex1);
}

uint32_t CTriangle::GetOutlineColor() const
{
    return m_outlineColor;
}

uint32_t CTriangle::GetFillColor() const
{
    return m_fillColor;
}

CPoint CTriangle::GetVertex1() const
{
    return m_vertex1;
}

CPoint CTriangle::GetVertex2() const
{
    return m_vertex2;
}

CPoint CTriangle::GetVertex3() const
{
    return m_vertex3;
}

std::string CTriangle::ToString() const
{
    std::ostringstream ss;
    ss << "Triangle:\n"
        << "  Vertex1: (" << m_vertex1.x << ", " << m_vertex1.y << ")\n"
        << "  Vertex2: (" << m_vertex2.x << ", " << m_vertex2.y << ")\n"
        << "  Vertex3: (" << m_vertex3.x << ", " << m_vertex3.y << ")\n"
        << "  Perimeter: " << GetPerimeter() << "\n"
        << "  Area: " << GetArea() << "\n"
        << "  Outline color: #" << std::hex << std::setw(6) << std::setfill('0') << m_outlineColor << "\n"
        << "  Fill color: #" << std::hex << std::setw(6) << std::setfill('0') << m_fillColor;
    return ss.str();
}

void CTriangle::Draw(ICanvas& canvas) const 
{
    std::vector<CPoint> points = { m_vertex1, m_vertex2, m_vertex3 };

    for (size_t i = 0; i < points.size(); ++i) 
    {
        size_t nextIndex = (i + 1) % points.size();
        canvas.DrawLine(points[i], points[nextIndex], m_outlineColor);
    }

    canvas.FillPolygon(points, m_fillColor);
}