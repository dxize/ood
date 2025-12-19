#pragma once
#include "geom.h"
#include "tile.h"
#include "cow.h"
#include <vector>
#include <string>

class Image 
{
public:
    Image(Size size, char color = ' ');

    Size GetSize() const noexcept;
    char GetPixel(Point) const noexcept;
    void SetPixel(Point, char);

private:
    Size size{};
    int tx = 0;
    int ty = 0;
    std::vector<CoW<Tile>> tiles;
};

void Print(const Image& img, std::ostream& o);
Image LoadImage(const std::string& px);
