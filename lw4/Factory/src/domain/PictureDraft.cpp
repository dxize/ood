#include "../../include/domain/PictureDraft.h"
#include "../../include/domain/Shape.h"

#include <stdexcept>

namespace domain
{
    std::size_t PictureDraft::GetShapeCount() const
    {
        return m_shapes.size();
    }

    const Shape& PictureDraft::GetShape(std::size_t index) const
    {
        if (index >= m_shapes.size())
        {
            throw std::out_of_range("Shape index out of range");
        }
        return *m_shapes[index];
    }

    void PictureDraft::AddShape(std::unique_ptr<Shape> shape)
    {
        if (!shape)
        {
            throw std::invalid_argument("Cannot add null shape");
        }
        m_shapes.push_back(std::move(shape));
    }
} 
