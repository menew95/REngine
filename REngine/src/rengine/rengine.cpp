#include <rengine/rengine.h>

#include <rengine/System/GraphicsSystem.h>

#include <rengine\core\sceneManager.h>
#include <rengine\core\resourcesManager.h>

#include <log/log.h>

namespace rengine
{
	void REngine::Initialize(void* desc)
	{
		if (Log::IsInitialized() == false)
		{
			Log::Initialize();
		}

		m_pGraphicsSystem = GraphicsSystem::GetInstance();

		m_pGraphicsSystem->Initialize();

		Log::Core_Info("Graphics System init succeed");

		SceneManager::GetInstance()->Initialize();

		Log::Core_Info("Scene Manager Init succeed");
	}

	bool REngine::Quit()
	{
		return false;
	}

	bool REngine::Update()
	{
		m_pGraphicsSystem->Render();

		m_pGraphicsSystem->Present();

		return false;
	}

	void REngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		// Todo : 아직 없음
		return;
	}
}