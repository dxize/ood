#pragma once

#include <istream>

#include "../domain/PictureDraft.h"

namespace app
{
    class IDesigner
    {
    public:
        virtual ~IDesigner() = default;
        virtual domain::PictureDraft CreateDraft(std::istream& input) const = 0;
    };
} 
