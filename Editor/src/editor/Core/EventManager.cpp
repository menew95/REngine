#include <Editor_pch.h>

#include <editor\Core\EventManager.h>

#include <editor\Core\EnginePlugin.h>

#include <editor\GUI\Document.h>

#include <editor\Editor.h>

#include <rengine\rengine.h>

#include <rengine\core\resources.h>

#include <rengine\core\scene\scene.h>

#include <rengine\core\sceneManager.h>

namespace editor
{
	DEFINE_SINGLETON_CLASS(EventManager,
		{
			Initailze();
		}, 
		{

		}, 
		{
			Instance.UnInitailze();
		});


	void EventManager::Initailze()
	{
	}

	void EventManager::UnInitailze()
	{
		m_curFocusObject = nullptr;

		m_editor = nullptr;
	}

	void editor::EventManager::SetFocusObject(rengine::Object* obj)
	{
		m_curFocusObject = obj;

		if(obj == nullptr)
			m_unselectGameObjectEvent.Invoke();
		else
		{
			if (obj->GetType() == TEXT("GameObject"))
				m_selectGameObjectEvent.Invoke(obj);
			else
				m_unselectGameObjectEvent.Invoke();
		}
	}

	void EventManager::SaveScene()
	{
		rengine::SceneManager::GetInstance()->SaveScene();
	}

	void EventManager::OpenScene(const tstring& path)
	{

	}

	void EventManager::SetEditorMode(EditorMode editorMode)
	{
		m_editorMode = editorMode;

		if(m_editorMode == EditorMode::Play)
			EnginePlugin::GetInstance()->SetEngineUpdate(true);
		else
			EnginePlugin::GetInstance()->SetEngineUpdate(false);
	}

	View* EventManager::GetView(const string& viewName)
	{
		return m_editor->m_pEditorDocment->GetView(viewName);
	}

	void EventManager::StartGame()
	{
		// 에디트 모드에서 실행할 때
		if (m_editorMode == EditorMode::Edit)
		{
			int button = MessageBox
			(
				nullptr,
				TEXT("Is it okay to save \n and enter play mode? "),
				TEXT("Play Mode"),
				MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2
			);

			switch (button)
			{
					// 진입 O
				case IDOK: break;

					// 진입 X
				case IDNO: return;
			}

			SaveScene();

			SetEditorMode(EditorMode::Play);
		}
	}

	void EventManager::PauseGame()
	{
		// 플레이 모드일 경우 pause
		if (m_editorMode == EditorMode::Play)
		{
			SetEditorMode(EditorMode::Pause);
		}
		else if (m_editorMode == EditorMode::Pause)
		{
			SetEditorMode(EditorMode::Play);
		}
	}

	void EventManager::StopGame()
	{
		if(m_curFocusObject != nullptr && m_curFocusObject->GetType() == TEXT("GameObject"))
			m_curFocusObject = nullptr;

		if (m_editorMode != EditorMode::Edit)
		{
			rengine::Scene* _currentScene = rengine::SceneManager::GetInstance()->GetCurrentScene().get();

			const tstring& _currentScenePath = _currentScene->GetPath();

			rengine::SceneManager::GetInstance()->LoadScene(_currentScenePath);

			m_unselectGameObjectEvent.Invoke();
		}

		SetEditorMode(EditorMode::Edit);
	}

	void EventManager::NextFrame()
	{
		// pause 모드일 경우 한 프레임 업데이트 합니다.
		if (m_editorMode == EditorMode::Pause)
		{
			EnginePlugin::GetInstance()->UpdateFrame();
		}
	}

	void EventManager::ExitEditor()
	{
		m_editor->m_editorExit = true;
	}

}