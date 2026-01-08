#pragma once

#include <functional>
#include <initializer_list>
#include <vector>

#include "ICanvas.h"

namespace canvas
{
// Прокидывает команды сразу на несколько холстов.
// Важно: MultiCanvas хранит ССЫЛКИ, поэтому Canvas-объекты должны жить дольше него.
class MultiCanvas final : public ICanvas
{
public:
    explicit MultiCanvas(std::initializer_list<std::reference_wrapper<ICanvas>> canvases);

    void SetColor(domain::Color color) override;
    void DrawLine(const domain::Point& from, const domain::Point& to) override;
    void DrawEllipse(const domain::Point& leftTop, double width, double height) override;

private:
    std::vector<std::reference_wrapper<ICanvas>> m_canvases;
};
} // namespace canvas
