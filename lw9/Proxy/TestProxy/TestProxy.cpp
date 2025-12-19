#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../tile.h"
#include "../image.cpp"

// ============ ТЕСТЫ ДЛЯ КЛАССА TILE ============

TEST_CASE("Конструктор тайла по умолчанию заполняет пробелами", "[tile]")
{
    Tile tile;

    for (int y = 0; y < Tile::SIZE; y++)
    {
        for (int x = 0; x < Tile::SIZE; x++)
        {
            REQUIRE(tile.GetPixel({ x, y }) == ' ');
        }
    }
}

TEST_CASE("Конструктор тайла с цветом заполняет весь тайл", "[tile]")
{
    Tile tile('#');

    for (int y = 0; y < Tile::SIZE; y++)
    {
        for (int x = 0; x < Tile::SIZE; x++)
        {
            REQUIRE(tile.GetPixel({ x, y }) == '#');
        }
    }
}

TEST_CASE("SetPixel тайла изменяет цвет пикселя", "[tile]")
{
    Tile tile('.');

    tile.SetPixel({ 3, 4 }, 'X');

    REQUIRE(tile.GetPixel({ 3, 4 }) == 'X');
    REQUIRE(tile.GetPixel({ 3, 3 }) == '.');
}

TEST_CASE("GetPixel тайла возвращает пробел для координат за границами", "[tile]")
{
    Tile tile('#');

    REQUIRE(tile.GetPixel({ -1, 0 }) == ' ');
    REQUIRE(tile.GetPixel({ 0, -1 }) == ' ');
    REQUIRE(tile.GetPixel({ 8, 0 }) == ' ');
    REQUIRE(tile.GetPixel({ 0, 8 }) == ' ');
}

TEST_CASE("SetPixel тайла ничего не делает для координат за границами", "[tile]")
{
    Tile tile('.');

    tile.SetPixel({ -1, 0 }, 'X');
    tile.SetPixel({ 10, 10 }, 'X');

    REQUIRE(tile.GetPixel({ 0, 0 }) == '.');
}

TEST_CASE("Счётчик экземпляров тайла отслеживает создание и уничтожение", "[tile]")
{
    int initialCount = Tile::GetInstanceCount();

    SECTION("Создание тайлов увеличивает счётчик")
    {
        Tile tile1;
        REQUIRE(Tile::GetInstanceCount() == initialCount + 1);

        Tile tile2;
        REQUIRE(Tile::GetInstanceCount() == initialCount + 2);
    }

    REQUIRE(Tile::GetInstanceCount() == initialCount);
}

TEST_CASE("Конструктор копирования тайла увеличивает счётчик экземпляров", "[tile]")
{
    int initialCount = Tile::GetInstanceCount();

    Tile tile1('#');
    REQUIRE(Tile::GetInstanceCount() == initialCount + 1);

    Tile tile2(tile1);
    REQUIRE(Tile::GetInstanceCount() == initialCount + 2);
    REQUIRE(tile2.GetPixel({ 0, 0 }) == '#');
}

// ============ ТЕСТЫ ДЛЯ КЛАССА IMAGE ============

TEST_CASE("Конструктор изображения создаёт правильный размер", "[image]")
{
    Image img({ 20, 15 }, '.');

    REQUIRE(img.GetSize().width == 20);
    REQUIRE(img.GetSize().height == 15);
}

TEST_CASE("Конструктор изображения заполняет указанным цветом", "[image]")
{
    Image img({ 10, 10 }, '#');

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            REQUIRE(img.GetPixel({ x, y }) == '#');
        }
    }
}

TEST_CASE("GetPixel изображения возвращает пробел для координат за границами", "[image]")
{
    Image img({ 10, 10 }, '.');

    REQUIRE(img.GetPixel({ -1, 0 }) == ' ');
    REQUIRE(img.GetPixel({ 0, -1 }) == ' ');
    REQUIRE(img.GetPixel({ 10, 0 }) == ' ');
    REQUIRE(img.GetPixel({ 0, 10 }) == ' ');
}

TEST_CASE("SetPixel изображения изменяет цвет пикселя", "[image]")
{
    Image img({ 10, 10 }, '.');

    img.SetPixel({ 5, 5 }, 'X');

    REQUIRE(img.GetPixel({ 5, 5 }) == 'X');
    REQUIRE(img.GetPixel({ 5, 4 }) == '.');
}

TEST_CASE("SetPixel изображения ничего не делает для координат за границами", "[image]")
{
    Image img({ 10, 10 }, '.');

    img.SetPixel({ -1, 0 }, 'X');
    img.SetPixel({ 100, 100 }, 'X');

    REQUIRE(img.GetPixel({ 0, 0 }) == '.');
}

TEST_CASE("Конструктор изображения бросает исключение для некорректных размеров", "[image]")
{
    REQUIRE_THROWS(Image({ 0, 10 }, '.'));
    REQUIRE_THROWS(Image({ 10, 0 }, '.'));
    REQUIRE_THROWS(Image({ -5, 10 }, '.'));
    REQUIRE_THROWS(Image({ 10, -5 }, '.'));
}

// ============ ТЕСТЫ COPY-ON-WRITE ============

TEST_CASE("Изображение изначально использует один общий тайл", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();

    Image img({ 16, 16 }, '.');

    // 4 тайла в сетке 2x2, но все ссылаются на один объект
    REQUIRE(Tile::GetInstanceCount() == initialCount + 1);
}

TEST_CASE("Изображение создаёт новый тайл при первой записи", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();

    Image img({ 16, 16 }, '.');
    REQUIRE(Tile::GetInstanceCount() == initialCount + 1);

    img.SetPixel({ 0, 0 }, 'X');

    REQUIRE(Tile::GetInstanceCount() == initialCount + 2);
}

TEST_CASE("Изображение создаёт отдельный тайл для записи в позицию (0,0)", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();
    Image img({ 16, 16 }, '.');

    img.SetPixel({ 0, 0 }, 'A');
    REQUIRE(Tile::GetInstanceCount() == initialCount + 2);
}

TEST_CASE("Изображение создаёт отдельный тайл для записи в позицию (1,0)", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();
    Image img({ 16, 16 }, '.');

    img.SetPixel({ 10, 0 }, 'B');
    REQUIRE(Tile::GetInstanceCount() == initialCount + 2);
}

TEST_CASE("Изображение создаёт отдельные тайлы при записи во все 4 позиции", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();
    Image img({ 16, 16 }, '.');

    // После создания: 1 базовый тайл
    REQUIRE(Tile::GetInstanceCount() == initialCount + 1);

    img.SetPixel({ 0, 0 }, 'A');    // тайл (0,0) копируется
    REQUIRE(Tile::GetInstanceCount() == initialCount + 2);

    img.SetPixel({ 10, 0 }, 'B');   // тайл (1,0) копируется
    REQUIRE(Tile::GetInstanceCount() == initialCount + 3);

    img.SetPixel({ 0, 10 }, 'C');   // тайл (0,1) копируется
    REQUIRE(Tile::GetInstanceCount() == initialCount + 4);

    img.SetPixel({ 10, 10 }, 'D');  // тайл (1,1) уже единственный, не копируется!
    REQUIRE(Tile::GetInstanceCount() == initialCount + 4); // ← правильное значение!

    // Проверяем что все пиксели установлены
    REQUIRE(img.GetPixel({ 0, 0 }) == 'A');
    REQUIRE(img.GetPixel({ 10, 0 }) == 'B');
    REQUIRE(img.GetPixel({ 0, 10 }) == 'C');
    REQUIRE(img.GetPixel({ 10, 10 }) == 'D');
}

TEST_CASE("Чтение изображения не создаёт новые тайлы", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();

    Image img({ 16, 16 }, '.');

    for (int i = 0; i < 100; i++)
    {
        char c = img.GetPixel({ 5, 5 });
        (void)c;
    }

    REQUIRE(Tile::GetInstanceCount() == initialCount + 1);
}

TEST_CASE("Многократная запись в один тайл не создаёт копии", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();

    Image img({ 16, 16 }, '.');
    img.SetPixel({ 0, 0 }, 'A');

    int countAfterFirstWrite = Tile::GetInstanceCount();

    img.SetPixel({ 1, 1 }, 'B');
    img.SetPixel({ 2, 2 }, 'C');
    img.SetPixel({ 3, 3 }, 'D');

    REQUIRE(Tile::GetInstanceCount() == countAfterFirstWrite);
}

TEST_CASE("Изображение вычисляет правильное количество тайлов в сетке", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();

    SECTION("Изображение 8x8 использует 1 тайл")
    {
        Image img({ 8, 8 }, '.');
        REQUIRE(Tile::GetInstanceCount() == initialCount + 1);
    }

    SECTION("Изображение 9x9 использует сетку 2x2")
    {
        Image img({ 9, 9 }, '.');
        REQUIRE(Tile::GetInstanceCount() == initialCount + 1); // CoW!
    }

    SECTION("Изображение 16x8 использует сетку 2x1")
    {
        Image img({ 16, 8 }, '.');
        REQUIRE(Tile::GetInstanceCount() == initialCount + 1); // CoW!
    }
}

TEST_CASE("CoW сохраняет общий тайл до записи", "[image][cow]")
{
    int initialCount = Tile::GetInstanceCount();
    Image img({ 24, 24 }, '#'); // 3x3 тайлов = 9 позиций

    REQUIRE(Tile::GetInstanceCount() == initialCount + 1);

    // Все 9 позиций читают один и тот же тайл
    REQUIRE(img.GetPixel({ 0, 0 }) == '#');
    REQUIRE(img.GetPixel({ 10, 10 }) == '#');
    REQUIRE(img.GetPixel({ 20, 20 }) == '#');

    REQUIRE(Tile::GetInstanceCount() == initialCount + 1);
}

TEST_CASE("LoadImage создаёт правильные размеры", "[image][load]")
{
    std::string data =
        "###\n"
        "# #\n"
        "###\n";

    Image img = LoadImage(data);

    REQUIRE(img.GetSize().width == 3);
    REQUIRE(img.GetSize().height == 3);
}

TEST_CASE("LoadImage корректно загружает пиксели", "[image][load]")
{
    std::string data =
        "ABC\n"
        "DEF\n";

    Image img = LoadImage(data);

    REQUIRE(img.GetPixel({ 0, 0 }) == 'A');
    REQUIRE(img.GetPixel({ 1, 0 }) == 'B');
    REQUIRE(img.GetPixel({ 2, 0 }) == 'C');
    REQUIRE(img.GetPixel({ 0, 1 }) == 'D');
    REQUIRE(img.GetPixel({ 1, 1 }) == 'E');
    REQUIRE(img.GetPixel({ 2, 1 }) == 'F');
}

TEST_CASE("LoadImage обрабатывает строки разной длины", "[image][load]")
{
    std::string data =
        "A\n"
        "ABC\n"
        "AB\n";

    Image img = LoadImage(data);

    REQUIRE(img.GetSize().width == 3);
    REQUIRE(img.GetSize().height == 3);
}
