#pragma once
#include "decorators/OutputStreamDecorator.h"
#include <array>
#include <cstdint>

class EncryptOutputStream final : public OutputStreamDecorator 
{
public:
    EncryptOutputStream(std::unique_ptr<IOutputDataStream> inner, uint32_t key);

    void WriteByte(uint8_t data) override;
    void WriteBlock(const void* srcData, std::streamsize size) override;

private:
    std::array<uint8_t, 256> m_table{};
};
