#pragma once

#include <memory>
#include <string>

namespace domain { class Shape; }

namespace factory
{
    class IShapeFactory
    {
    public:
        virtual ~IShapeFactory() = default;
        virtual std::unique_ptr<domain::Shape> CreateShape(const std::string& descr) const = 0;
    };
} 
