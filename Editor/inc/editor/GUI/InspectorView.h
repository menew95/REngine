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

namespace editor
{
	class EDITOR_API InspectorView :
		public View
	{
	public:
		InspectorView();

		~InspectorView();

		void Begin();

		void Render();

		void End();

		void SetObject();

	private:
	};

}
