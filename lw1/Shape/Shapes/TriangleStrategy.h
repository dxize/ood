#pragma once
#include "IShapeStrategy.h"

class TriangleStrategy : public IShapeStrategy 
{
public:
    TriangleStrategy(double x1, double y1, double x2, double y2, double x3, double y3);

    void Draw(ICanvas& canvas, const std::string& color) const override;
    void Move(double dx, double dy) override;
    std::string GetType() const override { return "triangle"; }
    std::vector<double> GetParameters() const override;
private:
    double m_x1, m_y1, m_x2, m_y2, m_x3, m_y3;
};