#include "CCommandParser.h"
#include "Commands.h"

CCommandParser::CCommandParser() noexcept
{
    m_factory["AddShape"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CAddShapeCommand>();
    };
    m_factory["MoveShape"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CMoveShapeCommand>();
    };
    m_factory["MovePicture"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CMovePictureCommand>();
    };
    m_factory["DeleteShape"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CDeleteShapeCommand>();
    };
    m_factory["ChangeColor"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CChangeColorCommand>();
    };
    m_factory["ChangeShape"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CChangeShapeCommand>();
    };
    m_factory["DrawShape"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CDrawShapeCommand>();
    };
    m_factory["DrawPicture"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CDrawPictureCommand>();
    };
    m_factory["List"] = []() -> std::unique_ptr<ICommand> 
    {
        return std::make_unique<CListCommand>();
    };
}

std::unique_ptr<ICommand> CCommandParser::Parse(const std::string& input)
{
    auto it = m_factory.find(input);
    if (it != m_factory.end()) {
        return it->second();
    }
    return nullptr;
}