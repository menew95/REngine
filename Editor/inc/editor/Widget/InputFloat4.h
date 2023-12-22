/**

    @file      InputFloat.h
    @brief     imgui float4 wrapper class
    @details   vector4 peoperty과 상호작용함
    @author    LWT
    @date      22.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

namespace editor
{
    class InputFloat4 : public Widget
    {
    public:
        InputFloat4(string name, float* handler, uint32 flags = 0);

        ~InputFloat4();

        EDITOR_API void Render() override;

    private:

        float* m_pHandler = nullptr;
    };
}