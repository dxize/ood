#include "../../include/factory/ShapeFactory.h"

#include "../../include/domain/Point.h"
#include "../../include/domain/shapes/Ellipse.h"
#include "../../include/domain/shapes/Rectangle.h"
#include "../../include/domain/shapes/RegularPolygon.h"
#include "../../include/domain/shapes/Triangle.h"
#include "../../include/util/ColorUtil.h"
#include "../../include/util/StringUtil.h"

#include <sstream>
#include <stdexcept>

namespace factory
{
    static double ReadDouble(std::istringstream& iss, const char* name)
    {
        double v = 0.0;
        if (!(iss >> v))
        {
            throw std::runtime_error(std::string("Expected ") + name);
        }
        return v;
    }

    static int ReadInt(std::istringstream& iss, const char* name)
    {
        int v = 0;
        if (!(iss >> v))
        {
            throw std::runtime_error(std::string("Expected ") + name);
        }
        return v;
    }

    std::unique_ptr<domain::Shape> ShapeFactory::CreateShape(const std::string& descr) const
    {
        std::istringstream iss(descr);

        std::string shapeToken;
        if (!(iss >> shapeToken))
        {
            throw std::runtime_error("Empty shape description");
        }
        const std::string shape = util::ToLowerAscii(shapeToken);

        std::string colorToken;
        if (!(iss >> colorToken))
        {
            throw std::runtime_error("Expected color after shape type");
        }
        const domain::Color color = util::ParseColor(colorToken);

        if (shape == "triangle")
        {
            const domain::Point v1{ReadDouble(iss, "x1"), ReadDouble(iss, "y1")};
            const domain::Point v2{ReadDouble(iss, "x2"), ReadDouble(iss, "y2")};
            const domain::Point v3{ReadDouble(iss, "x3"), ReadDouble(iss, "y3")};
            return std::make_unique<domain::Triangle>(color, v1, v2, v3);
        }

        if (shape == "rectangle")
        {
            const double left = ReadDouble(iss, "left");
            const double top = ReadDouble(iss, "top");
            const double right = ReadDouble(iss, "right");
            const double bottom = ReadDouble(iss, "bottom");
            return std::make_unique<domain::Rectangle>(color, domain::Point{left, top}, domain::Point{right, bottom});
        }

        if (shape == "ellipse")
        {
            const double cx = ReadDouble(iss, "cx");
            const double cy = ReadDouble(iss, "cy");
            const double rx = ReadDouble(iss, "rx");
            const double ry = ReadDouble(iss, "ry");
            return std::make_unique<domain::Ellipse>(color, domain::Point{cx, cy}, rx, ry);
        }

        if (shape == "polygon")
        {
            const double cx = ReadDouble(iss, "cx");
            const double cy = ReadDouble(iss, "cy");
            const double radius = ReadDouble(iss, "radius");
            const int vertexCount = ReadInt(iss, "vertexCount");
            return std::make_unique<domain::RegularPolygon>(color, domain::Point{cx, cy}, radius, vertexCount);
        }

        throw std::runtime_error("Unknown shape type: " + shapeToken);
    }
} 
