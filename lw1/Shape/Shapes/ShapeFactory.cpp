#include "ShapeFactory.h"
#include "CircleStrategy.h"
#include "RectangleStrategy.h"
#include "TriangleStrategy.h"
#include "LineStrategy.h"
#include "TextStrategy.h"
#include <stdexcept>
#include <sstream>

ShapeFactory::ShapeFactory() 
{
    m_creators["circle"] = [](const std::vector<std::string>& params) 
    {
        if (params.size() != 3) 
        {
            throw std::invalid_argument("Circle requires 3 parameters: x y radius");
        }
        double x = std::stod(params[0]);
        double y = std::stod(params[1]);
        double r = std::stod(params[2]);
        if (r < 0) 
        {
            throw std::invalid_argument("Circle radius cannot be negative");
        }
        return std::make_unique<CircleStrategy>(x, y, r);
    };

    m_creators["rectangle"] = [](const std::vector<std::string>& params) 
    {
        if (params.size() != 4) 
        {
            throw std::invalid_argument("Rectangle requires 4 parameters: left top width height");
        }
        double left = std::stod(params[0]);
        double top = std::stod(params[1]);
        double width = std::stod(params[2]);
        double height = std::stod(params[3]);
        if (width < 0 || height < 0) 
        {
            throw std::invalid_argument("Rectangle width and height cannot be negative");
        }
        return std::make_unique<RectangleStrategy>(left, top, width, height);
    };

    m_creators["triangle"] = [](const std::vector<std::string>& params) 
    {
        if (params.size() != 6) 
        {
            throw std::invalid_argument("Triangle requires 6 parameters: x1 y1 x2 y2 x3 y3");
        }
        double x1 = std::stod(params[0]);
        double y1 = std::stod(params[1]);
        double x2 = std::stod(params[2]);
        double y2 = std::stod(params[3]);
        double x3 = std::stod(params[4]);
        double y3 = std::stod(params[5]);
        return std::make_unique<TriangleStrategy>(x1, y1, x2, y2, x3, y3);
    };

    m_creators["line"] = [](const std::vector<std::string>& params) 
    {
        if (params.size() != 4) 
        {
            throw std::invalid_argument("Line requires 4 parameters: x1 y1 x2 y2");
        }
        double x1 = std::stod(params[0]);
        double y1 = std::stod(params[1]);
        double x2 = std::stod(params[2]);
        double y2 = std::stod(params[3]);
        return std::make_unique<LineStrategy>(x1, y1, x2, y2);
    };

    m_creators["text"] = [](const std::vector<std::string>& params) 
    {
        if (params.size() < 4) 
        {
            throw std::invalid_argument("Text requires at least 4 parameters: left top fontSize text");
        }
        double left = std::stod(params[0]);
        double top = std::stod(params[1]);
        double fontSize = std::stod(params[2]);
        if (fontSize < 0) 
        {
            throw std::invalid_argument("Text font size cannot be negative");
        }

        std::string text;
        for (size_t i = 3; i < params.size(); ++i) 
        {
            if (i > 3) text += " ";
            text += params[i];
        }

        return std::make_unique<TextStrategy>(left, top, fontSize, text);
    };
}

std::unique_ptr<IShapeStrategy> ShapeFactory::CreateStrategy(const std::string& type,
    const std::vector<std::string>& params) 
{
    auto it = m_creators.find(type);
    if (it == m_creators.end()) 
    {
        throw std::invalid_argument("Unknown shape type: " + type);
    }
    return it->second(params);
}