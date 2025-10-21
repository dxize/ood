#include "WeatherData.h"
#include <Windows.h>

int main()
{
    SetConsoleOutputCP(1251);
    CWeatherData wd;

    CDisplay display;
    CStatsDisplay statsDisplay;

    wd.RegisterObserver(display, 10);
    wd.RegisterObserver(statsDisplay, 1);

    wd.SetMeasurements(3, 0.7, 760, 5.0, 350);
    wd.SetMeasurements(4, 0.8, 761, 7.0, 10);
    wd.SetMeasurements(10, 0.8, 761, 3.0, 180);
    wd.SetMeasurements(-5, 0.6, 758, 6.0, 270);

    wd.RemoveObserver(statsDisplay);

    wd.SetMeasurements(8, 0.5, 762, 4.0, 90);

    return 0;
}
