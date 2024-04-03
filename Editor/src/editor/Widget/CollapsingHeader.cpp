#include <Editor_pch.h>

#include <editor\Widget\CollapsingHeader.h>

#include <editor\Core\EventManager.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>
namespace editor
{
	CollapsingHeader::CollapsingHeader(string name, uint32 flags)
	: WidgetContainer(name, flags)
	{
	
	}
	
	CollapsingHeader::~CollapsingHeader()
	{
	
	}

	void CollapsingHeader::Render()
	{
		ImGui::PushID(m_widgetName.c_str());
		if (ImGui::CollapsingHeader(GetWidgetName().c_str(), m_flags))
		{
			// 우클릭 이벤트 이다. CollapsingHeader widget이 현재로는 컴포넌트 전용으로만 사용이 됨으로 여기에 추가하였다.
			if (ImGui::IsItemClicked(1))
			{
				ImGui::OpenPopup("CollapsingHeader Popup");
			}

			if (ImGui::BeginPopup("CollapsingHeader Popup"))
			{
				if (ImGui::MenuItem("Reset Component"))
				{
					// 컴포넌트 요소를 초기 상태로 리셋
				}
				if (ImGui::MenuItem("Delete Component", NULL, false, m_widgetName == "Transform" ? false : true))
				{
					
					auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

					for (auto& _comp : _go->GetComponents())
					{
						if (_comp.lock()->GetTypeStr() == m_widgetName)
						{
							rengine::ObjectFactory::GetInstance()->ReserveDestroyObject(_comp.lock());
						}
					}

				}
				ImGui::EndPopup();
			}

			ImGui::Columns(2);

			for (auto& _widget : GetChilds())
			{
				_widget->Render();

				ImGui::NextColumn();
			}

			ImGui::Columns(1);

			//ImGui::EndColumns();
		}
		ImGui::PopID();
	}
}