#include "decorators/RleCompressOutputStream.h"
#include <vector>

RleCompressOutputStream::RleCompressOutputStream(std::unique_ptr<IOutputDataStream> inner)
    : OutputStreamDecorator(std::move(inner)) 
{
}

void RleCompressOutputStream::StartRun(uint8_t data)
{
    m_hasRun = true;
    m_runByte = data;
    m_runCount = 1;
}

void RleCompressOutputStream::WriteByte(uint8_t data)
{
    if (!m_hasRun)
    {
        StartRun(data);
        return;
    }

    if (data == m_runByte && m_runCount < 255)
    {
        ++m_runCount;
        return;
    }

    FlushRun();
    StartRun(data);
}


void RleCompressOutputStream::WriteBlock(const void* srcData, std::streamsize size) 
{
    if (size <= 0)
    {
        return;
    }

    auto p = reinterpret_cast<const uint8_t*>(srcData);

    for (std::streamsize i = 0; i < size; ++i) 
    {
        WriteByte(p[static_cast<std::size_t>(i)]);
    }
}

void RleCompressOutputStream::FlushRun()    
{
    if (!m_hasRun)
    {
        return;
    }

    Inner().WriteByte(m_runCount);
    Inner().WriteByte(m_runByte);

    m_hasRun = false;
    m_runCount = 0;
    m_runByte = 0;
}

void RleCompressOutputStream::Close()
{
    if (m_hasRun)
    {
        FlushRun();
    }

    OutputStreamDecorator::Close();
}
