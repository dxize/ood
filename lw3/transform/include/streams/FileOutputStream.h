#pragma once
#include "streams/IOutputDataStream.h"
#include <fstream>
#include <string>

class FileOutputStream final : public IOutputDataStream 
{
public:
    explicit FileOutputStream(const std::string& path);

    void WriteByte(uint8_t data) override;
    void WriteBlock(const void* srcData, std::streamsize size) override;
    void Close() override;

private:
    void EnsureOpen() const;

    std::ofstream m_out;
    bool m_closed = false;
};
