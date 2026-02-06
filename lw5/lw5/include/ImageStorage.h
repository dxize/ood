#pragma once
#include "Common.h"
#include <map>

class CImageStorage
{
public:
    explicit CImageStorage(Path docRoot);

    Path ImportImage(const Path& srcPath);

    Path AbsPath(const Path& rel) const;

    void MarkForDeletion(const Path& rel);
    void UnmarkDeletion(const Path& rel);
    bool IsMarked(const Path& rel) const;

    void PhysicalDelete(const Path& rel);

private:
    Path m_root;
    Path m_imagesDir;

    size_t m_counter = 0;
    std::map<Path, bool> m_marked; 
};
