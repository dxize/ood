#pragma once
#include "Interfaces.h"
#include "CommandHistory.h"
#include <memory>

class CResizeImageCommand;

class CImage final : public IImage, public std::enable_shared_from_this<CImage>
{
public:
    CImage(Path relPath, int w, int h, CCommandHistory* history);

    Path GetPath() const override;
    int GetWidth() const override;
    int GetHeight() const override;

    void Resize(int width, int height) override;

private:
    friend class CResizeImageCommand;
    void ResizeImpl(int w, int h);

private:
    Path m_relPath;
    int m_w = 1;
    int m_h = 1;
    CCommandHistory* m_history = nullptr;
};
