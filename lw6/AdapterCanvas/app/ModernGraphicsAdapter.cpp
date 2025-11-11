#include "ModernGraphicsAdapter.h"

namespace app
{
    CModernGraphicsAdapter::CModernGraphicsAdapter(modern_graphics_lib::ModernGraphicsRenderer& renderer)
        : m_renderer(renderer), m_x(0), m_y(0)
    {
    }

    void CModernGraphicsAdapter::MoveTo(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    void CModernGraphicsAdapter::LineTo(int x, int y)
    {
        m_renderer.DrawLine(
            modern_graphics_lib::CPoint(m_x, m_y),
            modern_graphics_lib::CPoint(x, y)
        );
        m_x = x;
        m_y = y;
    }

    void CModernGraphicsAdapter::BeginDraw()
    {
        m_renderer.BeginDraw();
    }
    void CModernGraphicsAdapter::EndDraw()
    {
        m_renderer.EndDraw();
    }
}
