#pragma once

#include <ostream>

#include "ICanvas.h"
#include "../domain/Color.h"

namespace canvas
{
    class ConsoleCanvas final : public ICanvas
    {
    public:
        explicit ConsoleCanvas(std::ostream& out);

        void SetColor(domain::Color color) override;
        void DrawLine(const domain::Point& from, const domain::Point& to) override;
        void DrawEllipse(const domain::Point& leftTop, double width, double height) override;

    private:
        std::ostream& m_out;
        domain::Color m_color = domain::Color::Black;
    };
} 
