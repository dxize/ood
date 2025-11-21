#pragma once
#include "Ishape.h"
#include "SimpleStyles.h"
#include "Icanvas.h"

class Shape : public IShape
{
public:
    Shape(const Rect& f)
        : frame(f)
        , lineStyle(std::make_shared<SimpleLineStyle>())
        , fillStyle(std::make_shared<SimpleFillStyle>())
    {
    }

    Rect GetFrame() const override 
    {
        return frame; 
    }

    void SetFrame(const Rect& rect) override
    { 
        frame = rect; 
    }

    std::shared_ptr<ILineStyle> GetLineStyle() override 
    { 
        return lineStyle;
    }

    std::shared_ptr<IFillStyle> GetFillStyle() override
    { 
        return fillStyle;
    }

    void Draw(ICanvas& canvas) const override
    {
        auto fillEn = fillStyle->GetEnabled();
        if (fillEn && *fillEn)
        {
            if (auto color = fillStyle->GetColor())
            {
                canvas.SetFillColor(*color);
                DoFill(canvas);
            }
        }

        auto lineEn = lineStyle->GetEnabled();
        if (lineEn && *lineEn)
        {
            if (auto color = lineStyle->GetColor())
                canvas.SetLineColor(*color);
            if (auto thick = lineStyle->GetThickness())
                canvas.SetLineWidth(*thick);

            DoDrawOutline(canvas);
        }
    }

protected:
    virtual void DoFill(ICanvas& canvas) const = 0;
    virtual void DoDrawOutline(ICanvas& canvas) const = 0;
    Rect frame;
    std::shared_ptr<SimpleLineStyle> lineStyle;
    std::shared_ptr<SimpleFillStyle> fillStyle;
};
