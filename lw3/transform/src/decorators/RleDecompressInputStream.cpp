#include "decorators/RleDecompressInputStream.h"
#include <algorithm>
#include <stdexcept>

RleDecompressInputStream::RleDecompressInputStream(std::unique_ptr<IInputDataStream> inner)
    : InputStreamDecorator(std::move(inner))
{
}

bool RleDecompressInputStream::IsEOF() const
{
    if (m_leftInRun > 0)
    {
        return false;
    }
    return Inner().IsEOF();
}

bool RleDecompressInputStream::EnsureRunLoadedOrEof()
{
    if (m_leftInRun > 0)
    {
        return true;
    }

    if (Inner().IsEOF())
    {
        return false;
    }

    uint8_t count = Inner().ReadByte();
    if (count == 0)
    {
        throw std::ios_base::failure("Invalid RLE: count=0");
    }

    if (Inner().IsEOF())
    {
        throw std::ios_base::failure("Invalid RLE: missing byte");
    }

    m_runByte = Inner().ReadByte();
    m_leftInRun = count;
    return true;
}

uint8_t RleDecompressInputStream::ReadByte()
{
    if (!EnsureRunLoadedOrEof())
    {
        throw std::ios_base::failure("Decompress ReadByte past EOF");
    }

    --m_leftInRun;
    return m_runByte;
}

std::streamsize RleDecompressInputStream::ReadBlock(void* dstBuffer, std::streamsize size)
{
    if (size <= 0)
    {
        return 0;
    }

    auto out = reinterpret_cast<uint8_t*>(dstBuffer);
    std::streamsize produced = 0;

    while (produced < size)
    {
        if (!EnsureRunLoadedOrEof())
        {
            break;
        }

        const std::streamsize can = std::min<std::streamsize>(size - produced, m_leftInRun);

        std::fill_n(out + produced, can, m_runByte);

        produced += can;
        m_leftInRun = static_cast<uint8_t>(m_leftInRun - can);
    }

    return produced;
}
