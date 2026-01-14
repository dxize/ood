#include "../../include/canvas/SvgCanvas.h"
#include "../../include/util/ColorUtil.h"

namespace canvas
{
    SvgCanvas::SvgCanvas(std::ostream& out, double width, double height)
        : m_out(out)
        , m_width(width)
        , m_height(height)
    {
        m_out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        m_out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
              << "width=\"" << m_width << "\" height=\"" << m_height << "\" "
              << "viewBox=\"0 0 " << m_width << " " << m_height << "\">\n";
        // небольшой фон, чтобы в браузере было видно границы
        m_out << "<rect x=\"0\" y=\"0\" width=\"" << m_width << "\" height=\"" << m_height
              << "\" fill=\"white\" stroke=\"lightgray\" stroke-width=\"1\" />\n";
    }

    SvgCanvas::~SvgCanvas()
    {
        CloseSvg();
    }

    void SvgCanvas::CloseSvg()
    {
        if (!m_closed)
        {
            m_out << "</svg>\n";
            m_closed = true;
        }
    }

    void SvgCanvas::SetColor(domain::Color color)
    {
        m_color = color;
    }

    void SvgCanvas::DrawLine(const domain::Point& from, const domain::Point& to)
    {
        m_out << "<line x1=\"" << from.x << "\" y1=\"" << from.y
              << "\" x2=\"" << to.x << "\" y2=\"" << to.y
              << "\" stroke=\"" << util::ToSvgColor(m_color)
              << "\" stroke-width=\"2\" />\n";
    }

    void SvgCanvas::DrawEllipse(const domain::Point& leftTop, double width, double height)
    {
        const double cx = leftTop.x + width / 2.0;
        const double cy = leftTop.y + height / 2.0;
        const double rx = width / 2.0;
        const double ry = height / 2.0;

        m_out << "<ellipse cx=\"" << cx << "\" cy=\"" << cy
              << "\" rx=\"" << rx << "\" ry=\"" << ry
              << "\" stroke=\"" << util::ToSvgColor(m_color)
              << "\" stroke-width=\"2\" fill=\"none\" />\n";
    }
} 
