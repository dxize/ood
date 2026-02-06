#ifdef _WIN32
#include <windows.h>
#endif
#include "Document.h"
#include "EditorApp.h"
#include <iostream>

int main()
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    try
    {
        Path workDir = fs::current_path() / "working_document";

        CDocument doc(workDir);
        CEditorApp app(doc);

        app.Run(std::cin, std::cout);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}