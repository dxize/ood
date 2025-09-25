#include "CShapeManager.h"
#include <iostream>
#include <algorithm>

void CShapeManager::AddShape(std::shared_ptr<IShape> shape)
{
    m_shapes.push_back(shape);
}

const std::vector<std::shared_ptr<IShape>>& CShapeManager::GetShapes() const
{
    return m_shapes;
}

std::shared_ptr<IShape> CShapeManager::FindShapeWithMaxArea() const
{
    if (m_shapes.empty()) return nullptr;

    return *std::max_element(m_shapes.begin(), m_shapes.end(),
        [](const auto& a, const auto& b) {
            return a->GetArea() < b->GetArea();
        });
}

std::shared_ptr<IShape> CShapeManager::FindShapeWithMinPerimeter() const
{
    if (m_shapes.empty()) return nullptr;

    return *std::min_element(m_shapes.begin(), m_shapes.end(),
        [](const auto& a, const auto& b) {
            return a->GetPerimeter() < b->GetPerimeter();
        });
}

void CShapeManager::PrintAll() const
{
    for (const auto& shape : m_shapes)
    {
        std::cout << shape->ToString() << "\n\n";
    }
    std::cout << "\n";
}
