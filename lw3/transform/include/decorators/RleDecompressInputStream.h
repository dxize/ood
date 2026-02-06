#pragma once
#include "decorators/InputStreamDecorator.h"
#include <cstdint>

class RleDecompressInputStream final : public InputStreamDecorator
{
public:
    explicit RleDecompressInputStream(std::unique_ptr<IInputDataStream> inner);

    bool IsEOF() const override;
    uint8_t ReadByte() override;
    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override;

private:
    bool EnsureRunLoadedOrEof();

    uint8_t m_runByte = 0;
    uint8_t m_leftInRun = 0;
};
