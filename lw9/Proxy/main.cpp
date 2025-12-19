#include "image.h"
#include "drawer.h"
#include <iostream>

int main() 
{
    Image img1 = LoadImage(
        " CCCC             \n"
        "CC  CC   ##    ## \n"
        "CC      ####  ####\n"
        "CC  CC   ##    ## \n"
        " CCCC             \n");
    Print(img1, std::cout);

    Image img({ 30,20 }, '.');
    DrawLine(img, { 3,2 }, { 26,5 }, '#');
    DrawLine(img, { 26,5 }, { 21,18 }, '#');
    DrawLine(img, { 21,18 }, { 3,2 }, '#');
    Print(img, std::cout);
    Image img2({ 30,20 }, '.');
    std::cout << "Tile instances = " << Tile::GetInstanceCount() << "\n";
}
