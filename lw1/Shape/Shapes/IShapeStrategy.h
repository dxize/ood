#pragma once
#include "DrawShapes/ICanvas.h"
#include <string>
#include <vector>
#include <memory>

class IShapeStrategy
{
public:
    //выяснить в какой ситуации без вирутального диструктора будет проблема
    virtual ~IShapeStrategy() = default;

    virtual void Draw(ICanvas& canvas, const std::string& color) const = 0;
    virtual void Move(double dx, double dy) = 0;

    virtual std::string GetType() const = 0;
    virtual std::vector<double> GetParameters() const = 0;//это кривое решение, надо стринг вместо дабл
    virtual std::string GetExtraInfo() const { return ""; }//спорно

};