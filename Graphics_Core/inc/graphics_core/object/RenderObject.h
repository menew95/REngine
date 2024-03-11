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
    enum class RenderType
    {
        MESH = 0,
        PARTICLE = 1,
    };

    class RenderObject
    {
    protected:
        struct PerObject;
    public:

        RenderObject(uuid uuid) : m_uuid(uuid) {}
        virtual ~RenderObject() {}

        GRAPHICS_API inline uuid GetUUID() { return m_uuid; }
        GRAPHICS_API inline bool GetEnable() { return m_bEnable; }
        GRAPHICS_API inline bool GetStatic() { return m_bIsStatic; }
        GRAPHICS_API inline bool GetCulling() { return m_bIsCull; }
        GRAPHICS_API inline const struct PerObject& GetTrans() { return m_perObject; }
        GRAPHICS_API inline const math::Matrix& GetWorld() { return m_perObject._world; }
        GRAPHICS_API inline const math::Matrix& GetWorldInv() { return m_perObject._worldInv; }

        GRAPHICS_API inline void SetEnable(bool val) { m_bEnable = val; }
        GRAPHICS_API inline void SetStatic(bool val) { m_bIsStatic = val; }
        GRAPHICS_API inline void SetCulling(bool val) { m_bIsCull = val; }
        GRAPHICS_API inline void SetWorld(math::Matrix val) 
        {
            m_perObject._world = val;
            m_perObject._worldInv = val.Invert();
        }

        virtual RenderType GetRenderType() abstract;

    protected:
        uuid m_uuid;
        
        bool m_bEnable = false;

        bool m_bIsStatic = false;

        bool m_bIsCull = false;

        struct PerObject
        {
            math::Matrix _world = math::Matrix::Identity;
            math::Matrix _worldInv = math::Matrix::Identity;
        };

        PerObject m_perObject;
    };
}
