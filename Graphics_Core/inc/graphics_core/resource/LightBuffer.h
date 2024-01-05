/**

    @file      LightBuffer.h
    @brief     graphics light buffer class
    @details   ~
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\resource\ResourceBuffer.h>

namespace graphics
{
    class LightBuffer : public ResourceBuffer
    {
    public:
        LightBuffer();
        ~LightBuffer();

        BufferType GetBufferType()
        {
            return BufferType::LIGHT;
        }

    private:

    };
}