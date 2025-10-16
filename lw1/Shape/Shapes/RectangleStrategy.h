#pragma once
#include "IShapeStrategy.h"

class RectangleStrategy : public IShapeStrategy 
{
public:
    RectangleStrategy(double left, double top, double width, double height);

    void Draw(ICanvas& canvas, const std::string& color) const override;
    void Move(double dx, double dy) override;
    std::string GetType() const override { return "rectangle"; }
    std::vector<double> GetParameters() const override;
private:
    double m_left, m_top, m_width, m_height;
};