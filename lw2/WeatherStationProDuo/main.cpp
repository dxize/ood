#include "WeatherData.h"
#include <Windows.h>

int main()
{
    SetConsoleOutputCP(1251);

    CWeatherData indoor("IN");
    CWeatherDataPro outdoor("OUT");

    CDisplay display;
    CStatsDisplay stats;

    indoor.RegisterObserver(display, 10);
    indoor.RegisterObserver(stats, 5);

    outdoor.RegisterObserver(display, 10);
    outdoor.RegisterObserver(stats, 5);

    indoor.SetMeasurements(22.5, 0.45, 745);
    outdoor.SetMeasurements(5.3, 0.85, 760, 5.5, 270);
    indoor.SetMeasurements(23.0, 0.50, 746);
    outdoor.SetMeasurements(3.0, 0.90, 761, 7.0, 290);

    return 0;
}
