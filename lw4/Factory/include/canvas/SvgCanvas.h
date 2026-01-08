#pragma once

#include <ostream>

#include "ICanvas.h"
#include "../domain/Color.h"

namespace canvas
{
class SvgCanvas final : public ICanvas
{
public:
    SvgCanvas(std::ostream& out, double width, double height);
    ~SvgCanvas() override;

    void SetColor(domain::Color color) override;
    void DrawLine(const domain::Point& from, const domain::Point& to) override;
    void DrawEllipse(const domain::Point& leftTop, double width, double height) override;

private:
    std::ostream& m_out;
    double m_width = 0.0;
    double m_height = 0.0;
    domain::Color m_color = domain::Color::Black;
    bool m_closed = false;

    void CloseSvg();
};
} // namespace canvas
