#include "Document.h"
#include "Commands.h"
#include "Utils.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

CDocument::CDocument(Path workingDir)
    : m_workDir(std::move(workingDir))
    , m_storage(m_workDir)
    , m_history(10)
{
    fs::create_directories(m_workDir);
    fs::create_directories(m_workDir / "images");
}

std::shared_ptr<IParagraph> CDocument::InsertParagraph(const std::string& text,
    std::optional<size_t> position)
{
    auto idx = position.value_or(m_items.size());
    if (idx > m_items.size())
    {
        throw std::runtime_error("Invalid position for InsertParagraph");
    }

    auto para = std::make_shared<CParagraph>(text, &m_history);
    m_history.AddAndExecute(std::make_shared<CInsertParagraphCommand>(*this, para, idx));
    return para;
}

std::shared_ptr<IImage> CDocument::InsertImage(const Path& path, int width, int height,
    std::optional<size_t> position)
{
    if (width < 1 || width > 10000 || height < 1 || height > 10000)
        throw std::runtime_error("Invalid image size (1..10000)");

    auto idx = position.value_or(m_items.size());
    if (idx > m_items.size())
        throw std::runtime_error("Invalid position for InsertImage");

    Path rel = m_storage.ImportImage(path);
    auto img = std::make_shared<CImage>(rel, width, height, &m_history);

    m_history.AddAndExecute(std::make_shared<CInsertImageCommand>(*this, img, idx, m_storage));
    return img;
}

size_t CDocument::GetItemsCount() const
{
    return m_items.size();
}

CConstDocumentItem CDocument::GetItem(size_t index) const
{
    if (index >= m_items.size())
    {
        throw std::runtime_error("Invalid index");
    }

    return CConstDocumentItem(m_items[index].GetParagraph(), m_items[index].GetImage());
}

CDocumentItem CDocument::GetItem(size_t index)
{
    if (index >= m_items.size())
    {
        throw std::runtime_error("Invalid index");
    }

    return m_items[index];
}

void CDocument::DeleteItem(size_t index)
{
    if (index >= m_items.size())
        throw std::runtime_error("Invalid position for DeleteItem");

    m_history.AddAndExecute(std::make_shared<CDeleteItemCommand>(*this, index, m_storage));
}

std::string CDocument::GetTitle() const
{
    return m_title;
}

void CDocument::SetTitle(const std::string& title)
{
    m_history.AddAndExecute(std::make_shared<CSetTitleCommand>(*this, title));
}

bool CDocument::CanUndo() const 
{ 
    return m_history.CanUndo(); 
}
void CDocument::Undo() 
{
    m_history.Undo(); 
}

bool CDocument::CanRedo() const 
{
    return m_history.CanRedo();
}
void CDocument::Redo() 
{
    m_history.Redo();
}

void CDocument::Save(const Path& htmlPath) const
{
    auto outDir = htmlPath.parent_path();
    if (outDir.empty())
    {
        outDir = fs::current_path();
    }

    fs::create_directories(outDir / "images");

    std::ostringstream html;
    html << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n";
    html << "<title>" << HtmlEscape(m_title) << "</title>\n";

    html << "<style>\n"
        "body{margin:0;font-family:Arial, sans-serif;background:#f5f5f5;}\n"
        ".container{max-width:900px;margin:0 auto;padding:30px 15px;}\n"
        "h1{text-align:center;margin-bottom:25px;}\n"
        "p{font-size:16px;line-height:1.5;text-align:center;margin:15px 0;}\n"
        "img{display:block;margin:20px auto;max-width:100%;height:auto;border-radius:8px;}\n"
        "</style>\n";

    html << "</head>\n<body>\n";

    html << "<div class=\"container\">\n";
    html << "<h1>" << HtmlEscape(m_title) << "</h1>\n";

    for (const auto& it : m_items)
    {
        if (auto p = it.GetParagraph())
        {
            html << "<p>" << HtmlEscape(p->GetText()) << "</p>\n";
        }
        else if (auto img = it.GetImage()) 
        {
            auto rel = img->GetPath();
            auto srcAbs = m_storage.AbsPath(rel);
            auto dstAbs = outDir / rel;

            fs::create_directories(dstAbs.parent_path());
            fs::copy_file(srcAbs, dstAbs, fs::copy_options::overwrite_existing);

            html << "<img src=\"" << HtmlEscape(rel.generic_string()) << "\" "
                << "width=\"" << img->GetWidth() << "\" "
                << "height=\"" << img->GetHeight() << "\" />\n";
        }
    }

    html << "</div>\n"; 
    html << "</body>\n</html>\n";

    std::ofstream out(htmlPath);
    if (!out)
        throw std::runtime_error("Cannot open file for save: " + htmlPath.string());

    out << html.str();
}

// приватные операции
void CDocument::InsertParagraphRaw(std::shared_ptr<CParagraph> para, size_t idx)
{
    m_items.insert(m_items.begin() + idx, CDocumentItem(para, nullptr));
}

void CDocument::InsertImageRaw(std::shared_ptr<CImage> img, size_t idx)
{
    m_items.insert(m_items.begin() + idx, CDocumentItem(nullptr, img));
}

void CDocument::InsertItemRaw(const CDocumentItem& item, size_t idx)
{
    m_items.insert(m_items.begin() + idx, item);
}

CDocumentItem CDocument::DeleteItemRaw(size_t idx)
{
    auto deleted = m_items.at(idx);
    m_items.erase(m_items.begin() + idx);
    return deleted;
}

void CDocument::SetTitleRaw(const std::string& t)
{
    m_title = t;
}
