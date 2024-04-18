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
        WidgetData(const string& id, rttr::instance& obj, rttr::property& prop, uint32 flags)
        :   Widget(id, flags)
        , m_instance(obj)
        , m_prop(prop)
        {}

        ~WidgetData() {}

        EDITOR_API void Render() override;

        EDITOR_API void SetHandler(const rttr::variant& var) { m_variant = var; }

        EDITOR_API const rttr::variant& GetHandler() { return m_variant; }

        void RegisterGetter(function<TData(void)> getter) { m_getter = getter; }

        void RegisterSetter(function<void(TData&)> setter) { m_setter = setter; }

    private:
        virtual void Draw() abstract;

        function<TData(void)> m_getter;
        function<void(TData&)> m_setter;

        // object instance;
        rttr::instance m_instance;

        rttr::variant m_variant;

        // property
        rttr::property m_prop;

    protected:
        TData m_data;

        bool m_isValChange;
    };

    template<typename TData>
    void WidgetData<TData>::Render()
    {
        if(m_getter != nullptr)
            m_data = m_getter();
        else
        {
            rttr::variant _var = m_prop.get_value(m_instance);

            assert(_var.can_convert<TData>());

            m_data = _var.convert<TData>();
        }

        Draw();

        if (m_isValChange)
        {
            if(m_setter != nullptr)
                m_setter(m_data);
            else
                assert(m_prop.set_value(m_instance, m_data));

            m_isValChange = false;
        }
    }
}