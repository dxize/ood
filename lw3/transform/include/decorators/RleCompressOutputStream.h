#pragma once
#include "decorators/OutputStreamDecorator.h"
#include <cstdint>

class RleCompressOutputStream final : public OutputStreamDecorator 
{
public:
    explicit RleCompressOutputStream(std::unique_ptr<IOutputDataStream> inner);

    void WriteByte(uint8_t data) override;
    void WriteBlock(const void* srcData, std::streamsize size) override;
    void Close() override;

private:
    void FlushRun();
    void StartRun(uint8_t data);

    bool m_hasRun = false;
    uint8_t m_runByte = 0;
    uint8_t m_runCount = 0;
};
