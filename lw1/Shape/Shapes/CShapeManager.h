#pragma once

#include "IShape.h"
#include <vector>
#include <memory>
#include <optional>

class CShapeManager
{
public:
    void AddShape(std::shared_ptr<IShape> shape);
    const std::vector<std::shared_ptr<IShape>>& GetShapes() const;

    std::shared_ptr<IShape> FindShapeWithMaxArea() const;
    std::shared_ptr<IShape> FindShapeWithMinPerimeter() const;

    void PrintAll() const;

private:
    std::vector<std::shared_ptr<IShape>> m_shapes;
};
