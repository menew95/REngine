﻿/**

    @file      InspectorView.h
    @brief     선택된 오브젝트의 프로퍼티를 보여주는 창
    @details   ~
    @author    Cool Guy
    @date      16.10.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/

#pragma once

#include <editor\GUI\View.h>

namespace rengine
{
	class GameObject;
	class Component;
}

namespace editor
{
	class InspectorView :
		public View
	{
	public:
		InspectorView();

		~InspectorView();

		void Begin() override;

		void Render() override;

		void End() override;

	private:
		/**
		  @brief component의 직렬화가 되는 정보들을 노출
		  @param comp - component pointer
		**/
		void DrawComponent(rengine::Component* comp);
	};

}