/**

    @file      ParticleObject.h
    @brief     particle object class
    @details   ~
    @author    LWT
    @date      8.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\object\RenderObject.h>

namespace graphics
{
    class Buffer;

    class ParticleObject : public RenderObject
    {
    public:
        ParticleObject(uuid  uuid) : RenderObject(uuid) {}
        ~ParticleObject() {}

        auto* GetStreamBuffer() { return m_pParticleStreamBuffer; }

    private:
        Buffer* m_pParticleStreamBuffer = nullptr;
    };
}
