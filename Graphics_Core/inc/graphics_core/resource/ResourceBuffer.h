/**

    @file      ResourceBuffer.h
    @brief     Resource Buffer base class
    @details   그래픽 리소스의 베이크 클래스
    @author    LWT
    @date      4.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <graphics_core\graphics_core_dllexport.h>

namespace graphics
{
    enum class BufferType
    {
        MESH = 0,
        MATERIAL = 1,
        TEXTURE = 2,
        CAMERA = 3,
        LIGHT = 4,
    };

    class ResourceBuffer
    {
    public:
        ResourceBuffer();

        virtual ~ResourceBuffer();

        GRAPHICS_API virtual BufferType GetBufferType() abstract;
    };
}