/**

    @file      Columns.h
    @brief     imgui columns widget wrapper
    @details   ~
    @author    LWT
    @date      17.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetContainer.h>

namespace editor
{
    template<size_t Size>
    class Columns : public WidgetContainer
    {
        static_assert(Size > 0 && Size <= 5, "Invalid scalar size(Size >= 1)");

        using WidgetContainer::m_childs;
        using WidgetContainer::m_id;
    public:
        Columns(const string& name, uint32 flags)
        : WidgetContainer(name, flags)
        {
            m_columnsWidth.fill(-1.f);
        }

        virtual void Render()
        {
            uint32 _counter = 0;

            ImGui::Columns(Size, m_id.c_str());

            for (auto iter = m_childs.begin(); iter != m_childs.end(); iter++)
            {
                (*iter)->Render();

                if (iter != m_childs.end())
                {
                    if (m_columnsWidth[_counter] != -1.f)
                    {
                        ImGui::SetColumnWidth(_counter, m_columnsWidth[_counter]);
                    }

                    ImGui::NextColumn();
                }

                ++_counter;

                if (_counter == Size)
                    _counter = 0;
            }

            ImGui::Columns(1);
        }

    protected:
        array<float, Size> m_columnsWidth;
    };
}