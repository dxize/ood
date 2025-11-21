#pragma once
#include "Istyle.h"
#include "GroupShape.h"
#include <functional>
#include <cstddef>  // for size_t

class CompoundLineStyle : public ILineStyle 
{
public:
    CompoundLineStyle(std::shared_ptr<GroupShape> g) : group(g) {}

    template<typename T>
    std::optional<T> GetUniformValue(std::function<std::optional<T>(const std::shared_ptr<const ILineStyle>&)> getter) const;

    std::optional<bool> GetEnabled() const override;
    void SetEnabled(bool enable) override;
    std::optional<RGBAColor> GetColor() const override;
    void SetColor(RGBAColor color) override;
    std::optional<double> GetThickness() const override;
    void SetThickness(double thickness) override;
private:
    std::shared_ptr<GroupShape> group;

};

template<typename T>
std::optional<T> CompoundLineStyle::GetUniformValue(std::function<std::optional<T>(const std::shared_ptr<const ILineStyle>&)> getter) const 
{
    if (group->GetShapeCount() == 0) 
    {
        return std::nullopt;
    }
    auto firstValue = getter(group->GetShapeAt(0)->GetLineStyle());
    if (!firstValue) 
    {
        return std::nullopt;
    }
    for (size_t i = 1; i < group->GetShapeCount(); ++i) 
    {
        auto value = getter(group->GetShapeAt(i)->GetLineStyle());
        if (!value || *value != *firstValue) 
        {
            return std::nullopt;
        }
    }
    return firstValue;
}

std::optional<bool> CompoundLineStyle::GetEnabled() const 
{
    return GetUniformValue<bool>([](const auto& style) { return style->GetEnabled(); });
}

void CompoundLineStyle::SetEnabled(bool enable) 
{
    for (size_t i = 0; i < group->GetShapeCount(); ++i) 
    {
        group->GetShapeAt(i)->GetLineStyle()->SetEnabled(enable);
    }
}

std::optional<RGBAColor> CompoundLineStyle::GetColor() const 
{
    return GetUniformValue<RGBAColor>([](const auto& style) { return style->GetColor(); });
}

void CompoundLineStyle::SetColor(RGBAColor color) 
{
    for (size_t i = 0; i < group->GetShapeCount(); ++i) 
    {
        group->GetShapeAt(i)->GetLineStyle()->SetColor(color);
    }
}

std::optional<double> CompoundLineStyle::GetThickness() const 
{
    return GetUniformValue<double>([](const auto& style) { return style->GetThickness(); });
}

void CompoundLineStyle::SetThickness(double thickness) 
{
    for (size_t i = 0; i < group->GetShapeCount(); ++i) 
    {
        group->GetShapeAt(i)->GetLineStyle()->SetThickness(thickness);
    }
}

class CompoundFillStyle : public IFillStyle 
{
public:
    CompoundFillStyle(std::shared_ptr<GroupShape> g) : group(g) {}

    template<typename T>
    std::optional<T> GetUniformValue(std::function<std::optional<T>(const std::shared_ptr<const IFillStyle>&)> getter) const;

    std::optional<bool> GetEnabled() const override;
    void SetEnabled(bool enable) override;
    std::optional<RGBAColor> GetColor() const override;
    void SetColor(RGBAColor color) override;
private:
    std::shared_ptr<GroupShape> group;
};

template<typename T>
std::optional<T> CompoundFillStyle::GetUniformValue(std::function<std::optional<T>(const std::shared_ptr<const IFillStyle>&)> getter) const 
{
    if (group->GetShapeCount() == 0) 
    {
        return std::nullopt;
    }
    auto firstValue = getter(group->GetShapeAt(0)->GetFillStyle());
    if (!firstValue) 
    {
        return std::nullopt;
    }
    for (size_t i = 1; i < group->GetShapeCount(); ++i) 
    {
        auto value = getter(group->GetShapeAt(i)->GetFillStyle());
        if (!value || *value != *firstValue) 
        {
            return std::nullopt;
        }
    }
    return firstValue;
}

std::optional<bool> CompoundFillStyle::GetEnabled() const 
{
    return GetUniformValue<bool>([](const auto& style) { return style->GetEnabled(); });
}

void CompoundFillStyle::SetEnabled(bool enable) 
{
    for (size_t i = 0; i < group->GetShapeCount(); ++i) 
    {
        group->GetShapeAt(i)->GetFillStyle()->SetEnabled(enable);
    }
}

std::optional<RGBAColor> CompoundFillStyle::GetColor() const 
{
    return GetUniformValue<RGBAColor>([](const auto& style) { return style->GetColor(); });
}

void CompoundFillStyle::SetColor(RGBAColor color) 
{
    for (size_t i = 0; i < group->GetShapeCount(); ++i) 
    {
        group->GetShapeAt(i)->GetFillStyle()->SetColor(color);
    }
}
