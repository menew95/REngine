/**

    @file      Components.h
    @brief     components class
    @details   한 종류의 컴포넌트들을 소유하는 클래스
    @author    LWT
    @date      9.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

namespace rengine
{
    class Component;

    class Components
    {
	public:
		Components(uint32 order, const tstring& componentName);
		virtual ~Components();

		bool operator==(const tstring& name)
		{
			return m_componentName == name;
		}

		/**
			@brief component를 등록
		**/
		virtual void AddComponent(std::shared_ptr<Component>& addComponent);

		/**
			@brief 등록된 컴포넌트 삭제
		**/
		virtual void DeleteComponent(std::shared_ptr<Component>& deleteComponent);

		/**
		    @brief m_ReserveAddComponents에 예약된 컴포넌트가 활성화 가능하면 Start를 호출한다, 아니면 다시 m_ReserveAddComponents에 Push
		**/
		virtual void StartComponents();

		/**
		    @brief m_Components를 순회하며 활성화된 컴포넌트들을 업데이트 한다.
		**/
		virtual void UpdateComponents();

		//void RenderComponents();

		/**
		    @brief m_ReserveDeleteComponents를 순회하며 파괴할 컴포넌트들을 m_Components에서 삭제 한다.
		**/
		virtual void DestroyComponents();

		uint32 GetOrder() { return m_Order; }
		void SetOrder(uint32 order) { m_Order = order; }

		bool m_IsRender = false;

	protected:
		uint32 m_Order;

		tstring m_componentName;

		std::vector<std::shared_ptr<Component>> m_components;
		std::queue<std::shared_ptr<Component>> m_reserveAddComponents;
		std::vector<std::shared_ptr<Component>> m_reserveDeleteComponents;
    };
}