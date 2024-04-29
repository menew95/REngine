/**

    @file      RenderComponents.h
    @brief     render components class
    @details   렌더링 관련 컴포넌트들을 관리
    @author    LWT
    @date      29.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

namespace rengine
{
    class Renderer;

    class RenderComponents
    {
    public:
		RenderComponents(const tstring& componentName);
		virtual ~RenderComponents();

		bool operator==(const tstring& name)
		{
			return m_componentName == name;
		}

		/**
			@brief component를 리스트에 추가
		**/
		void AddComponent(Renderer* addComponent);

		/**
			@brief 등록된 컴포넌트 리스트에서 해당 컴포넌트를 삭제
		**/
		void DeleteComponent(Renderer* deleteComponent);

		/**
			@brief m_Components를 순회하며 활성화된 컴포넌트들을 렌더를 호출.
		**/
		void RenderComponent();

	protected:

		tstring m_componentName;

		// 현재 업데이트 되고 있는 컴포넌트 리스트
		std::list<Renderer*> m_components;
	};
}