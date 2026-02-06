#pragma once
#include "Interfaces.h"
#include "CommandHistory.h"
#include <memory>

class CReplaceTextCommand;

class CParagraph final : public IParagraph, public std::enable_shared_from_this<CParagraph>
{
public:
    CParagraph(std::string text, CCommandHistory* history);

    std::string GetText() const override;
    void SetText(const std::string& text) override;

private:
    friend class CReplaceTextCommand;
    void SetTextImpl(const std::string& text);

private:
    std::string m_text;
    CCommandHistory* m_history = nullptr;
};
