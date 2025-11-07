#pragma once
#include "ICanvasDrawable.h"

namespace shape_drawing_lib
{
    class CCanvasPainter
    {
    public:
        CCanvasPainter(graphics_lib::ICanvas& canvas);
        void Draw(const ICanvasDrawable& drawable);

    private:
        graphics_lib::ICanvas& m_canvas;
    };
}
