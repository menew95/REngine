/**

    @file      MeshFilter.h
    @brief     mesh filter component
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\component\Component.h>

namespace rengine
{
    class RENGINE_API MeshFilter : public Component, public enable_shared_from_this<MeshFilter>
    {
    public:
        MeshFilter(std::shared_ptr<GameObject>& gameObj);

        MeshFilter(std::shared_ptr<GameObject>& gameObj, uuid uuid);

        MeshFilter(std::shared_ptr<GameObject>& gameObj, uuid uuid, tstring name);

        MeshFilter(const MeshFilter& filter) = default;

        MeshFilter(MeshFilter&& filter) = default;

        virtual ~MeshFilter();
    };
}