#pragma once
#include "Interfaces.h"
#include <memory>

class CConstDocumentItem
{
public:
    CConstDocumentItem() = default;

    CConstDocumentItem(std::shared_ptr<const IParagraph> p,
        std::shared_ptr<const IImage> i)
        : m_paragraph(std::move(p))
        , m_image(std::move(i))
    {
    }

    std::shared_ptr<const IImage> GetImage() const 
    {
        return m_image; 
    }
    std::shared_ptr<const IParagraph> GetParagraph() const
    {
        return m_paragraph; 
    }

protected:
    std::shared_ptr<const IParagraph> m_paragraph;
    std::shared_ptr<const IImage> m_image;
};

class CDocumentItem : public CConstDocumentItem
{
public:
    CDocumentItem() = default;

    CDocumentItem(std::shared_ptr<IParagraph> p,
        std::shared_ptr<IImage> i)
        : CConstDocumentItem(std::move(p), std::move(i)) 
    {
    }

    //возвращаем const-версии в область видимости
    using CConstDocumentItem::GetImage;
    using CConstDocumentItem::GetParagraph;

    //on-const доступ (можно менять)
    std::shared_ptr<IImage> GetImage()
    {
        return std::const_pointer_cast<IImage>(m_image);
    }

    std::shared_ptr<IParagraph> GetParagraph()
    {
        return std::const_pointer_cast<IParagraph>(m_paragraph);
    }
};
