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
    template<class Listener, class Ret, typename ... Args>
    class Event
    {
        using EventHander = std::function<Ret(Listener&, Args...)>;

    public:
        Event(Listener& listener, EventHander handler)
        : m_refListener(listener)
        , m_eventHandler(handler)
        {

        }

        void Invoke(Args ... arg)
        {
            m_eventHandler(m_refListener, arg ...);
        }

    private:
        Listener& m_refListener;
        EventHander m_eventHandler;
    };
}