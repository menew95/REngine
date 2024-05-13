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
    static math::Vector2 EncodeSelectionId(uint64 pickingIndex)
    {
        Vector2 _ret;

        memcpy(&_ret, &pickingIndex, sizeof(uint64));
        
        return _ret;

        return Vector2(
            (float)((pickingIndex) & 0xFFFF),
            (float)(pickingIndex >> 32 & 0xFFFF));
    }

    static uint64 DecodeSelectionId(math::Vector2 selectionId)
    {
        uint64 _ret;

        memcpy(&_ret, &selectionId, sizeof(math::Vector2));

        return _ret;
    }

    enum class RenderType
    {
        MESH = 0,
        PARTICLE = 1,
    };

    struct PerObject
    {
        math::Vector2 _objID;
        float _flags = 0;
        float _flags2 = 0;
        math::Matrix _world = math::Matrix::Identity;
        math::Matrix _worldInv = math::Matrix::Identity;
    };

    class RenderObject
    {
    public:

        RenderObject(uuid uuid) : m_uuid(uuid) 
        {
            auto _option = uuids::uuid::from_string(m_uuid);

            uuids::uuid _uuid;

            if (_option.has_value())
            {
                _uuid = _option.value();

                m_hash = std::hash<uuids::uuid>()(_uuid);

                m_perObject._objID = EncodeSelectionId(m_hash);
            }
        }
        virtual ~RenderObject() {}

        GRAPHICS_API inline uuid GetUUID() const { return m_uuid; }
        GRAPHICS_API inline uint64 GetHash() const { return m_hash; }
        GRAPHICS_API inline bool GetEnable() const { return m_bEnable; }
        GRAPHICS_API inline bool GetStatic() const { return m_bIsStatic; }
        GRAPHICS_API inline bool GetCulling() const { return m_bIsCull; }
        GRAPHICS_API inline const PerObject& GetTrans() const { return m_perObject; }
        GRAPHICS_API inline const math::Matrix& GetWorld() const { return m_perObject._world; }
        GRAPHICS_API inline const math::Matrix& GetWorldInv() const { return m_perObject._worldInv; }
       
        GRAPHICS_API inline const math::Vector3& GetBoundinBoxMin() const { return m_boundingBoxMin; }
        GRAPHICS_API inline const math::Vector3& GetBoundinBoxMax() const { return m_boundingBoxMax; }

        GRAPHICS_API inline void SetHash(uint64 val) { m_hash = val, m_perObject._objID = EncodeSelectionId(m_hash); }
        GRAPHICS_API inline void SetEnable(bool val) { m_bEnable = val; }
        GRAPHICS_API inline void SetStatic(bool val) { m_bIsStatic = val; }
        GRAPHICS_API inline void SetCulling(bool val) { m_bIsCull = val; }
        GRAPHICS_API inline void SetWorld(math::Matrix val) 
        {
            if (m_perObject._world == val)
            {
                m_bIsMove = false;
                return;
            }

            m_perObject._world = val;
            m_perObject._worldInv = val.Invert().Transpose();

            m_bIsMove= true;
        }

        GRAPHICS_API virtual void SetBoundinBoxMin(const math::Vector3& min) { m_boundingBoxMin = min; }
        GRAPHICS_API virtual void SetBoundinBoxMax(const math::Vector3& max) { m_boundingBoxMax = max; }

        virtual RenderType GetRenderType() abstract;

    protected:
        uuid m_uuid;

        // game object hash
        uint64 m_hash = 0u;
        
        bool m_bEnable = false;

        bool m_bIsStatic = false;

        bool m_bIsCull = false;

        bool m_bIsMove = false;

        PerObject m_perObject;

        math::Vector3 m_boundingBoxMin;
        math::Vector3 m_boundingBoxMax;

        friend class RenderQueue;
    };
}
