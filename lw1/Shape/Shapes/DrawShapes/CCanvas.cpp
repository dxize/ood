#define _USE_MATH_DEFINES
#include "CCanvas.h"
#include <GLFW/glfw3.h>
#include <cmath>

static inline void ApplyColor(uint32_t color)
{
    // распаковываем 0xRRGGBB → три GLubyte
    GLubyte r = static_cast<GLubyte>((color >> 16) & 0xFF);
    GLubyte g = static_cast<GLubyte>((color >> 8) & 0xFF);
    GLubyte b = static_cast<GLubyte>((color >> 0) & 0xFF);
    glColor3ub(r, g, b);
}

void CCanvas::DrawLine(CPoint from, CPoint to, uint32_t lineColor) const
{
    ApplyColor(lineColor);
    glBegin(GL_LINES);
    glVertex2f(static_cast<GLfloat>(from.x),
        static_cast<GLfloat>(from.y));
    glVertex2f(static_cast<GLfloat>(to.x),
        static_cast<GLfloat>(to.y));
    glEnd();
}

void CCanvas::FillPolygon(std::vector<CPoint> points, uint32_t fillColor) const
{
    ApplyColor(fillColor);
    glBegin(GL_POLYGON);
    for (const auto& pt : points)
    {
        glVertex2f(static_cast<GLfloat>(pt.x),
            static_cast<GLfloat>(pt.y));
    }
    glEnd();
}

void CCanvas::DrawCircle(CPoint center, double radius, uint32_t lineColor) const
{
    ApplyColor(lineColor);
    constexpr int segments = 360;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i)
    {
        double theta = 2.0 * M_PI * i / segments;
        double x = center.x + radius * std::cos(theta);
        double y = center.y + radius * std::sin(theta);
        glVertex2f(static_cast<GLfloat>(x),
            static_cast<GLfloat>(y));
    }
    glEnd();
}

void CCanvas::FillCircle(CPoint center, double radius, uint32_t fillColor) const
{
    ApplyColor(fillColor);
    constexpr int segments = 360;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(static_cast<GLfloat>(center.x),
        static_cast<GLfloat>(center.y));
    for (int i = 0; i <= segments; ++i)
    {
        double theta = 2.0 * M_PI * i / segments;
        double x = center.x + radius * std::cos(theta);
        double y = center.y + radius * std::sin(theta);
        glVertex2f(static_cast<GLfloat>(x),
            static_cast<GLfloat>(y));
    }
    glEnd();
}
