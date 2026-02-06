#include "ImageStorage.h"
#include <filesystem>
#include <stdexcept>
#include <string>

CImageStorage::CImageStorage(Path docRoot)
    : m_root(std::move(docRoot))
    , m_imagesDir(m_root / "images")
{
    fs::create_directories(m_imagesDir);
}

Path CImageStorage::ImportImage(const Path& srcPath)
{
    if (!fs::exists(srcPath))
        throw std::runtime_error("Image file does not exist: " + srcPath.string());

    auto ext = srcPath.extension().string();
    if (ext.empty())
    {
        ext = ".img";
    }

    Path rel;
    while (true)
    {
        ++m_counter;
        rel = Path("images") / ("img" + std::to_string(m_counter) + ext);
        if (!fs::exists(m_root / rel))
            break;
    }

    fs::copy_file(srcPath, m_root / rel, fs::copy_options::overwrite_existing);
    m_marked[rel] = false;  
    return rel;
}

Path CImageStorage::AbsPath(const Path& rel) const
{
    return m_root / rel;
}

void CImageStorage::MarkForDeletion(const Path& rel)
{
    m_marked[rel] = true;
}

void CImageStorage::UnmarkDeletion(const Path& rel)
{
    m_marked[rel] = false;
}

bool CImageStorage::IsMarked(const Path& rel) const
{
    auto it = m_marked.find(rel);
    if (it == m_marked.end())
    {
        return false;
    }
    return it->second;
}

void CImageStorage::PhysicalDelete(const Path& rel)
{
    std::error_code ec;
    fs::remove(AbsPath(rel), ec);
    m_marked.erase(rel);
}
