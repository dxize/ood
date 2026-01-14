#include "../../include/util/ColorUtil.h"
#include "../../include/util/StringUtil.h"

#include <stdexcept>
#include <unordered_map>

namespace util
{
    domain::Color ParseColor(const std::string& token)
    {
        // Важно: ToLowerAscii не умеет корректно менять регистр UTF-8.
        // Поэтому русские названия цветов ожидаются уже в нижнем регистре.
        const std::string t = ToLowerAscii(token);

        static const std::unordered_map<std::string, domain::Color> map = {
            {"green", domain::Color::Green},   {"зелёный", domain::Color::Green}, {"зеленый", domain::Color::Green},
            {"red", domain::Color::Red},       {"красный", domain::Color::Red},
            {"blue", domain::Color::Blue},     {"синий", domain::Color::Blue},
            {"yellow", domain::Color::Yellow}, {"жёлтый", domain::Color::Yellow}, {"желтый", domain::Color::Yellow},
            {"pink", domain::Color::Pink},     {"розовый", domain::Color::Pink},
            {"black", domain::Color::Black},   {"чёрный", domain::Color::Black}, {"черный", domain::Color::Black},
        };

        auto it = map.find(t);
        if (it == map.end())
        {
            throw std::runtime_error("Unknown color: " + token);
        }
        return it->second;
    }

    const char* ToSvgColor(domain::Color c)
    {
        switch (c)
        {
        case domain::Color::Green:  return "green";
        case domain::Color::Red:    return "red";
        case domain::Color::Blue:   return "blue";
        case domain::Color::Yellow: return "yellow";
        case domain::Color::Pink:   return "pink";
        case domain::Color::Black:  return "black";
        }
        return "black";
    }

    const char* ToTextColor(domain::Color c)
    {
        return ToSvgColor(c);
    }
} 
