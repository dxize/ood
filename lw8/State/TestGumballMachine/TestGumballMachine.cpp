#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include <sstream>
#include <string>
#include "../GumBallMachineWithDynamicallyCreatedState.h" 
#include "../GumBallMachineWithState.h" 

// Вспомогательный класс для перехвата вывода в cout
class CoutCapture
{
public:
    CoutCapture() 
        : m_old(std::cout.rdbuf(m_buffer.rdbuf())) 
        //std::streambuf* current = std::cout.rdbuf();
        // Просто ВОЗВРАЩАЕТ указатель на текущий буфер
        // НЕ изменяет ничего
        //std::streambuf* old = std::cout.rdbuf(new_buffer);
        // 1. Заменяет буфер на new_buffer
        // 2. ВОЗВРАЩАЕТ старый буфер (который был до замены)
    {
    }

    ~CoutCapture() 
    { 
        std::cout.rdbuf(m_old); 
    }

    std::string GetOutput() const 
    { 
        return m_buffer.str(); 
    }

    void Clear() 
    { 
        m_buffer.str(""); 
        m_buffer.clear(); 
    }

private:
    std::stringstream m_buffer;
    std::streambuf* m_old;
};

// ============================================================================
// Тесты для with_state::GumballMachine (паттерн State с состояниями-членами)
// ============================================================================

TEST_CASE("GumballMachine with_state - Начальные состояния", "[with_state][initial]")
{
    SECTION("Автомат с жвачками должен начинать в состоянии NoQuarter")
    {
        with_state::GumballMachine machine(5);
        std::string output = machine.ToString();
        REQUIRE(output.find("waiting for quarter") != std::string::npos);
        REQUIRE(output.find("5 gumballs") != std::string::npos);
    }

    SECTION("Автомат без жвачек должен начинать в состоянии SoldOut")
    {
        with_state::GumballMachine machine(0);
        std::string output = machine.ToString();
        REQUIRE(output.find("sold out") != std::string::npos);
        REQUIRE(output.find("0 gumball") != std::string::npos);
    }

    SECTION("Автомат с одной жвачкой должен использовать единственное число")
    {
        with_state::GumballMachine machine(1);
        std::string output = machine.ToString();
        REQUIRE(output.find("1 gumball\n") != std::string::npos);
    }

    SECTION("Автомат с несколькими жвачками должен использовать множественное число")
    {
        with_state::GumballMachine machine(10);
        std::string output = machine.ToString();
        REQUIRE(output.find("10 gumballs") != std::string::npos);
    }
}

// ============================================================================
// NoQuarterState - ВСЕ методы
// ============================================================================

TEST_CASE("with_state::NoQuarterState - Вставка монеты", "[with_state][no_quarter][insert]")
{
    with_state::GumballMachine machine(5);
    CoutCapture capture;

    machine.InsertQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You inserted a quarter") != std::string::npos);
    REQUIRE(machine.ToString().find("waiting for turn of crank") != std::string::npos);
}

TEST_CASE("with_state::NoQuarterState - Возврат монеты", "[with_state][no_quarter][eject]")
{
    with_state::GumballMachine machine(5);
    CoutCapture capture;

    machine.EjectQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You haven't inserted a quarter") != std::string::npos);
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
}

TEST_CASE("with_state::NoQuarterState - Поворот рукоятки", "[with_state][no_quarter][turn]")
{
    with_state::GumballMachine machine(5);
    CoutCapture capture;

    machine.TurnCrank();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You turned but there's no quarter") != std::string::npos);
    REQUIRE(output.find("You need to pay first") != std::string::npos);
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
    REQUIRE(machine.ToString().find("5 gumballs") != std::string::npos);
}

TEST_CASE("with_state::NoQuarterState - ToString", "[with_state][no_quarter][tostring]")
{
    with_state::GumballMachine machine(3);
    std::string output = machine.ToString();
    REQUIRE(output.find("waiting for quarter") != std::string::npos);
}

// ============================================================================
// HasQuarterState - ВСЕ методы
// ============================================================================

TEST_CASE("with_state::HasQuarterState - Вставка еще одной монеты", "[with_state][has_quarter][insert]")
{
    with_state::GumballMachine machine(5);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.InsertQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You can't insert another quarter") != std::string::npos);
    REQUIRE(machine.ToString().find("waiting for turn of crank") != std::string::npos);
}

TEST_CASE("with_state::HasQuarterState - Возврат монеты", "[with_state][has_quarter][eject]")
{
    with_state::GumballMachine machine(5);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.EjectQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("Quarter returned") != std::string::npos);
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
}

TEST_CASE("with_state::HasQuarterState - Поворот рукоятки", "[with_state][has_quarter][turn]")
{
    with_state::GumballMachine machine(5);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.TurnCrank();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You turned...") != std::string::npos);
    REQUIRE(output.find("A gumball comes rolling out the slot...") != std::string::npos);
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
    REQUIRE(machine.ToString().find("4 gumballs") != std::string::npos);
}

TEST_CASE("with_state::HasQuarterState - Dispense (не должна выдавать)", "[with_state][has_quarter][dispense]")
{
    // Dispense вызывается автоматически после TurnCrank, но мы проверяем что из HasQuarter она не выдает
    with_state::GumballMachine machine(5);
    machine.InsertQuarter();
    // После поворота рукоятки состояние меняется на Sold перед вызовом Dispense
    // Поэтому напрямую через публичный интерфейс мы не можем проверить Dispense из HasQuarter
    // Но проверяем что TurnCrank работает корректно
    machine.TurnCrank();
    REQUIRE(machine.ToString().find("4 gumballs") != std::string::npos);
}

TEST_CASE("with_state::HasQuarterState - ToString", "[with_state][has_quarter][tostring]")
{
    with_state::GumballMachine machine(3);
    machine.InsertQuarter();
    std::string output = machine.ToString();
    REQUIRE(output.find("waiting for turn of crank") != std::string::npos);
}

// ============================================================================
// SoldState - ВСЕ методы
// ============================================================================

TEST_CASE("with_state::SoldState - Вставка монеты (во время выдачи)", "[with_state][sold][insert]")
{
    // Примечание: Мы не можем напрямую вызывать методы в состоянии Sold через публичный интерфейс
    // потому что TurnCrank автоматически вызывает Dispense, которая переводит из этого состояния
    // Но мы можем проверить поведение косвенно
    with_state::GumballMachine machine(5);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.TurnCrank();
    std::string output = capture.GetOutput();

    // После TurnCrank автоматически вызывается Dispense и состояние меняется
    REQUIRE(output.find("You turned...") != std::string::npos);
    REQUIRE(output.find("A gumball comes rolling out the slot...") != std::string::npos);
}

TEST_CASE("with_state::SoldState - Dispense переход в NoQuarter", "[with_state][sold][dispense_no_quarter]")
{
    with_state::GumballMachine machine(5);
    machine.InsertQuarter();
    machine.TurnCrank();

    std::string output = machine.ToString();
    REQUIRE(output.find("4 gumballs") != std::string::npos);
    REQUIRE(output.find("waiting for quarter") != std::string::npos);
}

TEST_CASE("with_state::SoldState - Dispense переход в SoldOut при последней жвачке", "[with_state][sold][dispense_sold_out]")
{
    with_state::GumballMachine machine(1);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.TurnCrank();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("A gumball comes rolling out the slot...") != std::string::npos);
    REQUIRE(output.find("Oops, out of gumballs") != std::string::npos);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
    REQUIRE(machine.ToString().find("0 gumball") != std::string::npos);
}

TEST_CASE("with_state::SoldState - ToString", "[with_state][sold][tostring]")
{
    // Мы можем проверить ToString через переход
    with_state::GumballMachine machine(1);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.TurnCrank();

    // После выдачи состояние меняется, но в состоянии Sold ToString вернул бы "delivering a gumball"
    // Это проверяется косвенно через поведение автомата
    REQUIRE(capture.GetOutput().find("A gumball comes rolling out the slot...") != std::string::npos);
}

// ============================================================================
// SoldOutState - ВСЕ методы
// ============================================================================

TEST_CASE("with_state::SoldOutState - Вставка монеты", "[with_state][sold_out][insert]")
{
    with_state::GumballMachine machine(0);
    CoutCapture capture;

    machine.InsertQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You can't insert a quarter, the machine is sold out") != std::string::npos);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("with_state::SoldOutState - Возврат монеты", "[with_state][sold_out][eject]")
{
    with_state::GumballMachine machine(0);
    CoutCapture capture;

    machine.EjectQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You can't eject, you haven't inserted a quarter yet") != std::string::npos);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("with_state::SoldOutState - Поворот рукоятки", "[with_state][sold_out][turn]")
{
    with_state::GumballMachine machine(0);
    CoutCapture capture;

    machine.TurnCrank();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You turned but there's no gumballs") != std::string::npos);
    REQUIRE(output.find("No gumball dispensed") != std::string::npos);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("with_state::SoldOutState - Dispense", "[with_state][sold_out][dispense]")
{
    with_state::GumballMachine machine(0);
    CoutCapture capture;

    machine.TurnCrank(); // Это вызывает Dispense внутри
    std::string output = capture.GetOutput();

    REQUIRE(output.find("No gumball dispensed") != std::string::npos);
}

TEST_CASE("with_state::SoldOutState - ToString", "[with_state][sold_out][tostring]")
{
    with_state::GumballMachine machine(0);
    std::string output = machine.ToString();
    REQUIRE(output.find("sold out") != std::string::npos);
}

// ============================================================================
// GumballMachine - Сложные сценарии
// ============================================================================

TEST_CASE("with_state::GumballMachine - Несколько успешных покупок", "[with_state][scenarios]")
{
    with_state::GumballMachine machine(3);

    // Первая покупка
    machine.InsertQuarter();
    machine.TurnCrank();
    REQUIRE(machine.ToString().find("2 gumballs") != std::string::npos);

    // Вторая покупка
    machine.InsertQuarter();
    machine.TurnCrank();
    REQUIRE(machine.ToString().find("1 gumball") != std::string::npos);

    // Третья покупка (последняя)
    machine.InsertQuarter();
    machine.TurnCrank();
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("with_state::GumballMachine - Цикл вставки и возврата монеты", "[with_state][scenarios]")
{
    with_state::GumballMachine machine(5);

    machine.InsertQuarter();
    REQUIRE(machine.ToString().find("waiting for turn of crank") != std::string::npos);

    machine.EjectQuarter();
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);

    machine.InsertQuarter();
    REQUIRE(machine.ToString().find("waiting for turn of crank") != std::string::npos);

    machine.TurnCrank();
    REQUIRE(machine.ToString().find("4 gumballs") != std::string::npos);
}

TEST_CASE("with_state::GumballMachine - Все операции на пустом автомате", "[with_state][scenarios]")
{
    with_state::GumballMachine machine(1);
    machine.InsertQuarter();
    machine.TurnCrank();

    CoutCapture capture;

    // Теперь автомат пуст, пробуем все операции
    machine.InsertQuarter();
    machine.EjectQuarter();
    machine.TurnCrank();
    machine.InsertQuarter();

    std::string output = capture.GetOutput();
    // Все операции должны обрабатываться корректно
    REQUIRE(output.find("sold out") != std::string::npos);
}

// ============================================================================
// Тесты для with_dynamic_state::GumballMachine
// ============================================================================

TEST_CASE("GumballMachine with_dynamic_state - Начальные состояния", "[with_dynamic_state][initial]")
{
    SECTION("Автомат с жвачками начинает в состоянии NoQuarter")
    {
        with_dynamic_state::GumballMachine machine(5);
        std::string output = machine.ToString();
        REQUIRE(output.find("waiting for quarter") != std::string::npos);
        REQUIRE(output.find("5 gumballs") != std::string::npos);
    }

    SECTION("Автомат без жвачек начинает в состоянии SoldOut")
    {
        with_dynamic_state::GumballMachine machine(0);
        std::string output = machine.ToString();
        REQUIRE(output.find("sold out") != std::string::npos);
        REQUIRE(output.find("0 gumball") != std::string::npos);
    }
}

// ============================================================================
// with_dynamic_state::NoQuarterState - ВСЕ методы
// ============================================================================

TEST_CASE("with_dynamic_state::NoQuarterState - Вставка монеты", "[with_dynamic_state][no_quarter][insert]")
{
    with_dynamic_state::GumballMachine machine(5);
    CoutCapture capture;

    machine.InsertQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You inserted a quarter") != std::string::npos);
    REQUIRE(machine.ToString().find("waiting for turn of crank") != std::string::npos);
}

TEST_CASE("with_dynamic_state::NoQuarterState - Возврат монеты", "[with_dynamic_state][no_quarter][eject]")
{
    with_dynamic_state::GumballMachine machine(5);
    CoutCapture capture;

    machine.EjectQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You haven't inserted a quarter") != std::string::npos);
}

TEST_CASE("with_dynamic_state::NoQuarterState - Поворот рукоятки", "[with_dynamic_state][no_quarter][turn]")
{
    with_dynamic_state::GumballMachine machine(5);
    CoutCapture capture;

    machine.TurnCrank();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You turned but there's no quarter") != std::string::npos);
    REQUIRE(output.find("You need to pay first") != std::string::npos);
}

TEST_CASE("with_dynamic_state::NoQuarterState - ToString", "[with_dynamic_state][no_quarter][tostring]")
{
    with_dynamic_state::GumballMachine machine(3);
    std::string output = machine.ToString();
    REQUIRE(output.find("waiting for quarter") != std::string::npos);
}

// ============================================================================
// with_dynamic_state::HasQuarterState - ВСЕ методы
// ============================================================================

TEST_CASE("with_dynamic_state::HasQuarterState - Вставка еще одной монеты", "[with_dynamic_state][has_quarter][insert]")
{
    with_dynamic_state::GumballMachine machine(5);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.InsertQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You can't insert another quarter") != std::string::npos);
}

TEST_CASE("with_dynamic_state::HasQuarterState - Возврат монеты", "[with_dynamic_state][has_quarter][eject]")
{
    with_dynamic_state::GumballMachine machine(5);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.EjectQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("Quarter returned") != std::string::npos);
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
}

TEST_CASE("with_dynamic_state::HasQuarterState - Поворот рукоятки", "[with_dynamic_state][has_quarter][turn]")
{
    with_dynamic_state::GumballMachine machine(5);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.TurnCrank();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You turned...") != std::string::npos);
    REQUIRE(output.find("A gumball comes rolling out the slot...") != std::string::npos);
}

TEST_CASE("with_dynamic_state::HasQuarterState - ToString", "[with_dynamic_state][has_quarter][tostring]")
{
    with_dynamic_state::GumballMachine machine(3);
    machine.InsertQuarter();
    std::string output = machine.ToString();
    REQUIRE(output.find("waiting for turn of crank") != std::string::npos);
}

// ============================================================================
// with_dynamic_state::SoldState - ВСЕ методы
// ============================================================================

TEST_CASE("with_dynamic_state::SoldState - Dispense переход в NoQuarter", "[with_dynamic_state][sold][dispense]")
{
    with_dynamic_state::GumballMachine machine(5);
    machine.InsertQuarter();
    machine.TurnCrank();

    std::string output = machine.ToString();
    REQUIRE(output.find("4 gumballs") != std::string::npos);
    REQUIRE(output.find("waiting for quarter") != std::string::npos);
}

TEST_CASE("with_dynamic_state::SoldState - Dispense переход в SoldOut", "[with_dynamic_state][sold][sold_out]")
{
    with_dynamic_state::GumballMachine machine(1);
    machine.InsertQuarter();

    CoutCapture capture;
    machine.TurnCrank();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("Oops, out of gumballs") != std::string::npos);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("with_dynamic_state::SoldState - ToString", "[with_dynamic_state][sold][tostring]")
{
    with_dynamic_state::GumballMachine machine(2);
    machine.InsertQuarter();
    machine.TurnCrank();
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
}

// ============================================================================
// with_dynamic_state::SoldOutState - ВСЕ методы
// ============================================================================

TEST_CASE("with_dynamic_state::SoldOutState - Вставка монеты", "[with_dynamic_state][sold_out][insert]")
{
    with_dynamic_state::GumballMachine machine(0);
    CoutCapture capture;

    machine.InsertQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You can't insert a quarter, the machine is sold out") != std::string::npos);
}

TEST_CASE("with_dynamic_state::SoldOutState - Возврат монеты", "[with_dynamic_state][sold_out][eject]")
{
    with_dynamic_state::GumballMachine machine(0);
    CoutCapture capture;

    machine.EjectQuarter();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You can't eject, you haven't inserted a quarter yet") != std::string::npos);
}

TEST_CASE("with_dynamic_state::SoldOutState - Поворот рукоятки", "[with_dynamic_state][sold_out][turn]")
{
    with_dynamic_state::GumballMachine machine(0);
    CoutCapture capture;

    machine.TurnCrank();
    std::string output = capture.GetOutput();

    REQUIRE(output.find("You turned but there's no gumballs") != std::string::npos);
    REQUIRE(output.find("No gumball dispensed") != std::string::npos);
}

TEST_CASE("with_dynamic_state::SoldOutState - ToString", "[with_dynamic_state][sold_out][tostring]")
{
    with_dynamic_state::GumballMachine machine(0);
    std::string output = machine.ToString();
    REQUIRE(output.find("sold out") != std::string::npos);
}

// ============================================================================
// with_dynamic_state::GumballMachine - Сложные сценарии
// ============================================================================

TEST_CASE("with_dynamic_state::GumballMachine - Полный цикл покупок", "[with_dynamic_state][scenarios]")
{
    with_dynamic_state::GumballMachine machine(3);

    for (int i = 3; i > 0; --i)
    {
        machine.InsertQuarter();
        machine.TurnCrank();

        if (i == 1)
        {
            REQUIRE(machine.ToString().find("sold out") != std::string::npos);
        }
        else
        {
            REQUIRE(machine.ToString().find(std::to_string(i - 1) + " gumball") != std::string::npos);
        }
    }
}

TEST_CASE("with_dynamic_state::GumballMachine - Множественные операции возврата", "[with_dynamic_state][scenarios]")
{
    with_dynamic_state::GumballMachine machine(5);

    machine.InsertQuarter();
    machine.EjectQuarter();
    machine.InsertQuarter();
    machine.EjectQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.ToString().find("4 gumballs") != std::string::npos);
}

TEST_CASE("with_dynamic_state::GumballMachine - Проверка переходов состояний", "[with_dynamic_state][scenarios]")
{
    with_dynamic_state::GumballMachine machine(2);

    // NoQuarter -> HasQuarter
    machine.InsertQuarter();
    REQUIRE(machine.ToString().find("waiting for turn of crank") != std::string::npos);

    // HasQuarter -> Sold -> NoQuarter
    machine.TurnCrank();
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
    REQUIRE(machine.ToString().find("1 gumball") != std::string::npos);

    // NoQuarter -> HasQuarter -> Sold -> SoldOut
    machine.InsertQuarter();
    machine.TurnCrank();
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

// ============================================================================
// Граничные случаи и пограничные условия
// ============================================================================

TEST_CASE("Граничные случаи - Пограничные условия", "[edge_cases]")
{
    SECTION("with_state: Автомат ровно с 1 жвачкой")
    {
        with_state::GumballMachine machine(1);
        REQUIRE(machine.ToString().find("1 gumball\n") != std::string::npos);

        machine.InsertQuarter();
        machine.TurnCrank();
        REQUIRE(machine.ToString().find("sold out") != std::string::npos);
    }

    SECTION("with_dynamic_state: Автомат ровно с 1 жвачкой")
    {
        with_dynamic_state::GumballMachine machine(1);
        REQUIRE(machine.ToString().find("1 gumball\n") != std::string::npos);

        machine.InsertQuarter();
        machine.TurnCrank();
        REQUIRE(machine.ToString().find("sold out") != std::string::npos);
    }

    SECTION("with_state: Множественные операции на пустом автомате")
    {
        with_state::GumballMachine machine(0);
        CoutCapture capture;

        machine.InsertQuarter();
        machine.EjectQuarter();
        machine.TurnCrank();
        machine.InsertQuarter();

        std::string output = capture.GetOutput();
        // Все операции должны обрабатываться корректно
        REQUIRE(output.find("sold out") != std::string::npos);
    }

    SECTION("with_dynamic_state: Быстрая смена состояний")
    {
        with_dynamic_state::GumballMachine machine(10);

        for (int i = 0; i < 5; ++i)
        {
            machine.InsertQuarter();
            machine.EjectQuarter();
        }

        machine.InsertQuarter();
        machine.TurnCrank();

        REQUIRE(machine.ToString().find("9 gumballs") != std::string::npos);
    }
}