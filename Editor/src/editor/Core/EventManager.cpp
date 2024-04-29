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
			SetEditorMode(EditorMode::Edit);
		}, 
		{

		}, 
		{

		});


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

		if(m_editorMode == EditorMode::Edit)
			EnginePlugin::GetInstance()->SetEngineUpdate(false);
		else
			EnginePlugin::GetInstance()->SetEngineUpdate(true);
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
		SetEditorMode(EditorMode::Pause);
	}

	void EventManager::StopGame()
	{
		if(m_curFocusObject->GetType() == TEXT("GameObject"))
			m_curFocusObject = nullptr;

		if (m_editorMode != EditorMode::Edit)
		{
			rengine::Scene* _currentScene = rengine::SceneManager::GetInstance()->GetCurrentScene().get();

			const tstring& _currentScenePath = _currentScene->GetPath();

			rengine::SceneManager::GetInstance()->LoadScene(_currentScenePath);
		}

		SetEditorMode(EditorMode::Edit);
	}

	void EventManager::NextFrame()
	{
		if (m_editorMode == EditorMode::Play || m_editorMode == EditorMode::Pause)
			SetEditorMode(EditorMode::FRAME_BY_FRAME);
	}

	void EventManager::ExitEditor()
	{
		m_editor->m_editorExit = true;
	}

}