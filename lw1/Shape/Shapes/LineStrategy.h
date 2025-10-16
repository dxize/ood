#pragma once
#include "IShapeStrategy.h"

class LineStrategy : public IShapeStrategy 
{
public:
    LineStrategy(double x1, double y1, double x2, double y2);

    void Draw(ICanvas& canvas, const std::string& color) const override;
    void Move(double dx, double dy) override;
    std::string GetType() const override { return "line"; }
    std::vector<double> GetParameters() const override;
   /* std::unique_ptr<IShapeStrategy> Clone() const override;*/
private:
    double m_x1, m_y1, m_x2, m_y2;
};