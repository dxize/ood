#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../WeatherData.h"
#include <vector>
#include <string>

// ¬спомогательный наблюдатель, фиксирующий (sourceId, temperature) в пор€дке получени€
class CTestObserverSource : public IObserver<SWeatherInfo>
{
public:
    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        events.emplace_back(sourceId, data.temperature);
    }

    std::vector<std::pair<std::string, double>> events;
};

// “ест 1: один наблюдатель подписан на две станции Ч получает событи€ от "in" и "out"
TEST_CASE("Observer receives sourceId from both stations", "[observer][sourceId]")
{
    CWeatherData inStation("in");
    CWeatherData outStation("out");

    CTestObserverSource observer;

    // –егистрируем одного и того же наблюдател€ на обе станции
    inStation.RegisterObserver(observer, 5);
    outStation.RegisterObserver(observer, 5);

    // —игнал от внутренней станции
    inStation.SetMeasurements(12.34, 0.5, 760.0);

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].first == "in");
    REQUIRE(observer.events[0].second == Approx(12.34));

    // —игнал от внешней станции
    outStation.SetMeasurements(-3.2, 0.8, 759.0);

    REQUIRE(observer.events.size() == 2);
    REQUIRE(observer.events[1].first == "out");
    REQUIRE(observer.events[1].second == Approx(-3.2));
}

// “ест 2: повторна€ регистраци€ на той же станции игнорируетс€ (нет дублей)
TEST_CASE("Duplicate registration ignored (per observable)", "[observer][registration]")
{
    CWeatherData station("in");
    CTestObserverSource observer;

    station.RegisterObserver(observer, 5);
    station.RegisterObserver(observer, 10); // попытка повторной регистрации должна быть проигнорирована

    station.SetMeasurements(7.5, 0.6, 760.0);

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].first == "in");
    REQUIRE(observer.events[0].second == Approx(7.5));
}

// “ест 3: удалЄнный наблюдатель не получает уведомлений
TEST_CASE("Removed observer does not receive notifications", "[observer][remove]")
{
    CWeatherData station("out");
    CTestObserverSource observer;

    station.RegisterObserver(observer, 3);
    station.RemoveObserver(observer);

    station.SetMeasurements(1.0, 0.4, 760.0);

    REQUIRE(observer.events.empty());
}
