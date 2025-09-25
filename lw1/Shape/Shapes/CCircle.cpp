#define _USE_MATH_DEFINES
#include "CCircle.h"
#include <cmath>
#include <sstream>
#include <iomanip>


double Distance(const CPoint& a, const CPoint& b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return std::hypot(dx, dy);
}

double CCircle::GetArea() const
{
    return M_PI * m_radius * m_radius;
}

double CCircle::GetPerimeter() const
{
    return 2.0 * M_PI * m_radius;
}

uint32_t CCircle::GetOutlineColor() const
{
    return m_outlineColor;
}

uint32_t CCircle::GetFillColor() const
{
    return m_fillColor;
}

CPoint CCircle::GetCenter() const
{
    return m_center;
}

double CCircle::GetRadius() const
{
    return m_radius;
}

std::string CCircle::ToString() const
{
    std::ostringstream ss;
    ss << "Circle:\n"
        << "  Center: (" << m_center.x << ", " << m_center.y << ")\n"
        << "  Radius: " << m_radius << "\n"
        << "  Perimeter: " << GetPerimeter() << "\n"
        << "  Area: " << GetArea() << "\n"
        << "  Outline color: #" << std::hex << std::setw(6) << std::setfill('0') << m_outlineColor << "\n"
        << "  Fill color: #" << std::hex << std::setw(6) << std::setfill('0') << m_fillColor;
    return ss.str();
}

void CCircle::Draw(ICanvas& canvas) const
{
    canvas.DrawCircle(m_center, m_radius, m_outlineColor);
    canvas.FillCircle(m_center, m_radius, m_fillColor);
}
