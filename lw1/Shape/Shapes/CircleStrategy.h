#pragma once
#include "IShapeStrategy.h"

class CircleStrategy : public IShapeStrategy 
{
public:
    CircleStrategy(double centerX, double centerY, double radius);

    void Draw(ICanvas& canvas, const std::string& color) const override;
    void Move(double dx, double dy) override;
    std::string GetType() const override { return "circle"; }
    std::vector<double> GetParameters() const override;
private:
    double m_centerX, m_centerY, m_radius;
};