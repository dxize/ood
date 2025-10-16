#include "CommandProcessor.h"

void CommandProcessor::ProcessCommand(const std::vector<std::string>& args)
{
    if (args.empty()) return;

    const std::string& cmd = args[0];

    try
    {
        if (cmd == "AddShape") AddShape(args);
        else if (cmd == "DeleteShape") DeleteShape(args);
        else if (cmd == "MoveShape") MoveShape(args);
        else if (cmd == "MovePicture") MovePicture(args);
        else if (cmd == "ChangeColor") ChangeColor(args);
        else if (cmd == "ChangeShape") ChangeShape(args);
        else if (cmd == "DrawShape") DrawShape(args);
        else if (cmd == "DrawPicture") DrawPicture();
        else if (cmd == "List") ListShapes();
        else if (cmd == "Help") PrintHelp();
        else if (cmd == "Exit" || cmd == "exit") exit(0);
        else std::cout << "Unknown command: " << cmd << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void CommandProcessor::AddShape(const std::vector<std::string>& args)
{
    if (args.size() < 4)
    {
        throw std::invalid_argument("AddShape: not enough arguments");
    }
    std::string id = args[1];
    std::string color = args[2];
    std::string type = args[3];
    std::vector<std::string> params(args.begin() + 4, args.end());

    auto strategy = m_factory.CreateStrategy(type, params);
    auto shape = std::make_unique<shapes::Shape>(std::move(strategy), color, id);
    m_picture.AddShape(id, std::move(shape));
    std::cout << "Shape '" << id << "' added successfully.\n";
}

void CommandProcessor::DeleteShape(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        throw std::invalid_argument("DeleteShape: missing id");
    }
    m_picture.DeleteShape(args[1]);
    std::cout << "Shape '" << args[1] << "' deleted successfully.\n";
}

void CommandProcessor::MoveShape(const std::vector<std::string>& args)
{
    if (args.size() < 4)
    {
        throw std::invalid_argument("MoveShape: missing parameters");
    }
    double dx = std::stod(args[2]);
    double dy = std::stod(args[3]);
    m_picture.MoveShape(args[1], dx, dy);
    std::cout << "Shape '" << args[1] << "' moved by (" << dx << ", " << dy << ")\n";
}

void CommandProcessor::MovePicture(const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        throw std::invalid_argument("MovePicture: missing parameters");
    }
    double dx = std::stod(args[1]);
    double dy = std::stod(args[2]);
    m_picture.MoveAllShapes(dx, dy);
    std::cout << "All shapes moved by (" << dx << ", " << dy << ")\n";
}

void CommandProcessor::ChangeColor(const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        throw std::invalid_argument("ChangeColor: missing parameters");
    }
    m_picture.ChangeShapeColor(args[1], args[2]);
    std::cout << "Shape '" << args[1] << "' color changed to " << args[2] << "\n";
}

void CommandProcessor::ChangeShape(const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        throw std::invalid_argument("ChangeShape: missing parameters");
    }
    std::string id = args[1];
    std::string type = args[2];
    std::vector<std::string> params(args.begin() + 3, args.end());

    auto newStrategy = m_factory.CreateStrategy(type, params);
    m_picture.ChangeShape(id, std::move(newStrategy));
    std::cout << "Shape '" << id << "' type changed to " << type << "\n";
}

void CommandProcessor::DrawShape(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        throw std::invalid_argument("DrawShape: missing id");
    }
    m_picture.DrawShape(m_canvas, args[1]);
    std::cout << "Shape '" << args[1] << "' drawn successfully.\n";
}

void CommandProcessor::DrawPicture()
{
    m_picture.DrawAllShapes(m_canvas);
    std::cout << "All shapes drawn successfully.\n";
}

void CommandProcessor::ListShapes()
{
    std::cout << m_picture.ListShapes();
}

void CommandProcessor::PrintHelp()
{
    std::cout << "Available commands:\n"
        << "  AddShape <id> <color> <type> <params...>\n"
        << "  DeleteShape <id>\n"
        << "  MoveShape <id> <dx> <dy>\n"
        << "  MovePicture <dx> <dy>\n"
        << "  ChangeColor <id> <color>\n"
        << "  ChangeShape <id> <type> <params...>\n"
        << "  DrawShape <id>\n"
        << "  DrawPicture\n"
        << "  List\n"
        << "  Help\n"
        << "  Exit\n";
}
