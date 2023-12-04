/**

    @file      Camera.h
    @brief     Camera component
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\component\Component.h>

namespace rengine
{
    class RENGINE_API Camera : public Component, public enable_shared_from_this<Camera>
    {
    public:
        Camera(std::shared_ptr<GameObject>& gameObj);

        Camera(std::shared_ptr<GameObject>& gameObj, uuid uuid);

        Camera(std::shared_ptr<GameObject>& gameObj, uuid uuid, tstring name);

        Camera(const Camera& filter) = default;

        Camera(Camera&& filter) = default;

        virtual ~Camera();
}