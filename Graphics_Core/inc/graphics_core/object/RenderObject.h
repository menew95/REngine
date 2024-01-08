/**

    @file      RenderObject.h
    @brief     render object base class
    @details   rendering 할 오브젝트의 베이스 클래스 
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common\common.h>

#include <common\math.h>

#include <graphics_core\Graphics_core_dllexport.h>

namespace graphics
{
    class RenderObject
    {
    public:
        RenderObject(uuid uuid) : m_uuid(uuid) {}
        virtual ~RenderObject() {}

        GRAPHICS_API inline uuid GetUUID() { return m_uuid; }
        GRAPHICS_API inline bool GetStatic() { return m_bIsStatic; }
        GRAPHICS_API inline bool GetCulling() { return m_bIsCull; }
        GRAPHICS_API inline math::Matrix GetWorld() { return m_world; }

        GRAPHICS_API inline void SetStatic(bool val) { m_bIsStatic = val; }
        GRAPHICS_API inline void SetCulling(bool val) { m_bIsCull = val; }
        GRAPHICS_API inline void SetWorld(math::Matrix val) { m_world = val; }

    protected:
        uuid m_uuid;
        
        bool m_bIsStatic = false;

        bool m_bIsCull = false;

        math::Matrix m_world = math::Matrix::Identity;
    };
}
