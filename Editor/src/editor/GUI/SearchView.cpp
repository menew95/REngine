#include <Editor_pch.h>
#include <editor\GUI\SearchView.h>

#include <rengine\System\ObjectFactory.h>

namespace editor
{
	SearchView* g_pSearchView = nullptr;

	SearchView::SearchView()
		: View("Search")
		, m_findObjectListEvent(nullptr)
		, m_clickItemEvent(nullptr)
	{
		assert(g_pSearchView == nullptr);

		g_pSearchView = this;

		m_bOpen = false;
	}
	
	SearchView::~SearchView()
	{
	
	}

	void SearchView::Begin()
	{
		__super::Begin();

	}
	
	void SearchView::Render()
	{
		__super::Render();

		//auto _objectMap = m_findObjectListEvent();

		int (*FindObject)(ImGuiInputTextCallbackData*)
			= [](ImGuiInputTextCallbackData* data)
			{
				tstring _searchName = StringHelper::ToWString(data->Buf);

				std::transform(_searchName.begin(), _searchName.end(), _searchName.begin(), ::tolower);

				ImGUiUserData* _userData = reinterpret_cast<ImGUiUserData*>(data->UserData);

				_userData->_objectList.clear();

				for (auto& _pair : _userData->_objectMap)
				{
					auto _objectName = _pair.second->GetName();

					std::transform(_objectName.begin(), _objectName.end(), _objectName.begin(), ::tolower);

					if (_objectName.find(_searchName) != tstring::npos)
					{
						_userData->_objectList.push_back(_pair.second);
					}
				}

				return 0;
			};

		if (ImGui::InputTextEx("Object Name", "", m_searchTextBuf, IM_ARRAYSIZE(m_searchTextBuf), ImVec2(.0f, 0.f), ImGuiInputTextFlags_CallbackEdit, FindObject, &m_userData))
		{
			// 텍스트 비교로 오브젝트를 필터링 따로 이벤트 매니저에서 처리할 방법은 없나?
		}

		ImGui::PushID("Find Object");
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		if (ImGui::ButtonEx("None##0", ImVec2(ImGui::GetWindowWidth() * 0.9f, 0.f), ImGuiButtonFlags_PressedOnDoubleClick))
		{
			m_clickItemEvent(nullptr);
			m_bOpen = false;
		}

		for (auto& _obj : m_userData._objectList)
		{
			if (ImGui::ButtonEx(_obj->GetNameStr().c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 0.f), ImGuiButtonFlags_PressedOnDoubleClick))
			{
				m_clickItemEvent(_obj);
				m_bOpen = false;
			}
		}

		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
		ImGui::PopID();
	}
	
	void SearchView::End()
	{
		__super::End();

	}

	SearchView& SearchView::GetInstance()
	{
		return *g_pSearchView;
	}

	void SearchView::OpenSeachView(string searchType
	, const std::function<void(const shared_ptr<rengine::Object>&)>& itemClickEvent
	, const std::function< map<uuid, std::shared_ptr<rengine::Object>>()>& objectFilterFunc
	)
	{
		g_pSearchView->m_bOpen = true;
		g_pSearchView->m_searchType = searchType;

		g_pSearchView->m_clickItemEvent = itemClickEvent;
		
		g_pSearchView->m_findObjectListEvent = objectFilterFunc;

		memset(g_pSearchView->m_searchTextBuf, 0, sizeof(m_searchTextBuf));

		g_pSearchView->m_userData._objectMap = objectFilterFunc();//rengine::ObjectFactory::GetInstance()->FindObjectsOfType(StringHelper::StringToWString(searchType));

		g_pSearchView->m_userData._objectList.clear();

		g_pSearchView->m_userData._objectList.reserve(g_pSearchView->m_userData._objectMap.size());

		for (auto _pair : g_pSearchView->m_userData._objectMap)
		{
			g_pSearchView->m_userData._objectList.push_back(_pair.second);
		}
	}
}