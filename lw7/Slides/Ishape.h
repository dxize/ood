#pragma once
#include "Types.h"
#include "Istyle.h"
#include <memory>

class ICanvas;

class IShape {
public:
    virtual ~IShape() = default;
    virtual Rect GetFrame() const = 0;
    virtual void SetFrame(const Rect& rect) = 0;
    virtual std::shared_ptr<ILineStyle> GetLineStyle() = 0;
    virtual std::shared_ptr<IFillStyle> GetFillStyle() = 0;
    virtual void Draw(ICanvas& canvas) const = 0;
};