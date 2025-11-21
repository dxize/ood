#pragma once
#include "Shape.h"

class RectangleShape : public Shape
{
public:
    RectangleShape(const Rect& r)
        : Shape(r)
    {
    }

protected:
    void DoFill(ICanvas& canvas) const override
    {
        std::vector<Point> points = {
            {frame.x, frame.y},
            {frame.x + frame.width, frame.y},
            {frame.x + frame.width, frame.y + frame.height},
            {frame.x, frame.y + frame.height}
        };
        canvas.FillPolygon(points);
    }

    void DoDrawOutline(ICanvas& canvas) const override
    {
        Point tl{ frame.x, frame.y };
        Point tr{ frame.x + frame.width, frame.y };
        Point br{ frame.x + frame.width, frame.y + frame.height };
        Point bl{ frame.x, frame.y + frame.height };

        canvas.DrawLine(tl, tr);
        canvas.DrawLine(tr, br);
        canvas.DrawLine(br, bl);
        canvas.DrawLine(bl, tl);
    }
};

class EllipseShape : public Shape
{
public:
    EllipseShape(const Rect& r)
        : Shape(r)
    {
    }

protected:
    void DoFill(ICanvas& canvas) const override
    {
        Point c{ frame.x + frame.width / 2, frame.y + frame.height / 2 };
        double rx = frame.width / 2;
        double ry = frame.height / 2;

        canvas.FillEllipse(c, rx, ry);
    }

    void DoDrawOutline(ICanvas& canvas) const override
    {
        Point c{ frame.x + frame.width / 2, frame.y + frame.height / 2 };
        double rx = frame.width / 2;
        double ry = frame.height / 2;

        canvas.DrawEllipse(c, rx, ry);
    }
};

class TriangleShape : public Shape
{
public:
    TriangleShape(const Rect& r)
        : Shape(r)
    {
    }

protected:
    std::vector<Point> GetPoints() const
    {
        return {
            {frame.x + frame.width / 2, frame.y},
            {frame.x + frame.width, frame.y + frame.height},
            {frame.x, frame.y + frame.height}
        };
    }

    void DoFill(ICanvas& canvas) const override
    {
        canvas.FillPolygon(GetPoints());
    }

    void DoDrawOutline(ICanvas& canvas) const override
    {
        auto pts = GetPoints();
        canvas.DrawLine(pts[0], pts[1]);
        canvas.DrawLine(pts[1], pts[2]);
        canvas.DrawLine(pts[2], pts[0]);
    }
};