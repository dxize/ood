#pragma once
#include "IDocument.h"
#include "CommandHistory.h"
#include "ImageStorage.h"
#include "Paragraph.h"
#include "Image.h"
#include <vector>

class CInsertParagraphCommand;
class CInsertImageCommand;
class CDeleteItemCommand;
class CSetTitleCommand;

class CDocument final : public IDocument
{
public:
    explicit CDocument(Path workingDir);

    std::shared_ptr<IParagraph> InsertParagraph(const std::string& text,
        std::optional<size_t> position = std::nullopt) override;

    std::shared_ptr<IImage> InsertImage(const Path& path, int width, int height,
        std::optional<size_t> position = std::nullopt) override;

    size_t GetItemsCount() const override;

    CConstDocumentItem GetItem(size_t index) const override;
    CDocumentItem GetItem(size_t index) override;

    void DeleteItem(size_t index) override;

    std::string GetTitle() const override;
    void SetTitle(const std::string& title) override;

    bool CanUndo() const override;
    void Undo() override;

    bool CanRedo() const override;
    void Redo() override;

    void Save(const Path& path) const override;

private:
    friend class CInsertParagraphCommand;
    friend class CInsertImageCommand;
    friend class CDeleteItemCommand;
    friend class CSetTitleCommand;

    // raw operations (NO history)
    void InsertParagraphRaw(std::shared_ptr<CParagraph> para, size_t idx);
    void InsertImageRaw(std::shared_ptr<CImage> img, size_t idx);

    void InsertItemRaw(const CDocumentItem& item, size_t idx);
    CDocumentItem DeleteItemRaw(size_t idx);

    void SetTitleRaw(const std::string& t);

private:
    Path m_workDir;
    mutable CImageStorage m_storage;
    mutable CCommandHistory m_history;

    std::vector<CDocumentItem> m_items;
    std::string m_title;
};
