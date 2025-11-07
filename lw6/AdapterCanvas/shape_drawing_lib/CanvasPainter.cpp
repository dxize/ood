#include "CanvasPainter.h"

namespace shape_drawing_lib
{
    CCanvasPainter::CCanvasPainter(graphics_lib::ICanvas& canvas)
        : m_canvas(canvas)
    {
    }

    void CCanvasPainter::Draw(const ICanvasDrawable& drawable)
    {
        drawable.Draw(m_canvas);
    }
}
