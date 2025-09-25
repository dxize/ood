#pragma once

#include "../CPoint.h"
#include "ICanvas.h"
#include <cstdint>
#include <vector>

class CCanvas: public ICanvas
{
public:
	void DrawLine(CPoint from, CPoint to, uint32_t lineColor) const override;
	void FillPolygon(std::vector<CPoint> points, uint32_t fillColor) const override;
	void DrawCircle(CPoint center, double radius, uint32_t lineColor) const override;
	void FillCircle(CPoint center, double radius, uint32_t fillColor) const override;
};
