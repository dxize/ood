#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include <limits>
#include "Observer.h"

// --- структура для обычных данных ---
struct SWeatherInfo
{
    double temperature = 0;
    double humidity = 0;
    double pressure = 0;
};

// --- структура для Pro-версии (включает ветер) ---
struct SWeatherInfoPro
{
    double temperature = 0;
    double humidity = 0;
    double pressure = 0;
    double windSpeed = 0;     // м/с
    double windDirection = 0; // градусы
};

// --- статистика по числовому параметру ---
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

// --- статистика по ветру ---
class CWindStats
{
public:
    void Update(double speed, double directionDeg)
    {
        m_speedStats.Update(speed);
        double rad = directionDeg * M_PI / 180.0;
        m_sumX += std::cos(rad);
        m_sumY += std::sin(rad);
        ++m_count;
    }

    double GetAverageSpeed() const { return m_speedStats.GetAverage(); }

    double GetAverageDirection() const
    {
        if (m_count == 0)
            return 0;
        double avgRad = std::atan2(m_sumY / m_count, m_sumX / m_count);
        double avgDeg = avgRad * 180.0 / M_PI;
        if (avgDeg < 0)
            avgDeg += 360.0;
        return avgDeg;
    }

    double GetMinSpeed() const { return m_speedStats.GetMin(); }
    double GetMaxSpeed() const { return m_speedStats.GetMax(); }

private:
    CStatsData m_speedStats;
    double m_sumX = 0;
    double m_sumY = 0;
    unsigned m_count = 0;
};

// --- обычный дисплей (поддерживает обе версии) ---
class CDisplay : public IObserver<SWeatherInfo>, public IObserver<SWeatherInfoPro>
{
private:
    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        std::cout << "Display [" << sourceId << "]:" << std::endl;
        std::cout << "  Temp=" << data.temperature << std::endl;
        std::cout << "  Hum=" << data.humidity << std::endl;
        std::cout << "  Pres=" << data.pressure << std::endl;
        std::cout << "----------------" << std::endl;
    }

    void Update(SWeatherInfoPro const& data, const std::string& sourceId) override
    {
        std::cout << "Display PRO [" << sourceId << "]:" << std::endl;
        std::cout << "  Temp=" << data.temperature << std::endl;
        std::cout << "  Hum=" << data.humidity << std::endl;
        std::cout << "  Pres=" << data.pressure << std::endl;
        std::cout << "  Wind speed=" << data.windSpeed << " m/s" << std::endl;
        std::cout << "  Wind dir=" << data.windDirection << "°" << std::endl;
        std::cout << "----------------" << std::endl;
    }
};

// --- статистический дисплей (поддерживает обе версии) ---
class CStatsDisplay : public IObserver<SWeatherInfo>, public IObserver<SWeatherInfoPro>
{
private:
    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        UpdateCommon(data.temperature, data.humidity, data.pressure);
        Print(sourceId, false);
    }

    void Update(SWeatherInfoPro const& data, const std::string& sourceId) override
    {
        UpdateCommon(data.temperature, data.humidity, data.pressure);
        m_windStats.Update(data.windSpeed, data.windDirection);
        Print(sourceId, true);
    }

    void UpdateCommon(double t, double h, double p)
    {
        m_stats["Temp"].Update(t);
        m_stats["Hum"].Update(h);
        m_stats["Pressure"].Update(p);
    }

    void Print(const std::string& sourceId, bool showWind)
    {
        std::cout << "Stats Display [" << sourceId << "]:" << std::endl;
        for (const auto& kv : m_stats)
        {
            const auto& name = kv.first;
            const auto& stat = kv.second;
            std::cout << "  " << name << ": Min=" << stat.GetMin()
                << " Max=" << stat.GetMax()
                << " Avg=" << stat.GetAverage() << std::endl;
        }
        if (showWind)
        {
            std::cout << "  Wind: Min=" << m_windStats.GetMinSpeed()
                << " Max=" << m_windStats.GetMaxSpeed()
                << " Avg=" << m_windStats.GetAverageSpeed()
                << " Dir=" << m_windStats.GetAverageDirection() << "°" << std::endl;
        }
        std::cout << "----------------" << std::endl;
    }

    std::map<std::string, CStatsData> m_stats;
    CWindStats m_windStats;
};

// --- обычная станция ---
class CWeatherData : public CObservable<SWeatherInfo>
{
public:
    explicit CWeatherData(std::string id = "unknown") : m_id(std::move(id)) {}

    void SetMeasurements(double t, double h, double p)
    {
        m_temperature = t;
        m_humidity = h;
        m_pressure = p;
        NotifyObservers();
    }

    std::string GetId() const override { return m_id; }

protected:
    SWeatherInfo GetChangedData() const override
    {
        return { m_temperature, m_humidity, m_pressure };
    }

private:
    std::string m_id;
    double m_temperature = 0, m_humidity = 0, m_pressure = 760;
};

// --- PRO станция (с ветром) ---
class CWeatherDataPro : public CObservable<SWeatherInfoPro>
{
public:
    explicit CWeatherDataPro(std::string id = "unknown") : m_id(std::move(id)) {}

    void SetMeasurements(double t, double h, double p, double windSpeed, double windDir)
    {
        m_temperature = t;
        m_humidity = h;
        m_pressure = p;
        m_windSpeed = windSpeed;
        m_windDir = windDir;
        NotifyObservers();
    }

    std::string GetId() const override { return m_id; }

protected:
    SWeatherInfoPro GetChangedData() const override
    {
        return { m_temperature, m_humidity, m_pressure, m_windSpeed, m_windDir };
    }

private:
    std::string m_id;
    double m_temperature = 0, m_humidity = 0, m_pressure = 760;
    double m_windSpeed = 0, m_windDir = 0;
};
