#pragma once
#include "Istyle.h"

class SimpleLineStyle : public ILineStyle 
{
public:
    std::optional<bool> GetEnabled() const override 
    { 
        return enabled; 
    }
    void SetEnabled(bool enable) override 
    { 
        enabled = enable; 
    }
    std::optional<RGBAColor> GetColor() const override
    { 
        return color; 
    }
    void SetColor(RGBAColor c) override 
    {
        color = c; 
    }
    std::optional<double> GetThickness() const override 
    { 
        return thickness; 
    }
    void SetThickness(double t) override 
    { 
        thickness = t; 
    }
private:
    bool enabled = true;
    RGBAColor color = { 0, 0, 0, 255 };
    double thickness = 1.0;

};

class SimpleFillStyle : public IFillStyle 
{
public:
    std::optional<bool> GetEnabled() const override 
    { 
        return enabled;
    }
    void SetEnabled(bool enable) override 
    {
        enabled = enable;
    }
    std::optional<RGBAColor> GetColor() const override 
    { 
        return color; 
    }
    void SetColor(RGBAColor c) override 
    { 
        color = c; 
    }
private:
    bool enabled = true;
    RGBAColor color = { 255, 255, 255, 255 };

};