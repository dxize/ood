#pragma once
#include "streams/IInputDataStream.h"
#include <cstddef>
#include <vector>

class MemoryInputStream final : public IInputDataStream 
{
public:
    explicit MemoryInputStream(std::vector<uint8_t> data);

    bool IsEOF() const override;
    uint8_t ReadByte() override;
    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override;

private:
    std::vector<uint8_t> m_data;
    std::size_t m_pos = 0;
};
