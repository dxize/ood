#pragma once
#include "IDocument.h"
#include <istream>
#include <ostream>
#include <string>

class CEditorApp
{
public:
    explicit CEditorApp(IDocument& document);

    // основной цикл приложения (чтение команд)
    void Run(std::istream& input, std::ostream& output);

private:
    void PrintHelp(std::ostream& output) const;
    void PrintList(std::ostream& output) const;

    void ExecuteLine(const std::string& line, std::ostream& output);

private:
    IDocument& m_document;
};
