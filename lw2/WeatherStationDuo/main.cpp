#include "WeatherData.h"
#include <iostream>

int main()
{
    // —оздаЄм две станции Ч внутреннюю и внешнюю
    CWeatherData indoor("IN");
    CWeatherData outdoor("OUT");

    // —оздаЄм наблюдателей
    CDisplay display("Display");
    CStatsDisplay statsDisplay("Stats");

    // –егистрируем наблюдателей на обе станции
    indoor.RegisterObserver(display, 10);
    indoor.RegisterObserver(statsDisplay, 5);

    outdoor.RegisterObserver(display, 10);
    outdoor.RegisterObserver(statsDisplay, 5);

    // Ёмулируем поступление данных с двух станций
    std::cout << "--- Indoor measurements ---" << std::endl;
    indoor.SetMeasurements(22.5, 0.45, 745);

    std::cout << "\n--- Outdoor measurements ---" << std::endl;
    outdoor.SetMeasurements(5.3, 0.85, 760);

    std::cout << "\n--- Indoor changed ---" << std::endl;
    indoor.SetMeasurements(23.0, 0.5, 746);

    std::cout << "\n--- Outdoor changed ---" << std::endl;
    outdoor.SetMeasurements(3.0, 0.9, 761);

    return 0;
}
