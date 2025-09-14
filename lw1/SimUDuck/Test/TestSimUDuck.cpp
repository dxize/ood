#include <memory>
#include <cassert>
#include "../lib/Duck/Duck.h"
#include "../lib/Duck/Fly/IFlyBehavior.h"
#include "../lib/Duck/Dance/IDanceBehavior.h"
#include "../lib/Duck/Dance/DanceNoWay.h"
#include "../lib/Duck/Fly/FlyNoWay.h" 
#include "../lib/Duck/Quack/MuteQuackBehavior.h"

class MockFly : public IFlyBehavior
{
public:
    mutable int flyCount = 0;

    void Fly() const override {}

    int GetCountFly() const override
    {
        return flyCount;
    }

    void SetCountFly() override
    {
        flyCount++; 
    }
};

class MockQuack : public IQuackBehavior
{
public:
    mutable int quackCount = 0;

    void Quack() override
    {
        quackCount++;
    }
};

class MockDance : public IDanceBehavior
{
public:
    mutable bool danced = false;

    void Dance() const override
    {
        danced = true;
    }
};

class TestDuck : public Duck
{
public:
    TestDuck(std::unique_ptr<IFlyBehavior>&& fly,
        std::unique_ptr<IQuackBehavior>&& quack,
        std::unique_ptr<IDanceBehavior>&& dance)
        : Duck(std::move(fly), std::move(quack), std::move(dance))
    {}

    void Display() const override {}
};

int main()
{
    //Тест 1: Dance
    auto mockDance1 = std::make_unique<MockDance>();
    MockDance* mockDancePtr1 = mockDance1.get();

    auto testDuck1 = std::make_unique<TestDuck>(
        std::make_unique<FlyNoWay>(),
        std::make_unique<MuteQuackBehavior>(),
        std::move(mockDance1)
    );

    assert(mockDancePtr1->danced == false);
    testDuck1->Dance();
    assert(mockDancePtr1->danced == true);

    // Тест 2: Fly + Quack после каждого 2-го полёта 
    auto mockFly2 = std::make_unique<MockFly>();
    MockFly* flyPtr2 = mockFly2.get();

    auto mockQuack2 = std::make_unique<MockQuack>();
    MockQuack* quackPtr2 = mockQuack2.get();

    auto mockDance2 = std::make_unique<DanceNoWay>(); 

    auto testDuck2 = std::make_unique<TestDuck>(
        std::move(mockFly2),
        std::move(mockQuack2),
        std::move(mockDance2)
    );

    assert(flyPtr2->flyCount == 0);
    assert(quackPtr2->quackCount == 0);

    testDuck2->Fly();
    assert(flyPtr2->flyCount == 1);
    assert(quackPtr2->quackCount == 0);

    testDuck2->Fly();
    assert(flyPtr2->flyCount == 2);
    assert(quackPtr2->quackCount == 1);

    testDuck2->Fly();
    assert(flyPtr2->flyCount == 3);
    assert(quackPtr2->quackCount == 1);

    testDuck2->Fly();
    assert(flyPtr2->flyCount == 4);
    assert(quackPtr2->quackCount == 2);

    return 0;
}
