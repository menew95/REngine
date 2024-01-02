#pragma once

#include <common/common.h>
#include <common/singleton.h>

#include <rengine/rengine_dllexport.h>

namespace rengine
{
	class RENGINE_API Application
	{
		DECLARE_SINGLETON_CLASS(Application)

	public:
		bool Update();

		bool Quit();

		bool Initallize(bool showCmd,
			HINSTANCE hInstance,
			const tstring& windowClassName,
			const tstring& windowName,
			UINT width,
			UINT height);

	protected:
		/*class Window* m_Window;
		class ComponentSystem* m_ComponentSystem;
		class GraphicsSystem* m_GraphicsSystem;
		class SceneSystem* m_SceneSystem;
		class Resources* m_Resources;

		class Time* m_GameTime;
		class Input* m_Input;*/

		class Window* m_pWindow = nullptr;
		class GraphicsSystem* m_pGraphicsSystem = nullptr;
		class EditorSystem* m_pEditorSystem = nullptr;

		double m_dTickTime = 0;
		double m_TickCnt = 0;
	};
}