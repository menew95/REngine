#include <rengine\rengine.h>

#include <rengine\system\GraphicsSystem.h>
#include <rengine\system\Time.h>
#include <rengine\system\Input.h>
#include <rengine\system\ObjectFactory.h>

#include <rengine\core\SceneManager.h>
#include <rengine\core\ComponentManager.h>
#include <rengine\core\Resources.h>

#include <rengine\utility\Timer.h>

#include <log/log.h>

namespace rengine
{
	void REngine::Initialize(void* desc)
	{
		REngineDesc* _desc = reinterpret_cast<REngineDesc*>(desc);

		m_desc = *_desc;

		if (Log::IsInitialized() == false)
		{
			Log::Initialize();
		}

		GraphicsSystem::GetInstance()->Initialize(m_desc._windowInfo);

		Log::Core_Info("Graphics System init succeed");

		Time::GetInstance()->Initialize();

		Log::Core_Info("Time init succeed");

		Input::GetInstance()->Initialize(m_desc._windowInfo._hWnd);

		Log::Core_Info("Input init succeed");

		Resources::GetInstance()->Initialize();

		Log::Core_Info("Resources init succeed");

		GraphicsSystem::GetInstance()->LoadGraphicsResource();

		Log::Core_Info("Load Graphics Resource succeed");

		SceneManager::GetInstance()->Initialize();

		Log::Core_Info("Scene Manager Init succeed");
	}

	bool REngine::Quit()
	{
		GraphicsSystem::Release();

		return true;
	}

	bool REngine::Update()
	{
		Time::GetInstance()->PreTick();
		Input::GetInstance()->PreTick();

		ObjectFactory::GetInstance()->Update();

		Timer _timer;

		_timer.Lap();
		{
			ComponentManager::GetInstance()->UpdateComponent();

			ComponentManager::GetInstance()->RenderComponent();
		}

		auto _gameTick = _timer.Lap().AsMillis();

		{
			GraphicsSystem::GetInstance()->Present();

			GraphicsSystem::GetInstance()->Render();
		}
		_timer.Lap();

		auto _renderTick = _timer.Lap().AsMillis();

		m_dTickTime += _gameTick + _renderTick;
		m_dTickCnt++;

		if (m_dTickTime > 1000)
		{
			auto _avgTick = m_dTickTime / m_dTickCnt;

			Log::Core_Info(std::format("Total AvgTick : ({0} ms) Game Tick : ({1} ms) Graphics Tick : ({2} ms)", _avgTick, _gameTick, _renderTick));

			// init
			m_dTickTime -= 1000;
			m_dTickCnt = 0;
		}

		return true;
	}

	bool REngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_MOUSEMOVE:
			{
				// 마우스 움직임
				return 0;
			}
			case WM_SIZE:
			{
				m_desc._windowInfo._width = LOWORD(lParam);
				m_desc._windowInfo._height = HIWORD(lParam);

				GraphicsSystem::GetInstance()->ResizeWindow(m_desc._windowInfo._width, m_desc._windowInfo._height);

				break;
			}
		}
		return true;
	}
}