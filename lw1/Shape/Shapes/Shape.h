#pragma once
#include "IShapeStrategy.h"
#include "DrawShapes/ICanvas.h"
#include <memory>
#include <string>

namespace shapes
{
    class Shape
    {
    public:
        Shape(std::unique_ptr<IShapeStrategy> strategy, const std::string& color, const std::string& id);

        void Draw(ICanvas& canvas) const;
        void Move(double dx, double dy);

        void SetColor(const std::string& color);
        void SetStrategy(std::unique_ptr<IShapeStrategy> strategy);

        std::string GetType() const;
        std::string GetColor() const { return m_color; }
        std::string GetId() const { return m_id; }
        std::vector<double> GetParameters() const;
        std::string ToString() const;
    private:
        std::unique_ptr<IShapeStrategy> m_strategy;
        std::string m_color; 
        std::string m_id;//подумать как при такой архитектуре пробелму с индефикаторами фигуры при её клонировании
    };
}