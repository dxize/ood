#pragma once

#include "Color.h"

namespace canvas { class ICanvas; }

namespace domain
{
    class Shape
    {
    public:
        explicit Shape(Color color)
            : m_color(color)
        {
        }

        virtual ~Shape() = default;

        Color GetColor() const 
        {
            return m_color; 
        }

        virtual void Draw(canvas::ICanvas& canvas) const = 0;

    private:
        Color m_color;
    };
}
