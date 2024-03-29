﻿#include <Editor_pch.h>

#include <editor\GUI\EditorDocument.h>
#include <editor\GUI\GameView.h>
#include <editor\GUI\HierarchyView.h>
#include <editor\GUI\InspectorView.h>
#include <editor\GUI\ProjectView.h>
#include <editor\GUI\SearchView.h>

#include <rengine\core\SceneManager.h>

namespace editor
{
	EditorDocument::EditorDocument()
		: Document("Editor View")
	{
		m_pGameView = new GameView();
		m_pInspectorView = new InspectorView();
		m_pHierarchyView = new HierarchyView();
		m_pProjectView = new ProjectView();
		m_pSearchView = new SearchView();

		m_childs.push_back(m_pGameView);
		m_childs.push_back(m_pInspectorView);
		m_childs.push_back(m_pHierarchyView);
		m_childs.push_back(m_pProjectView);
		m_childs.push_back(m_pSearchView);
	}

	EditorDocument::~EditorDocument()
	{
		delete m_pGameView;
		delete m_pInspectorView;
		delete m_pHierarchyView;
		delete m_pProjectView;
		delete m_pSearchView;
	}

	void EDITOR_API EditorDocument::Begin()
	{
		m_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		m_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		m_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		/*ImGui::PushStyleColor(ImGuiCol_FrameBg,		EditorStyle::GetColor(ImGuiCol_FrameBg));
		ImGui::PushStyleColor(ImGuiCol_WindowBg,	EditorStyle::GetColor(ImGuiCol_WindowBg));
		ImGui::PushStyleColor(ImGuiCol_Border,		EditorStyle::GetColor(ImGuiCol_Border));
		ImGui::PushStyleColor(ImGuiCol_TitleBg,		EditorStyle::GetColor(ImGuiCol_TitleBg));
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg,	EditorStyle::GetColor(ImGuiCol_MenuBarBg));
		ImGui::PushStyleColor(ImGuiCol_Header,		EditorStyle::GetColor(ImGuiCol_Header));*/

		__super::Begin();


		/*ImVec2 _pos = { 0, 0 };
		ImGui::SetWindowPos(m_ViewName.c_str(), _pos);
		ImVec2 _size = { 1280, 720 };
		ImGui::SetWindowSize(m_ViewName.c_str(), _size);*/

		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiID dockspaceID = ImGui::GetID(m_documentName.c_str());


		if (!ImGui::DockBuilderGetNode(dockspaceID)) {
			ImGui::DockBuilderRemoveNode(dockspaceID);
			ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);

			ImGuiID dock_main_id = dockspaceID;
			ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
			ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
			ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
			ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
			ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);

			//ImGui::DockBuilderDockWindow("Hierarchy View", dock_right_id);
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
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Flag: Save"))
				{
					rengine::SceneManager::GetInstance()->SaveScene();
				}
				if (ImGui::MenuItem("Flag: Exit"))
				{

				}

				ImGui::EndMenu();
			}

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
				ImGui::BeginChild("Another WindowX", ImVec2(300, 200), true);//    Pass a pointer to our bool variable(the window will have a closing button that will clear the bool when clicked)
					ImGui::Button("Hello from another window!");

				ImGui::EndChild();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

	}

	void EDITOR_API EditorDocument::Render()
	{
		__super::Render();

		for (auto* _view : m_childs)
		{
			if (_view->GetOpen())
			{
				_view->Begin();

				_view->Render();

				_view->End();
			}
		}
	}

	void EDITOR_API EditorDocument::End()
	{
		__super::End();

		//ImGui::PopStyleColor(6);
	}
}