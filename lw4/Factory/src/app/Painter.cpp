#include "../../include/app/Painter.h"
#include "../../include/canvas/ICanvas.h"
#include "../../include/domain/PictureDraft.h"
#include "../../include/domain/Shape.h"

namespace app
{
    void Painter::DrawPicture(const domain::PictureDraft& draft, canvas::ICanvas& canvas) const
    {
        for (std::size_t i = 0; i < draft.GetShapeCount(); ++i)
        {
            draft.GetShape(i).Draw(canvas);
        }
    }
} 
