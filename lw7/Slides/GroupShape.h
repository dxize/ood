#pragma once
#include "Ishape.h"
#include <vector>
#include <memory>

class GroupShape : public IShape, public std::enable_shared_from_this<GroupShape> 
{
public:
    void AddShape(std::shared_ptr<IShape> shape);
    void RemoveShape(size_t index);
    size_t GetShapeCount() const;
    std::shared_ptr<IShape> GetShapeAt(size_t index);
    Rect GetFrame() const override;
    void SetFrame(const Rect& rect) override;
    std::shared_ptr<ILineStyle> GetLineStyle() override;
    std::shared_ptr<IFillStyle> GetFillStyle() override;
    void Draw(ICanvas& canvas) const override;
private:
    std::vector<std::shared_ptr<IShape>> shapes;
    std::shared_ptr<ILineStyle> lineStyle;
    std::shared_ptr<IFillStyle> fillStyle;
    void InvalidateStyles();
};