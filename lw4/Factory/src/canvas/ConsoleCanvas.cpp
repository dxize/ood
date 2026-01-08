#include "../../include/canvas/ConsoleCanvas.h"
#include "../../include/util/ColorUtil.h"

namespace canvas
{
    ConsoleCanvas::ConsoleCanvas(std::ostream& out)
        : m_out(out)
    {
    }

    void ConsoleCanvas::SetColor(domain::Color color)
    {
        m_color = color;
        m_out << "SetColor " << util::ToTextColor(color) << "\n";
    }

    void ConsoleCanvas::DrawLine(const domain::Point& from, const domain::Point& to)
    {
        m_out << "DrawLine " << from.x << " " << from.y << " " << to.x << " " << to.y << "\n";
    }

    void ConsoleCanvas::DrawEllipse(const domain::Point& leftTop, double width, double height)
    {
        m_out << "DrawEllipse " << leftTop.x << " " << leftTop.y << " " << width << " " << height << "\n";
    }
} 
