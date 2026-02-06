#pragma once
#include <cstdint>

enum class StepKind 
{ 
    Encrypt, 
    Decrypt, 
    Compress, 
    Decompress 
};

struct Step 
{
    StepKind kind;
    uint32_t key = 0;
};
