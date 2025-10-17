#pragma once

#include <iostream>
#include <limits>
#include <map>
#include <string>
#include "Observer.h"

struct SWeatherInfo
{
    double temperature = 0;
    double humidity = 0;
    double pressure = 0;
};

// Статистика по одному параметру
class CStatsData
{
public:
    void Update(double value)
    {
        if (m_min > value) m_min = value;
        if (m_max < value) m_max = value;
        m_acc += value;
        ++m_count;
    }

    bool HasData() const { return m_count != 0; }
    double GetMin() const { return HasData() ? m_min : 0; }
    double GetMax() const { return HasData() ? m_max : 0; }
    double GetAverage() const { return m_count ? (m_acc / m_count) : 0; }

private:
    double m_min = std::numeric_limits<double>::infinity();
    double m_max = -std::numeric_limits<double>::infinity();
    double m_acc = 0;
    unsigned m_count = 0;
};

// CDisplay — выводит данные и метку источника (sourceId)
class CDisplay : public IObserver<SWeatherInfo>
{
public:
    // конструктор с опциональным заголовком дисплея
    explicit CDisplay(std::string title = {}) : m_title(std::move(title)) {}

    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        if (!m_title.empty())
        {
            std::cout << m_title << " ";
        }
        std::cout << "[" << sourceId << "] "
            << "Temp=" << data.temperature
            << " Hum=" << data.humidity
            << " Pres=" << data.pressure
            << std::endl;
    }

private:
    std::string m_title;
};

// CStatsDisplay — собирает статистику по показателям, отмечая источник в выводе
class CStatsDisplay : public IObserver<SWeatherInfo>
{
public:
    explicit CStatsDisplay(std::string title = {}) : m_title(std::move(title)) {}

    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        auto& statTemp = m_stats["Temp"];
        auto& statHum = m_stats["Hum"];
        auto& statPressure = m_stats["Pressure"];

        statTemp.Update(data.temperature);
        statHum.Update(data.humidity);
        statPressure.Update(data.pressure);

        Print(sourceId);
    }

    void Print(const std::string& sourceId)
    {
        if (!m_title.empty())
        {
            std::cout << m_title << " ";
        }
        std::cout << "[" << sourceId << "] Stats:" << std::endl;
        for (const auto& kv : m_stats)
        {
            const std::string& name = kv.first;
            const CStatsData& stat = kv.second;

            if (stat.HasData())
            {
                std::cout << "  Max " << name << " " << stat.GetMax() << std::endl;
                std::cout << "  Min " << name << " " << stat.GetMin() << std::endl;
                std::cout << "  Average " << name << " " << stat.GetAverage() << std::endl;
            }
            else
            {
                std::cout << "  No " << name << " data" << std::endl;
            }
        }
    }

private:
    std::map<std::string, CStatsData> m_stats;
    std::string m_title;
};

// Датчик погоды — теперь хранит id и реализует GetId()
class CWeatherData : public CObservable<SWeatherInfo>
{
public:
    explicit CWeatherData(std::string id = "unknown")
        : m_id(std::move(id))
    {
    }

    double GetTemperature() const { return m_temperature; }
    double GetHumidity() const { return m_humidity; }
    double GetPressure() const { return m_pressure; }

    void MeasurementsChanged() { NotifyObservers(); }

    void SetMeasurements(double temp, double humidity, double pressure)
    {
        m_temperature = temp;
        m_humidity = humidity;
        m_pressure = pressure;
        MeasurementsChanged();
    }

    // реализация GetId() из IObservable
    std::string GetId() const { return m_id; }

protected:
    SWeatherInfo GetChangedData() const override
    {
        SWeatherInfo info;
        info.temperature = GetTemperature();
        info.humidity = GetHumidity();
        info.pressure = GetPressure();
        return info;
    }

private:
    std::string m_id;
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;
};
