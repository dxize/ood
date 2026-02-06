#include "EditorApp.h"
#include "Utils.h"
#include <sstream>
#include <iostream>

CEditorApp::CEditorApp(IDocument& document)
    : m_document(document)
{
}

void CEditorApp::Run(std::istream& input, std::ostream& output)
{
    PrintHelp(output);

    std::string line;
    while (std::getline(input, line))
    {
        if (line.empty())
            continue;

        try
        {
            ExecuteLine(line, output);
        }
        catch (const std::exception& e)
        {
            output << "Error: " << e.what() << "\n";
        }
    }
}

void CEditorApp::PrintHelp(std::ostream& output) const
{
    output
        << "Commands:\n"
        << "  InsertParagraph <pos>|end <text>\n"
        << "  InsertImage <pos>|end <width> <height> <path>\n"
        << "  SetTitle <title>\n"
        << "  List\n"
        << "  ReplaceText <pos> <text>\n"
        << "  ResizeImage <pos> <width> <height>\n"
        << "  DeleteItem <pos>\n"
        << "  Undo\n"
        << "  Redo\n"
        << "  Save <path>\n"
        << "  Help\n";
}

void CEditorApp::PrintList(std::ostream& output) const
{
    output << "Title: " << m_document.GetTitle() << "\n";

    for (size_t i = 0; i < m_document.GetItemsCount(); ++i)
    {
        size_t idxHuman = i + 1;
        auto item = m_document.GetItem(i);

        if (auto p = item.GetParagraph())
        {
            output << idxHuman << ". Paragraph: " << p->GetText() << "\n";
        }
        else if (auto img = item.GetImage())
        {
            output << idxHuman << ". Image: "
                << img->GetWidth() << " " << img->GetHeight() << " "
                << img->GetPath().generic_string() << "\n";
        }
    }
}


//как будто бы метод страшный и его надо разделить
void CEditorApp::ExecuteLine(const std::string& line, std::ostream& output)
{
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    if (cmd == "Help")
    {
        PrintHelp(output);
    }
    else if (cmd == "List")
    {
        PrintList(output);
    }
    else if (cmd == "SetTitle")
    {
        std::string title;
        std::getline(iss, title);
        m_document.SetTitle(TrimLeft(title));
    }
    else if (cmd == "InsertParagraph")
    {
        std::string posTok;
        iss >> posTok;

        std::string text;
        std::getline(iss, text);
        text = TrimLeft(text);

        if (posTok == "end")
        {
            m_document.InsertParagraph(text);
        }
        else
        {
            size_t pos1 = ParseIndex1Based(posTok);
            size_t idx0 = pos1 - 1;

            if (idx0 > m_document.GetItemsCount())
                throw std::runtime_error("Invalid position");

            m_document.InsertParagraph(text, idx0);
        }
    }
    else if (cmd == "InsertImage")
    {
        std::string posTok;
        int w, h;
        std::string pathStr;

        iss >> posTok >> w >> h >> pathStr;
        if (pathStr.empty())
            throw std::runtime_error("Invalid InsertImage args");

        if (posTok == "end")
        {
            m_document.InsertImage(pathStr, w, h);
        }
        else
        {
            size_t pos1 = ParseIndex1Based(posTok);
            size_t idx0 = pos1 - 1;

            if (idx0 > m_document.GetItemsCount())
                throw std::runtime_error("Invalid position");

            m_document.InsertImage(pathStr, w, h, idx0);
        }
    }
    else if (cmd == "ReplaceText")
    {
        std::string posTok;
        iss >> posTok;

        std::string text;
        std::getline(iss, text);
        text = TrimLeft(text);

        size_t pos1 = ParseIndex1Based(posTok);
        size_t idx0 = pos1 - 1;

        if (idx0 >= m_document.GetItemsCount())
            throw std::runtime_error("Invalid position");

        auto item = m_document.GetItem(idx0);
        if (!item.GetParagraph())
            throw std::runtime_error("Item is not a paragraph");

        m_document.GetItem(idx0).GetParagraph()->SetText(text);
    }
    else if (cmd == "ResizeImage")
    {
        std::string posTok;
        int w, h;
        iss >> posTok >> w >> h;

        size_t pos1 = ParseIndex1Based(posTok);
        size_t idx0 = pos1 - 1;

        if (idx0 >= m_document.GetItemsCount())
            throw std::runtime_error("Invalid position");

        auto item = m_document.GetItem(idx0);
        if (!item.GetImage())
            throw std::runtime_error("Item is not an image");

        m_document.GetItem(idx0).GetImage()->Resize(w, h);
    }
    else if (cmd == "DeleteItem")
    {
        std::string posTok;
        iss >> posTok;

        size_t pos1 = ParseIndex1Based(posTok);
        size_t idx0 = pos1 - 1;

        m_document.DeleteItem(idx0);
    }
    else if (cmd == "Undo")
    {
        m_document.Undo();
    }
    else if (cmd == "Redo")
    {
        m_document.Redo();
    }
    else if (cmd == "Save")
    {
        std::string pathStr;
        iss >> pathStr;

        if (pathStr.empty())
            throw std::runtime_error("Save path is empty");

        m_document.Save(pathStr);
        output << "Saved to: " << pathStr << "\n";
    }
    else
    {
        output << "Unknown command. Type Help.\n";
    }
}
