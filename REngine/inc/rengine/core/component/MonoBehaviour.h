/**

    @file      MonoBehaviour.h
    @brief     
    @details   ~
    @author    LWT
    @date      3.05.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <rengine\core\component\Behaviour.h>

namespace rengine
{
    class MonoBehaviour : public Behaviour
    {
    public:
        MonoBehaviour(const uuid& uuid, const tstring& type = TEXT("MonoBehaviour"));

        MonoBehaviour(const MonoBehaviour& component) = default;

        MonoBehaviour(MonoBehaviour&& component) = default;

        virtual ~MonoBehaviour();

    protected:

        friend class GameObject;

        RTTR_ENABLE(Behaviour);

        RTTR_REGISTRATION_FRIEND
    };
}