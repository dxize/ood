#pragma once
#include <iostream>
#include <stdexcept>

namespace modern_graphics_lib
{

    struct CPoint
    {
        CPoint(int x, int y)
            : x(x)
            , y(y)
        {}

        int x;
        int y;
    };

    class ModernGraphicsRenderer
    {
    public:
        ModernGraphicsRenderer(std::ostream& strm)
            : m_out(strm)
            , m_drawing(false)
        {}

        ~ModernGraphicsRenderer()
        {
            if (m_drawing)
            {
                EndDraw();
            }
        }

        void BeginDraw()
        {
            if (m_drawing)
            {
                throw std::logic_error("Drawing has already begun");
            }
            m_out << "<draw>" << std::endl;
            m_drawing = true;
        }

        void DrawLine(const CPoint& from, const CPoint& to)
        {
            if (!m_drawing)
            {
                throw std::logic_error("DrawLine is allowed only between BeginDraw()/EndDraw()");
            }

            m_out << "  <line fromX=\"" << from.x << "\" fromY=\"" << from.y
                << "\" toX=\"" << to.x << "\" toY=\"" << to.y << "\" />" << std::endl;
        }

        void EndDraw()
        {
            if (!m_drawing)
            {
                throw std::logic_error("Drawing has not been started");
            }
            m_out << "</draw>" << std::endl;
            m_drawing = false;
        }

    private:
        std::ostream& m_out;
        bool m_drawing;
    };

}
