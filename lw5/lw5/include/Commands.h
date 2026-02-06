#pragma once
#include "CommandHistory.h"
#include "DocumentItem.h"
#include "Paragraph.h"
#include "Image.h"
#include "ImageStorage.h"
#include <string>

class CDocument;

// вставка параграфа
class CInsertParagraphCommand final : public ICommand
{
public:
    CInsertParagraphCommand(CDocument& doc, std::shared_ptr<CParagraph> para, size_t index);
    void Execute() override;
    void Unexecute() override;

private:
    CDocument& m_doc;
    std::shared_ptr<CParagraph> m_para;
    size_t m_index = 0;
};

// вставка изображения
class CInsertImageCommand final : public ICommand
{
public:
    CInsertImageCommand(CDocument& doc, std::shared_ptr<CImage> img, size_t index, CImageStorage& storage);

    void Execute() override;
    void Unexecute() override;

    void OnDrop(bool isExecuted) override;

private:
    CDocument& m_doc;
    std::shared_ptr<CImage> m_img;
    size_t m_index = 0;
    CImageStorage& m_storage;
};

// удаление элемента
class CDeleteItemCommand final : public ICommand
{
public:
    CDeleteItemCommand(CDocument& doc, size_t index, CImageStorage& storage);

    void Execute() override;
    void Unexecute() override;

    void OnDrop(bool isExecuted) override;

private:
    CDocument& m_doc;
    size_t m_index = 0;

    CDocumentItem m_deleted;
    bool m_hasDeleted = false;

    bool m_deletedWasImage = false;
    Path m_deletedImagePath;
    CImageStorage& m_storage;
};

// смена заголовка
class CSetTitleCommand final : public ICommand
{
public:
    CSetTitleCommand(CDocument& doc, std::string newTitle);

    void Execute() override;
    void Unexecute() override;

    bool MergeWith(const std::shared_ptr<ICommand>& other) override;

private:
    CDocument& m_doc;
    std::string m_old;
    std::string m_new;
    bool m_firstExecute = true;
};

// заменить текст
class CReplaceTextCommand final : public ICommand
{
public:
    CReplaceTextCommand(std::shared_ptr<CParagraph> para, std::string newText);

    void Execute() override;
    void Unexecute() override;
    bool MergeWith(const std::shared_ptr<ICommand>& other) override;

private:
    std::shared_ptr<CParagraph> m_para;
    std::string m_old;
    std::string m_new;
    bool m_firstExecute = true;
};

// изменить размер
class CResizeImageCommand final : public ICommand
{
public:
    CResizeImageCommand(std::shared_ptr<CImage> img, int w, int h);

    void Execute() override;
    void Unexecute() override;
    bool MergeWith(const std::shared_ptr<ICommand>& other) override;

private:
    std::shared_ptr<CImage> m_img;
    int m_oldW = 1, m_oldH = 1;
    int m_newW = 1, m_newH = 1;
    bool m_firstExecute = true;
};
