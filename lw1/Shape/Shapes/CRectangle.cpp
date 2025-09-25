#include "CRectangle.h"
#include <sstream>
#include <iomanip>
#include <cmath>

uint32_t CRectangle::GetOutlineColor() const
{
    return m_outlineColor;
}

uint32_t CRectangle::GetFillColor() const
{
    return m_fillColor;
}

void CRectangle::Draw(ICanvas& canvas) const
{
    std::vector<CPoint> points = {
        m_leftTop,
        CPoint{m_leftTop.x + m_width, m_leftTop.y},
        CPoint{m_leftTop.x + m_width, m_leftTop.y + m_height},
        CPoint{m_leftTop.x, m_leftTop.y + m_height}
    };

    canvas.SetColor(m_fillColor);

    for (size_t i = 0; i < points.size(); ++i)
    {
        size_t nextIndex = (i + 1) % points.size();
        canvas.LineTo(points[i], points[nextIndex], m_outlineColor);
    }
}