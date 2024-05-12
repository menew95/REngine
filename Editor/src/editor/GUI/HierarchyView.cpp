#include <Editor_pch.h>
#include <ImGui\imgui_internal.h>

#include <editor\GUI\HierarchyView.h>
#include <editor\GUI\EditorStyle.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Transform.h>
#include <rengine\core\scene\scene.h>
#include <rengine\core\sceneManager.h>
#include <rengine\System\Input.h>

rengine::GameObject* g_rootGO = nullptr;

namespace editor
{
	HierarchyView::HierarchyView()
		: View("Hierarchy View")
	{

	}

	HierarchyView::~HierarchyView()
	{

	}

	void HierarchyView::Begin()
	{
		__super::Begin();

	}

	void HierarchyView::Render()
	{
		__super::Render();

		auto& g = *ImGui::GetCurrentContext();

		ImVec2 pos = ImGui::GetCursorPos();
		ImGui::Dummy(ImGui::GetContentRegionAvail());
		if (ImGui::BeginDragDropTarget())
		{
			auto* _payload = ImGui::AcceptDragDropPayload("HIERARCHY_ITEM");

			if (_payload != nullptr)
			{
				rengine::GameObject** _dropGO = reinterpret_cast<rengine::GameObject**>(_payload->Data);

				if ((*_dropGO)->GetTransform()->GetParent() != nullptr)
				{
					(*_dropGO)->GetTransform()->SetParent(nullptr);
				}
			}

			ImGui::EndDragDropTarget();
		}
		ImGui::SetCursorPos(pos);

		for (auto& _root : rengine::SceneManager::GetInstance()->GetCurrentScene()->GetRootGameObjects())
		{
			DrawTreeNode(_root.get());
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("HierarchyView Popup(None Focus)");
		}

		if (ImGui::BeginPopup("HierarchyView Popup(None Focus)"))
		{
			if (ImGui::MenuItem("Add Object"))
			{
				auto _newGO = rengine::GameObject::Instantiate(nullptr);

				_newGO->SetName(L"Game Object");

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void HierarchyView::End()
	{
		__super::End();

	}

	void HierarchyView::SetScene()
	{

	}

	void HierarchyView::DrawTreeNode(rengine::GameObject* gameObj)
	{
		if(gameObj == nullptr)
			return;

		ImGuiTreeNodeFlags nodeFlag = ImGuiTreeNodeFlags_None;
		nodeFlag |= ImGuiTreeNodeFlags_OpenOnArrow;

		if (gameObj->GetTransform()->GetChildSize() == 0)
		{
			nodeFlag |= ImGuiTreeNodeFlags_Leaf;
		}

		auto _iter = std::ranges::find_if(m_controlList,
			[&](auto& iter)
			{
				return gameObj->GetUUID() == iter->GetUUID();
			});

#pragma region push style
		/// 특정 객체를 선택하면 그 객체만 아닌 자식 객체도 모두 색칠되어 일단 주석처리
		if (_iter != m_controlList.end())
		{
			ImGui::PushStyleColor(ImGuiCol_Header, EditorStyle::GetColor(ImGuiCol_Header));          // 기본 색상
			ImGui::PushStyleColor(ImGuiCol_Text, EditorStyle::GetColor(ImGuiCol_Text));
		}

		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EditorStyle::GetColor(ImGuiCol_HeaderHovered));
#pragma endregion

		bool open;
		if (gameObj->GetName().empty() == true)
		{
			open = ImGui::TreeNodeEx(gameObj, nodeFlag, "UnNamed Object");
		}
		else
		{
			open = ImGui::TreeNodeEx(gameObj, nodeFlag, gameObj->GetNameStr().c_str());
		}

#pragma region pop style

		ImGui::PopStyleColor(1);

		if (_iter != m_controlList.end())
		{
			ImGui::PopStyleColor(2);
		}

#pragma endregion

		ImGui::Spacing();

#pragma region Input Event

		if (ImGui::IsItemClicked())
		{
			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::LeftShift))
			{
				m_controlList.push_back(gameObj);
			}
			else
			{
				m_controlList.clear();

				m_controlList.push_back(gameObj);

				EventManager::GetInstance()->SetFocusObject(gameObj);
			}
		}

		DrawPopUp(open, gameObj);

		if (!ImGui::GetDragDropPayload() && ImGui::BeginDragDropSource()) 
		{
			ImGui::SetDragDropPayload("HIERARCHY_ITEM", &gameObj, sizeof(rengine::GameObject*), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			auto* _payload = ImGui::AcceptDragDropPayload("HIERARCHY_ITEM");

			if (_payload != nullptr)
			{
				rengine::GameObject** _dropGO = reinterpret_cast<rengine::GameObject**>(_payload->Data);

				(*_dropGO)->GetTransform()->SetParent(gameObj->GetTransform());
			}

			ImGui::EndDragDropTarget();
		}

#pragma endregion

		if (open)
		{
			for (uint32 i = 0; i < gameObj->GetTransform()->GetChildSize(); i++)
			{
				auto _child = gameObj->GetTransform()->GetChild(i);
				if (_child != nullptr && _child->GetGameObject().lock())
				{
					DrawTreeNode(gameObj->GetTransform()->GetChild(i)->GetGameObject().lock().get());
				}
			}
			ImGui::TreePop();
		}
	}

	void HierarchyView::DrawPopUp(bool& open, rengine::GameObject* gameObj)
	{
		if (ImGui::IsItemClicked(1))
		{
			m_controlList.clear();

			m_controlList.push_back(gameObj);

			EventManager::GetInstance()->SetFocusObject(gameObj);

			m_bPopUpMenu = true;
		}

		if (m_bPopUpMenu == true)
		{
			ImGui::PushID("HierarchyView Popup");
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Add Object"))
				{
					// 부모가 존재하는 오브젝트를 생성
					auto _newGO = rengine::GameObject::Instantiate(m_controlList.at(0)->GetTransform());

					_newGO->SetName(L"Game Object");

					m_bPopUpMenu = false;
				}

				if (ImGui::MenuItem("Delete Object"))
				{
					rengine::Object::Destroy(m_controlList.at(0)->GetShared());

					EventManager::GetInstance()->SetFocusObject(nullptr);

					m_bPopUpMenu = false;
				}

				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
	}
}