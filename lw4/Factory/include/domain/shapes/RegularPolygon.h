#pragma once

#include "../Point.h"
#include "../Shape.h"

namespace domain
{
class RegularPolygon final : public Shape
{
public:
    RegularPolygon(Color color, Point center, double radius, int vertexCount);

    void Draw(canvas::ICanvas& canvas) const override;

    Point GetCenter() const { return m_center; }
    double GetRadius() const { return m_radius; }
    int GetVertexCount() const { return m_vertexCount; }

private:
    Point m_center;
    double m_radius = 0.0;
    int m_vertexCount = 0;
};
} // namespace domain
