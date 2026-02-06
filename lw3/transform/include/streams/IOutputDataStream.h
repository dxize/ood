#pragma once
#include <cstdint>
#include <ios>

class IOutputDataStream 
{
public:
    virtual void WriteByte(uint8_t data) = 0;
    virtual void WriteBlock(const void* srcData, std::streamsize size) = 0;
    virtual void Close() = 0;
    virtual ~IOutputDataStream() = default;
};
