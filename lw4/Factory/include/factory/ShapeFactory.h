#pragma once

#include "../../include/factory/IShapeFactory.h"

namespace factory
{
    class ShapeFactory final : public IShapeFactory
    {
    public:
        std::unique_ptr<domain::Shape> CreateShape(const std::string& descr) const override;
    };
} 
