#include "LineStrategy.h"
#include <vector>

LineStrategy::LineStrategy(double x1, double y1, double x2, double y2)
    : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2) {}

void LineStrategy::Draw(ICanvas& canvas, const std::string& color) const 
{
    canvas.SetColor(color);
    canvas.MoveTo(m_x1, m_y1);
    canvas.LineTo(m_x2, m_y2);
}

void LineStrategy::Move(double dx, double dy) 
{
    m_x1 += dx;
    m_y1 += dy;
    m_x2 += dx;
    m_y2 += dy;
}

std::vector<double> LineStrategy::GetParameters() const 
{
    return { m_x1, m_y1, m_x2, m_y2 };
}