#include "../../include/canvas/MultiCanvas.h"

namespace canvas
{
    MultiCanvas::MultiCanvas(std::initializer_list<std::reference_wrapper<ICanvas>> canvases)
        : m_canvases(canvases)
    {
    }

    void MultiCanvas::SetColor(domain::Color color)
    {
        for (ICanvas& c : m_canvases)
        {
            c.SetColor(color);
        }
    }

    void MultiCanvas::DrawLine(const domain::Point& from, const domain::Point& to)
    {
        for (ICanvas& c : m_canvases)
        {
            c.DrawLine(from, to);
        }
    }

    void MultiCanvas::DrawEllipse(const domain::Point& leftTop, double width, double height)
    {
        for (ICanvas& c : m_canvases)
        {
            c.DrawEllipse(leftTop, width, height);
        }
    }
} 
