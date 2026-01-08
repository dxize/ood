#pragma once

#include "../domain/Color.h"
#include "../domain/Point.h"

namespace canvas
{
class ICanvas
{
public:
    virtual ~ICanvas() = default;

    virtual void SetColor(domain::Color color) = 0;
    virtual void DrawLine(const domain::Point& from, const domain::Point& to) = 0;

    // leftTop = левый верхний угол описывающего прямоугольника
    virtual void DrawEllipse(const domain::Point& leftTop, double width, double height) = 0;
};
} // namespace canvas
