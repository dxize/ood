#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../WeatherData.h"
#include <vector>
#include <string>

class CTestObserverSource : public IObserver<SWeatherInfo>
{
public:
    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        events.emplace_back(sourceId, data.temperature);
    }

    std::vector<std::pair<std::string, double>> events;
};

TEST_CASE("Наблюдатель получает sourceId от обеих станций")
{
    CWeatherData inStation("in");
    CWeatherData outStation("out");

    CTestObserverSource observer;

    inStation.RegisterObserver(observer, 5);
    outStation.RegisterObserver(observer, 5);

    inStation.SetMeasurements(12.34, 0.5, 760.0);

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].first == "in");
    REQUIRE(observer.events[0].second == Approx(12.34));

    outStation.SetMeasurements(-3.2, 0.8, 759.0);

    REQUIRE(observer.events.size() == 2);
    REQUIRE(observer.events[1].first == "out");
    REQUIRE(observer.events[1].second == Approx(-3.2));
}

TEST_CASE("Повторная регистрация на одной станции игнорируется")
{
    CWeatherData station("in");
    CTestObserverSource observer;

    station.RegisterObserver(observer, 5);
    station.RegisterObserver(observer, 10);

    station.SetMeasurements(7.5, 0.6, 760.0);

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].first == "in");
    REQUIRE(observer.events[0].second == Approx(7.5));
}

TEST_CASE("Удалённый наблюдатель не получает уведомлений")
{
    CWeatherData station("out");
    CTestObserverSource observer;

    station.RegisterObserver(observer, 3);
    station.RemoveObserver(observer);

    station.SetMeasurements(1.0, 0.4, 760.0);

    REQUIRE(observer.events.empty());
}
