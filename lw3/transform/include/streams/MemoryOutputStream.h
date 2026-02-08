#pragma once
#include "streams/IOutputDataStream.h"
#include <vector>

class MemoryOutputStream final : public IOutputDataStream 
{
public:
    void WriteByte(uint8_t data) override;
    void WriteBlock(const void* srcData, std::streamsize size) override;
    void Close() override;

    const std::vector<uint8_t>& GetData() const;

private:
    void EnsureOpen() const;

    std::vector<uint8_t> m_data;
    bool m_closed = false;
};
