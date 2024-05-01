/**

    @file      RenderQueue.h
    @brief     렌더 오브젝트를 생성 및 관리
    @details   ~
    @author    LWT
    @date      12.03.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <graphics_core\GraphicsEngine_api.h>

#include <graphics_core\object\MeshObject.h>
#include <graphics_core\object\ParticleObject.h>

namespace graphics
{
    class RenderQueue
    {
        DECLARE_SINGLETON_CLASS_EXPORT(GRAPHICS_API, RenderQueue)
    public:
            
        /**
            @brief render object를 추가
            @param type - 렌더 오브젝트의 타입
            @param uuid - 렌더 오브젝트에 할당할 uuid
            @retval  - 추가한 render object pointer
        **/
        GRAPHICS_API RenderObject* CreateRenderObject(const RenderType type, uuid uuid);

        /**
            @brief 저장되어있는 render object를 삭제
            @param object - 삭제할 render object pointer
        **/
        GRAPHICS_API void DeleteObject(RenderObject* object);

        /**
            @brief 프러스텀 컬링을 실시한다. 컬링 패스를 못한 렌더 오브젝트의 컬링 상태를 false로 설정한다.
            @param frustum - 컬링을 실시할 프러스텀
        **/
        void FrustumCulling(const struct Frustum& frustum);

    private:
        unordered_map<uuid, std::unique_ptr<MeshObject>> m_meshObjectList;
        unordered_map<uuid, std::unique_ptr<ParticleObject>> m_particleObjectList;

        friend class ShadowRenderPass;
    };
}