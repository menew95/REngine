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
	struct WindowSettings
	{
		bool _notitleBar = false;
		bool _noResize = false;
		bool _noMove = false;
		bool _noScrollbar = false;
		bool _noScrollWithMouse = false;
		bool _noCollapse = false;
		bool _alwaysAutoResize = false;
		bool _noBackground = false;
		bool _noInput = false;
		bool _noSavedSettings = false;
		bool _useMouseInput = false;
		bool _hasMenuBar = false;
		bool _horizontalScrollbar = false;
		bool _noFocusOnAppearing = false;
		bool _noBringToFrontOnFocus = false;
		bool _alwaysVerticalScrollbar = false;
		bool _alwaysHorizontalScrollbar = false;
		bool _alwaysUseWindowPadding = false;
		bool _noNavInputs = false;
		bool _noNavFocus = false;
		bool _unsavedDocument = false;
		bool _noDocking = false;
	};

	struct PanelWindowSetting
	{
		bool closable = false;
		bool resizable = true;
		bool movable = true;
		bool dockable = false;
		bool scrollable = true;
		bool hideBackground = false;
		bool forceHorizontalScrollbar = false;
		bool forceVerticalScrollbar = false;
		bool allowHorizontalScrollbar = false;
		bool bringToFrontOnFocus = true;
		bool collapsable = false;
		bool allowInputs = true;
		bool titleBar = true;
		bool autoSize = false;
	};

	class View : public GUI, public WidgetContainer
	{
	public:
		View(std::string name, WindowSettings windowSettings = WindowSettings());

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

		WindowSettings m_windowSettings;

		Vector2 m_viewportSize = Vector2::One;
		Vector2 m_viewportBounds[2];
	};
}