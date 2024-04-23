/**

    @file      InputScalar.h
    @brief     imgui input scalar widget wrapper
    @details   scalar property와 상호작용함
    @author    LWT
    @date      23.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetData.h>

namespace editor
{
    template<typename TData, size_t Size>
    class InputScalar : public WidgetData<array<TData, Size>>
    {
        static_assert(Size > 0, "Invalid scalar size(Size >= 1)");
        static_assert(std::is_scalar_v<TData>, "Invalid scalar type. (Scalar expected)");
        using WidgetData<array<TData, Size>>::m_data;
        using WidgetData<array<TData, Size>>::m_isValChange;
        using WidgetData<array<TData, Size>>::m_lable;
        using WidgetData<array<TData, Size>>::m_idString;
        using WidgetData<array<TData, Size>>::m_flags;

    public:

        InputScalar(const string& lable, float step, float stepFast, string format, uint32 flags)
            : WidgetData<array<TData, Size>>(lable, flags)
            , m_step(step)
            , m_stepFast(stepFast)
            , m_format(format)
        {
            if constexpr (std::is_same_v<TData, int8>)
                m_dataType = ImGuiDataType_S8;
            else if constexpr (std::is_same_v<TData, uint8>)
                m_dataType = ImGuiDataType_U8;
            else if constexpr (std::is_same_v<TData, int16>)
                m_dataType = ImGuiDataType_S16;
            else if constexpr (std::is_same_v<TData, uint16>)
                m_dataType = ImGuiDataType_U16;
            else if constexpr (std::is_same_v<TData, int32>)
                m_dataType = ImGuiDataType_S32;
            else if constexpr (std::is_same_v<TData, uint32>)
                m_dataType = ImGuiDataType_U32;
            else if constexpr (std::is_same_v<TData, int64>)
                m_dataType = ImGuiDataType_S64;
            else if constexpr (std::is_same_v<TData, uint64>)
                m_dataType = ImGuiDataType_U64;
            else if constexpr (std::is_same_v<TData, float>)
                m_dataType = ImGuiDataType_Float;
            else if constexpr (std::is_same_v<TData, double>)
                m_dataType = ImGuiDataType_Double;
            else
                m_dataType = ImGuiDataType_COUNT;

            m_data.fill(0);
        }

        ~InputScalar() = default;

    protected:
        void Draw() override
        {
            if (ImGui::InputScalarN((m_lable + m_idString).c_str(), m_dataType, m_data.data(), Size,
                &m_step, &m_stepFast, m_format.c_str(), m_flags))
                {
                    m_isValChange = true;
            }
        }

    protected:
        float m_step = 0.0f;
        float m_stepFast = 0.0f;

        string m_format = "%.3f";

        int m_dataType = 0;
    };
}