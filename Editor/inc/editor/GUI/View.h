/**

    @file      View.h
    @brief     view base
    @author    LWT
    @date      16.10.2023

**/
#pragma once

#include <common\common.h>

#include <common\math.h>

#include <editor\Editor_dllexport.h>

#include <editor\GUI\GUI.h>

#include <editor\Widget\WidgetManager.h>

namespace editor
{
	class View : public GUI, public WidgetContainer
	{
	public:
		View(std::string name);

		virtual EDITOR_API ~View();

		virtual EDITOR_API void Begin();

		virtual EDITOR_API void Render();

		virtual EDITOR_API void End();

		inline bool GetOpen() { return m_bOpen; }
		inline void SetOpen(bool val) { m_bOpen = val; }

		inline const string& GetName() { return m_viewName; }

	protected:
		bool _test = false;
		
		std::string m_viewName;

		bool m_bOpen = true;

		uint32 m_flags = 0;

		Vector2 m_viewportSize = Vector2::One;
		Vector2 m_viewportBounds[2];
	};
}