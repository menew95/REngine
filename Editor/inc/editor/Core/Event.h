/**

    @file      Event.h
    @brief     event class
    @details   ~
    @author    LWT
    @date      21.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>
#include <any>

namespace editor
{
    using EventListenerID = uint64;

    template<class Ret, typename ... Args>
    class Event final
    {
        using EventListener = std::function<Ret(Args...)>;

    public:
        Event()
            : _listeners(std::unordered_map<EventListenerID, EventListener>())
            , _nextListenerID(0)
        {

        }

        ~Event()
        {
            _listeners.clear();
        }

        Event(const Event&) = default;

        Event& operator=(const Event&) = default;

        Event(Event&&) = default;

        Event& operator=(Event&&) = default;

        EventListenerID AddListener(EventListener callback)
        {
            EventListenerID listenerID = _nextListenerID++;

            _listeners.emplace(listenerID, callback);

            return listenerID;
        }

        EventListenerID operator+=(EventListener callback)
        {
            return AddListener(callback);
        }

        bool RemoveListener(EventListenerID listenerID)
        {
            return _listeners.erase(listenerID) != 0;
        }

        bool operator-=(EventListenerID listenerID)
        {
            return RemoveListener(listenerID);
        }

        void RemoveAllListeners()
        {
            _listeners.clear();
        }

        uint64_t GetListenerCount()
        {
            return _listeners.size();
        }

        std::vector<Ret> Invoke(Args... args)
        {
            std::vector<Ret> ret;

            ret.reserve(_listeners.size());

            for (auto const& [key, value] : _listeners)
            {
                Ret val = value(args...);

                ret.push_back(val);
            }

            return ret;
        }

    private:
        std::unordered_map<EventListenerID, EventListener> _listeners;

        EventListenerID _nextListenerID;
    };

    template <typename ...Types>
    class Event<void, Types...> final
    {
        using EventListener = std::function<void(Types...)>;

    public:
        Event();

        ~Event();

        Event(const Event&) = default;

        Event& operator=(const Event&) = default;

        Event(Event&&) = default;

        Event& operator=(Event&&) = default;

        EventListenerID AddListener(EventListener callback);

        EventListenerID operator+=(EventListener callback);

        bool RemoveListener(EventListenerID listenerID);

        bool operator-=(EventListenerID listenerID);

        void RemoveAllListeners();

        uint64_t GetListenerCount();

        void Invoke(Types... args);

    private:
        std::unordered_map<EventListenerID, EventListener> _listeners;

        EventListenerID _nextListenerID;
    };

    template <typename ... Types>
    Event<void, Types...>::Event() :
        _listeners(std::unordered_map<EventListenerID, EventListener>())
        , _nextListenerID(0)
    {

    }

    template <typename ... Types>
    Event<void, Types...>::~Event()
    {
        _listeners.clear();
    }

    template <typename ... Types>
    EventListenerID Event<void, Types...>::AddListener(EventListener callback)
    {
        EventListenerID listenerID = _nextListenerID++;

        _listeners.emplace(listenerID, callback);

        return listenerID;
    }

    template <typename ... Types>
    EventListenerID Event<void, Types...>::operator+=(EventListener callback)
    {
        return this->AddListener(callback);
    }

    template <typename ... Types>
    bool Event<void, Types...>::RemoveListener(EventListenerID listenerID)
    {
        return _listeners.erase(listenerID) != 0;
    }

    template <typename ... Types>
    bool Event<void, Types...>::operator-=(EventListenerID listenerID)
    {
        return this->RemoveListener(listenerID);
    }

    template <typename ... Types>
    void Event<void, Types...>::RemoveAllListeners()
    {
        _listeners.clear();
    }

    template <typename ... Types>
    uint64_t Event<void, Types...>::GetListenerCount()
    {
        return _listeners.size();
    }

    template <typename ... Types>
    void Event<void, Types...>::Invoke(Types... args)
    {
        for (auto const& [key, value] : _listeners)
            value(args...);
    }

}