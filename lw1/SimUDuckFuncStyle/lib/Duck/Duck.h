#ifndef DUCK_H
#define DUCK_H

#include "Fly/IFlyBehavior.h"
#include "Quack/IQuakBehavior.h"
#include "Dance/IDanceBehavior.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

class Duck
{
public:
    Duck(FlyBehavior flyBehavior,
        QuackBehavior quackBehavior,
        DanceBehavior danceBehavior)
        : m_flyBehavior(std::move(flyBehavior))
        , m_quackBehavior(std::move(quackBehavior))
        , m_danceBehavior(std::move(danceBehavior))
    {
        assert(m_flyBehavior.Fly);
        assert(m_flyBehavior.GetCount);
        assert(m_quackBehavior.Quack);
        assert(m_danceBehavior.Dance);
    }

    void Quack() const
    {
        m_quackBehavior.Quack();
    }

    void Swim() const
    {
        std::cout << "I'm swimming" << std::endl;
    }

    void Fly()
    {
        m_flyBehavior.Fly();

        int flightNumber = m_flyBehavior.GetCount();

        if (flightNumber % 2 == 0 && flightNumber != 0)
        {
            m_quackBehavior.Quack();
            m_flyBehavior.Fly();
        }
    }

    void Dance() const
    {
        m_danceBehavior.Dance();
    }

    void SetFlyBehavior(FlyBehavior flyBehavior)
    {
        assert(flyBehavior.Fly);
        assert(flyBehavior.GetCount);
        m_flyBehavior = std::move(flyBehavior);
    }

    void SetQuackBehavior(QuackBehavior quackBehavior)
    {
        assert(quackBehavior.Quack);
        m_quackBehavior = std::move(quackBehavior);
    }

    void SetDanceBehavior(DanceBehavior danceBehavior)
    {
        assert(danceBehavior.Dance);
        m_danceBehavior = std::move(danceBehavior);
    }

    virtual void Display() const = 0;
    virtual ~Duck() = default;

private:
    FlyBehavior m_flyBehavior;
    QuackBehavior m_quackBehavior;
    DanceBehavior m_danceBehavior;
};

#endif