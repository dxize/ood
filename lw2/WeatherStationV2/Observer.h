#pragma once

#include <map>
#include <set>
#include <functional>

template <typename T>
class IObserver
{
public:
    virtual void Update(T const& data) = 0;
    virtual ~IObserver() = default;
};

template <typename T>
class IObservable
{
public:
    virtual ~IObservable() = default;
    virtual void RegisterObserver(IObserver<T>& observer, int priority = 0) = 0;
    virtual void NotifyObservers() = 0;
    virtual void RemoveObserver(IObserver<T>& observer) = 0;
};


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
 
        if (m_observerToPriority.find(&observer) != m_observerToPriority.end())
        {
            return;
        }

        auto pit = m_priorityToObservers.try_emplace(priority).first;
        auto& observersSet = pit->second;

        auto insertResult = observersSet.insert(&observer);
        if (!insertResult.second)
        {
            // уже есть в set (дубликат) — ничего не делаем
            return;
        }

        // 3) Попытка вставить в обратную таблицу. Если emplace бросит — откатим вставку в set.
        try
        {
            m_observerToPriority.emplace(&observer, priority);
        }
        catch (...)
        {
            // В случае исключения откатываем изменения в set и удаляем пустой контейнер priority
            observersSet.erase(&observer);
            if (observersSet.empty())
            {
                m_priorityToObservers.erase(pit);
            }
            throw; // пробрасываем дальше
        }
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
