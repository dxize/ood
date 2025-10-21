#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../WeatherData.h"
#include "../Observer.h"

class CSelfRemovingObserver : public IObserver<SWeatherInfo>
{
public:
    CSelfRemovingObserver(CObservable<SWeatherInfo>& observable)
        : m_observable(observable)
    {
    }

    void Update(SWeatherInfo const& data) override
    {
        ++m_updateCount;
        m_observable.RemoveObserver(*this);
    }

    int GetUpdateCount() const { return m_updateCount; }

private:
    CObservable<SWeatherInfo>& m_observable;
    int m_updateCount = 0;
};

class CCountingObserver : public IObserver<SWeatherInfo>
{
public:
    void Update(SWeatherInfo const& data) override
    {
        ++m_updateCount;
    }

    int GetUpdateCount() const { return m_updateCount; }

private:
    int m_updateCount = 0;
};

TEST_CASE("Наблюдатель безопасно удаляет себя во время уведомления", "[WeatherStation]")
{
    CWeatherData weatherData;

    CSelfRemovingObserver selfRemover(weatherData);
    CCountingObserver counter;

    weatherData.RegisterObserver(selfRemover);
    weatherData.RegisterObserver(counter);


    SECTION("После удаления уведомляются только оставшиеся наблюдатели")
    {
        weatherData.SetMeasurements(25.0, 0.5, 760.0);
        weatherData.SetMeasurements(20.0, 0.6, 761.0);

        REQUIRE(selfRemover.GetUpdateCount() == 1);  
        REQUIRE(counter.GetUpdateCount() == 2);
    }
}
