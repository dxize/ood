#pragma once
#include "decorators/InputStreamDecorator.h"
#include <array>
#include <cstdint>

class DecryptInputStream final : public InputStreamDecorator 
{
public:
    DecryptInputStream(std::unique_ptr<IInputDataStream> inner, uint32_t key);

    uint8_t ReadByte() override;
    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override;

private:
    std::array<uint8_t, 256> m_inv{};
};
