#include "crypto/SubstitutionTable.h"
#include <algorithm>
#include <random>

std::array<uint8_t, 256> MakeSubstTable(uint32_t key) 
{
    std::array<uint8_t, 256> t{};

    for (int i = 0; i < 256; ++i)
    {
        t[i] = static_cast<uint8_t>(i);
    }

    std::mt19937 gen(key);
    std::shuffle(t.begin(), t.end(), gen);
    return t;
}

std::array<uint8_t, 256> InvertTable(const std::array<uint8_t, 256>& t) 
{
    std::array<uint8_t, 256> inv{};

    for (int i = 0; i < 256; ++i) 
    {
        inv[t[i]] = static_cast<uint8_t>(i);
    }

    return inv;
}
