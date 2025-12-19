#pragma once

struct Point 
{ 
    int x = 0, y = 0; 
};

struct Size 
{ 
    int width = 0, height = 0;
};

// Точка передаётся в локальных координатах.
inline bool IsPointInSize(Point p, Size s) noexcept 
{
    return p.x >= 0 && p.y >= 0 && p.x < s.width && p.y < s.height;
}
