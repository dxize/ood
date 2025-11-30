#include "WithStateMulti.h"
#include "NaiveMulti.h"
#include <iostream>
#include <Windows.h>

using namespace std;

template <typename GumballMachineType>
void TestGumballMachine(GumballMachineType& m)
{
	cout << m.ToString() << endl;

	m.InsertQuarter();
	m.TurnCrank();

	cout << m.ToString() << endl;

	m.InsertQuarter();
	m.EjectQuarter();
	m.TurnCrank();

	cout << m.ToString() << endl;

	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();
	m.EjectQuarter();

	cout << m.ToString() << endl;

	m.InsertQuarter();
	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();

	cout << m.ToString() << endl;
}

template <typename GumballMachineType>
void TestMaxQuarters(GumballMachineType& m)
{
	cout << "\n=== Тест: До 5 монеток ===\n";
	cout << m.ToString() << endl;

	cout << "Вставляем 5 монеток:\n";
	for (int i = 0; i < 5; ++i)
	{
		m.InsertQuarter();
	}
	cout << m.ToString() << endl;

	cout << "Пытаемся вставить 6-ю:\n";
	m.InsertQuarter();
	cout << m.ToString() << endl;
}

template <typename GumballMachineType>
void TestMoreQuartersThanGumballs(GumballMachineType& m)
{
	cout << "\n=== Тест: Монеток больше чем жвачек ===\n";
	cout << m.ToString() << endl;

	cout << "Вставляем 3 монетки (жвачек осталось 2):\n";
	m.InsertQuarter();
	m.InsertQuarter();
	m.InsertQuarter();
	cout << m.ToString() << endl;

	cout << "Покупаем последние 2 жвачки:\n";
	m.TurnCrank();
	m.TurnCrank();
	cout << m.ToString() << endl;

	cout << "Возвращаем оставшуюся монетку:\n";
	m.EjectQuarter();
	cout << m.ToString() << endl;
}

void TestNaiveGumballMachine()
{
	cout << "\n\nНАИВНАЯ РЕАЛИЗАЦИЯ\n";

	naive_multi::MultiGumballMachine m(5);
	TestGumballMachine(m);

	naive_multi::MultiGumballMachine m2(10);
	TestMaxQuarters(m2);

	naive_multi::MultiGumballMachine m3(2);
	TestMoreQuartersThanGumballs(m3);
}

void TestGumballMachineWithState()
{
	cout << "\n\nПАТТЕРН STATE\n";

	with_state_multi::MultiGumballMachine m(5);
	TestGumballMachine(m);

	with_state_multi::MultiGumballMachine m2(10);
	TestMaxQuarters(m2);

	with_state_multi::MultiGumballMachine m3(2);
	TestMoreQuartersThanGumballs(m3);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	TestNaiveGumballMachine();
	cout << "\n-----------------\n";
	TestGumballMachineWithState();

	return 0;
}