#pragma once

#include "IShape.h"
#include "CPoint.h"

class CLineSegment : public IShape
{
public:
    CLineSegment(const CPoint& start, const CPoint& end, uint32_t color)
        : IShape(color)  
        , m_start(start)
        , m_end(end)
    {}

    void Draw(ICanvas& canvas) const override;

private:
    CPoint m_start, m_end;
};
