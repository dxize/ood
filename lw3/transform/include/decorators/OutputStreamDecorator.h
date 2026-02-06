#pragma once
#include "streams/IOutputDataStream.h"
#include <memory>
#include <stdexcept>

class OutputStreamDecorator : public IOutputDataStream 
{
public:
    explicit OutputStreamDecorator(std::unique_ptr<IOutputDataStream> inner)
        : m_inner(std::move(inner)) 
    {
        if (!m_inner)
        {
            throw std::invalid_argument("inner output stream is null");
        }
    }

    void WriteByte(uint8_t data) override 
    { 
        m_inner->WriteByte(data); 
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        m_inner->WriteBlock(srcData, size);
    }

    void Close() override
    {
        m_inner->Close();
    }

protected:
    IOutputDataStream& Inner() 
    { 
        return *m_inner;
    }

private:
    std::unique_ptr<IOutputDataStream> m_inner;
};
