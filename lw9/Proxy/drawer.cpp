#include "drawer.h"
#include <cmath>
#include <cassert>

int Sign(int v)
{
    return (v > 0) - (v < 0);
}

void DrawSteepLine(Image& img, Point a, Point b, char c)
{
    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);

    if (a.y > b.y) std::swap(a, b);

    int sx = Sign(b.x - a.x);
    int deltaErr = dx + 1;
    int errorThreshold = dy + 1;
    int err = deltaErr / 2;

    for (Point p = a; p.y <= b.y; ++p.y)
    {
        img.SetPixel(p, c);

        err += deltaErr;
        if (err >= errorThreshold)
        {
            p.x += sx;
            err -= errorThreshold;
        }
    }
}

void DrawSlopeLine(Image& img, Point a, Point b, char c)
{
    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);

    if (a.x > b.x)
    {
        std::swap(a, b);
    }

    int sy = Sign(b.y - a.y);
    int deltaErr = dy + 1;
    int errorThreshold = dx + 1;
    int err = deltaErr / 2;

    for (Point p = a; p.x <= b.x; ++p.x)
    {
        img.SetPixel(p, c);

        err += deltaErr;
        if (err >= errorThreshold)
        {
            p.y += sy;
            err -= errorThreshold;
        }
    }
}

void DrawLine(Image& img, Point a, Point b, char c)
{
    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);

    if (dy > dx)
        DrawSteepLine(img, a, b, c);
    else
        DrawSlopeLine(img, a, b, c);
}
