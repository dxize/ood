#include "PaintPicture.h"
#include "ModernGraphicsAdapter.h"
#include "../graphics_lib/Canvas.cpp"
#include "../modern_graphics_lib/ModernGraphicsRenderer.h"
#include "../shape_drawing_lib/Triangle.h"
#include "../shape_drawing_lib/Rectangle.h"

namespace app
{
    void PaintPicture(shape_drawing_lib::CCanvasPainter& painter)
    {
        using namespace shape_drawing_lib;
        CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 });
        CRectangle rectangle({ 30, 40 }, 18, 24);

        painter.Draw(rectangle);
        painter.Draw(triangle);
    }

    void PaintPictureOnCanvas()
    {
        graphics_lib::CCanvas canvas;
        shape_drawing_lib::CCanvasPainter painter(canvas);
        PaintPicture(painter);
    }

    void PaintPictureOnModernGraphicsRenderer()
    {
        modern_graphics_lib::ModernGraphicsRenderer renderer(std::cout);
        CModernGraphicsAdapter adapter(renderer);
        shape_drawing_lib::CCanvasPainter painter(adapter);

        PaintPicture(painter);
    }
}
