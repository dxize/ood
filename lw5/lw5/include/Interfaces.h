#pragma once
#include "Common.h"
#include <string>

class IParagraph
{
public:
    virtual std::string GetText() const = 0;
    virtual void SetText(const std::string& text) = 0;
    virtual ~IParagraph() = default;
};

class IImage
{
public:
    virtual Path GetPath() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual void Resize(int width, int height) = 0;
    virtual ~IImage() = default;
};
