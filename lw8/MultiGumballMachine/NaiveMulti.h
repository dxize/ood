#pragma once

#include <format>
#include <iostream>

// ============================================================================
// ÍÀÈÂÍÀß ÐÅÀËÈÇÀÖÈß
// ============================================================================

namespace naive_multi
{
	class MultiGumballMachine
	{
	public:
		enum class State
		{
			SoldOut,
			NoQuarter,
			HasQuarter,
			Sold,
		};

		static constexpr unsigned MAX_QUARTERS = 5;

		MultiGumballMachine(unsigned count)
			: m_count(count)
			, m_quarterCount(0)
			, m_state(count > 0 ? State::NoQuarter : State::SoldOut)
		{
		}

		void InsertQuarter()
		{
			using namespace std;
			switch (m_state)
			{
			case State::SoldOut:
				cout << "You can't insert a quarter, the machine is sold out\n";
				break;
			case State::NoQuarter:
				if (m_quarterCount < MAX_QUARTERS)
				{
					++m_quarterCount;
					cout << "You inserted a quarter\n";
					m_state = State::HasQuarter;
				}
				else
				{
					cout << "You can't insert another quarter\n";
				}
				break;
			case State::HasQuarter:
				if (m_quarterCount < MAX_QUARTERS)
				{
					++m_quarterCount;
					cout << "You inserted a quarter\n";
				}
				else
				{
					cout << "You can't insert another quarter\n";
				}
				break;
			case State::Sold:
				cout << "Please wait, we're already giving you a gumball\n";
				break;
			}
		}

		void EjectQuarter()
		{
			using namespace std;
			switch (m_state)
			{
			case State::HasQuarter:
				cout << std::format("{} quarter{} returned\n", m_quarterCount, m_quarterCount != 1 ? "s" : "");
				m_quarterCount = 0;
				m_state = State::NoQuarter;
				break;
			case State::NoQuarter:
				cout << "You haven't inserted a quarter\n";
				break;
			case State::Sold:
				cout << "Sorry you already turned the crank\n";
				break;
			case State::SoldOut:
				if (m_quarterCount > 0)
				{
					cout << std::format("{} quarter{} returned\n", m_quarterCount, m_quarterCount != 1 ? "s" : "");
					m_quarterCount = 0;
				}
				else
				{
					cout << "You can't eject, you haven't inserted a quarter yet\n";
				}
				break;
			}
		}

		void TurnCrank()
		{
			using namespace std;
			switch (m_state)
			{
			case State::SoldOut:
				cout << "You turned but there's no gumballs\n";
				break;
			case State::NoQuarter:
				cout << "You turned but there's no quarter\n";
				break;
			case State::HasQuarter:
				cout << "You turned...\n";
				m_state = State::Sold;
				Dispense();
				break;
			case State::Sold:
				cout << "Turning twice doesn't get you another gumball\n";
				break;
			}
		}

		std::string ToString() const
		{
			std::string state = (m_state == State::SoldOut)
				? "sold out"
				: (m_state == State::NoQuarter) ? "waiting for quarter"
				: (m_state == State::HasQuarter) ? "waiting for turn of crank"
				: "delivering a gumball";

			return std::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with multi-quarter support)
Inventory: {} gumball{}
Quarters: {}
Machine is {}
)",
m_count, m_count != 1 ? "s" : "", m_quarterCount, state);
		}

		unsigned GetQuarterCount() const
		{
			return m_quarterCount;
		}

	private:
		void Dispense()
		{
			using namespace std;
			switch (m_state)
			{
			case State::Sold:
				cout << "A gumball comes rolling out the slot\n";
				--m_count;
				--m_quarterCount;

				if (m_count == 0)
				{
					cout << "Oops, out of gumballs\n";
					m_state = State::SoldOut;
				}
				else if (m_quarterCount > 0)
				{
					m_state = State::HasQuarter;
				}
				else
				{
					m_state = State::NoQuarter;
				}
				break;
			case State::NoQuarter:
				cout << "You need to pay first\n";
				break;
			case State::SoldOut:
			case State::HasQuarter:
				cout << "No gumball dispensed\n";
				break;
			}
		}

		unsigned m_count;
		unsigned m_quarterCount;
		State m_state;
	};
} // namespace naive_multi