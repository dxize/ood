#include "../../../include/domain/shapes/Triangle.h"
#include "../../../include/canvas/ICanvas.h"

namespace domain
{
Triangle::Triangle(Color color, Point v1, Point v2, Point v3)
    : Shape(color)
    , m_v1(v1)
    , m_v2(v2)
    , m_v3(v3)
{
}

void Triangle::Draw(canvas::ICanvas& canvas) const
{
    canvas.SetColor(GetColor());
    canvas.DrawLine(m_v1, m_v2);
    canvas.DrawLine(m_v2, m_v3);
    canvas.DrawLine(m_v3, m_v1);
}
} // namespace domain
