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

#include <rengine\core\RenderComponents.h>

namespace rengine
{
    class ComponentManager
    {
        DECLARE_SINGLETON_CLASS(ComponentManager);

    public:
        //void RegistComponent(uint32 order, const tstring& componentName, bool render = false);

        /**
            @brief 컴포넌트 add 대기열에 등록
            @param component - 등록할 컴포넌트
        **/
        void ReserveAddComponent(const std::shared_ptr<Component>& component);

        void ReserveDeleteComponent(const std::shared_ptr<Component>& component);

        /**
            @brief components들을 update 
        **/
        void UpdateComponent();

        void DestoryComponent();

        void RenderComponent();

        void ClearComponentsList();

    private:
        /**
            @brief 오브젝트 팩토리에서 컴포넌트가 삭제 될경우 컴포넌트 목록으로부터 삭제함
            @param component - 삭제된 컴포넌트
        **/
        void DeleteComponent(const std::shared_ptr<Component>& component);

        vector<pair<tstring, Components>> m_componentsList;

        vector<pair<tstring, RenderComponents>> m_renderComponentsList;

        friend class ObjectFactory;
    };
}