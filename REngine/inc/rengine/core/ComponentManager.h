/**

    @file      ComponentManager.h
    @brief     component manager class
    @details   컴포넌트 이벤트 호출을 관리
    @author    LWT
    @date      9.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <rengine\core\Components.h>

namespace rengine
{
    class ComponentManager
    {
        DECLARE_SINGLETON_CLASS(ComponentManager);

    public:
        void RegistComponent(uint32 order, const tstring& componentName, bool render = false);

        void ReserveAddComponent(std::shared_ptr<Component>& component);

        void ReserveDeleteComponent(std::shared_ptr<Component>& component);

        void UpdateComponent();

        void RenderComponent();

        void ClearComponentsList();

    private:
        vector<pair<tstring, Components>> m_componentsList;

        vector<pair<tstring, Components>> m_renderComponentsList;
    };
}