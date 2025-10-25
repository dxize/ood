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

class CStatsData
{
public:
    void Update(double value)
    {
        if (m_min > value)
        {
            m_min = value;
        }
        if (m_max < value)
        {
            m_max = value;
        }
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

class CDisplay : public IObserver<SWeatherInfo>
{
private:
    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        std::cout << "Display [" << sourceId << "]:" << std::endl;
        std::cout << "  Temp=" << data.temperature << std::endl;
        std::cout << "  Hum=" << data.humidity << std::endl;
        std::cout << "  Pres=" << data.pressure << std::endl;

    }
};

class CStatsDisplay : public IObserver<SWeatherInfo>
{
private:
    void Update(SWeatherInfo const& data, const std::string& sourceId) override
    {
        auto& sourceStats = m_statsPerSource[sourceId]; 
        sourceStats["Temp"].Update(data.temperature);
        sourceStats["Hum"].Update(data.humidity);
        sourceStats["Pressure"].Update(data.pressure);

        Print(sourceId);
    }


    void Print(const std::string& sourceId)
    {
        std::cout << "Stats Display [" << sourceId << "]:" << std::endl;
        auto it = m_statsPerSource.find(sourceId);
        if (it != m_statsPerSource.end())
        {
            const auto& stats = it->second;
            for (const auto& kv : stats)
            {
                const std::string& name = kv.first;
                const CStatsData& stat = kv.second;

                std::cout << "  Max " << name << " " << stat.GetMax() << std::endl;
                std::cout << "  Min " << name << " " << stat.GetMin() << std::endl;
                std::cout << "  Average " << name << " " << stat.GetAverage() << std::endl;
            }
        }
        else
        {
            std::cout << "Нет данных для станции " << sourceId << std::endl;
        }
    }
    std::map<std::string, std::map<std::string, CStatsData>> m_statsPerSource;

};

class CWeatherData : public CObservable<SWeatherInfo>
{
public:
    CWeatherData(std::string id = "unknown")
        : m_id(std::move(id))
    {
    }

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

    void MeasurementsChanged() 
    { 
        NotifyObservers(); 
    }

    void SetMeasurements(double temp, double humidity, double pressure)
    {
        m_temperature = temp;
        m_humidity = humidity;
        m_pressure = pressure;
        MeasurementsChanged();
    }

protected:
    SWeatherInfo GetChangedData() const override
    {
        SWeatherInfo info;
        info.temperature = GetTemperature();
        info.humidity = GetHumidity();
        info.pressure = GetPressure();
        return info;
    }

    std::string GetId() const override
    {
        return m_id;
    }

private:
    std::string m_id;
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;
};
