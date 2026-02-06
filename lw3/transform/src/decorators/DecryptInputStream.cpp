#include "decorators/DecryptInputStream.h"
#include "crypto/SubstitutionTable.h"
#include <vector>

DecryptInputStream::DecryptInputStream(std::unique_ptr<IInputDataStream> inner, uint32_t key)
    : InputStreamDecorator(std::move(inner)) {
    auto enc = MakeSubstTable(key);
    m_inv = InvertTable(enc);
}

uint8_t DecryptInputStream::ReadByte() {
    uint8_t b = Inner().ReadByte();
    return m_inv[b];
}

std::streamsize DecryptInputStream::ReadBlock(void* dstBuffer, std::streamsize size) {
    if (size <= 0) return 0;
    std::vector<uint8_t> tmp(static_cast<std::size_t>(size));
    std::streamsize got = Inner().ReadBlock(tmp.data(), size);

    auto out = reinterpret_cast<uint8_t*>(dstBuffer);
    for (std::streamsize i = 0; i < got; ++i) {
        out[i] = m_inv[tmp[static_cast<std::size_t>(i)]];
    }
    return got;
}
