#include "Rectangle.h"

namespace shape_drawing_lib
{
    CRectangle::CRectangle(const Point& leftTop, int width, int height)
        : m_leftTop(leftTop), m_width(width), m_height(height)
    {
    }

    void CRectangle::Draw(graphics_lib::ICanvas& canvas) const
    {
        int x = m_leftTop.x;
        int y = m_leftTop.y;

        canvas.MoveTo(x, y);
        canvas.LineTo(x + m_width, y);
        canvas.LineTo(x + m_width, y + m_height);
        canvas.LineTo(x, y + m_height);
        canvas.LineTo(x, y);
    }
}
