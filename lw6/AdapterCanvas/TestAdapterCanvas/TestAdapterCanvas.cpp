#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include <sstream>
#include "../app/ModernGraphicsAdapter.h"
#include "../app/ModernGraphicsAdapter.cpp"
#include "../modern_graphics_lib/ModernGraphicsRenderer.h"

TEST_CASE("CModernGraphicsAdapter", "[adapter]")
{
    std::stringstream output;
    modern_graphics_lib::ModernGraphicsRenderer renderer(output);
    app::CModernGraphicsAdapter adapter(renderer);

    SECTION("BeginDraw and EndDraw")
    {
        adapter.BeginDraw();
        adapter.EndDraw();

        std::string expected = "<draw>\n</draw>\n";
        REQUIRE(output.str() == expected);
    }

    SECTION("BeginDraw throws if already drawing")
    {
        adapter.BeginDraw();
        REQUIRE_THROWS_AS(adapter.BeginDraw(), std::logic_error);
        adapter.EndDraw(); 
    }

    SECTION("EndDraw throws if not drawing")
    {
        REQUIRE_THROWS_AS(adapter.EndDraw(), std::logic_error);
    }

    SECTION("MoveTo sets position without drawing")
    {
        adapter.BeginDraw();
        adapter.MoveTo(10, 20);
        adapter.EndDraw();

        std::string expected = "<draw>\n</draw>\n";
        REQUIRE(output.str() == expected);
    }

    SECTION("LineTo draws line and updates position")
    {
        adapter.BeginDraw();
        adapter.MoveTo(0, 0);
        adapter.LineTo(100, 200);
        adapter.EndDraw();

        std::string expected = "<draw>\n"
            "  <line fromX=\"0\" fromY=\"0\" toX=\"100\" toY=\"200\" />\n"
            "</draw>\n";
        REQUIRE(output.str() == expected);
    }

    SECTION("Multiple LineTo calls chain positions")
    {
        adapter.BeginDraw();
        adapter.MoveTo(0, 0);
        adapter.LineTo(100, 100);
        adapter.LineTo(200, 200);
        adapter.EndDraw();

        std::string expected = "<draw>\n"
            "  <line fromX=\"0\" fromY=\"0\" toX=\"100\" toY=\"100\" />\n"
            "  <line fromX=\"100\" fromY=\"100\" toX=\"200\" toY=\"200\" />\n"
            "</draw>\n";
        REQUIRE(output.str() == expected);
    }

    SECTION("LineTo without BeginDraw throws")
    {
        REQUIRE_THROWS_AS(adapter.LineTo(10, 20), std::logic_error);
    }
}