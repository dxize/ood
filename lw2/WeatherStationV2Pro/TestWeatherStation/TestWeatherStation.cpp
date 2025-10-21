#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../WeatherData.h"
#include <vector>
#include <string>

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


TEST_CASE("Наблюдатели уведомляются в порядке приоритета (реальный лог вызовов)", "[observer]")
{
    CWeatherData wd;

    std::vector<std::string> callLog;

    class CTestObserverLog : public IObserver<SWeatherInfo>
    {
    public:
        CTestObserverLog(std::string name, std::vector<std::string>& log)
            : m_name(std::move(name)), m_log(log) {}

        void Update(SWeatherInfo const& data) override
        {
            m_log.push_back(m_name);
        }

    private:
        std::string m_name;
        std::vector<std::string>& m_log;
    };

    CTestObserverLog low("Low", callLog);
    CTestObserverLog mid("Mid", callLog);
    CTestObserverLog high("High", callLog);

    wd.RegisterObserver(low, 1);
    wd.RegisterObserver(mid, 5);
    wd.RegisterObserver(high, 10);

    wd.SetMeasurements(25, 0.7, 760);

    REQUIRE(callLog.size() == 3);

    std::vector<std::string> expectedOrder = { "High", "Mid", "Low" };
    REQUIRE(callLog == expectedOrder);
}

TEST_CASE("Повторная регистрация наблюдателя игнорируется", "[observer]")
{
    CWeatherData wd;
    CTestObserver obs("Unique");

    wd.RegisterObserver(obs, 5);
    wd.RegisterObserver(obs, 10);

    wd.SetMeasurements(42, 0.8, 761);

    REQUIRE(obs.notifications.size() == 1);
    REQUIRE(obs.lastTemp == Approx(42.0));
}

TEST_CASE("Удалённый наблюдатель не получает уведомления", "[observer]")
{
    CWeatherData wd;
    CTestObserver obs("Temp");

    wd.RegisterObserver(obs, 3);
    wd.RemoveObserver(obs);

    wd.SetMeasurements(10, 0.5, 760);

    REQUIRE(obs.notifications.empty());
}
