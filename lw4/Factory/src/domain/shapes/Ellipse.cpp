#include "../../../include/domain/shapes/Ellipse.h"
#include "../../../include/canvas/ICanvas.h"

#include <stdexcept>

namespace domain
{
    Ellipse::Ellipse(Color color, Point center, double rx, double ry)
        : Shape(color)
        , m_center(center)
        , m_rx(rx)
        , m_ry(ry)
    {
        if (m_rx <= 0 || m_ry <= 0)
        {
            throw std::invalid_argument("Ellipse radii must be positive");
        }
    }

    void Ellipse::Draw(canvas::ICanvas& canvas) const
    {
        canvas.SetColor(GetColor());

        const Point leftTop{m_center.x - m_rx, m_center.y - m_ry};
        const double width = 2.0 * m_rx;
        const double height = 2.0 * m_ry;

        canvas.DrawEllipse(leftTop, width, height);
    }
} 
