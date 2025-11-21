#include "GroupShape.h"
#include "CompoundStyles.h"
#include <cstddef>  
#include <stdexcept>  

void GroupShape::InvalidateStyles() 
{
    lineStyle = nullptr;
    fillStyle = nullptr;
}

void GroupShape::AddShape(std::shared_ptr<IShape> shape) 
{
    shapes.push_back(shape);
    InvalidateStyles();
}

void GroupShape::RemoveShape(size_t index) 
{
    if (index >= shapes.size()) 
    {
        throw std::out_of_range("Index out of range");
    }
    shapes.erase(shapes.begin() + index);
    InvalidateStyles();  // Инвалидируем стили после удаления
}

size_t GroupShape::GetShapeCount() const { return shapes.size(); }

std::shared_ptr<IShape> GroupShape::GetShapeAt(size_t index) { return shapes.at(index); }

Rect GroupShape::GetFrame() const {
    if (shapes.empty()) 
    {
        return Rect();
    }
    Rect frame = shapes[0]->GetFrame();
    for (size_t i = 1; i < shapes.size(); ++i) {
        frame = Union(frame, shapes[i]->GetFrame());
    }
    return frame;
}

void GroupShape::SetFrame(const Rect& newFrame) {
    Rect oldFrame = GetFrame();
    if (oldFrame.IsEmpty() || newFrame.IsEmpty()) {
        return; // Avoid division by zero
    }
    double scaleX = newFrame.width / oldFrame.width;
    double scaleY = newFrame.height / oldFrame.height;
    double offsetX = newFrame.x - oldFrame.x * scaleX;
    double offsetY = newFrame.y - oldFrame.y * scaleY;

    for (auto& shape : shapes) {
        Rect childFrame = shape->GetFrame();
        Rect newChildFrame(
            childFrame.x * scaleX + offsetX,
            childFrame.y * scaleY + offsetY,
            childFrame.width * scaleX,
            childFrame.height * scaleY
        );
        shape->SetFrame(newChildFrame);
    }
    InvalidateStyles();
}

std::shared_ptr<ILineStyle> GroupShape::GetLineStyle() {
    if (!lineStyle) {
        lineStyle = std::make_shared<CompoundLineStyle>(shared_from_this());
    }
    return lineStyle;
}

std::shared_ptr<IFillStyle> GroupShape::GetFillStyle() {
    if (!fillStyle) {
        fillStyle = std::make_shared<CompoundFillStyle>(shared_from_this());
    }
    return fillStyle;
}

void GroupShape::Draw(ICanvas& canvas) const 
{
    for (const auto& shape : shapes) 
    {
        shape->Draw(canvas);
    }
}