/**

    @file      EventManager.h
    @brief     Editor event manager class
    @details   editor 안에서 일어나는 이벤트들을 모아둔 클래스
    @author    LWT
    @date      21.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    class EventManager
    {
        DECLARE_SINGLETON_CLASS(EventManager);

    public:
        
        inline void SetFocusObject(rengine::Object* obj)
        {
            m_curFocusObject = obj;
        }

        inline auto* GetFocusObject() 
        {
            return m_curFocusObject;
        }

    private:
        rengine::Object* m_curFocusObject = nullptr;
    };
}