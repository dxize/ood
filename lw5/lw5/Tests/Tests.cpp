#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"

#include "Document.h"
#include "EditorApp.h"
#include "Commands.h"
#include "CommandHistory.h"
#include "ImageStorage.h"
#include "Utils.h"
#include "Common.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <random>

static std::string RandomSuffix()
{
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(static_cast<unsigned long long>(now));
    return std::to_string(rng());
}

struct TempDir
{
    fs::path path;
    explicit TempDir(const fs::path& base = fs::current_path())
    {
        path = base / ("tmp_editor_tests_" + RandomSuffix());
        fs::remove_all(path);
        fs::create_directories(path);
    }
    ~TempDir()
    {
        std::error_code ec;
        fs::remove_all(path, ec);
    }
};

static void WriteFile(const fs::path& p, const std::string& data = "dummy")
{
    fs::create_directories(p.parent_path());
    std::ofstream f(p, std::ios::binary);
    f << data;
    f.close();
}

static std::string ReadFile(const fs::path& p)
{
    std::ifstream f(p, std::ios::binary);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

TEST_CASE("Utils::TrimLeft удал€ет пробелы слева", "[utils]")
{
    REQUIRE(TrimLeft("   abc") == "abc");
    REQUIRE(TrimLeft("\t\n  x") == "x");
    REQUIRE(TrimLeft("nochange") == "nochange");
}

TEST_CASE("Utils::HtmlEscape экранирует спецсимволы HTML", "[utils]")
{
    REQUIRE(HtmlEscape("<>&\"'") == "&lt;&gt;&amp;&quot;&#39;");
    REQUIRE(HtmlEscape("abc") == "abc");
}

TEST_CASE("Utils::ParseIndex1Based парсит индекс (>=1) и кидает исключение иначе", "[utils]")
{
    REQUIRE(ParseIndex1Based("1") == 1);
    REQUIRE_THROWS(ParseIndex1Based("0"));
    REQUIRE_THROWS(ParseIndex1Based("-2"));
    REQUIRE_THROWS(ParseIndex1Based("abc"));
}

TEST_CASE("ImageStorage импортирует изображени€ в images/ и сохран€ет расширение", "[storage]")
{
    TempDir td;
    fs::path root = td.path / "work";
    fs::path src = td.path / "cat.png";
    WriteFile(src, "img");

    CImageStorage storage(root);
    auto rel1 = storage.ImportImage(src);
    REQUIRE(rel1.generic_string().find("images/") == 0);
    REQUIRE(rel1.extension() == ".png");
    REQUIRE(fs::exists(root / rel1));

    auto rel2 = storage.ImportImage(src);
    REQUIRE(rel2 != rel1);
    REQUIRE(fs::exists(root / rel2));
}

TEST_CASE("ImageStorage: метка удалени€, сн€тие метки и физическое удаление работают корректно", "[storage]")
{
    TempDir td;
    fs::path root = td.path / "work";
    fs::path src = td.path / "dog.jpg";
    WriteFile(src, "img");

    CImageStorage storage(root);
    auto rel = storage.ImportImage(src);
    REQUIRE(storage.IsMarked(rel) == false);

    storage.MarkForDeletion(rel);
    REQUIRE(storage.IsMarked(rel) == true);

    storage.UnmarkDeletion(rel);
    REQUIRE(storage.IsMarked(rel) == false);

    storage.MarkForDeletion(rel);
    storage.PhysicalDelete(rel);
    REQUIRE(fs::exists(root / rel) == false);
    REQUIRE(storage.IsMarked(rel) == false);
}

TEST_CASE("ƒокумент: вставка параграфов в конец и в позицию", "[doc]")
{
    TempDir td;
    CDocument doc(td.path / "working_document");

    doc.InsertParagraph("A");                 // в конец
    doc.InsertParagraph("B", 0);              // в начало

    REQUIRE(doc.GetItemsCount() == 2);
    REQUIRE(doc.GetItem(0).GetParagraph() != nullptr);
    REQUIRE(doc.GetItem(0).GetParagraph()->GetText() == "B");
    REQUIRE(doc.GetItem(1).GetParagraph()->GetText() == "A");
}

TEST_CASE("ƒокумент: InsertParagraph с неверной позицией кидает исключение", "[doc]")
{
    TempDir td;
    CDocument doc(td.path / "working_document");
    doc.InsertParagraph("A");
    REQUIRE_THROWS(doc.InsertParagraph("X", 5));
}

TEST_CASE("SetTitle: последовательные изменени€ заголовка склеиваютс€ в один Undo", "[merge]")
{
    TempDir td;
    CDocument doc(td.path / "working_document");

    doc.SetTitle("One");
    doc.SetTitle("Two");
    REQUIRE(doc.GetTitle() == "Two");

    doc.Undo();
    REQUIRE(doc.GetTitle() == ""); // изначально пустой
    doc.Redo();
    REQUIRE(doc.GetTitle() == "Two");
}

TEST_CASE("SetTitle: не склеиваетс€, если между SetTitle была друга€ команда", "[merge]")
{
    TempDir td;
    CDocument doc(td.path / "working_document");

    doc.SetTitle("A");
    doc.InsertParagraph("X"); 
    doc.SetTitle("B");

    REQUIRE(doc.GetTitle() == "B");
    doc.Undo(); 
    REQUIRE(doc.GetTitle() == "A");
}

TEST_CASE("ReplaceText: последовательные изменени€ одного параграфа склеиваютс€ в один Undo", "[merge]")
{
    TempDir td;
    CDocument doc(td.path / "working_document");

    auto p = doc.InsertParagraph("start");
    p->SetText("a");
    p->SetText("b");
    REQUIRE(p->GetText() == "b");

    doc.Undo();
    REQUIRE(p->GetText() == "start");
}

TEST_CASE("ReplaceText: не склеиваетс€, если между изменени€ми был другой параграф/команда", "[merge]")
{
    TempDir td;
    CDocument doc(td.path / "working_document");

    auto p1 = doc.InsertParagraph("p1");
    auto p2 = doc.InsertParagraph("p2");

    p1->SetText("A");        // команда 1 (p1)
    p2->SetText("B");        // команда 2 (p2) разрывает цепочку
    p1->SetText("C");        // команда 3 (p1) уже не должна склеитьс€ с командой 1

    REQUIRE(p1->GetText() == "C");
    doc.Undo();            
    REQUIRE(p1->GetText() == "A");
}

TEST_CASE("InsertImage копирует файл в working_document/images, а Resize склеиваетс€", "[image]")
{
    TempDir td;
    fs::path work = td.path / "working_document";
    fs::path src = td.path / "src_img.png";
    WriteFile(src, "imgdata");

    CDocument doc(work);
    auto img = doc.InsertImage(src, 10, 20);

    auto rel = img->GetPath();
    REQUIRE(fs::exists(work / rel));

    // последовательные Resize должны склеитьс€ в один Undo
    img->Resize(30, 40);
    img->Resize(50, 60);
    REQUIRE(img->GetWidth() == 50);
    REQUIRE(img->GetHeight() == 60);

    doc.Undo();
    REQUIRE(img->GetWidth() == 10);
    REQUIRE(img->GetHeight() == 20);
}

TEST_CASE("Resize: проверка диапазона размеров изображени€", "[image]")
{
    TempDir td;
    fs::path work = td.path / "working_document";
    fs::path src = td.path / "src_img.png";
    WriteFile(src, "imgdata");

    CDocument doc(work);
    auto img = doc.InsertImage(src, 10, 20);

    REQUIRE_THROWS(img->Resize(0, 10));
    REQUIRE_THROWS(img->Resize(10, 10001));
}

TEST_CASE("Undo вставки изображени€ + нова€ команда должны физически удалить помеченный файл (очистка redo-ветки)", "[resources]")
{
    TempDir td;
    fs::path work = td.path / "working_document";
    fs::path src = td.path / "cat.png";
    WriteFile(src, "imgdata");

    CDocument doc(work);
    auto img = doc.InsertImage(src, 10, 10);
    auto rel = img->GetPath();
    REQUIRE(fs::exists(work / rel)); 

    doc.Undo();
    REQUIRE(fs::exists(work / rel));

    doc.SetTitle("new-title");
    REQUIRE(fs::exists(work / rel) == false);
}

TEST_CASE("”далЄнное изображение физически удал€етс€, когда команда удалени€ выталкиваетс€ из истории (лимит 10)", "[resources]")
{
    TempDir td;
    fs::path work = td.path / "working_document";
    fs::path src = td.path / "dog.png";
    WriteFile(src, "imgdata");

    CDocument doc(work);
    auto img = doc.InsertImage(src, 10, 10);
    auto rel = img->GetPath();
    REQUIRE(fs::exists(work / rel));

    doc.DeleteItem(0);
    REQUIRE(fs::exists(work / rel));

    // набиваем историю командами, которые Ќ≈ склеиваютс€: InsertParagraph
    // лимит = 10, уже есть 2 команды, добавл€ем 9 => 11 => выкидываетс€ сама€ стара€ (InsertImage)
    for (int i = 0; i < 9; ++i)
    {
        doc.InsertParagraph("p" + std::to_string(i));
    }

    REQUIRE(fs::exists(work / rel));

    // добавл€ем ещЄ одну команду => истори€ снова переполнитс€,
    // теперь выкидываетс€ выполненна€ команда DeleteItem => файл должен удалитьс€ физически
    doc.InsertParagraph("overflow");
    REQUIRE(fs::exists(work / rel) == false);
}

TEST_CASE("Save: сохран€ет HTML с экранированием и копирует изображени€ в output/images", "[save]")
{
    TempDir td;
    fs::path work = td.path / "working_document";
    fs::path outDir = td.path / "export_dir";
    fs::path outHtml = outDir / "doc.html";

    fs::path src = td.path / "a.png";
    WriteFile(src, "imgdata");

    CDocument doc(work);
    doc.SetTitle("<T&\"'>");
    doc.InsertParagraph("Hello <&\"'>");
    auto img = doc.InsertImage(src, 12, 34);
    auto rel = img->GetPath();

    doc.Save(outHtml);

    REQUIRE(fs::exists(outHtml));
    REQUIRE(fs::exists(outDir / rel)); 

    auto html = ReadFile(outHtml);
    REQUIRE(html.find("&lt;T&amp;&quot;&#39;&gt;") != std::string::npos);       
    REQUIRE(html.find("Hello &lt;&amp;&quot;&#39;&gt;") != std::string::npos);
    REQUIRE(html.find("src=\"images/") != std::string::npos);                  
    REQUIRE(html.find("width=\"12\"") != std::string::npos);
    REQUIRE(html.find("height=\"34\"") != std::string::npos);
}

TEST_CASE("EditorApp: выполн€ет команды из потока и корректно печатает List", "[app]")
{
    TempDir td;
    CDocument doc(td.path / "working_document");
    CEditorApp app(doc);

    std::istringstream in(
        "SetTitle MyDoc\n"
        "InsertParagraph end Hello\n"
        "List\n"
    );
    std::ostringstream out;

    app.Run(in, out);
    auto s = out.str();

    REQUIRE(s.find("Title: MyDoc") != std::string::npos);
    REQUIRE(s.find("1. Paragraph: Hello") != std::string::npos);
}

TEST_CASE("EditorApp: при неверных аргументах команды печатает Error", "[app]")
{
    TempDir td;
    CDocument doc(td.path / "working_document");
    CEditorApp app(doc);

    std::istringstream in(
        "InsertParagraph 10 TooFar\n" 
    );
    std::ostringstream out;
    app.Run(in, out);

    auto s = out.str();
    REQUIRE(s.find("Error:") != std::string::npos);
}
