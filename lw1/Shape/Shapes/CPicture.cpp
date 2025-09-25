#include "CPicture.h"
#include <sstream>
#include <algorithm>

bool CPicture::AddShape(const std::string& id, std::unique_ptr<IShape> shape) {
    if (m_shapes.find(id) != m_shapes.end()) {
        return false;
    }
    m_shapes[id] = std::move(shape);
    m_shapeOrder.push_back(id);
    return true;
}

bool CPicture::DeleteShape(const std::string& id) {
    auto it = m_shapes.find(id);
    if (it == m_shapes.end()) {
        return false;
    }
    m_shapes.erase(it);
    m_shapeOrder.erase(std::remove(m_shapeOrder.begin(), m_shapeOrder.end(), id), m_shapeOrder.end());
    return true;
}

void CPicture::MoveShape(const std::string& id, double dx, double dy) {
    auto it = m_shapes.find(id);
    if (it != m_shapes.end()) {
        it->second->Move(dx, dy);
    }
}

void CPicture::MoveAllShapes(double dx, double dy) {
    for (auto& pair : m_shapes) {
        pair.second->Move(dx, dy);
    }
}

void CPicture::ChangeShapeColor(const std::string& id, uint32_t newColor) {
    // Реализация зависит от конкретной архитектуры
    // Можно добавить метод SetOutlineColor в IShape, если нужно
}

void CPicture::ChangeShape(const std::string& id, std::unique_ptr<IShape> newShape) {
    auto it = m_shapes.find(id);
    if (it != m_shapes.end()) {
        it->second = std::move(newShape);
    }
}

std::string CPicture::ListShapes() const {
    std::stringstream ss;
    for (const auto& id : m_shapeOrder) {
        auto it = m_shapes.find(id);
        if (it != m_shapes.end()) {
            ss << "Shape " << id << ": " << it->second->ToString() << "\n";
        }
    }
    return ss.str();
}

IShape* CPicture::GetShape(const std::string& id) {
    auto it = m_shapes.find(id);
    return it != m_shapes.end() ? it->second.get() : nullptr;
}

const IShape* CPicture::GetShape(const std::string& id) const {
    auto it = m_shapes.find(id);
    return it != m_shapes.end() ? it->second.get() : nullptr;
}