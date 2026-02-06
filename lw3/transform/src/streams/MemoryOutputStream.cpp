#include "streams/MemoryOutputStream.h"
#include <cstring>
#include <stdexcept>

void MemoryOutputStream::EnsureOpen() const {
    if (m_closed) throw std::logic_error("Stream is closed");
}

void MemoryOutputStream::WriteByte(uint8_t data) {
    EnsureOpen();
    m_data.push_back(data);
}

void MemoryOutputStream::WriteBlock(const void* srcData, std::streamsize size) {
    EnsureOpen();
    if (size <= 0) return;
    auto p = reinterpret_cast<const uint8_t*>(srcData);
    m_data.insert(m_data.end(), p, p + size);
}

void MemoryOutputStream::Close() {
    m_closed = true;
}

const std::vector<uint8_t>& MemoryOutputStream::GetData() const {
    return m_data;
}
