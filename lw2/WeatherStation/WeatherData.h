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
	void Update(SWeatherInfo const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

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
	}

	std::map<std::string, CStatsData> m_stats;
};

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

private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
};
