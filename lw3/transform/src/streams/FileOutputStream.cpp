#include "streams/FileOutputStream.h"
#include <stdexcept>

FileOutputStream::FileOutputStream(const std::string& path) 
{
    m_out.exceptions(std::ios::badbit);
    m_out.open(path, std::ios::binary);
    if (!m_out.is_open()) 
    {
        throw std::ios_base::failure("Failed to open output file: " + path);
    }
}

void FileOutputStream::EnsureOpen() const 
{
    if (m_closed)
    {
        throw std::logic_error("Stream is closed");
    }
}

void FileOutputStream::WriteByte(uint8_t data) 
{
    EnsureOpen();
    char ch = static_cast<char>(data);
    m_out.put(ch);

    if (!m_out)
    {
        throw std::ios_base::failure("WriteByte failed");
    }
}

void FileOutputStream::WriteBlock(const void* srcData, std::streamsize size) 
{
    EnsureOpen();

    if (size <= 0)
    {
        return;
    }

    m_out.write(reinterpret_cast<const char*>(srcData), size);

    if (!m_out)
    {
        throw std::ios_base::failure("WriteBlock failed");
    }
}

void FileOutputStream::Close()
{
    if (m_closed)
    {
        return;
    }

    m_out.flush();
    m_out.close();
    m_closed = true;
}
