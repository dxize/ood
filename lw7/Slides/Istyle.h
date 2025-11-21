#pragma once
#include "Types.h"
#include <optional>

class ILineStyle 
{
public:
    virtual ~ILineStyle() = default;
    virtual std::optional<bool> GetEnabled() const = 0;
    virtual void SetEnabled(bool enable) = 0;
    virtual std::optional<RGBAColor> GetColor() const = 0;
    virtual void SetColor(RGBAColor color) = 0;
    virtual std::optional<double> GetThickness() const = 0;
    virtual void SetThickness(double thickness) = 0;
};

class IFillStyle 
{
public:
    virtual ~IFillStyle() = default;
    virtual std::optional<bool> GetEnabled() const = 0;
    virtual void SetEnabled(bool enable) = 0;
    virtual std::optional<RGBAColor> GetColor() const = 0;
    virtual void SetColor(RGBAColor color) = 0;
};