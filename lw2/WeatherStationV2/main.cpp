#include "WeatherData.h"

int main()
{
    CWeatherData wd;

    CDisplay display;
    CStatsDisplay statsDisplay;

    // Регистрируем наблюдателей с разными приоритетами
    // Более высокий приоритет — раньше получает уведомления
    wd.RegisterObserver(display, 10);       // Display — высокий приоритет
    wd.RegisterObserver(statsDisplay, 1);   // StatsDisplay — низкий приоритет

    // Изменяем данные — уведомления идут в порядке приоритетов
    wd.SetMeasurements(3, 0.7, 760);
    wd.SetMeasurements(4, 0.8, 761);

    // Удаляем один наблюдатель
    wd.RemoveObserver(statsDisplay);

    // Теперь только Display будет получать уведомления
    wd.SetMeasurements(10, 0.8, 761);
    wd.SetMeasurements(-10, 0.8, 761);

    return 0;
}
