#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../GroupShape.cpp"
#include "../Primitives.h"
#include "../StdOutCanvas.h"
#include <memory>
#include <optional>
#include <vector>

/*
  Полное покрытие Composite-классов:
  - GroupShape: AddShape, RemoveShape, GetShapeAt, GetShapeCount
  - GetFrame / SetFrame (масштабирование дочерних фигур)
  - Draw (проверка, что не бросает)
  - CompoundLineStyle / CompoundFillStyle:
      * SetColor / GetColor (uniform и non-uniform)
      * SetThickness / GetThickness
      * SetEnabled / GetEnabled
  - Поведение на пустой группе и инвалидация кэша стилей
*/

TEST_CASE("Базовые операции и рамка группы", "[GroupShape][Composite]") {
    auto group = std::make_shared<GroupShape>();

    auto r = std::make_shared<RectangleShape>(Rect{ 10, 10, 20, 10 });
    auto e = std::make_shared<EllipseShape>(Rect{ 40, 5, 10, 30 });
    auto t = std::make_shared<TriangleShape>(Rect{ 60, 15, 20, 20 });

    SECTION("Добавление, удаление и доступ к фигурам") {
        REQUIRE(group->GetShapeCount() == 0u);

        group->AddShape(r);
        REQUIRE(group->GetShapeCount() == 1u);
        REQUIRE(group->GetShapeAt(0) == r);

        group->AddShape(e);
        group->AddShape(t);
        REQUIRE(group->GetShapeCount() == 3u);
        REQUIRE(group->GetShapeAt(1) == e);
        REQUIRE(group->GetShapeAt(2) == t);

        group->RemoveShape(1);
        REQUIRE(group->GetShapeCount() == 2u);
        REQUIRE(group->GetShapeAt(0) == r);
        REQUIRE(group->GetShapeAt(1) == t);

        REQUIRE_THROWS_AS(group->GetShapeAt(99), std::out_of_range);
        REQUIRE_THROWS_AS(group->RemoveShape(99), std::out_of_range);
    }

    SECTION("Вычисление рамки группы (объединение рамок фигур)") {
        group->AddShape(r);
        group->AddShape(e);
        group->AddShape(t);

        Rect frame = group->GetFrame();
        REQUIRE(frame.x == Approx(10.0));
        REQUIRE(frame.y == Approx(5.0));
        REQUIRE(frame.width == Approx(70.0));
        REQUIRE(frame.height == Approx(30.0));
    }

    SECTION("Масштабирование и перенос фигур при изменении рамки группы") {
        group->AddShape(r);
        group->AddShape(e);

        Rect oldFrame = group->GetFrame();
        REQUIRE_FALSE(oldFrame.IsEmpty());

        Rect newFrame{ 0, 0, oldFrame.width * 2.0, oldFrame.height * 2.0 };
        group->SetFrame(newFrame);

        Rect updated = group->GetFrame();
        REQUIRE(updated.width == Approx(newFrame.width));
        REQUIRE(updated.height == Approx(newFrame.height));

        Rect rf = r->GetFrame();
        Rect ef = e->GetFrame();

        REQUIRE(rf.width == Approx(40.0));
        REQUIRE(rf.height == Approx(20.0));
        REQUIRE(ef.width == Approx(20.0));
        REQUIRE(ef.height == Approx(60.0));
    }

    SECTION("Отрисовка группы не выбрасывает исключений") {
        group->AddShape(r);
        group->AddShape(e);
        r->GetFillStyle()->SetEnabled(true);
        r->GetFillStyle()->SetColor({ 200,10,20,255 });
        e->GetLineStyle()->SetEnabled(true);
        e->GetLineStyle()->SetColor({ 0,0,0,255 });
        StdoutCanvas canvas;
        REQUIRE_NOTHROW(group->Draw(canvas));
    }
}

TEST_CASE("Полное покрытие CompoundLineStyle", "[CompoundStyles][Composite]") {
    auto group = std::make_shared<GroupShape>();
    auto r1 = std::make_shared<RectangleShape>(Rect{ 0,0,10,10 });
    auto e1 = std::make_shared<EllipseShape>(Rect{ 20,0,10,10 });
    auto t1 = std::make_shared<TriangleShape>(Rect{ 40,0,10,10 });

    group->AddShape(r1);
    group->AddShape(e1);
    group->AddShape(t1);

    SECTION("Установка и получение цвета для всех фигур") {
        RGBAColor blue{ 0,0,255,255 };
        group->GetLineStyle()->SetColor(blue);

        std::vector<std::shared_ptr<IShape>> shapes = { r1, e1, t1 };
        for (auto& shape : shapes) {
            auto cOpt = shape->GetLineStyle()->GetColor();
            REQUIRE(cOpt.has_value());
            REQUIRE(cOpt.value() == blue);
        }

        r1->GetLineStyle()->SetColor({ 1,2,3,255 });
        REQUIRE(!group->GetLineStyle()->GetColor().has_value());
    }

    SECTION("Установка и получение толщины линии") {
        double thick = 4.5;
        group->GetLineStyle()->SetThickness(thick);

        std::vector<std::shared_ptr<IShape>> shapes = { r1, e1, t1 };
        for (auto& shape : shapes) {
            auto tOpt = shape->GetLineStyle()->GetThickness();
            REQUIRE(tOpt.has_value());
            REQUIRE(tOpt.value() == Approx(thick));
        }

        e1->GetLineStyle()->SetThickness(1.0);
        REQUIRE(!group->GetLineStyle()->GetThickness().has_value());
    }

    SECTION("Установка и получение включения линии") {
        group->GetLineStyle()->SetEnabled(false);

        std::vector<std::shared_ptr<IShape>> shapes = { r1, e1, t1 };
        for (auto& shape : shapes) {
            auto eOpt = shape->GetLineStyle()->GetEnabled();
            REQUIRE(eOpt.has_value());
            REQUIRE(eOpt.value() == false);
        }

        t1->GetLineStyle()->SetEnabled(true);
        REQUIRE(!group->GetLineStyle()->GetEnabled().has_value());
    }

    SECTION("Проверка поведения на пустой группе") {
        auto emptyGroup = std::make_shared<GroupShape>();
        auto lineStyle = emptyGroup->GetLineStyle();

        REQUIRE(!lineStyle->GetColor().has_value());
        REQUIRE(!lineStyle->GetThickness().has_value());
        REQUIRE(!lineStyle->GetEnabled().has_value());

        REQUIRE_NOTHROW(lineStyle->SetColor({ 1,1,1,255 }));
        REQUIRE_NOTHROW(lineStyle->SetThickness(2.0));
        REQUIRE_NOTHROW(lineStyle->SetEnabled(true));
    }

    SECTION("Инвалидация кэша стилей при изменении группы") {
        auto sty1 = group->GetLineStyle();
        group->RemoveShape(0);
        auto sty2 = group->GetLineStyle();
        REQUIRE(sty1 != sty2);
    }
}

TEST_CASE("Полное покрытие CompoundFillStyle", "[CompoundStyles][Composite]") {
    auto group = std::make_shared<GroupShape>();
    auto r1 = std::make_shared<RectangleShape>(Rect{ 0,0,10,10 });
    auto e1 = std::make_shared<EllipseShape>(Rect{ 20,0,10,10 });
    auto t1 = std::make_shared<TriangleShape>(Rect{ 40,0,10,10 });

    group->AddShape(r1);
    group->AddShape(e1);
    group->AddShape(t1);

    SECTION("Установка и получение цвета заливки") {
        RGBAColor gray{ 128,128,128,255 };
        group->GetFillStyle()->SetColor(gray);

        std::vector<std::shared_ptr<IShape>> shapes = { r1, e1, t1 };
        for (auto& shape : shapes) {
            auto cOpt = shape->GetFillStyle()->GetColor();
            REQUIRE(cOpt.has_value());
            REQUIRE(cOpt.value() == gray);
        }

        e1->GetFillStyle()->SetColor({ 0,255,0,255 });
        REQUIRE(!group->GetFillStyle()->GetColor().has_value());
    }

    SECTION("Установка и получение включения заливки") {
        group->GetFillStyle()->SetEnabled(true);

        std::vector<std::shared_ptr<IShape>> shapes = { r1, e1, t1 };
        for (auto& shape : shapes) {
            auto eOpt = shape->GetFillStyle()->GetEnabled();
            REQUIRE(eOpt.has_value());
            REQUIRE(eOpt.value() == true);
        }

        t1->GetFillStyle()->SetEnabled(false);
        REQUIRE(!group->GetFillStyle()->GetEnabled().has_value());
    }

    SECTION("Проверка поведения на пустой группе") {
        auto emptyGroup = std::make_shared<GroupShape>();
        auto fillStyle = emptyGroup->GetFillStyle();

        REQUIRE(!fillStyle->GetColor().has_value());
        REQUIRE(!fillStyle->GetEnabled().has_value());

        REQUIRE_NOTHROW(fillStyle->SetColor({ 10,10,10,255 }));
        REQUIRE_NOTHROW(fillStyle->SetEnabled(true));
    }

    SECTION("Инвалидация кэша стилей при изменении группы") {
        auto fs1 = group->GetFillStyle();
        group->RemoveShape(0);
        auto fs2 = group->GetFillStyle();
        REQUIRE(fs1 != fs2);
    }
}
