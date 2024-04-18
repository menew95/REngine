/**

    @file      DragScalar.h
    @brief     imgui drag scalar widget wrapper
    @details   scalar property와 상호작용함
    @author    LWT
    @date      17.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetData.h>

namespace editor
{
    template<typename TData, size_t Size>
    class DragScalar : public WidgetData<array<TData, Size>>
    {
        static_assert(Size > 0, "Invalid scalar size(Size >= 1)");
        static_assert(std::is_scalar_v<TData>, "Invalid scalar type. (Scalar expected)");
        using WidgetData<array<TData, Size>>::m_data;
        using WidgetData<array<TData, Size>>::m_isValChange;
        using WidgetData<array<TData, Size>>::m_id;

    public:
        using WidgetData<array<TData, Size>>::SetHandler;
        DragScalar(const string& id, rttr::instance& obj, rttr::property& prop, TData min, TData max, float speed, string format, uint32 flags)
        : WidgetData<array<TData, Size>>(id, obj, prop, flags)
        , m_min(min)
        , m_max(max)
        , m_speed(speed)
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

        ~DragScalar() = default;

    protected:
        void Draw() override
        {
            if (m_max < m_min)
                m_max = m_min;

            for (size_t i = 0; i < Size; i++)
            {
                if (m_data[i] < m_min)
                    m_data[i] = m_min;
                else if (m_data[i] > m_max)
                    m_data[i] = m_max;
            }

            if (ImGui::DragScalarN(m_id.c_str(), m_dataType, m_data.data(), Size,
                m_speed, &m_min, &m_max, m_format.c_str()))
            {
                m_isValChange = true;
            }
        }

    protected:
        TData m_min = std::numeric_limits<TData>::lowest();
        TData m_max = std::numeric_limits<TData>::max();

        float m_speed = 1.0f;
        
        string m_format = "%.3f";

        int m_dataType = 0;
    };
}