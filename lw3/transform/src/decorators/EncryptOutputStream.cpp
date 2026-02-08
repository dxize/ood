#include "decorators/EncryptOutputStream.h"
#include "crypto/SubstitutionTable.h"
#include <vector>

EncryptOutputStream::EncryptOutputStream(std::unique_ptr<IOutputDataStream> inner, uint32_t key)
    : OutputStreamDecorator(std::move(inner))
    , m_table(MakeSubstTable(key)) 
{
}

void EncryptOutputStream::WriteByte(uint8_t data) 
{
    Inner().WriteByte(m_table[data]);
}

void EncryptOutputStream::WriteBlock(const void* srcData, std::streamsize size) 
{
    if (size <= 0)
    {
        return;
    }

    auto p = reinterpret_cast<const uint8_t*>(srcData);
    std::vector<uint8_t> tmp(static_cast<std::size_t>(size));

    for (std::size_t i = 0; i < tmp.size(); ++i)
    {
        tmp[i] = m_table[p[i]];
    }

    Inner().WriteBlock(tmp.data(), static_cast<std::streamsize>(tmp.size()));
}
