#include "../../../include/domain/shapes/RegularPolygon.h"
#include "../../../include/canvas/ICanvas.h"

#include <cmath>
#include <stdexcept>

namespace domain
{
    static constexpr double kPi = 3.14159265358979323846;

    RegularPolygon::RegularPolygon(Color color, Point center, double radius, int vertexCount)
        : Shape(color)
        , m_center(center)
        , m_radius(radius)
        , m_vertexCount(vertexCount)
    {
        if (m_radius <= 0)
        {
            throw std::invalid_argument("Polygon radius must be positive");
        }
        if (m_vertexCount < 3)
        {
            throw std::invalid_argument("Polygon vertexCount must be >= 3");
        }
    }

    void RegularPolygon::Draw(canvas::ICanvas& canvas) const
    {
        canvas.SetColor(GetColor());

        const double step = (2.0 * kPi) / static_cast<double>(m_vertexCount);
        const double startAngle = -kPi / 2.0; // чтобы первая вершина была сверху

        auto vertexAt = [&](int i) -> Point { //Она считает угол для вершины i
            const double a = startAngle + step * static_cast<double>(i);
            return Point{
                m_center.x + m_radius * std::cos(a),
                m_center.y + m_radius * std::sin(a),
            };
        };

        for (int i = 0; i < m_vertexCount; ++i)
        {
            const Point p1 = vertexAt(i);
            const Point p2 = vertexAt((i + 1) % m_vertexCount);
            canvas.DrawLine(p1, p2);
        }
    }
} 
