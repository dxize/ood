#pragma once
#include <array>
#include <cstdint>

std::array<uint8_t, 256> MakeSubstTable(uint32_t key);
std::array<uint8_t, 256> InvertTable(const std::array<uint8_t, 256>& t);
