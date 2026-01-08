#pragma once

#include "../Point.h"
#include "../Shape.h"

namespace domain
{
class Rectangle final : public Shape
{
public:
    Rectangle(Color color, Point leftTop, Point rightBottom);

    void Draw(canvas::ICanvas& canvas) const override;

    Point GetLeftTop() const { return m_leftTop; }
    Point GetRightBottom() const { return m_rightBottom; }

private:
    Point m_leftTop;
    Point m_rightBottom;
};
} // namespace domain
