#pragma once
#include "Shape.h"
#include "DrawShapes/ICanvas.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace shapes
{
    class CPicture
    {
    public:
        void AddShape(const std::string& id, std::unique_ptr<Shape> shape);
        void DeleteShape(const std::string& id);
        void MoveShape(const std::string& id, double dx, double dy);
        void MoveAllShapes(double dx, double dy);
        void ChangeShapeColor(const std::string& id, const std::string& newColor);
        void ChangeShape(const std::string& id, std::unique_ptr<IShapeStrategy> newStrategy);

        void DrawShape(ICanvas& canvas, const std::string& id) const;
        void DrawAllShapes(ICanvas& canvas) const;

        const std::vector<std::string>& GetShapeOrder() const { return m_shapeOrder; }
        const Shape* GetShape(const std::string& id) const;
        Shape* GetShape(const std::string& id);

        std::string ListShapes() const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Shape>> m_shapes;
        std::vector<std::string> m_shapeOrder;
    };
}
