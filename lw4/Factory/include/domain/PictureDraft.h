#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "Shape.h"

namespace domain
{
    class PictureDraft
    {
    public:
        std::size_t GetShapeCount() const;
        const Shape& GetShape(std::size_t index) const;

        void AddShape(std::unique_ptr<Shape> shape);

    private:
        std::vector<std::unique_ptr<Shape>> m_shapes;
    };
}
