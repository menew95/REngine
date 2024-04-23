#include <Editor_pch.h>

#include <editor\Core\EventManager.h>

#include <editor\GUI\Document.h>

#include <editor\Editor.h>

#include <rengine\rengine.h>

#include <rengine\core\resources.h>

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

	}

	void EventManager::PauseGame()
	{

	}

	void EventManager::StopGame()
	{

	}

	void EventManager::NextFrame()
	{

	}

	void EventManager::ExitEditor()
	{
		m_editor->Quit();
	}

}