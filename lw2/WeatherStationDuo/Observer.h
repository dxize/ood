#pragma once

#include <map>
#include <set>
#include <string>

template <typename T>
class IObserver
{
public:
    virtual void Update(T const& data, const std::string& sourceId) = 0;
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
            return;
        }

        try
        {
            m_observerToPriority.emplace(&observer, priority);
        }
        catch (...)
        {
            observersSet.erase(&observer);
            if (observersSet.empty())
            {
                m_priorityToObservers.erase(pit);
            }
            throw;
        }
    }

    void RemoveObserver(ObserverType& observer) override
    {
        auto it = m_observerToPriority.find(&observer);
        if (it == m_observerToPriority.end())
        {
            return;
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

    void NotifyObservers() override
    {
        T data = GetChangedData();
        auto snapshot = m_priorityToObservers;

        for (auto rit = snapshot.rbegin(); rit != snapshot.rend(); ++rit)
        {
            for (auto observer : rit->second)
            {
                if (m_observerToPriority.find(observer) != m_observerToPriority.end())
                {
                    observer->Update(data, GetId());
                }
            }
        }
    }

protected:
    virtual T GetChangedData() const = 0;
    // конструктор класс observable должег принимать айди в виде строки и реализация должна находится в базовом классе
    virtual std::string GetId() const = 0;

private:
    std::map<ObserverType*, int> m_observerToPriority;
    std::map<int, std::set<ObserverType*>> m_priorityToObservers;
};
//pull и push модель в observer