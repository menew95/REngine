#include <Editor_pch.h>
#include <editor\Widget\ObjectButton.h>

#include <editor\Core\AssetManager.h>
#include <rengine/core/component/Component.h>


namespace editor
{
	ObjectButton::ObjectButton(string name, rengine::Object* handler, rttr::property& prop, math::Vector2 size, uint32 flags)
		: Button(name, handler, prop, size, flags)
		, m_rectSize{ size.x, size.y }
		, m_pHandler(handler)
		, m_prop(prop)
	{
	}

	ObjectButton::~ObjectButton()
	{

	}

	void ObjectButton::Render()
	{
		ImGui::Columns(2);
		ImGui::Text(GetWidgetName().c_str());
		ImGui::NextColumn();

		string _itemName;

		auto _var = m_prop.get_value(m_pHandler);

		if (_var.is_sequential_container())
		{
			auto _seq = _var.create_sequential_view();

			if (_seq.get_size() == 0)
			{
				_itemName = "None";

				auto _type = _var.get_type();

				string _name;

				if (_type.is_wrapper())
				{
					_name = _var.get_type().get_wrapped_type().get_name().to_string();
				}
				else
				{
					_name = _var.get_type().get_name().to_string();
				}

				_itemName += _name;

				if (ImGui::ButtonEx(_itemName.c_str(), m_rectSize, GetFlags()))
				{
					//ImGui::OpenPopup("FindObjectPopup");
				}
			}
			else
			{
				for (const auto& item : _seq)
				{
					auto _extractVar = item.extract_wrapped_value();

					if (_var == nullptr)
					{
						_itemName = "None";

						auto _type = _var.get_type();

						string _name;

						if (_type.is_wrapper())
						{
							_name = _var.get_type().get_wrapped_type().get_name().to_string();
						}
						else
						{
							_name = _var.get_type().get_name().to_string();
						}

						_itemName += _name;
					}

					if (ImGui::ButtonEx(_itemName.c_str(), m_rectSize, GetFlags()))
					{
						ImGui::OpenPopup("FindObjectPopup");
					}
				}
			}
		}
		else
		{
			if (_var == nullptr)
			{
				_itemName = "None";

				auto _type = _var.get_type();

				string _name;

				if (_type.is_wrapper())
				{
					_name = _var.get_type().get_wrapped_type().get_name().to_string();
				}
				else
				{
					_name = _var.get_type().get_name().to_string();
				}

				_itemName += _name;
			}

			if (ImGui::ButtonEx(_itemName.c_str(), m_rectSize, GetFlags()))
			{
				ImGui::OpenPopup("FindObjectPopup");
			}
		}
		ImGui::EndColumns();

		static char _buf[256];
		static vector<string> _objectList;
		int (*FindObject)(ImGuiInputTextCallbackData*)
			= [](ImGuiInputTextCallbackData* data)
			{
				string _componentName = data->Buf;

				_objectList.clear();

				for (auto& _comp : AssetManager::GetInstance()->GetComponentList())
				{
					if (_comp.find(_componentName) != string::npos)
					{
						_objectList.push_back(_comp);
					}
				}

				return 0;
			};

		// 버튼을 클릭하면 오브젝트 목록 팝업을 보여줌
		if (ImGui::BeginPopup("FindObjectPopup"))
		{
			if (ImGui::InputTextEx("", "", _buf, IM_ARRAYSIZE(_buf), ImVec2(.0f, 0.f), ImGuiInputTextFlags_CallbackEdit, FindComponent))
			{
				int a = 0;
			}

			ImGui::PushID("Find Object");

			for (auto _comp : _objectList)
			{
				if (ImGui::Button(_comp.c_str()))
				{
					/*auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

					_go->AddComponent(_comp);*/

					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::PopID();


			ImGui::EndPopup();
		}
	}
}