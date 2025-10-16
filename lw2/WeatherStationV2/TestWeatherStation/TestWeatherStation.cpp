#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../WeatherData.h"
#include <vector>
#include <string>

// ----------------------------------------
// ��������������� �������� �����������
// ----------------------------------------
class CTestObserver : public IObserver<SWeatherInfo>
{
public:
    explicit CTestObserver(std::string name)
        : m_name(std::move(name)) {}

    void Update(SWeatherInfo const& data) override
    {
        notifications.push_back(m_name);
        lastTemp = data.temperature;
    }

    std::vector<std::string> notifications;
    double lastTemp = 0.0;
    std::string m_name;
};

// ----------------------------------------
// ���� �1 � �������� ������� ������ ������������ �� �����������
// ----------------------------------------
TEST_CASE("Observers are notified in order of priority", "[observer]")
{
    CWeatherData wd;

    CTestObserver low("Low");
    CTestObserver mid("Mid");
    CTestObserver high("High");

    wd.RegisterObserver(low, 1);
    wd.RegisterObserver(mid, 5);
    wd.RegisterObserver(high, 10);

    wd.SetMeasurements(25, 0.7, 760);

    // ���������, ��� ��� �������� �����������
    REQUIRE(low.notifications.size() == 1);
    REQUIRE(mid.notifications.size() == 1);
    REQUIRE(high.notifications.size() == 1);

    // ��������� ������� (�� �����������)
    std::vector<std::string> actualOrder = {
        high.notifications.front(),
        mid.notifications.front(),
        low.notifications.front()
    };

    std::vector<std::string> expectedOrder = { "High", "Mid", "Low" };
    REQUIRE(actualOrder == expectedOrder);
}

// ----------------------------------------
// ���� �2 � ��������� ����������� �� ������ ����������
// ----------------------------------------
TEST_CASE("Duplicate observer registration is ignored", "[observer]")
{
    CWeatherData wd;
    CTestObserver obs("Unique");

    wd.RegisterObserver(obs, 5);
    wd.RegisterObserver(obs, 10); // ������� ��������� �����������

    wd.SetMeasurements(42, 0.8, 761);

    REQUIRE(obs.notifications.size() == 1);
    REQUIRE(obs.lastTemp == Approx(42.0));
}

// ----------------------------------------
// ���� �3 � �������� ����������� �� �������� ����������
// ----------------------------------------
TEST_CASE("Removed observer does not receive notifications", "[observer]")
{
    CWeatherData wd;
    CTestObserver obs("Temp");

    wd.RegisterObserver(obs, 3);
    wd.RemoveObserver(obs);

    wd.SetMeasurements(10, 0.5, 760);

    REQUIRE(obs.notifications.empty());
}
