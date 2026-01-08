#pragma once

#include "../Point.h"
#include "../Shape.h"

namespace domain
{
class Triangle final : public Shape
{
public:
    Triangle(Color color, Point v1, Point v2, Point v3);

    void Draw(canvas::ICanvas& canvas) const override;

    Point GetVertex1() const { return m_v1; }
    Point GetVertex2() const { return m_v2; }
    Point GetVertex3() const { return m_v3; }

private:
    Point m_v1, m_v2, m_v3;
};
} // namespace domain
