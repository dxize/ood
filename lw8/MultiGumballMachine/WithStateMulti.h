#pragma once

#include <format>
#include <iostream>


// ============================================================================
// –≈¿À»«¿÷»ﬂ — œ¿““≈–ÕŒÃ STATE
// ============================================================================

namespace with_state_multi
{
	struct IState
	{
		virtual void InsertQuarter() = 0;
		virtual void EjectQuarter() = 0;
		virtual void TurnCrank() = 0;
		virtual void Dispense() = 0;
		virtual std::string ToString() const = 0;
		virtual ~IState() = default;
	};

	struct IMultiGumballMachine
	{
		virtual void ReleaseBall() = 0;
		virtual unsigned GetBallCount() const = 0;
		virtual void AddQuarter() = 0;
		virtual unsigned GetQuarterCount() const = 0;
		virtual unsigned GetMaxQuarters() const = 0;
		virtual void ReleaseQuarters() = 0;

		virtual void SetSoldOutState() = 0;
		virtual void SetNoQuarterState() = 0;
		virtual void SetSoldState() = 0;
		virtual void SetHasQuarterState() = 0;

		virtual ~IMultiGumballMachine() = default;
	};

	class SoldState : public IState
	{
	public:
		SoldState(IMultiGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			std::cout << "Please wait, we're already giving you a gumball\n";
		}

		void EjectQuarter() override
		{
			std::cout << "Sorry you already turned the crank\n";
		}

		void TurnCrank() override
		{
			std::cout << "Turning twice doesn't get you another gumball\n";
		}

		void Dispense() override
		{
			m_gumballMachine.ReleaseBall();
			if (m_gumballMachine.GetBallCount() == 0)
			{
				std::cout << "Oops, out of gumballs\n";
				m_gumballMachine.SetSoldOutState();
			}
			else if (m_gumballMachine.GetQuarterCount() > 0)
			{
				m_gumballMachine.SetHasQuarterState();
			}
			else
			{
				m_gumballMachine.SetNoQuarterState();
			}
		}

		std::string ToString() const override
		{
			return "delivering a gumball";
		}

	private:
		IMultiGumballMachine& m_gumballMachine;
	};

	class SoldOutState : public IState
	{
	public:
		SoldOutState(IMultiGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			std::cout << "You can't insert a quarter, the machine is sold out\n";
		}

		void EjectQuarter() override
		{
			if (m_gumballMachine.GetQuarterCount() > 0)
			{
				m_gumballMachine.ReleaseQuarters();
			}
			else
			{
				std::cout << "You can't eject, you haven't inserted a quarter yet\n";
			}
		}

		void TurnCrank() override
		{
			std::cout << "You turned but there's no gumballs\n";
		}

		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}

		std::string ToString() const override
		{
			return "sold out";
		}

	private:
		IMultiGumballMachine& m_gumballMachine;
	};

	class HasQuarterState : public IState
	{
	public:
		HasQuarterState(IMultiGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			if (m_gumballMachine.GetQuarterCount() < m_gumballMachine.GetMaxQuarters())
			{
				m_gumballMachine.AddQuarter();
				std::cout << "You inserted a quarter\n";
			}
			else
			{
				std::cout << "You can't insert another quarter\n";
			}
		}

		void EjectQuarter() override
		{
			m_gumballMachine.ReleaseQuarters();
			m_gumballMachine.SetNoQuarterState();
		}

		void TurnCrank() override
		{
			std::cout << "You turned...\n";
			m_gumballMachine.SetSoldState();
		}

		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}

		std::string ToString() const override
		{
			return "waiting for turn of crank";
		}

	private:
		IMultiGumballMachine& m_gumballMachine;
	};

	class NoQuarterState : public IState
	{
	public:
		NoQuarterState(IMultiGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			m_gumballMachine.AddQuarter();
			std::cout << "You inserted a quarter\n";
			m_gumballMachine.SetHasQuarterState();
		}

		void EjectQuarter() override
		{
			std::cout << "You haven't inserted a quarter\n";
		}

		void TurnCrank() override
		{
			std::cout << "You turned but there's no quarter\n";
		}

		void Dispense() override
		{
			std::cout << "You need to pay first\n";
		}

		std::string ToString() const override
		{
			return "waiting for quarter";
		}

	private:
		IMultiGumballMachine& m_gumballMachine;
	};

	class MultiGumballMachine : private IMultiGumballMachine
	{
	public:
		static constexpr unsigned MAX_QUARTERS = 5;

		MultiGumballMachine(unsigned numBalls)
			: m_soldState(*this)
			, m_soldOutState(*this)
			, m_noQuarterState(*this)
			, m_hasQuarterState(*this)
			, m_state(&m_soldOutState)
			, m_count(numBalls)
			, m_quarterCount(0)
		{
			if (m_count > 0)
			{
				m_state = &m_noQuarterState;
			}
		}

		void EjectQuarter()
		{
			m_state->EjectQuarter();
		}

		void InsertQuarter()
		{
			m_state->InsertQuarter();
		}

		void TurnCrank()
		{
			m_state->TurnCrank();
			m_state->Dispense();
		}

		std::string ToString() const
		{
			return std::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with multi-quarter support)
Inventory: {} gumball{}
Quarters: {}
Machine is {}
)",
m_count, m_count != 1 ? "s" : "", m_quarterCount, m_state->ToString());
		}

		unsigned GetQuarterCount() const
		{
			return m_quarterCount;
		}

	private:
		unsigned GetBallCount() const override
		{
			return m_count;
		}

		void ReleaseBall() override
		{
			if (m_count != 0)
			{
				std::cout << "A gumball comes rolling out the slot...\n";
				--m_count;
				if (m_quarterCount > 0)
				{
					--m_quarterCount;
				}
			}
		}

		void AddQuarter() override
		{
			if (m_quarterCount < MAX_QUARTERS)
			{
				++m_quarterCount;
			}
		}

		unsigned GetMaxQuarters() const override
		{
			return MAX_QUARTERS;
		}

		void ReleaseQuarters() override
		{
			std::cout << std::format("{} quarter{} returned\n", m_quarterCount, m_quarterCount != 1 ? "s" : "");
			m_quarterCount = 0;
		}

		void SetSoldOutState() override
		{
			m_state = &m_soldOutState;
		}

		void SetNoQuarterState() override
		{
			m_state = &m_noQuarterState;
		}

		void SetSoldState() override
		{
			m_state = &m_soldState;
		}

		void SetHasQuarterState() override
		{
			m_state = &m_hasQuarterState;
		}

	private:
		unsigned m_count;
		unsigned m_quarterCount;
		SoldState m_soldState;
		SoldOutState m_soldOutState;
		NoQuarterState m_noQuarterState;
		HasQuarterState m_hasQuarterState;
		IState* m_state;
	};
} // namespace with_state_multi