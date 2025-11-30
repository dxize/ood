#include "Slide.h"
#include "Primitives.h"
#include "StdOutCanvas.h"
#include <iostream>
#include <memory>

int main() 
{
    auto slide = std::make_shared<Slide>();

    auto base = std::make_shared<RectangleShape>(Rect(100, 200, 200, 150));
    base->GetFillStyle()->SetColor({ 210, 180, 140, 255 }); 
    base->GetLineStyle()->SetColor({ 0, 0, 0, 255 });
    base->GetLineStyle()->SetThickness(2.0);

    auto roof = std::make_shared<TriangleShape>(Rect(80, 100, 240, 100));   
    roof->GetFillStyle()->SetColor({ 139, 0, 0, 255 }); 
    roof->GetLineStyle()->SetColor({ 0, 0, 0, 255 });
    roof->GetLineStyle()->SetThickness(2.0);

    auto houseGroup = std::make_shared<GroupShape>();
    houseGroup->AddShape(base);
    houseGroup->AddShape(roof);

    auto sun = std::make_shared<EllipseShape>(Rect(400, 50, 100, 100));
    sun->GetFillStyle()->SetColor({ 255, 215, 0, 255 }); 
    sun->GetLineStyle()->SetEnabled(false);

    slide->AddShape(houseGroup);
    slide->AddShape(sun);

    StdoutCanvas canvas;
    slide->Draw(canvas);

    std::cout << "\n\n\n\n\n";

    auto houseLineStyle = houseGroup->GetLineStyle();
    if (auto color = houseLineStyle->GetColor())
    {
        std::cout << "House group line color is uniform: "
            << static_cast<int>(color->r) << ","
            << static_cast<int>(color->g) << ","
            << static_cast<int>(color->b) << std::endl;
    }

    houseLineStyle->SetThickness(5.0); 

    
    slide->Draw(canvas);

    return 0;
}