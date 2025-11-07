#include "ModernGraphicsAdapter.h"

namespace app
{
    CModernGraphicsAdapter::CModernGraphicsAdapter(modern_graphics_lib::ModernGraphicsRenderer& renderer)
        : m_renderer(renderer), m_x(0), m_y(0), m_hasCurrentPos(false)
    {
        m_renderer.BeginDraw();
    }

    CModernGraphicsAdapter::~CModernGraphicsAdapter()
    {
        try 
        { 
            m_renderer.EndDraw(); 
        }
        catch (const std::exception& e)
        { 
            /* suppress, destructor must not throw */ 
            std::cerr << "EndDraw() failed: " << e.what() << std::endl;
        }
    }

    void CModernGraphicsAdapter::MoveTo(int x, int y)
    {
        m_x = x;
        m_y = y;
        m_hasCurrentPos = true;
    }

    void CModernGraphicsAdapter::LineTo(int x, int y)
    {
        if (!m_hasCurrentPos)
        {
            m_hasCurrentPos = true;
        }

        m_renderer.DrawLine(
            modern_graphics_lib::CPoint(m_x, m_y),
            modern_graphics_lib::CPoint(x, y)
        );
        m_x = x;
        m_y = y;
    }
}
