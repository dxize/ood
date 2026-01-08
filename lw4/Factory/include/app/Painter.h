#pragma once

namespace canvas { class ICanvas; }
namespace domain { class PictureDraft; }

namespace app
{
    class Painter
    {
    public:
        void DrawPicture(const domain::PictureDraft& draft, canvas::ICanvas& canvas) const;
    };
}
