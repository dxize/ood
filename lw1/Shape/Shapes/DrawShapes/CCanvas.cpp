#include "CCanvas.h"
#include <iomanip>

CCanvas::CCanvas(const std::string& filename, int width, int height) 
{
    m_svgFile.open(filename);
    if (!m_svgFile.is_open()) {
        throw std::runtime_error("Cannot open SVG file: " + filename);
    }

    m_svgFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    m_svgFile << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" "
        << "width=\"" << width << "\" height=\"" << height << "\">\n";
    m_isOpen = true;
}

CCanvas::~CCanvas() 
{
    Finish();
}

void CCanvas::Finish() 
{
    if (m_isOpen) {
        m_svgFile << "</svg>\n";
        m_svgFile.flush();
        m_svgFile.close();
        m_isOpen = false;
    }
}

void CCanvas::SetColor(const std::string& color) 
{
    m_currentColor = color;
}

void CCanvas::MoveTo(double x, double y) 
{
    m_currentX = x;
    m_currentY = y;
}

void CCanvas::LineTo(double x, double y) 
{
    m_svgFile << "<line x1=\"" << m_currentX << "\" y1=\"" << m_currentY
        << "\" x2=\"" << x << "\" y2=\"" << y
        << "\" stroke=\"" << m_currentColor
        << "\" stroke-width=\"2\"/>\n";
    m_currentX = x;
    m_currentY = y;
}

void CCanvas::DrawEllipse(double cx, double cy, double rx, double ry) 
{
    m_svgFile << "<ellipse cx=\"" << cx << "\" cy=\"" << cy
        << "\" rx=\"" << rx << "\" ry=\"" << ry
        << "\" fill=\"" << m_currentColor << "\"/>\n";
}

std::string CCanvas::EscapeText(const std::string& text) 
{
    std::string escaped;
    for (char c : text) {
        switch (c) {
        case '&': escaped += "&amp;"; break;
        case '<': escaped += "&lt;"; break;
        case '>': escaped += "&gt;"; break;
        default: escaped += c; break;
        }
    }
    return escaped;
}

void CCanvas::DrawText(double left, double top, double fontSize, const std::string& text) 
{
    m_svgFile << "<text x=\"" << left << "\" y=\"" << top
        << "\" font-size=\"" << fontSize
        << "\" fill=\"" << m_currentColor << "\">"
        << EscapeText(text) << "</text>\n";
}
