﻿#include <rengine/rengine.h>

#include <rengine/System/GraphicsSystem.h>

#include <rengine\core\sceneManager.h>
#include <rengine\core\resourcesManager.h>

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

		m_pGraphicsSystem = GraphicsSystem::GetInstance();

		m_pGraphicsSystem->Initialize(m_desc._windowInfo);

		Log::Core_Info("Graphics System init succeed");

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
		// todo : 일단 에디터에서 에디터 ui를 그릴 필요가 있다. 물론 나중에 바궈야함 그래픽 엔진에서 카메라와 타깃이 되는 메인 프레임 버퍼만 그리도록 구조를 만들어야함
		m_pGraphicsSystem->Present();

		m_pGraphicsSystem->Render();

		//m_pGraphicsSystem->Present();

		return false;
	}

	bool REngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		// Todo : 아직 없음
		return true;
	}
}