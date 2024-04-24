#include <Editor_pch.h>

#include <editor\Core\EventManager.h>

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
			// Play Event ..?

			// TODO : 저장할 것입니다. 그래도 실행하시겠습니까 ..?

			int button = MessageBox
			(
				nullptr,
				TEXT("Play Mode"),
				TEXT("Is it okay to save \n and enter play mode? "),
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
		}
		// 아닌 경우
		else
		{
			// 그대로 스타트 하는건가 ?
		}
	}

	void EventManager::PauseGame()
	{
		SetEditorMode(EditorMode::Pause);
	}

	void EventManager::StopGame()
	{
		if (m_editorMode != EditorMode::Edit)
		{
			rengine::Scene* _currentScene = rengine::SceneManager::GetInstance()->GetCurrentScene().get();

			const tstring& _currentSceneName = _currentScene->GetName();

			
			//ameObjectUnselectedEvent.Invoke();

			// 현재 씬 파일을 자동으로 로드해서 가져옵니다. 여기서 Scene Changed Event On..


			//rengine::SceneManager::GetInstance()->LoadSceneFileFrom(currentScene->GetPath());
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