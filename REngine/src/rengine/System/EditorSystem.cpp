#include <rengine/System/EditorSystem.h>

#include <editor/EditorAPI.h>

namespace rengine
{
	DEFINE_SINGLETON_CLASS(EditorSystem, 
	{
	}, 
	{
	})

	void EditorSystem::Initialize(void* hwnd, void* device, void* context)
	{
		LoadModule();

		editor::EditorDesc _desc;

		_desc._hwnd = hwnd;
		_desc._device = device;
		_desc._deviceContext = context;

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

#ifdef _DEBUG
#ifdef x64
		auto _editorModule = LoadLibrary(_T("Editor_x64_Debug.dll"));
#else
		auto _editorModule = LoadLibrary(_T("Editor_x86_Debug.dll"));
#endif // x64
#else
#ifdef x64
		auto _editorModule = LoadLibrary(_T("Editore_x64_Release.dll"));
#else
		auto _editorModule = LoadLibrary(_T("Editor_x86_Release.dll"));
#endif // x64
#endif

		assert(_editorModule != NULL);

		auto _editorConstructor = (EditorConstructor)GetProcAddress(_editorModule, "CreateEditor");

		assert(_editorConstructor != NULL);

		auto _editorDestructor = (EditorDestructor)GetProcAddress(_editorModule, "ReleaseEditor");

		assert(_editorDestructor != NULL);

		m_pEditor = _editorConstructor();

		assert(m_pEditor != nullptr);
	}
}