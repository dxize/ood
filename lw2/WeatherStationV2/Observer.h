#pragma once

#include <map>
#include <set>
#include <functional>

// ============================================
// Интерфейс наблюдателя
// ============================================
template <typename T>
class IObserver
{
public:
    virtual void Update(T const& data) = 0;
    virtual ~IObserver() = default;
};

// ============================================
// Интерфейс наблюдаемого объекта
// ============================================
template <typename T>
class IObservable
{
public:
    virtual ~IObservable() = default;
    virtual void RegisterObserver(IObserver<T>& observer, int priority = 0) = 0;
    virtual void NotifyObservers() = 0;
    virtual void RemoveObserver(IObserver<T>& observer) = 0;
};

// ============================================
// Реализация наблюдаемого объекта с приоритетами
// ============================================
template <class T>
class CObservable : public IObservable<T>
{
public:
    using ObserverType = IObserver<T>;

    // --------------------------------------------
    // Регистрация наблюдателя с приоритетом
    // --------------------------------------------
    void RegisterObserver(ObserverType& observer, int priority = 0) override
    {
        // Проверяем, подписан ли уже наблюдатель
        if (m_observerToPriority.find(&observer) != m_observerToPriority.end())
        {
            return; // уже подписан — повторная регистрация не выполняется
        }

        // Вставка в обе структуры с O(log n)
        m_observerToPriority[&observer] = priority;
        m_priorityToObservers[priority].insert(&observer);
    }

    // --------------------------------------------
    // Удаление наблюдателя
    // --------------------------------------------
    void RemoveObserver(ObserverType& observer) override
    {
        auto it = m_observerToPriority.find(&observer);
        if (it == m_observerToPriority.end())
        {
            return; // не найден
        }

        int priority = it->second;
        m_observerToPriority.erase(it);

        auto pit = m_priorityToObservers.find(priority);
        if (pit != m_priorityToObservers.end())
        {
            pit->second.erase(&observer);
            if (pit->second.empty())
            {
                m_priorityToObservers.erase(pit);
            }
        }
    }

    // --------------------------------------------
    // Оповещение всех наблюдателей
    // --------------------------------------------
    void NotifyObservers() override
    {
        T data = GetChangedData();

        // Делаем копию для безопасного обхода
        auto snapshot = m_priorityToObservers;

        // От более высокого приоритета к низкому
        for (auto rit = snapshot.rbegin(); rit != snapshot.rend(); ++rit)
        {
            for (auto observer : rit->second)
            {
                if (m_observerToPriority.find(observer) != m_observerToPriority.end())
                {
                    observer->Update(data);
                }
            }
        }
    }

protected:
    virtual T GetChangedData() const = 0;

private:
    // Быстрый поиск приоритета по наблюдателю
    std::map<ObserverType*, int> m_observerToPriority;

    // Наблюдатели, отсортированные по приоритету
    std::map<int, std::set<ObserverType*>> m_priorityToObservers;
};
