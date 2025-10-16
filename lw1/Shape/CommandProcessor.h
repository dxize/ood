#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Shapes/CPicture.h"
#include "Shapes/DrawShapes/CCanvas.h"
#include "Shapes/ShapeFactory.h"

class CommandProcessor
{
public:
    CommandProcessor(shapes::CPicture& picture, CCanvas& canvas)
        : m_picture(picture), m_canvas(canvas) {}

    void ProcessCommand(const std::vector<std::string>& args);

private:
    shapes::CPicture& m_picture;
    CCanvas& m_canvas;
    ShapeFactory m_factory;

    void AddShape(const std::vector<std::string>& args);
    void DeleteShape(const std::vector<std::string>& args);
    void MoveShape(const std::vector<std::string>& args);
    void MovePicture(const std::vector<std::string>& args);
    void ChangeColor(const std::vector<std::string>& args);
    void ChangeShape(const std::vector<std::string>& args);
    void DrawShape(const std::vector<std::string>& args);
    void DrawPicture();
    void ListShapes();
    void PrintHelp();
};
