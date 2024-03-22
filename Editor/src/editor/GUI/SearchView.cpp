#include <Editor_pch.h>
#include <editor\GUI\SearchView.h>

#include <rengine\System\ObjectFactory.h>

namespace editor
{
	SearchView* g_pSearchView = nullptr;

	SearchView::SearchView()
		: View("Search")
	{
		assert(g_pSearchView == nullptr);

		g_pSearchView = this;
	}
	
	SearchView::~SearchView()
	{
	
	}

	void SearchView::Begin()
	{
		if(!m_bIsOpen) return;

		__super::Begin();

	}
	
	void SearchView::Render()
	{
		if (!m_bIsOpen) return;

		__super::Render();

		struct UserData
		{
			map<uuid, std::shared_ptr<rengine::Object>> _objectMap;
			vector<shared_ptr<rengine::Object>> _objectList;
		};

		auto _objectMap = rengine::ObjectFactory::GetInstance()->FindObjectsOfType(StringHelper::StringToWString(m_searchType));

		static char _buf[256];
		int (*FindObject)(ImGuiInputTextCallbackData*)
			= [](ImGuiInputTextCallbackData* data)
			{
				tstring _searchName = StringHelper::ToWString(data->Buf);

				ImGUiUserData* _userData = reinterpret_cast<ImGUiUserData*>(data->UserData);

				_userData->_objectList.clear();

				for (auto& _pair : _userData->_objectMap)
				{
					if (_pair.second->GetName().find(_searchName) != tstring::npos)
					{
						_userData->_objectList.push_back(_pair.second);
					}
				}

				return 0;
			};

		if (ImGui::InputTextEx("Object Name", "", _buf, IM_ARRAYSIZE(_buf), ImVec2(.0f, 0.f), ImGuiInputTextFlags_CallbackEdit, FindObject, &m_userData))
		{
			int a = 0;
		}

		ImGui::PushID("Find Object");

		for (auto _obj : m_userData._objectList)
		{
			if (ImGui::Button(_obj->GetNameStr().c_str()))
			{
				/*auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

				_go->AddComponent(_comp);*/
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::PopID();
	}
	
	void SearchView::End()
	{
		if (!m_bIsOpen) return;

		__super::End();

	}

	void SearchView::OpenSeachView(string searchType)
	{
		g_pSearchView->m_bIsOpen = true;
		g_pSearchView->m_searchType = searchType;

		g_pSearchView->m_userData._objectMap = rengine::ObjectFactory::GetInstance()->FindObjectsOfType(StringHelper::StringToWString(searchType));

		g_pSearchView->m_userData._objectList.clear();

		g_pSearchView->m_userData._objectList.reserve(g_pSearchView->m_userData._objectMap.size());

		for (auto _pair : g_pSearchView->m_userData._objectMap)
		{
			g_pSearchView->m_userData._objectList.push_back(_pair.second);
		}
	}
}