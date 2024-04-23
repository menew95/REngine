#include <Editor_pch.h>

#include <editor\GUI\Document.h>
#include <editor\GUI\View.h>

#include <ImGui\ImGuizmo.h>

namespace editor
{
	Document::Document(std::string name)
	: m_documentName(name)
	{

	}

	Document::~Document()
	{
		
	}

	void Document::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (m_isDockingEnable)
		{
			SetDockingSpace();
		}
	}

	void Document::Render()
	{
		for (auto& _view : m_childs)
		{
			if (_view->GetOpen())
			{
				_view->Begin();

				_view->Render();

				_view->End();
			}
		}
	}

	void Document::End()
	{
		ImGui::Render();

		ImDrawData* drawData = ImGui::GetDrawData();

		if (drawData != nullptr)
			ImGui_ImplDX11_RenderDrawData(drawData);

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto _context = ImGui::GetCurrentContext();

			ImGui::UpdatePlatformWindows();

			ImGui::RenderPlatformWindowsDefault();

			ImGui::SetCurrentContext(_context);
		}
	}

	View* Document::GetView(const string& name)
	{
		for (auto& _view : m_childs)
		{
			if(_view->GetName() == name)
				return _view.get();
		}

		return nullptr;
	}

	void Document::SetDockingSpace()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("##dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::SetWindowPos({ 0.f, 0.f });
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		ImGui::SetWindowSize({ (float)displaySize.x, (float)displaySize.y });
		ImGui::End();

		ImGui::PopStyleVar(3);
	}
}