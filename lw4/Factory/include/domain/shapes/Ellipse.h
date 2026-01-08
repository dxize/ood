#pragma once

#include "../Point.h"
#include "../Shape.h"

namespace domain
{
class Ellipse final : public Shape
{
    public:
        // cx,cy - центр, rx,ry - радиусы
        Ellipse(Color color, Point center, double rx, double ry);

        void Draw(canvas::ICanvas& canvas) const override;

        Point GetCenter() const 
        { 
            return m_center; 
        }
        double GetHorizontalRadius() const 
        { 
            return m_rx;
        }
        double GetVerticalRadius() const 
        { 
            return m_ry;
        }

    private:
        Point m_center;
        double m_rx = 0.0;
        double m_ry = 0.0;
    };
} 
