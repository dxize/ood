#include "WeatherData.h"
#include <iostream>

int main()
{
    CWeatherData indoor("IN");
    CWeatherData outdoor("OUT");

    CDisplay display;
    CStatsDisplay statsDisplay;

    indoor.RegisterObserver(display, 10);
    indoor.RegisterObserver(statsDisplay, 5);

    outdoor.RegisterObserver(display, 10);
    outdoor.RegisterObserver(statsDisplay, 5);

   
    indoor.SetMeasurements(22.5, 0.45, 745);
    outdoor.SetMeasurements(5.3, 0.85, 760);
    indoor.SetMeasurements(23.0, 0.5, 746);
    outdoor.SetMeasurements(3.0, 0.9, 761);

    return 0;
}
