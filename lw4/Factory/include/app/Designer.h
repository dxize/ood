#pragma once

#include "IDesigner.h"

namespace factory { class IShapeFactory; }

namespace app
{
    class Designer final : public IDesigner
    {
    public:
        explicit Designer(const factory::IShapeFactory& factory);

        domain::PictureDraft CreateDraft(std::istream& input) const override;

    private:
        const factory::IShapeFactory& m_factory;
    };
} 
