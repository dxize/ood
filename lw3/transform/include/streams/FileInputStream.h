#pragma once
#include "streams/IInputDataStream.h"
#include <fstream>
#include <string>

class FileInputStream final : public IInputDataStream 
{
public:
    explicit FileInputStream(const std::string& path);

    bool IsEOF() const override;
    uint8_t ReadByte() override;
    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override;

private:
    mutable std::ifstream m_in;
};
