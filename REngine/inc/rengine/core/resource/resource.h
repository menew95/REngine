/**

    @file      resource.h
    @brief     resource base
    @author    LWT
    @date      4.12.2023

**/
#pragma once

#include <rengine\core\object\object.h>

namespace rengine
{
    enum class ResourceType
    {
        UNKNOWN = 0,

        MATERIAL = 1,
        TEXTURE = 2,
        MESH = 3,
        SKINNED_MESH = 4,

        ANMATOR_CONTROLLER = 5,
        ANIMATION_CLIP = 6,

        AUDIO_CLIP = 7,
    };

    class RENGINE_API Resource : public Object
    {
    public:
        Resource(uuid uuid, tstring name);

        Resource(uuid uuid, tstring name, tstring type);

        Resource(const Resource& res) = default;

        Resource(Resource&& res) = default;

        virtual ~Resource() = default;

        virtual ResourceType GetType() abstract;

        inline bool GetIsLoad()
        {
            return m_bIsLoad;
        }

        virtual ResourceType GetResourceType() abstract;

    protected:

        // 이 리소스가 가리키는 파일 위치
        tstring m_path;

        // 리소스가 메모리에 로드가 되었는지 여부
        bool m_bIsLoad = false;
    };
}