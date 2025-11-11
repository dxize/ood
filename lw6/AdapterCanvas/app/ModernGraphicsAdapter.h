#pragma once
#include "../graphics_lib/ICanvas.h"
#include "../modern_graphics_lib/ModernGraphicsRenderer.h"

namespace app
{
    class CModernGraphicsAdapter : public graphics_lib::ICanvas
    {
    public:
        CModernGraphicsAdapter(modern_graphics_lib::ModernGraphicsRenderer& renderer);

        void MoveTo(int x, int y) override;
        void LineTo(int x, int y) override;

        void BeginDraw();
        void EndDraw();

    private:
        modern_graphics_lib::ModernGraphicsRenderer& m_renderer;
        int m_x;
        int m_y;
    };
}
