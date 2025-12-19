#include "image.h"
#include <stdexcept>
#include <sstream>
#include <algorithm>

Image::Image(Size s, char c) 
    :size(s) 
{
    if (s.width <= 0 || s.height <= 0)
    {
        throw std::out_of_range("Invalid size");
    }

    tx = (s.width + Tile::SIZE - 1) / Tile::SIZE;
    ty = (s.height + Tile::SIZE - 1) / Tile::SIZE;

    CoW<Tile> base{ Tile(c) };
    tiles.assign(tx * ty, base);
}

Size Image::GetSize() const noexcept 
{
    return size; 
}

char Image::GetPixel(Point p) const noexcept 
{
    if (!IsPointInSize(p, size))
    {
        return ' ';
    }

    int ix = p.x / Tile::SIZE;
    int iy = p.y / Tile::SIZE;

    return tiles[iy * tx + ix]->GetPixel({ p.x % Tile::SIZE,p.y % Tile::SIZE });
}

void Image::SetPixel(Point p, char c) 
{
    if (!IsPointInSize(p, size))
    {
        return;
    }

    int ix = p.x / Tile::SIZE;
    int iy = p.y / Tile::SIZE;

    tiles[iy * tx + ix].Write()->SetPixel({ p.x % Tile::SIZE,p.y % Tile::SIZE }, c);
}

void Print(const Image& img, std::ostream& o) 
{
    auto s = img.GetSize();
    for (int y = 0; y < s.height; y++, o << '\n')
    {
        for (int x = 0;x < s.width;x++)
        {
            o << img.GetPixel({ x,y });
        }
    }
}

Image LoadImage(const std::string& px) 
{
    std::istringstream in(px); 
    Size s{};
    std::string l;

    while (std::getline(in, l)) 
    {
        s.width = std::max(s.width, static_cast<int>(l.size()));
        s.height++;
    }

    Image img{ s };

    in = std::istringstream(px);
    for (int y = 0; y < s.height; y++)
    {
        if (!std::getline(in, l))
        {
            break;
        }

        for (int x = 0; x < l.size(); x++)
        {
            img.SetPixel({ x,y }, l[x]);
        }
    }

    return img;
}
