#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <cmath>
#include "Observer.h"

struct SWeatherInfo
{
    double temperature = 0;
    double humidity = 0;
    double pressure = 0;
    double windSpeed = 0;       // м/с
    double windDirection = 0;   // градусы (0 — север)
};

class CStatsData
{
public:
    void Update(double value)
    {
        if (m_min > value)
            m_min = value;
        if (m_max < value)
            m_max = value;
        m_acc += value;
        ++m_count;
    }

    bool HasData() const 
    { 
        return m_count != 0; 
    }
    double GetMin() const 
    { 
        return HasData() ? m_min : 0;
    }
    double GetMax() const
    { 
        return HasData() ? m_max : 0; 
    }
    double GetAverage() const 
    { 
        return m_count ? (m_acc / m_count) : 0; 
    }

private:
    double m_min = std::numeric_limits<double>::infinity();
    double m_max = -std::numeric_limits<double>::infinity();
    double m_acc = 0;
    unsigned m_count = 0;
};

// --- статистика по ветру ---
class CWindStatsData
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

    double GetAverageSpeed() const 
    { 
        return m_speedStats.GetAverage();
    }

    double GetAverageDirection() const
    {
        if (m_count == 0)
        {
            return 0;
        }
        double avgRad = std::atan2(m_sumY / m_count, m_sumX / m_count);
        double avgDeg = avgRad * 180.0 / M_PI;
        if (avgDeg < 0)
        {
            avgDeg += 360.0;
        }
        return avgDeg;
    }

    double GetMinSpeed() const 
    { 
        return m_speedStats.GetMin();
    }
    double GetMaxSpeed() const 
    { 
        return m_speedStats.GetMax(); 
    }

private:
    CStatsData m_speedStats;
    double m_sumX = 0.0;
    double m_sumY = 0.0;
    unsigned m_count = 0;
};

// --- обычный дисплей ---
class CDisplay : public IObserver<SWeatherInfo>
{
private:
    void Update(SWeatherInfo const& data) override
    {
        std::cout << "Current Temp " << data.temperature << std::endl;
        std::cout << "Current Hum " << data.humidity << std::endl;
        std::cout << "Current Pressure " << data.pressure << std::endl;
        std::cout << "Wind Speed " << data.windSpeed << " m/s" << std::endl;
        std::cout << "Wind Direction " << data.windDirection << "°" << std::endl;
        std::cout << "----------------" << std::endl;
    }
};

// --- статистический дисплей ---
class CStatsDisplay : public IObserver<SWeatherInfo>
{
private:
    void Update(SWeatherInfo const& data) override
    {
        auto& statTemp = m_stats["Temp"];
        auto& statHum = m_stats["Hum"];
        auto& statPressure = m_stats["Pressure"];

        statTemp.Update(data.temperature);
        statHum.Update(data.humidity);
        statPressure.Update(data.pressure);
        m_windStats.Update(data.windSpeed, data.windDirection);

        Print();
    }

    void Print()
    {
        for (const auto& kv : m_stats)
        {
            const std::string& name = kv.first;
            const CStatsData& stat = kv.second;

            if (stat.HasData())
            {
                std::cout << "Max " << name << " " << stat.GetMax() << std::endl;
                std::cout << "Min " << name << " " << stat.GetMin() << std::endl;
                std::cout << "Average " << name << " " << stat.GetAverage() << std::endl;
            }
            else
            {
                std::cout << "No " << name << " data" << std::endl;
            }
        }

        std::cout << "--- Wind ---" << std::endl;
        std::cout << "Max speed: " << m_windStats.GetMaxSpeed() << " m/s" << std::endl;
        std::cout << "Min speed: " << m_windStats.GetMinSpeed() << " m/s" << std::endl;
        std::cout << "Average speed: " << m_windStats.GetAverageSpeed() << " m/s" << std::endl;
        std::cout << "Average direction: " << m_windStats.GetAverageDirection() << "°" << std::endl;
        std::cout << "----------------" << std::endl;
    }

    std::map<std::string, CStatsData> m_stats;
    CWindStatsData m_windStats;
};

// --- источник данных ---
class CWeatherData : public CObservable<SWeatherInfo>
{
public:
    double GetTemperature() const 
    { 
        return m_temperature;
    }
    double GetHumidity() const 
    { 
        return m_humidity; 
    }
    double GetPressure() const 
    { 
        return m_pressure;
    }
    double GetWindSpeed() const
    {
        return m_windSpeed; 
    }
    double GetWindDirection() const 
    {
        return m_windDirection; 
    }

    void MeasurementsChanged()
    {
        NotifyObservers();
    }

    void SetMeasurements(double temp, double humidity, double pressure,
        double windSpeed, double windDirection)
    {
        m_temperature = temp;
        m_humidity = humidity;
        m_pressure = pressure;
        m_windSpeed = windSpeed;
        m_windDirection = windDirection;

        MeasurementsChanged();
    }

protected:
    SWeatherInfo GetChangedData() const override
    {
        SWeatherInfo info;
        info.temperature = GetTemperature();
        info.humidity = GetHumidity();
        info.pressure = GetPressure();
        info.windSpeed = GetWindSpeed();
        info.windDirection = GetWindDirection();
        return info;
    }

private:
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;
    double m_windSpeed = 0.0;
    double m_windDirection = 0.0;
};
