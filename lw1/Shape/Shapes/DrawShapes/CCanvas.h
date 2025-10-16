#pragma once
#include "ICanvas.h"
#include <fstream>
#include <string>
#include <stdexcept>

class CCanvas : public ICanvas 
{
public:
    CCanvas(const std::string& filename, int width = 800, int height = 600);
    ~CCanvas();

    void SetColor(const std::string& color) override;
    void MoveTo(double x, double y) override;
    void LineTo(double x, double y) override;
    void DrawEllipse(double cx, double cy, double rx, double ry) override;
    void DrawText(double left, double top, double fontSize, const std::string& text) override;
    void Finish();

private:
    std::ofstream m_svgFile;
    std::string m_currentColor = "#000000";
    double m_currentX = 0;
    double m_currentY = 0;
    bool m_isOpen = false;

    std::string EscapeText(const std::string& text);
};
