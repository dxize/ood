#pragma once
#include "IShape.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

class CPicture 
{
public:
    bool AddShape(const std::string& id, std::unique_ptr<IShape> shape);
    bool DeleteShape(const std::string& id);
    void MoveShape(const std::string& id, double dx, double dy);
    void MoveAllShapes(double dx, double dy);
    void ChangeShapeColor(const std::string& id, uint32_t newColor);
    void ChangeShape(const std::string& id, std::unique_ptr<IShape> newShape);

    const std::vector<std::string>& GetShapeOrder() const { return m_shapeOrder; }
    const IShape* GetShape(const std::string& id) const;

    std::string ListShapes() const;

    IShape* GetShape(const std::string& id);
private:
    std::unordered_map<std::string, std::unique_ptr<IShape>> m_shapes;
    std::vector<std::string> m_shapeOrder;
};