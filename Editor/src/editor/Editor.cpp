#include "Editor_pch.h"

#include <editor/Editor.h>


namespace editor
{
	void Editor::Initialize(const EditorDesc& desc)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui_ImplWin32_Init(desc._hwnd);
		ImGui_ImplDX11_Init(reinterpret_cast<ID3D11Device*>(desc._device), reinterpret_cast<ID3D11DeviceContext*>(desc._deviceContext));
	}

	void Editor::InitImGui()
	{

	}
}