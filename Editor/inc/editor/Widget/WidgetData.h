/**

    @file      WidgetData.h
    @brief     data 관련 작업을 수행하는 위젯
    @details   인스턴스된 객체로부터 data를 get / set 작업을 수행
    @author    LWT
    @date      17.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <editor\Widget\Widget.h>

#include <rttr\registration>

namespace editor
{
    template<typename TData>
    class WidgetData : public Widget
    {
    public:
        WidgetData(const string& lable, uint32 flags = 0)
        :   Widget(lable, flags)
        {}

        ~WidgetData() {}

        EDITOR_API void Render() override;

        void RegisterGetter(function<TData(void)> getter) { m_getter = getter; }

        void RegisterSetter(function<void(TData&)> setter) { m_setter = setter; }

    private:
        virtual void Draw() abstract;

    protected:
        function<TData(void)> m_getter;
        function<void(TData&)> m_setter;

        TData m_data;

        bool m_isValChange = false;
    };

    template<typename TData>
    void WidgetData<TData>::Render()
    {
        if(m_getter != nullptr)
        {
            m_data = m_getter();
        }

        Draw();

        if (m_isValChange)
        {
            if(m_setter != nullptr)
            {
                m_setter(m_data);
            }

            m_isValChange = false;
        }
    }
}