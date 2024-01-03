#include <Editor_pch.h>

#include <editor\Editor.h>

#include <editor\Core\EnginePlugin.h>

#include <editor\GUI\EditorDocument.h>

#include <rengine\System\GraphicsSystem.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace editor
{
	void Editor::Initialize(void* desc)
	{
		m_desc = *reinterpret_cast<EditorDesc*>(desc);

		m_hwnd = (HWND)m_desc._windowInfo._hWnd;

		EnginePlugin::GetInstance()->Initialize(m_desc._windowInfo);

		m_pDevice = rengine::GraphicsSystem::GetInstance()->GetDevice();
		m_pContext = rengine::GraphicsSystem::GetInstance()->GetContext();

		InitImGui();

		m_pEditorDocment = new EditorDocument();
	}

	bool Editor::Update()
	{
		Begin();

		Render();

		End();

		return EnginePlugin::GetInstance()->GetEngine()->Update();;
	}

	bool Editor::Quit()
	{
		delete m_pEditorDocment;

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		assert(EnginePlugin::GetInstance()->GetEngine()->Quit());

		return true;
	}

	void Editor::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		m_pEditorDocment->Begin();
	}

	bool show_demo_window = true;
	bool show_another_window = false;

	void Editor::Render()
	{
		m_pEditorDocment->Render();
	}

	void Editor::End()
	{
		m_pEditorDocment->End();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)m_Width, (float)m_Height);

		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto _context = ImGui::GetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			ImGui::SetCurrentContext(_context);
		}
	}

	
	bool Editor::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return true;

		return false;
	}

	void Editor::InitImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking Controls
		io.WantCaptureMouse = false;

		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());


		ImGui::StyleColorsLight();

		ImGui_ImplWin32_Init(m_hwnd);
		ImGui_ImplDX11_Init(reinterpret_cast<ID3D11Device*>(m_pDevice), reinterpret_cast<ID3D11DeviceContext*>(m_pContext));
	}

	void Editor::InitResource()
	{
		
	}
	void Editor::LoadInFile(const char* file)
	{
		ImGui::ClearIniSettings();

		std::filesystem::path dir(/*GetEditorDir().string() + */file);
		ImGui::LoadIniSettingsFromDisk(dir.string().c_str());
	}

	//std::filesystem::path Editor::GetEditorDir()
	//{
	//	//Constructs an absolute path the directoy that contains information about the Editor.
	//	std::string workingDir = std::filesystem::current_path().string();

	//	auto editorDir = workingDir + "\\ProjectSettings\\";

	//	std::cout << "Editor Directory: " << editorDir << "\n";

	//	return std::filesystem::path(editorDir);
	//}

	void Editor::GetEditorDir(void* path)
	{
		std::filesystem::path* _path = reinterpret_cast<std::filesystem::path*>(path);

		std::string workingDir = std::filesystem::current_path().string();

		auto editorDir = workingDir + "\\ProjectSettings\\";

		std::cout << "Editor Directory: " << editorDir << "\n";

		*_path = std::filesystem::path(editorDir);
	}

	void Editor::LoadLayout(std::string aLayout, void* someContext)
	{
		//InitializeContext

		//Read the json file
		//ember::Json layoutData = ember::Json::Read(aLayout);

		////Check if the json file exists
		//if (!layoutData) return;

		////Reduntant check to make sure that the io.InitFilename is indeed nullptr
		//ImGuiIO& io = ImGui::GetIO();
		//io.IniFilename = NULL;

		////Fetch the file address for the .ini file and the layout structure.
		//auto imguiIni = layoutData.Get<std::string>("imguiData");
		//auto layout = layoutData.Get<ember::Json>("layout");

		////Clear any previous editors that were alive beforehand.
		//myRootContext->RemoveAllChildren();

		////Iterate through the layout and create an instance of each type stored.
		//for (size_t i = 0; i < layout.GetArraySize(); i++)
		//{
		//	auto data = layoutData.Get("layout", i);
		//	auto ins = CreateInstance(data.Get<StringID>("type"), someContext);
		//}

		//Load the init file.
		//LoadIniFile(imguiIni.c_str());
	}
}