#include <Editor_pch.h>

#include <editor\GUI\EditorView.h>
#include <editor\GUI\GameView.h>
#include <editor\GUI\HierarchyView.h>
#include <editor\GUI\InspectorView.h>
#include <editor\GUI\ProjectView.h>




namespace editor
{
	EditorView::EditorView()
		: View("Editor View")
	{
		m_bOpen = true;

		m_pGameView = new GameView();
		m_pInspectorView = new InspectorView();
		m_pHierarchyView = new HierarchyView();
		m_pProjectView = new ProjectView();

		m_views.push_back(m_pGameView);
		m_views.push_back(m_pInspectorView);
		m_views.push_back(m_pHierarchyView);
		m_views.push_back(m_pProjectView);
	}

	EditorView::~EditorView()
	{
		delete m_pGameView;
		delete m_pInspectorView;
		delete m_pHierarchyView;
		delete m_pProjectView;
	}

	void EDITOR_API EditorView::Begin()
	{
		// virtual를 통해 세개의 인자를 다 넘겨 받는 형식으로 바꿔야하나?
		//__super::Begin();

		m_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		m_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		m_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleColor(ImGuiCol_FrameBg,		ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg,	ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border,		ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg,		ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg,		ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Header,		ImVec4(0.8f, 0.8f, 0.8f, 1.0f));

		__super::Begin();

		ImGui::PopStyleColor(6);

		/*ImVec2 _pos = { 0, 0 };
		ImGui::SetWindowPos(m_ViewName.c_str(), _pos);
		ImVec2 _size = { 1280, 720 };
		ImGui::SetWindowSize(m_ViewName.c_str(), _size);*/

		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiID dockspaceID = ImGui::GetID(m_ViewName.c_str());


		if (!ImGui::DockBuilderGetNode(dockspaceID)) {
			ImGui::DockBuilderRemoveNode(dockspaceID);
			ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);

			ImGuiID dock_main_id = dockspaceID;
			ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
			ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
			ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
			ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
			ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);

			ImGui::DockBuilderDockWindow("Hierarchy View", dock_right_id);
			/*ImGui::DockBuilderDockWindow("Actions", dock_up_id);
			ImGui::DockBuilderDockWindow("Hierarchy", dock_right_id);
			ImGui::DockBuilderDockWindow("Inspector", dock_left_id);
			ImGui::DockBuilderDockWindow("Console", dock_down_id);
			ImGui::DockBuilderDockWindow("Project", dock_down_right_id);
			ImGui::DockBuilderDockWindow("Scene", dock_main_id);*/

		//	 Disable tab bar for custom toolbar
			ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_up_id);
			node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

			ImGui::DockBuilderFinish(dock_main_id);
		}
		ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("View"))
			{

				if (ImGui::MenuItem("Flag: GameView", "", m_pGameView->GetOpen() != 0))
				{
					m_pGameView->SetOpen(!m_pGameView->GetOpen());
				}
				if (ImGui::MenuItem("Flag: HierarchyView", "", m_pHierarchyView->GetOpen() != 0))
				{
					m_pHierarchyView->SetOpen(!m_pHierarchyView->GetOpen());
				}
				if (ImGui::MenuItem("Flag: InspectorView", "", m_pInspectorView->GetOpen() != 0))
				{
					m_pInspectorView->SetOpen(!m_pInspectorView->GetOpen());
				}
				if (ImGui::MenuItem("Flag: ProjectView", "", m_pProjectView->GetOpen() != 0))
				{
					m_pProjectView->SetOpen(!m_pProjectView->GetOpen());
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Object"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Layout"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

	}

	//bool show_demo_window = true;
	//bool show_another_window = false;

	void EDITOR_API EditorView::Render()
	{
		__super::Render();

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//if (show_demo_window)
		//	ImGui::ShowDemoWindow(&show_demo_window);

		//static float f = 0.0f;
		//static int counter = 0;

		//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		//auto& io = ImGui::GetIO();

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//ImGui::End();

		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//	ImGui::Text("Hello from another window!");
		//	if (ImGui::Button("Close Me"))
		//		show_another_window = false;
		//	ImGui::End();
		//}


		for (auto* _view : m_views)
		{
			if (_view->GetOpen())
			{
				_view->Begin();

				_view->Render();

				_view->End();
			}
		}
	}

	void EDITOR_API EditorView::End()
	{
		__super::End();
	}
}