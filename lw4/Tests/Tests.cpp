#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

#include "../Factory/include/app/Designer.h"
#include "../Factory/include/app/Painter.h"

#include "../Factory/include/canvas/ICanvas.h"
#include "../Factory/include/canvas/MultiCanvas.h"
#include "../Factory/include/canvas/ConsoleCanvas.h"
#include "../Factory/include/canvas/SvgCanvas.h"

#include "../Factory/include/domain/PictureDraft.h"
#include "../Factory/include/domain/Shape.h"
#include "../Factory/include/domain/Color.h"
#include "../Factory/include/domain/Point.h"

#include "../Factory/include/domain/shapes/Triangle.h"
#include "../Factory/include/domain/shapes/Rectangle.h"
#include "../Factory/include/domain/shapes/Ellipse.h"
#include "../Factory/include/domain/shapes/RegularPolygon.h"

#include "../Factory/include/factory/IShapeFactory.h"
#include "../Factory/include/factory/ShapeFactory.h"

#include "../Factory/include/util/ColorUtil.h"
#include "../Factory/include/util/StringUtil.h"

namespace
{
    // ---------- Моки/фейки для DIP ----------

    struct CanvasCall
    {
        enum class Type { SetColor, Line, Ellipse } type;
        domain::Color color = domain::Color::Black;
        domain::Point p1{};
        domain::Point p2{};
        double w = 0.0;
        double h = 0.0;
    };

    class MockCanvas final : public canvas::ICanvas
    {
    public:
        std::vector<CanvasCall> calls;

        void SetColor(domain::Color color) override
        {
            CanvasCall c;
            c.type = CanvasCall::Type::SetColor;
            c.color = color;
            calls.push_back(c);
        }

        void DrawLine(const domain::Point& from, const domain::Point& to) override
        {
            CanvasCall c;
            c.type = CanvasCall::Type::Line;
            c.p1 = from;
            c.p2 = to;
            calls.push_back(c);
        }

        void DrawEllipse(const domain::Point& leftTop, double width, double height) override
        {
            CanvasCall c;
            c.type = CanvasCall::Type::Ellipse;
            c.p1 = leftTop;
            c.w = width;
            c.h = height;
            calls.push_back(c);
        }
    };

    class SpyShape final : public domain::Shape
    {
    public:
        explicit SpyShape(domain::Color color)
            : Shape(color)
        {
        }

        void Draw(canvas::ICanvas& canvas) const override
        {
            ++drawCount;
            lastCanvas = &canvas;
        }

        mutable int drawCount = 0;
        mutable canvas::ICanvas* lastCanvas = nullptr;
    };

    class FakeShapeFactory final : public factory::IShapeFactory
    {
    public:
        // если descr совпал — отдаём заранее подготовленную фигуру
        std::unique_ptr<domain::Shape> CreateShape(const std::string& descr) const override
        {
            if (throwOn && descr == throwOnDescr)
            {
                throw std::runtime_error(throwMsg);
            }

            if (descr == "S1")
            {
                return std::make_unique<domain::Triangle>(domain::Color::Red,
                    domain::Point{ 0,0 }, domain::Point{ 10,0 }, domain::Point{ 0,10 });
            }
            if (descr == "S2")
            {
                return std::make_unique<domain::Ellipse>(domain::Color::Green,
                    domain::Point{ 5,5 }, 2, 3);
            }

            throw std::runtime_error("Неизвестное описание в фейковой фабрике: " + descr);
        }

        bool throwOn = false;
        std::string throwOnDescr;
        std::string throwMsg = "boom";
    };

    static bool Almost(double a, double b, double eps = 1e-9)
    {
        return std::fabs(a - b) <= eps;
    }
}

// -------------------- util --------------------

TEST_CASE(u8"Trim обрезает пробелы по краям", "[util][Trim]")
{
    REQUIRE(util::Trim("   abc  ") == "abc");
    REQUIRE(util::Trim("\t\r\nabc\n") == "abc");
    REQUIRE(util::Trim("abc") == "abc");
}

TEST_CASE(u8"IsBlankOrComment пропускает пустые и комментарии", "[util][IsBlankOrComment]")
{
    REQUIRE(util::IsBlankOrComment("   "));
    REQUIRE(util::IsBlankOrComment("\t  # comment"));
    REQUIRE_FALSE(util::IsBlankOrComment(" #not blank?")); // всё равно комментарий
    REQUIRE_FALSE(util::IsBlankOrComment("triangle red 0 0 1 1 2 2"));
}

TEST_CASE(u8"ParseColor понимает английские и русские названия", "[util][Color]")
{
    REQUIRE(util::ParseColor("red") == domain::Color::Red);
    REQUIRE(util::ParseColor("ReD") == domain::Color::Red);
    REQUIRE(util::ParseColor("зелёный") == domain::Color::Green);
    REQUIRE(util::ParseColor("зеленый") == domain::Color::Green);
    REQUIRE(util::ParseColor("жёлтый") == domain::Color::Yellow);
    REQUIRE(util::ParseColor("желтый") == domain::Color::Yellow);
}

TEST_CASE(u8"ParseColor кидает исключение на неизвестный цвет", "[util][Color]")
{
    REQUIRE_THROWS_WITH(util::ParseColor("purple"), Catch::Matchers::Contains("Unknown color"));
}

// -------------------- domain::PictureDraft --------------------

TEST_CASE(u8"PictureDraft добавляет фигуры и возвращает корректное количество", "[draft]")
{
    domain::PictureDraft draft;

    auto s1 = std::make_unique<SpyShape>(domain::Color::Black);
    auto s2 = std::make_unique<SpyShape>(domain::Color::Red);

    draft.AddShape(std::move(s1));
    draft.AddShape(std::move(s2));

    REQUIRE(draft.GetShapeCount() == 2);
}

TEST_CASE(u8"PictureDraft выдаёт фигуру по индексу и валидирует границы", "[draft]")
{
    domain::PictureDraft draft;
    auto s1 = std::make_unique<SpyShape>(domain::Color::Black);
    SpyShape* raw = s1.get();
    draft.AddShape(std::move(s1));

    REQUIRE(&draft.GetShape(0) == raw);
    REQUIRE_THROWS_AS(draft.GetShape(1), std::out_of_range);
}

TEST_CASE(u8"PictureDraft не принимает null фигуру", "[draft]")
{
    domain::PictureDraft draft;
    std::unique_ptr<domain::Shape> nullShape;
    REQUIRE_THROWS_AS(draft.AddShape(std::move(nullShape)), std::invalid_argument);
}

// -------------------- shapes --------------------

TEST_CASE(u8"Треугольник рисует 3 линии и ставит цвет", "[shape][triangle]")
{
    MockCanvas canvas;

    domain::Triangle tri(domain::Color::Blue,
        domain::Point{ 0,0 }, domain::Point{ 10,0 }, domain::Point{ 0,10 });

    tri.Draw(canvas);

    REQUIRE(canvas.calls.size() == 4);
    REQUIRE(canvas.calls[0].type == CanvasCall::Type::SetColor);
    REQUIRE(canvas.calls[0].color == domain::Color::Blue);

    REQUIRE(canvas.calls[1].type == CanvasCall::Type::Line);
    REQUIRE(canvas.calls[2].type == CanvasCall::Type::Line);
    REQUIRE(canvas.calls[3].type == CanvasCall::Type::Line);
}

TEST_CASE(u8"Прямоугольник рисует 4 стороны и ставит цвет", "[shape][rectangle]")
{
    MockCanvas canvas;

    domain::Rectangle r(domain::Color::Red, domain::Point{ 0,0 }, domain::Point{ 10,20 });
    r.Draw(canvas);

    REQUIRE(canvas.calls.size() == 5);
    REQUIRE(canvas.calls[0].type == CanvasCall::Type::SetColor);
    REQUIRE(canvas.calls[0].color == domain::Color::Red);

    int lineCount = 0;
    for (auto& c : canvas.calls)
        if (c.type == CanvasCall::Type::Line) ++lineCount;

    REQUIRE(lineCount == 4);
}

TEST_CASE(u8"Эллипс переводит центр/радиусы в leftTop/width/height и ставит цвет", "[shape][ellipse]")
{
    MockCanvas canvas;

    domain::Ellipse e(domain::Color::Green, domain::Point{ 10, 10 }, 4, 3);
    e.Draw(canvas);

    REQUIRE(canvas.calls.size() == 2);
    REQUIRE(canvas.calls[0].type == CanvasCall::Type::SetColor);
    REQUIRE(canvas.calls[0].color == domain::Color::Green);

    REQUIRE(canvas.calls[1].type == CanvasCall::Type::Ellipse);
    REQUIRE(Almost(canvas.calls[1].p1.x, 6.0));   // 10 - 4
    REQUIRE(Almost(canvas.calls[1].p1.y, 7.0));   // 10 - 3
    REQUIRE(Almost(canvas.calls[1].w, 8.0));      // 2*4
    REQUIRE(Almost(canvas.calls[1].h, 6.0));      // 2*3
}

TEST_CASE(u8"Эллипс требует положительные радиусы", "[shape][ellipse]")
{
    REQUIRE_THROWS_AS(domain::Ellipse(domain::Color::Black, domain::Point{ 0,0 }, 0, 1), std::invalid_argument);
    REQUIRE_THROWS_AS(domain::Ellipse(domain::Color::Black, domain::Point{ 0,0 }, 1, -1), std::invalid_argument);
}

TEST_CASE(u8"Правильный многоугольник рисует N отрезков и ставит цвет", "[shape][polygon]")
{
    MockCanvas canvas;

    domain::RegularPolygon p(domain::Color::Pink, domain::Point{ 0, 0 }, 10, 4);
    p.Draw(canvas);

    REQUIRE(canvas.calls.size() == 1 + 4); // SetColor + 4 линии
    REQUIRE(canvas.calls[0].type == CanvasCall::Type::SetColor);
    REQUIRE(canvas.calls[0].color == domain::Color::Pink);

    int lineCount = 0;
    for (auto& c : canvas.calls)
        if (c.type == CanvasCall::Type::Line) ++lineCount;
    REQUIRE(lineCount == 4);

    // Проверим первую вершину: при startAngle=-pi/2 должна быть сверху: (0, -radius)
    // Первая линия начинается с вершины 0.
    const auto& firstLine = canvas.calls[1];
    REQUIRE(firstLine.type == CanvasCall::Type::Line);
    REQUIRE(Almost(firstLine.p1.x, 0.0, 1e-6));
    REQUIRE(Almost(firstLine.p1.y, -10.0, 1e-6));
}

TEST_CASE(u8"Правильный многоугольник валидирует параметры", "[shape][polygon]")
{
    REQUIRE_THROWS_AS(domain::RegularPolygon(domain::Color::Black, domain::Point{ 0,0 }, 0, 3), std::invalid_argument);
    REQUIRE_THROWS_AS(domain::RegularPolygon(domain::Color::Black, domain::Point{ 0,0 }, 10, 2), std::invalid_argument);
}

// -------------------- factory::ShapeFactory --------------------

TEST_CASE(u8"Фабрика создаёт треугольник из строки (с русским цветом)", "[factory]")
{
    factory::ShapeFactory f;
    auto s = f.CreateShape("triangle красный 0 0 10 0 0 10");
    REQUIRE(s != nullptr);

    MockCanvas canvas;
    s->Draw(canvas);

    REQUIRE(canvas.calls.size() == 4);
    REQUIRE(canvas.calls[0].type == CanvasCall::Type::SetColor);
    REQUIRE(canvas.calls[0].color == domain::Color::Red);
}

TEST_CASE(u8"Фабрика создаёт прямоугольник, эллипс и многоугольник", "[factory]")
{
    factory::ShapeFactory f;

    auto r = f.CreateShape("rectangle green 0 0 10 20");
    auto e = f.CreateShape("ellipse blue 10 10 4 3");
    auto p = f.CreateShape("polygon yellow 0 0 10 5");

    REQUIRE(r != nullptr);
    REQUIRE(e != nullptr);
    REQUIRE(p != nullptr);
}

TEST_CASE(u8"Фабрика не принимает неизвестный тип фигуры", "[factory]")
{
    factory::ShapeFactory f;
    REQUIRE_THROWS_WITH(f.CreateShape("hexagon red 0 0 1 1"), Catch::Matchers::Contains("Unknown shape type"));
}

TEST_CASE(u8"Фабрика ругается на нехватку параметров", "[factory]")
{
    factory::ShapeFactory f;
    REQUIRE_THROWS_WITH(f.CreateShape("triangle red 0 0 1 1"), Catch::Matchers::Contains("Expected x3"));
    REQUIRE_THROWS_WITH(f.CreateShape("ellipse red 0 0 10"), Catch::Matchers::Contains("Expected ry"));
}

// -------------------- app::Designer --------------------

TEST_CASE(u8"Designer читает строки, пропускает пустые и комментарии", "[designer]")
{
    FakeShapeFactory ff;
    app::Designer d(ff);

    std::istringstream in(
        "   \n"
        "#comment\n"
        "  S1   \n"
        "S2\n"
    );

    domain::PictureDraft draft = d.CreateDraft(in);
    REQUIRE(draft.GetShapeCount() == 2);
}

TEST_CASE(u8"Designer оборачивает исключение фабрики с номером строки", "[designer]")
{
    FakeShapeFactory ff;
    ff.throwOn = true;
    ff.throwOnDescr = "BAD";
    ff.throwMsg = "ошибка фабрики";

    app::Designer d(ff);

    std::istringstream in(
        "S1\n"
        "   # коммент\n"
        "BAD\n"
    );

    try
    {
        (void)d.CreateDraft(in);
        FAIL("Ожидалось исключение");
    }
    catch (const std::exception& e)
    {
        // BAD находится на 3-й строке (2-я — комментарий, но строка считается)
        REQUIRE(std::string(e.what()).find("Line 3:") != std::string::npos);
        REQUIRE(std::string(e.what()).find("ошибка фабрики") != std::string::npos);
    }
}

// -------------------- app::Painter --------------------

TEST_CASE(u8"Painter вызывает Draw у всех фигур в макете", "[painter]")
{
    domain::PictureDraft draft;
    auto s1 = std::make_unique<SpyShape>(domain::Color::Black);
    auto s2 = std::make_unique<SpyShape>(domain::Color::Red);

    SpyShape* p1 = s1.get();
    SpyShape* p2 = s2.get();

    draft.AddShape(std::move(s1));
    draft.AddShape(std::move(s2));

    MockCanvas canvas;
    app::Painter painter;

    painter.DrawPicture(draft, canvas);

    REQUIRE(p1->drawCount == 1);
    REQUIRE(p2->drawCount == 1);
    REQUIRE(p1->lastCanvas == &canvas);
    REQUIRE(p2->lastCanvas == &canvas);
}

// -------------------- canvas::MultiCanvas --------------------

TEST_CASE(u8"MultiCanvas дублирует вызовы на все канвасы", "[canvas][multi]")
{
    MockCanvas c1, c2;
    canvas::MultiCanvas m({ c1, c2 });

    m.SetColor(domain::Color::Yellow);
    m.DrawLine({ 0,0 }, { 1,1 });
    m.DrawEllipse({ 2,3 }, 10, 20);

    REQUIRE(c1.calls.size() == 3);
    REQUIRE(c2.calls.size() == 3);

    REQUIRE(c1.calls[0].type == CanvasCall::Type::SetColor);
    REQUIRE(c2.calls[0].type == CanvasCall::Type::SetColor);
}

// -------------------- canvas::ConsoleCanvas / SvgCanvas (быстрые smoke-тесты) --------------------

TEST_CASE(u8"ConsoleCanvas пишет команды в поток", "[canvas][console]")
{
    std::ostringstream out;
    canvas::ConsoleCanvas c(out);

    c.SetColor(domain::Color::Red);
    c.DrawLine({ 1,2 }, { 3,4 });
    c.DrawEllipse({ 10,20 }, 30, 40);

    const std::string s = out.str();
    REQUIRE(s.find("SetColor red") != std::string::npos);
    REQUIRE(s.find("DrawLine 1 2 3 4") != std::string::npos);
    REQUIRE(s.find("DrawEllipse 10 20 30 40") != std::string::npos);
}

TEST_CASE(u8"SvgCanvas создаёт svg и закрывает тег в деструкторе", "[canvas][svg]")
{
    std::ostringstream out;

    {
        canvas::SvgCanvas c(out, 800, 600);
        c.SetColor(domain::Color::Blue);
        c.DrawLine({ 0,0 }, { 10,10 });
        c.DrawEllipse({ 10,20 }, 30, 40);
    } // деструктор должен дописать </svg>

    const std::string s = out.str();
    REQUIRE(s.find("<svg") != std::string::npos);
    REQUIRE(s.find("<line") != std::string::npos);
    REQUIRE(s.find("<ellipse") != std::string::npos);
    REQUIRE(s.find("</svg>") != std::string::npos);
}
