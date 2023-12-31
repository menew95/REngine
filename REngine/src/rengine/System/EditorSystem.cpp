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

	void (*_Initialize)(editor::Editor&, const editor::EditorDesc&);
	void (*_Begin)(editor::Editor&);
	void (*_Render)(editor::Editor&);
	void (*_End)(editor::Editor&);
	bool (*_WndProc)(editor::Editor&, HWND, UINT, WPARAM, LPARAM);

	void EditorSystem::Initialize(void* hwnd, void* device, void* context)
	{
		LoadModule();

		editor::EditorDesc _desc;

		_desc._hwnd = hwnd;
		_desc._device = device;
		_desc._deviceContext = context;
		_desc._application = Application::GetInstance();

		_Initialize(*m_pEditor, _desc);
		//m_pEditor->_Initialize(_desc);
	}

	void EditorSystem::Render()
	{
		_Begin(*m_pEditor);

		_Render(*m_pEditor);

		_End(*m_pEditor);

		//m_pEditor->Begin();

		//m_pEditor->Render();

		//m_pEditor->End();
	}

	bool EditorSystem::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		//return m_pEditor->WndProc(hWnd, message, wParam, lParam);
		return m_pEditor != nullptr ? _WndProc(*m_pEditor, hWnd, message, wParam, lParam) : false;
	}

	void EditorSystem::LoadModule()
	{
		g_pEditorModule = Module::Load(Module::GetModuleFilename("Editor").c_str());

		auto _editorConstructor = (EditorConstructor)g_pEditorModule->LoadProcedure("CreateEditor");

		auto _editorDestructor = (EditorDestructor)g_pEditorModule->LoadProcedure("ReleaseEditor");

		_Initialize	= (void (*)(editor::Editor&,const editor::EditorDesc&))g_pEditorModule->LoadProcedure("?Initialize@Editor@editor@@QEAAXAEBUEditorDesc@2@@Z");
		_Begin		= (void (*)(editor::Editor&))g_pEditorModule->LoadProcedure("?Begin@Editor@editor@@QEAAXXZ");
		_Render		= (void (*)(editor::Editor&))g_pEditorModule->LoadProcedure("?Render@Editor@editor@@QEAAXXZ");
		_End		= (void (*)(editor::Editor&))g_pEditorModule->LoadProcedure("?End@Editor@editor@@QEAAXXZ");
		_WndProc	= (bool (*)(editor::Editor&,HWND, UINT, WPARAM, LPARAM))g_pEditorModule->LoadProcedure("?WndProc@Editor@editor@@QEAA_NPEAUHWND__@@I_K_J@Z");

		m_pEditor = _editorConstructor();

		assert(m_pEditor != nullptr);
	}
}