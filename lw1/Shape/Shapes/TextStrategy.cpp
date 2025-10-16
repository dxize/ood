#include "TextStrategy.h"
#include <vector>

TextStrategy::TextStrategy(double left, double top, double fontSize, const std::string& text)
    : m_left(left), m_top(top), m_fontSize(fontSize), m_text(text) {}

void TextStrategy::Draw(ICanvas& canvas, const std::string& color) const 
{
    canvas.SetColor(color);
    canvas.DrawText(m_left, m_top, m_fontSize, m_text);
}

void TextStrategy::Move(double dx, double dy) 
{
    m_left += dx;
    m_top += dy;
}

std::vector<double> TextStrategy::GetParameters() const 
{
    return { m_left, m_top, m_fontSize };
}