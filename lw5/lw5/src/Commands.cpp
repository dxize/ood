#include "Commands.h"
#include "Document.h"

CInsertParagraphCommand::CInsertParagraphCommand(CDocument& doc, std::shared_ptr<CParagraph> para, size_t index)
    : m_doc(doc), m_para(std::move(para)), m_index(index)
{
}

void CInsertParagraphCommand::Execute()
{
    m_doc.InsertParagraphRaw(m_para, m_index);
}

void CInsertParagraphCommand::Unexecute()
{
    m_doc.DeleteItemRaw(m_index);
}

CInsertImageCommand::CInsertImageCommand(CDocument& doc, std::shared_ptr<CImage> img, size_t index, CImageStorage& storage)
    : m_doc(doc), m_img(std::move(img)), m_index(index), m_storage(storage)
{
}

void CInsertImageCommand::Execute()
{
    m_doc.InsertImageRaw(m_img, m_index);
    m_storage.UnmarkDeletion(m_img->GetPath());
}

void CInsertImageCommand::Unexecute()
{
    m_doc.DeleteItemRaw(m_index);
    m_storage.MarkForDeletion(m_img->GetPath());
}

void CInsertImageCommand::OnDrop(bool isExecuted)
{
    if (!isExecuted)
    {
        if (m_storage.IsMarked(m_img->GetPath()))
            m_storage.PhysicalDelete(m_img->GetPath());
    }
}

CDeleteItemCommand::CDeleteItemCommand(CDocument& doc, size_t index, CImageStorage& storage)
    : m_doc(doc), m_index(index), m_storage(storage)
{
}

void CDeleteItemCommand::Execute()
{
    if (!m_hasDeleted)
    {
        m_deleted = m_doc.DeleteItemRaw(m_index);
        m_hasDeleted = true;

        if (auto img = m_deleted.GetImage())
        {
            m_deletedWasImage = true;
            m_deletedImagePath = img->GetPath();
            m_storage.MarkForDeletion(m_deletedImagePath);
        }
    }
    else
    {
        m_doc.DeleteItemRaw(m_index);
        if (m_deletedWasImage)
            m_storage.MarkForDeletion(m_deletedImagePath);
    }
}

void CDeleteItemCommand::Unexecute()
{
    m_doc.InsertItemRaw(m_deleted, m_index);

    if (m_deletedWasImage)
        m_storage.UnmarkDeletion(m_deletedImagePath);
}

void CDeleteItemCommand::OnDrop(bool isExecuted)
{
    if (isExecuted && m_deletedWasImage)
        m_storage.PhysicalDelete(m_deletedImagePath);
}

CSetTitleCommand::CSetTitleCommand(CDocument& doc, std::string newTitle)
    : m_doc(doc), m_new(std::move(newTitle))
{
}

void CSetTitleCommand::Execute()
{
    if (m_firstExecute)
    {
        m_old = m_doc.GetTitle();
        m_firstExecute = false;
    }
    m_doc.SetTitleRaw(m_new);
}

void CSetTitleCommand::Unexecute()
{
    m_doc.SetTitleRaw(m_old);
}

bool CSetTitleCommand::MergeWith(const std::shared_ptr<ICommand>& other)
{
    auto o = std::dynamic_pointer_cast<CSetTitleCommand>(other);
    if (!o)
    {
        return false;
    }
    m_new = o->m_new;
    return true;
}

CReplaceTextCommand::CReplaceTextCommand(std::shared_ptr<CParagraph> para, std::string newText)
    : m_para(std::move(para)), m_new(std::move(newText))
{
}

void CReplaceTextCommand::Execute()
{
    if (m_firstExecute)
    {
        m_old = m_para->GetText();
        m_firstExecute = false;
    }
    m_para->SetTextImpl(m_new);
}

void CReplaceTextCommand::Unexecute()
{
    m_para->SetTextImpl(m_old);
}

bool CReplaceTextCommand::MergeWith(const std::shared_ptr<ICommand>& other)
{
    auto o = std::dynamic_pointer_cast<CReplaceTextCommand>(other);
    if (!o)
    {
        return false;
    }
    if (o->m_para.get() != m_para.get())
    {
        return false;
    }
    m_new = o->m_new;
    return true;
}

CResizeImageCommand::CResizeImageCommand(std::shared_ptr<CImage> img, int w, int h)
    : m_img(std::move(img)), m_newW(w), m_newH(h)
{
}

void CResizeImageCommand::Execute()
{
    if (m_firstExecute)
    {
        m_oldW = m_img->GetWidth();
        m_oldH = m_img->GetHeight();
        m_firstExecute = false;
    }
    m_img->ResizeImpl(m_newW, m_newH);
}

void CResizeImageCommand::Unexecute()
{
    m_img->ResizeImpl(m_oldW, m_oldH);
}

bool CResizeImageCommand::MergeWith(const std::shared_ptr<ICommand>& other)
{
    auto o = std::dynamic_pointer_cast<CResizeImageCommand>(other);
    if (!o)
    {
        return false;
    }
    if (o->m_img.get() != m_img.get())
    {
        return false;
    }
    m_newW = o->m_newW;
    m_newH = o->m_newH;
    return true;
}
