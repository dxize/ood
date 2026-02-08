#include "streams/MemoryInputStream.h"
#include <cstring>
#include <stdexcept>
#include <algorithm>

MemoryInputStream::MemoryInputStream(std::vector<uint8_t> data)
    : m_data(std::move(data)) 
{
}

bool MemoryInputStream::IsEOF() const 
{
    return m_pos >= m_data.size();
}

uint8_t MemoryInputStream::ReadByte()
{
    if (IsEOF())
    {
        throw std::ios_base::failure("ReadByte past EOF");
    }

    return m_data[m_pos++];
}

std::streamsize MemoryInputStream::ReadBlock(void* dstBuffer, std::streamsize size) 
{
    if (size <= 0)
    {
        return 0;
    }

    if (IsEOF())
    {
        return 0;
    }

    std::size_t want = static_cast<std::size_t>(size);
    std::size_t can = std::min(want, m_data.size() - m_pos);
    std::memcpy(dstBuffer, m_data.data() + m_pos, can);
    m_pos += can;

    return static_cast<std::streamsize>(can);
}
