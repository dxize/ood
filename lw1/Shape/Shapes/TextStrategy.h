#pragma once
#include "IShapeStrategy.h"
#include <string>

class TextStrategy : public IShapeStrategy 
{

public:
    TextStrategy(double left, double top, double fontSize, const std::string& text);

    void Draw(ICanvas& canvas, const std::string& color) const override;
    void Move(double dx, double dy) override;
    std::string GetType() const override { return "text"; }
    std::vector<double> GetParameters() const override;

    void SetText(const std::string& text) { m_text = text; }
    std::string GetExtraInfo() const override { return m_text; }

private:
    double m_left, m_top, m_fontSize;
    std::string m_text;

};