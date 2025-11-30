#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include <sstream>
#include "../WithStateMulti.h"
#include "../NaiveMulti.h"

class CoutCapture
{
public:
    CoutCapture() : m_old(std::cout.rdbuf(m_buffer.rdbuf())) {}
    ~CoutCapture() { std::cout.rdbuf(m_old); }
    std::string GetOutput() const { return m_buffer.str(); }
    void Clear() { m_buffer.str(""); m_buffer.clear(); }
private:
    std::stringstream m_buffer;
    std::streambuf* m_old;
};

// ============================================================================
// ТЕСТЫ ДЛЯ НАИВНОЙ РЕАЛИЗАЦИИ - НОВЫЙ ФУНКЦИОНАЛ
// ============================================================================

TEST_CASE("naive - Вставка 1 монетки", "[naive][insert]")
{
    naive_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 1);
    REQUIRE(machine.ToString().find("Quarters: 1") != std::string::npos);
}

TEST_CASE("naive - Вставка 2 монеток", "[naive][insert]")
{
    naive_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 2);
}

TEST_CASE("naive - Вставка 3 монеток", "[naive][insert]")
{
    naive_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 3);
}

TEST_CASE("naive - Вставка 5 монеток (максимум)", "[naive][insert]")
{
    naive_multi::MultiGumballMachine machine(10);

    for (unsigned i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    REQUIRE(machine.GetQuarterCount() == 5);
    REQUIRE(machine.ToString().find("Quarters: 5") != std::string::npos);
}

TEST_CASE("naive - Блокировка 6-й монетки", "[naive][insert]")
{
    naive_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    for (unsigned i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    capture.Clear();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 5);
    REQUIRE(capture.GetOutput().find("You can't insert another quarter") != std::string::npos);
}

TEST_CASE("naive - Блокировка 7-й монетки", "[naive][insert]")
{
    naive_multi::MultiGumballMachine machine(10);

    for (unsigned i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    machine.InsertQuarter();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 5);
}

TEST_CASE("naive - Возврат 1 монетки", "[naive][eject]")
{
    naive_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    machine.InsertQuarter();

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("1 quarter returned") != std::string::npos);
}

TEST_CASE("naive - Возврат 3 монеток", "[naive][eject]")
{
    naive_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("3 quarters returned") != std::string::npos);
}

TEST_CASE("naive - Возврат 5 монеток (максимум)", "[naive][eject]")
{
    naive_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    for (int i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("5 quarters returned") != std::string::npos);
}

TEST_CASE("naive - Возврат из NoQuarter", "[naive][eject]")
{
    naive_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    machine.EjectQuarter();

    REQUIRE(capture.GetOutput().find("You haven't inserted a quarter") != std::string::npos);
}

TEST_CASE("naive - Покупка с 2 монетками подряд", "[naive][purchase]")
{
    naive_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();

    machine.TurnCrank();
    REQUIRE(machine.GetQuarterCount() == 1);
    REQUIRE(machine.ToString().find("9 gumballs") != std::string::npos);

    machine.TurnCrank();
    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(machine.ToString().find("8 gumballs") != std::string::npos);
}

TEST_CASE("naive - Покупка с 5 монетками подряд", "[naive][purchase]")
{
    naive_multi::MultiGumballMachine machine(10);

    for (int i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    for (int i = 5; i > 0; --i)
    {
        machine.TurnCrank();
        REQUIRE(machine.GetQuarterCount() == i - 1);
    }

    REQUIRE(machine.ToString().find("5 gumballs") != std::string::npos);
}

TEST_CASE("naive - Добавление монетки после покупки", "[naive][purchase]")
{
    naive_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);

    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 2);
}

TEST_CASE("naive - Добавление нескольких монеток во время покупок", "[naive][purchase]")
{
    naive_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 4);

    machine.TurnCrank();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 2);
}

TEST_CASE("naive - Монеток больше чем жвачек (2 жвачки, 3 монетки)", "[naive][soldout]")
{
    naive_multi::MultiGumballMachine machine(2);
    CoutCapture capture;

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    machine.TurnCrank();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("naive - Возврат монеток после распродажи", "[naive][soldout]")
{
    naive_multi::MultiGumballMachine machine(1);
    CoutCapture capture;

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("1 quarter returned") != std::string::npos);
}

TEST_CASE("naive - Возврат нескольких монеток после распродажи", "[naive][soldout]")
{
    naive_multi::MultiGumballMachine machine(2);
    CoutCapture capture;

    for (int i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    machine.TurnCrank();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 3);

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
    REQUIRE(capture.GetOutput().find("3 quarters returned") != std::string::npos);
}

TEST_CASE("naive - Переход Sold -> HasQuarter когда монетки остались", "[naive][transitions]")
{
    naive_multi::MultiGumballMachine machine(5);

    machine.InsertQuarter();
    machine.InsertQuarter();

    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);
    REQUIRE(machine.ToString().find("waiting for turn of crank") != std::string::npos);
}

TEST_CASE("naive - Переход Sold -> NoQuarter когда монеток не осталось", "[naive][transitions]")
{
    naive_multi::MultiGumballMachine machine(5);

    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
}

TEST_CASE("naive - Вставка монетки в состоянии Sold", "[naive][error]")
{
    naive_multi::MultiGumballMachine machine(5);
    CoutCapture capture;

    machine.InsertQuarter();
    machine.TurnCrank();

    // Пытаемся вставить еще (но уже поздно, выдача началась)
    // Это не проверить через публичный API, т.к. TurnCrank сразу делает Dispense
}

TEST_CASE("naive - Граничный случай: 5 жвачек и 5 монеток", "[naive][edge]")
{
    naive_multi::MultiGumballMachine machine(5);

    for (int i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    for (int i = 0; i < 5; ++i)
    {
        machine.TurnCrank();
    }

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("naive - Сложный сценарий с чередованием операций", "[naive][complex]")
{
    naive_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    machine.TurnCrank();
    machine.TurnCrank();

    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
}

// ============================================================================
// ТЕСТЫ ДЛЯ РЕАЛИЗАЦИИ С ПАТТЕРНОМ STATE - НОВЫЙ ФУНКЦИОНАЛ
// ============================================================================

TEST_CASE("state - Вставка 1 монетки", "[state][insert]")
{
    with_state_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 1);
    REQUIRE(machine.ToString().find("Quarters: 1") != std::string::npos);
}

TEST_CASE("state - Вставка 2 монеток", "[state][insert]")
{
    with_state_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 2);
}

TEST_CASE("state - Вставка 3 монеток", "[state][insert]")
{
    with_state_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 3);
}

TEST_CASE("state - Вставка 5 монеток (максимум)", "[state][insert]")
{
    with_state_multi::MultiGumballMachine machine(10);

    for (unsigned i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    REQUIRE(machine.GetQuarterCount() == 5);
    REQUIRE(machine.ToString().find("Quarters: 5") != std::string::npos);
}

TEST_CASE("state - Блокировка 6-й монетки", "[state][insert]")
{
    with_state_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    for (unsigned i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    capture.Clear();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 5);
    REQUIRE(capture.GetOutput().find("You can't insert another quarter") != std::string::npos);
}

TEST_CASE("state - Блокировка 7-й монетки", "[state][insert]")
{
    with_state_multi::MultiGumballMachine machine(10);

    for (unsigned i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    machine.InsertQuarter();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 5);
}

TEST_CASE("state - Возврат 1 монетки", "[state][eject]")
{
    with_state_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    machine.InsertQuarter();

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("1 quarter returned") != std::string::npos);
}

TEST_CASE("state - Возврат 3 монеток", "[state][eject]")
{
    with_state_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("3 quarters returned") != std::string::npos);
}

TEST_CASE("state - Возврат 5 монеток (максимум)", "[state][eject]")
{
    with_state_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    for (int i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("5 quarters returned") != std::string::npos);
}

TEST_CASE("state - Возврат из NoQuarter", "[state][eject]")
{
    with_state_multi::MultiGumballMachine machine(10);
    CoutCapture capture;

    machine.EjectQuarter();

    REQUIRE(capture.GetOutput().find("You haven't inserted a quarter") != std::string::npos);
}

TEST_CASE("state - Покупка с 2 монетками подряд", "[state][purchase]")
{
    with_state_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();

    machine.TurnCrank();
    REQUIRE(machine.GetQuarterCount() == 1);
    REQUIRE(machine.ToString().find("9 gumballs") != std::string::npos);

    machine.TurnCrank();
    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(machine.ToString().find("8 gumballs") != std::string::npos);
}

TEST_CASE("state - Покупка с 5 монетками подряд", "[state][purchase]")
{
    with_state_multi::MultiGumballMachine machine(10);

    for (int i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    for (int i = 5; i > 0; --i)
    {
        machine.TurnCrank();
        REQUIRE(machine.GetQuarterCount() == i - 1);
    }

    REQUIRE(machine.ToString().find("5 gumballs") != std::string::npos);
}

TEST_CASE("state - Добавление монетки после покупки", "[state][purchase]")
{
    with_state_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);

    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 2);
}

TEST_CASE("state - Добавление нескольких монеток во время покупок", "[state][purchase]")
{
    with_state_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    REQUIRE(machine.GetQuarterCount() == 4);

    machine.TurnCrank();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 2);
}

TEST_CASE("state - Монеток больше чем жвачек (2 жвачки, 3 монетки)", "[state][soldout]")
{
    with_state_multi::MultiGumballMachine machine(2);
    CoutCapture capture;

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    machine.TurnCrank();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("state - Возврат монеток после распродажи", "[state][soldout]")
{
    with_state_multi::MultiGumballMachine machine(1);
    CoutCapture capture;

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("1 quarter returned") != std::string::npos);
}

TEST_CASE("state - Возврат нескольких монеток после распродажи", "[state][soldout]")
{
    with_state_multi::MultiGumballMachine machine(2);
    CoutCapture capture;

    for (int i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    machine.TurnCrank();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 3);

    capture.Clear();
    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(capture.GetOutput().find("3 quarters returned") != std::string::npos);
}

TEST_CASE("state - Переход Sold -> HasQuarter когда монетки остались", "[state][transitions]")
{
    with_state_multi::MultiGumballMachine machine(5);

    machine.InsertQuarter();
    machine.InsertQuarter();

    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 1);
    REQUIRE(machine.ToString().find("waiting for turn of crank") != std::string::npos);
}

TEST_CASE("state - Переход Sold -> NoQuarter когда монеток не осталось", "[state][transitions]")
{
    with_state_multi::MultiGumballMachine machine(5);

    machine.InsertQuarter();
    machine.TurnCrank();

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(machine.ToString().find("waiting for quarter") != std::string::npos);
}

TEST_CASE("state - Граничный случай: 5 жвачек и 5 монеток", "[state][edge]")
{
    with_state_multi::MultiGumballMachine machine(5);

    for (int i = 0; i < 5; ++i)
    {
        machine.InsertQuarter();
    }

    for (int i = 0; i < 5; ++i)
    {
        machine.TurnCrank();
    }

    REQUIRE(machine.GetQuarterCount() == 0);
    REQUIRE(machine.ToString().find("sold out") != std::string::npos);
}

TEST_CASE("state - Сложный сценарий с чередованием операций", "[state][complex]")
{
    with_state_multi::MultiGumballMachine machine(10);

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.TurnCrank();

    machine.InsertQuarter();
    machine.InsertQuarter();
    machine.InsertQuarter();

    machine.TurnCrank();
    machine.TurnCrank();

    machine.EjectQuarter();

    REQUIRE(machine.GetQuarterCount() == 0);
}