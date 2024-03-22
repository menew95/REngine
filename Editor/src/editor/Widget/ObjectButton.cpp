#include <Editor_pch.h>
#include <editor\GUI\SearchView.h>
#include <editor\Widget\ObjectButton.h>

#include <editor\Core\AssetManager.h>
#include <rengine\core\component\Component.h>
#include <rengine\System\ObjectFactory.h>
namespace editor
{
	ObjectButton::ObjectButton(string widgetName,rengine::Object* handler, rttr::property& prop, math::Vector2 size, uint32 flags)
		: Button(widgetName, handler, prop, size, flags)
		, m_rectSize{ size.x, size.y }
		, m_pHandler(handler)
		, m_prop(prop)
	{
	}

	ObjectButton::~ObjectButton()
	{

	}

	string GetObjectName(rttr::variant& var)
	{
		string _objName;

		auto _varType = var.get_type();

		if (var.is_sequential_container())
		{
			auto _seq = var.create_sequential_view();

			_varType = _seq.get_value_type();
		}

		if (_varType.is_wrapper())
			_varType = _varType.get_wrapped_type();
		if (_varType.is_pointer())
			_varType = _varType.get_raw_type();

		_objName = _varType.get_name().to_string();

		return std::move(_objName);
	}

	string GetLableName(rttr::variant& var)
	{
		rengine::Object* _obj = nullptr;

		if (var.get_type().is_wrapper())
		{
			auto _wrap = var.extract_wrapped_value();
			_obj = _wrap.get_value<rengine::Object*>();
		}
		else
			_obj = var.get_value<rengine::Object*>();

		assert(_obj = nullptr);

		return _obj->GetNameStr();
	}

	void ObjectButton::Render()
	{
		ImGui::Columns(2);
		ImGui::Text(GetWidgetName().c_str());
		ImGui::NextColumn();

		auto _var = m_prop.get_value(m_pHandler);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		string _objName = GetObjectName(_var);
		string _lable = "None(" + _objName + ")";


		_test = false;

		if (_var.is_sequential_container())
		{
			auto _seq = _var.create_sequential_view();

			if (_seq.get_size() == 0)
			{
				if (ImGui::ButtonEx(_lable.c_str(), m_rectSize, GetFlags()))
				{
					ImGui::OpenPopup("FindObjectPopup");
					_test = true;
				}
			}
			else
			{
				for (const auto& item : _seq)
				{
					auto _extractVar = item.extract_wrapped_value();

					if (_extractVar != nullptr)
						_lable = GetLableName(_extractVar);

					if (ImGui::ButtonEx(_lable.c_str(), m_rectSize, GetFlags()))
					{
						ImGui::OpenPopup("FindObjectPopup");
						_test = true;
					}
				}
			}
		}
		else
		{
			if(_var != nullptr)
				_lable = GetLableName(_var);

			if (ImGui::ButtonEx(_lable.c_str(), m_rectSize, GetFlags()))
			{
				ImGui::OpenPopup("FindObjectPopup");
				_test = true;
			}
		}
		
		ImGui::EndColumns();

		if (_test)
		{
			SearchView::OpenSeachView(_objName);
		}

		//auto _objectMap = rengine::ObjectFactory::GetInstance()->FindObjectsOfType(StringHelper::StringToWString(_objName));

		//static char _buf[256];
		//static vector<string> _objectList;
		//int (*FindObject)(ImGuiInputTextCallbackData*)
		//	= [](ImGuiInputTextCallbackData* data)
		//	{
		//		tstring _componentName = StringHelper::ToWString(data->Buf);

		//		_objectList.clear();

		//		map<uuid, std::shared_ptr<rengine::Object>>* _map = (map<uuid, std::shared_ptr<rengine::Object>>*)((data->UserData));

		//		for (auto& _comp : *_map)
		//		{
		//			/*if (_comp.find(_componentName) != tstring::npos)
		//			{
		//				_objectList.push_back(_comp);
		//			}*/
		//		}

		//		return 0;
		//	};

		//// 버튼을 클릭하면 오브젝트 목록 팝업을 보여줌
		//if (_test && ImGui::BeginPopup("FindObjectPopup"))
		//{
		//	if (ImGui::InputTextEx("Object Name", "", _buf, IM_ARRAYSIZE(_buf), ImVec2(.0f, 0.f), ImGuiInputTextFlags_CallbackEdit, FindObject, &_objectMap))
		//	{
		//		int a = 0;
		//	}

		//	ImGui::PushID("Find Object");

		//	for (auto _comp : _objectList)
		//	{
		//		if (ImGui::Button(_comp.c_str()))
		//		{
		//			/*auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

		//			_go->AddComponent(_comp);*/
		//			_test = false;
		//			ImGui::CloseCurrentPopup();
		//		}
		//	}
		//	ImGui::PopID();


		//	ImGui::EndPopup();
		//}


		ImGui::PopStyleColor();
	}
	int ObjectButton::FindObjects(ImGuiInputTextCallbackData* data)
	{
		return 0;
	}
}