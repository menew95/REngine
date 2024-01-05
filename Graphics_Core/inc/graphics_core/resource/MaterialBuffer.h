/**

    @file      MaterialBuffer.h
    @brief     graphics material buffer class
    @details   ~
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\resource\ResourceBuffer.h>

namespace graphics
{
    class MaterialBuffer : public ResourceBuffer
    {
    public:
        MaterialBuffer();
        ~MaterialBuffer();

        BufferType GetBufferType()
        {
            return BufferType::MATERIAL;
        }

    private:

    };
}