#pragma once
#include "streams/IInputDataStream.h"
#include <memory>
#include <stdexcept>

class InputStreamDecorator : public IInputDataStream 
{
public:
    explicit InputStreamDecorator(std::unique_ptr<IInputDataStream> inner)
        : m_inner(std::move(inner)) 
    {
        if (!m_inner)
        {
            throw std::invalid_argument("inner input stream is null");
        }
    }

    bool IsEOF() const override 
    { 
        return m_inner->IsEOF(); 
    }

    uint8_t ReadByte() override 
    { 
        return m_inner->ReadByte(); 
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override 
    {
        return m_inner->ReadBlock(dstBuffer, size);
    }

protected:
    IInputDataStream& Inner() 
    { 
        return *m_inner; 
    }

    const IInputDataStream& Inner() const 
    {
        return *m_inner; 
    }

private:
    std::unique_ptr<IInputDataStream> m_inner;
};
