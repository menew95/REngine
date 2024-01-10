﻿/**

    @file      MeshObject.h
    @brief     mesh object class
    @details   ~
    @author    LWT
    @date      8.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\object\RenderObject.h>

namespace Graphics
{
    class MeshObject : public RenderObject
    {
    public:
        MeshObject(uuid  uuid) : RenderObject(uuid) {}
        ~MeshObject() {}

        GRAPHICS_API inline class MeshBuffer* GetMeshBuffer() { return m_pMeshBuffer; }
        GRAPHICS_API inline vector<class MaterialBuffer*>& GetMaterialBuffers() { return m_materialBuffers; }

        GRAPHICS_API void SetMeshBuffer(class MeshBuffer* val) { m_pMeshBuffer = val; }
        GRAPHICS_API void SetMaterialBuffer(size_t idx, class MaterialBuffer* val) { m_materialBuffers[idx] = val; }

    private:
        class MeshBuffer* m_pMeshBuffer = nullptr;

        vector<class MaterialBuffer*> m_materialBuffers;
    };
}