#include "streams/FileInputStream.h"
#include <stdexcept>

FileInputStream::FileInputStream(const std::string& path) 
{
    m_in.exceptions(std::ios::badbit);
    m_in.open(path, std::ios::binary);
    if (!m_in.is_open()) 
    {
        throw std::ios_base::failure("Failed to open input file: " + path); 
    }
}

bool FileInputStream::IsEOF() const 
{
    int c = m_in.peek();
    return (c == EOF);
}

uint8_t FileInputStream::ReadByte() 
{
    char ch;
    if (!m_in.get(ch)) 
    {
        throw std::ios_base::failure("ReadByte failed (EOF or error)");
    }
    return static_cast<uint8_t>(static_cast<unsigned char>(ch));
}

std::streamsize FileInputStream::ReadBlock(void* dstBuffer, std::streamsize size) 
{
    if (size <= 0)
    {
        return 0;
    }

    if (IsEOF())
    {
        return 0;
    }

    m_in.read(reinterpret_cast<char*>(dstBuffer), size);
    std::streamsize got = m_in.gcount();

    if (got == 0 && !m_in.eof() && m_in.fail()) 
    {
        throw std::ios_base::failure("ReadBlock failed");
    }

    return got;
}
