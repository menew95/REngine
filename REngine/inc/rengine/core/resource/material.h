/**

    @file      material.h
    @brief     material class
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\resource\Resource.h>

namespace graphics
{
    class MaterialBuffer;
}

namespace rengine
{
    class RENGINE_API Material : public Resource
    {
    public:
        Material(uuid uuid);

        Material(const Material& mat) = delete;

        Material(Material&& mat) = delete;

        virtual ~Material();

        ResourceType GetResourceType() override
        {
            return ResourceType::MATERIAL;
        };

        /**
            @brief  material 리소스의 메모리 로드
            @retval  - 로드 성공 여부
        **/
        bool LoadMemory();

        /**
            @brief  material 리소스의 메모리 해제
            @retval  - 해제 성공 여부
        **/
        bool UnLoadMemory();

    private:
        graphics::MaterialBuffer* m_pMaterialBuffer = nullptr;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}