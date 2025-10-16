#define CATCH_CONFIG_MAIN
#include "../../../catch.hpp"
#include "../WeatherData.h"
#include <vector>
#include <string>

// ----------------------------------------
// Вспомогательный тестовый наблюдатель
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
// Тест №1 — Проверка порядка вызова наблюдателей по приоритетам (реальный лог вызовов)
// ----------------------------------------
TEST_CASE("Наблюдатели уведомляются в порядке приоритета (реальный лог вызовов)", "[observer]")
{
    CWeatherData wd;

    // общий лог, в который будут писать все наблюдатели в порядке вызова
    std::vector<std::string> callLog;

    // Наблюдатель, пишущий в общий лог (локальный класс внутри теста)
    class CTestObserverLog : public IObserver<SWeatherInfo>
    {
    public:
        CTestObserverLog(std::string name, std::vector<std::string>& log)
            : m_name(std::move(name)), m_log(log) {}

        void Update(SWeatherInfo const& /*data*/) override
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

    // Все трое должны были быть вызваны
    REQUIRE(callLog.size() == 3);

    // Проверяем фактический порядок вызовов: от высокого приоритета к низкому
    std::vector<std::string> expectedOrder = { "High", "Mid", "Low" };
    REQUIRE(callLog == expectedOrder);
}

// ----------------------------------------
// Тест №2 — Повторная регистрация не создаёт дубликатов
// ----------------------------------------
TEST_CASE("Повторная регистрация наблюдателя игнорируется", "[observer]")
{
    CWeatherData wd;
    CTestObserver obs("Unique");

    wd.RegisterObserver(obs, 5);
    wd.RegisterObserver(obs, 10); // попытка повторной регистрации

    wd.SetMeasurements(42, 0.8, 761);

    REQUIRE(obs.notifications.size() == 1);
    REQUIRE(obs.lastTemp == Approx(42.0));
}

// ----------------------------------------
// Тест №3 — Удалённый наблюдатель не получает обновления
// ----------------------------------------
TEST_CASE("Удалённый наблюдатель не получает уведомления", "[observer]")
{
    CWeatherData wd;
    CTestObserver obs("Temp");

    wd.RegisterObserver(obs, 3);
    wd.RemoveObserver(obs);

    wd.SetMeasurements(10, 0.5, 760);

    REQUIRE(obs.notifications.empty());
}
