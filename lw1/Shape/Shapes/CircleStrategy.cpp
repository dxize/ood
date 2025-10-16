#include "CircleStrategy.h"
#include <stdexcept>

CircleStrategy::CircleStrategy(double centerX, double centerY, double radius)
    : m_centerX(centerX), m_centerY(centerY), m_radius(radius)
{
    if (radius < 0) {
        throw std::invalid_argument("Circle radius cannot be negative");
    }
}

void CircleStrategy::Draw(ICanvas& canvas, const std::string& color) const 
{
    canvas.SetColor(color);
    canvas.DrawEllipse(m_centerX, m_centerY, m_radius, m_radius);
}

void CircleStrategy::Move(double dx, double dy) 
{
    m_centerX += dx;
    m_centerY += dy;
}

std::vector<double> CircleStrategy::GetParameters() const 
{
    return { m_centerX, m_centerY, m_radius };
}
