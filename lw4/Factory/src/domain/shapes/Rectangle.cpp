#include "../../../include/domain/shapes/Rectangle.h"
#include "../../../include/canvas/ICanvas.h"

namespace domain
{
Rectangle::Rectangle(Color color, Point leftTop, Point rightBottom)
    : Shape(color)
    , m_leftTop(leftTop)
    , m_rightBottom(rightBottom)
{
}

void Rectangle::Draw(canvas::ICanvas& canvas) const
{
    canvas.SetColor(GetColor());

    const Point leftBottom{m_leftTop.x, m_rightBottom.y};
    const Point rightTop{m_rightBottom.x, m_leftTop.y};

    canvas.DrawLine(m_leftTop, rightTop);
    canvas.DrawLine(rightTop, m_rightBottom);
    canvas.DrawLine(m_rightBottom, leftBottom);
    canvas.DrawLine(leftBottom, m_leftTop);
}
} // namespace domain
