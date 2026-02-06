#include "Image.h"
#include "Commands.h"
#include <stdexcept>

CImage::CImage(Path relPath, int w, int h, CCommandHistory* history)
    : m_relPath(std::move(relPath))
    , m_w(w)
    , m_h(h)
    , m_history(history)
{
}

Path CImage::GetPath() const 
{ 
    return m_relPath;
}
int CImage::GetWidth() const 
{ 
    return m_w;
}
int CImage::GetHeight() const 
{ 
    return m_h; 
}

void CImage::ResizeImpl(int w, int h)
{
    m_w = w;
    m_h = h;
}

void CImage::Resize(int width, int height)
{
    if (width < 1 || width > 10000 || height < 1 || height > 10000)
    {
        throw std::runtime_error("Invalid image size (1..10000)");
    }

    if (!m_history)
    {
        throw std::runtime_error("Image has no history");
    }

    m_history->AddAndExecute(std::make_shared<CResizeImageCommand>(shared_from_this(), width, height));
}
