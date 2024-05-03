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
			@brief component를 추가 예약 목록에 등록하고 Awake를 호출
		**/
		virtual void AddComponent(const std::shared_ptr<Component>& addComponent);

		/**
			@brief 등록된 컴포넌트 또는 추가 대기중이던 목록에서 해당 컴포넌트를 삭제
		**/
		virtual void DeleteComponent(const std::shared_ptr<Component>& deleteComponent);

		/**
		    @brief m_ReserveAddComponents에 예약된 컴포넌트가 활성화 가능하면 Start를 호출하고 컴포넌트 목록으로 이동
		**/
		virtual void StartComponents();

		/**
		    @brief m_Components를 순회하며 활성화된 컴포넌트들을 업데이트 한다.
		**/
		virtual void UpdateComponents();

	protected:
		uint32 m_Order;

		tstring m_componentName;

		// 현재 업데이트 되고 있는 컴포넌트 리스트
		std::vector<std::shared_ptr<Component>> m_updateComponents;

		// 다음 업데이트에 추가될 컴포넌트 리스트
		std::list<std::shared_ptr<Component>> m_reserveAddComponents;
    };
}