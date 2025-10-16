#include "CPicture.h"
#include <sstream>
#include <algorithm>
#include <stdexcept> 


void shapes::CPicture::AddShape(const std::string& id, std::unique_ptr<Shape> shape) 
{
    if (m_shapes.find(id) != m_shapes.end()) 
    {
        throw std::runtime_error("Shape with id '" + id + "' already exists");
    }
    m_shapes[id] = std::move(shape);
    m_shapeOrder.push_back(id);
}

void shapes::CPicture::DeleteShape(const std::string& id) 
{
    auto it = m_shapes.find(id);
    if (it == m_shapes.end()) 
    {
        throw std::runtime_error("Shape with id '" + id + "' not found");
    }
    m_shapes.erase(it);
    m_shapeOrder.erase(std::remove(m_shapeOrder.begin(), m_shapeOrder.end(), id), m_shapeOrder.end());
}

void shapes::CPicture::MoveShape(const std::string& id, double dx, double dy) 
{
    auto it = m_shapes.find(id);
    if (it == m_shapes.end()) 
    {
        throw std::runtime_error("Shape with id '" + id + "' not found");
    }
    it->second->Move(dx, dy);
}

void shapes::CPicture::MoveAllShapes(double dx, double dy) 
{
    for (auto& pair : m_shapes) 
    {
        pair.second->Move(dx, dy);
    }
}

void shapes::CPicture::ChangeShapeColor(const std::string& id, const std::string& newColor) 
{
    auto it = m_shapes.find(id);
    if (it == m_shapes.end()) 
    {
        throw std::runtime_error("Shape with id '" + id + "' not found");
    }
    it->second->SetColor(newColor);
}

void shapes::CPicture::ChangeShape(const std::string& id, std::unique_ptr<IShapeStrategy> newStrategy) 
{
    auto it = m_shapes.find(id);
    if (it == m_shapes.end()) 
    {
        throw std::runtime_error("Shape with id '" + id + "' not found");
    }
    it->second->SetStrategy(std::move(newStrategy));
}

void shapes::CPicture::DrawShape(ICanvas& canvas, const std::string& id) const 
{
    auto it = m_shapes.find(id);
    if (it == m_shapes.end()) 
    {
        throw std::runtime_error("Shape with id '" + id + "' not found");
    }
    it->second->Draw(canvas);
}

void shapes::CPicture::DrawAllShapes(ICanvas& canvas) const 
{
    for (const auto& id : m_shapeOrder) 
    {
        auto it = m_shapes.find(id);
        if (it != m_shapes.end()) 
        {
            it->second->Draw(canvas);
        }
    }
}

std::string shapes::CPicture::ListShapes() const 
{
    std::stringstream ss;
    ss << "Shapes in picture (" << m_shapes.size() << " total):\n";

    int index = 1;
    for (const auto& id : m_shapeOrder) 
    {
        auto it = m_shapes.find(id);
        if (it != m_shapes.end()) 
        {
            ss << index << ". " << it->second->ToString() << "\n";
            index++;
        }
    }

    return ss.str();
}

shapes::Shape* shapes::CPicture::GetShape(const std::string& id) 
{
    auto it = m_shapes.find(id);
    if (it == m_shapes.end()) 
    {
        throw std::runtime_error("Shape with id '" + id + "' not found");
    }
    return it->second.get();
}

const shapes::Shape* shapes::CPicture::GetShape(const std::string& id) const 
{
    auto it = m_shapes.find(id);
    if (it == m_shapes.end()) 
    {
        throw std::runtime_error("Shape with id '" + id + "' not found");
    }
    return it->second.get();
}
