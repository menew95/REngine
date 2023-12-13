#include <common\Module.h>

#include <rengine/System/EditorSystem.h>

#include <rengine\application\application.h>

#include <editor/EditorAPI.h>

std::unique_ptr<Module> g_pEditorModule;

namespace rengine
{
	DEFINE_SINGLETON_CLASS(EditorSystem, 
	{

	}, 
	{

	},
	{
		if (g_pEditorModule != nullptr)
		{
			g_pEditorModule.reset();
		}
	}
	)

	void EditorSystem::Initialize(void* hwnd, void* device, void* context)
	{
		LoadModule();

		editor::EditorDesc _desc;

		_desc._hwnd = hwnd;
		_desc._device = device;
		_desc._deviceContext = context;
		_desc._application = Application::GetInstance();

		m_pEditor->Initialize(_desc);
	}

	void EditorSystem::Render()
	{
		m_pEditor->Begin();

		m_pEditor->Render();

		m_pEditor->End();
	}

	bool EditorSystem::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return m_pEditor->WndProc(hWnd, message, wParam, lParam);
	}

	void EditorSystem::LoadModule()
	{
		g_pEditorModule = Module::Load(Module::GetModuleFilename("Editor").c_str());

		auto _editorConstructor = (EditorConstructor)g_pEditorModule->LoadProcedure("CreateEditor");

		auto _editorDestructor = (EditorDestructor)g_pEditorModule->LoadProcedure("ReleaseEditor");

		m_pEditor = _editorConstructor();

		assert(m_pEditor != nullptr);
	}
}