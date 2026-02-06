#include "Paragraph.h"
#include "Commands.h"
#include <stdexcept>

CParagraph::CParagraph(std::string text, CCommandHistory* history)
    : m_text(std::move(text))
    , m_history(history)
{
}

std::string CParagraph::GetText() const
{
    return m_text;
}

void CParagraph::SetTextImpl(const std::string& text)
{
    m_text = text;
}

void CParagraph::SetText(const std::string& text)
{
    if (!m_history)
    {
        throw std::runtime_error("Paragraph has no history");
    }

    m_history->AddAndExecute(std::make_shared<CReplaceTextCommand>(shared_from_this(), text));
}
