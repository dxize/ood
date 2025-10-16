#include "RectangleStrategy.h"
#include <vector>

RectangleStrategy::RectangleStrategy(double left, double top, double width, double height)
    : m_left(left), m_top(top), m_width(width), m_height(height) {}

void RectangleStrategy::Draw(ICanvas& canvas, const std::string& color) const 
{
    canvas.SetColor(color);

    canvas.MoveTo(m_left, m_top);
    canvas.LineTo(m_left + m_width, m_top);
    canvas.LineTo(m_left + m_width, m_top + m_height);
    canvas.LineTo(m_left, m_top + m_height);
    canvas.LineTo(m_left, m_top);
}

void RectangleStrategy::Move(double dx, double dy) 
{
    m_left += dx;
    m_top += dy;
}

std::vector<double> RectangleStrategy::GetParameters() const 
{
    return { m_left, m_top, m_width, m_height };
}