#include "CLineSegment.h"
#include <cmath>
#include <sstream>
#include <iomanip>

void CLineSegment::Draw(ICanvas& canvas) const
{
    canvas.SetColor(m_outlineColor);
    canvas.LineTo(m_start, m_end);
}