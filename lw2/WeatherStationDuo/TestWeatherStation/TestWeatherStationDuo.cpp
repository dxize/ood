#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../WeatherData.h"
#include <vector>
#include <string>

// ��������������� �����������, ����������� (sourceId, temperature) � ������� ���������
class CTestObserverSource : public IObserver<SWeatherInfo>
{
public:
    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        events.emplace_back(sourceId, data.temperature);
    }

    std::vector<std::pair<std::string, double>> events;
};

// ���� 1: ���� ����������� �������� �� ��� ������� � �������� ������� �� "in" � "out"
TEST_CASE("Observer receives sourceId from both stations", "[observer][sourceId]")
{
    CWeatherData inStation("in");
    CWeatherData outStation("out");

    CTestObserverSource observer;

    // ������������ ������ � ���� �� ����������� �� ��� �������
    inStation.RegisterObserver(observer, 5);
    outStation.RegisterObserver(observer, 5);

    // ������ �� ���������� �������
    inStation.SetMeasurements(12.34, 0.5, 760.0);

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].first == "in");
    REQUIRE(observer.events[0].second == Approx(12.34));

    // ������ �� ������� �������
    outStation.SetMeasurements(-3.2, 0.8, 759.0);

    REQUIRE(observer.events.size() == 2);
    REQUIRE(observer.events[1].first == "out");
    REQUIRE(observer.events[1].second == Approx(-3.2));
}

// ���� 2: ��������� ����������� �� ��� �� ������� ������������ (��� ������)
TEST_CASE("Duplicate registration ignored (per observable)", "[observer][registration]")
{
    CWeatherData station("in");
    CTestObserverSource observer;

    station.RegisterObserver(observer, 5);
    station.RegisterObserver(observer, 10); // ������� ��������� ����������� ������ ���� ���������������

    station.SetMeasurements(7.5, 0.6, 760.0);

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].first == "in");
    REQUIRE(observer.events[0].second == Approx(7.5));
}

// ���� 3: �������� ����������� �� �������� �����������
TEST_CASE("Removed observer does not receive notifications", "[observer][remove]")
{
    CWeatherData station("out");
    CTestObserverSource observer;

    station.RegisterObserver(observer, 3);
    station.RemoveObserver(observer);

    station.SetMeasurements(1.0, 0.4, 760.0);

    REQUIRE(observer.events.empty());
}
