#pragma once

#include "../CPoint.h"
#include <cstdint>
#include <vector>

class ICanvas
{
public:
	virtual void DrawLine(CPoint from, CPoint to, uint32_t lineColor) const = 0;
	virtual void FillPolygon(std::vector<CPoint> points, uint32_t fillColor) const = 0;
	virtual void DrawCircle(CPoint center, double radius, uint32_t lineColor) const = 0;
	virtual void FillCircle(CPoint center, double radius, uint32_t fillColor) const = 0;
	virtual ~ICanvas() = default;
};
	