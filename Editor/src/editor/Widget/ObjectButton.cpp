#include <Editor_pch.h>
#include <editor\GUI\SearchView.h>
#include <editor\Widget\ObjectButton.h>

#include <editor\Core\AssetManager.h>
#include <rengine\core\component\Component.h>
#include <rengine\System\ObjectFactory.h>
#include <rengine\core\resource\Mesh.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace editor
{
	ObjectButton::ObjectButton(const string& lable, math::Vector2 size, uint32 flags)
		: WidgetData<rttr::variant>(lable, flags)
		, m_rectSize{ size.x, size.y }
	{
		m_flags |= ImGuiButtonFlags_PressedOnDoubleClick;

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

		assert(_obj != nullptr);

		return _obj->GetNameStr();
	}

	void ObjectButton::Draw()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		string _objName = GetObjectName(m_data);
		string _lable = "None(" + _objName + ")";

		ButtonEvent _buttonEvent = nullptr;
		bool _openSeachView = false;

		if (m_data.is_sequential_container())
		{
			auto _seq = m_data.create_sequential_view();
			m_bIsArray = true;

			if (_seq.get_size() == 0)
			{
				if (ImGui::ButtonEx((_lable + m_idString).c_str(), m_rectSize, GetFlags()))
					_openSeachView = true;

				if (ImGui::BeginDragDropTarget())
				{
					auto* _payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");

					if (_payload != nullptr && _payload->IsDelivery())
					{
						const wchar_t* _wstr = static_cast<const wchar_t*>(_payload->Data);

						fs::path _payloadPath(_wstr);

						auto _type_name = StringHelper::StringToWString(m_data.get_type().get_name().to_string());

						auto _asset = AssetManager::GetInstance()->AssetFromPath(
							fs::absolute(_payloadPath).c_str()
							, StringHelper::StringToWString(_objName)
						);

						// 만약 에셋이 다른 타입일 경우를 처리하기 위해 일단 이렇게 처리
						if (_asset != nullptr)
							SetProperty(this, _asset);
					}

					ImGui::EndDragDropTarget();
				}
			}
			else
			{
				for (size_t i = 0; i < _seq.get_size(); i++)
				{
					auto _extractVar = _seq.get_value(i).extract_wrapped_value();

					if (_extractVar != nullptr)
						_lable = GetLableName(_extractVar);

					if (ImGui::ButtonEx((_lable + m_idString).c_str(), m_rectSize, GetFlags()))
					{
						_openSeachView = true;
						m_arrayIndex = i;
					}

					if (ImGui::BeginDragDropTarget())
					{
						auto* _payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");

						if (_payload != nullptr && _payload->IsDelivery())
						{
							const wchar_t* _wstr = static_cast<const wchar_t*>(_payload->Data);

							fs::path _payloadPath(_wstr);

							auto _asset = AssetManager::GetInstance()->AssetFromPath(
								fs::absolute(_payloadPath).c_str()
								, StringHelper::StringToWString(_objName)
							);

							// 만약 에셋이 다른 타입일 경우를 처리하기 위해 일단 이렇게 처리
							if (_asset != nullptr)
								SetProperty(this, _asset);
						}

						ImGui::EndDragDropTarget();
					}
				}
			}
		}
		else
		{
			m_bIsArray = false;

			if (m_data != nullptr)
				_lable = GetLableName(m_data);

			if (ImGui::ButtonEx((_lable + m_idString).c_str(), m_rectSize, GetFlags()))
			{
				_openSeachView = true;
			}

			if (ImGui::BeginDragDropTarget())
			{
				auto* _payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");

				if (_payload != nullptr && _payload->IsDelivery())
				{
					const wchar_t* _wstr = static_cast<const wchar_t*>(_payload->Data);

					fs::path _payloadPath(_wstr);

					auto _asset = AssetManager::GetInstance()->AssetFromPath(
						fs::absolute(_payloadPath).c_str()
						, StringHelper::StringToWString(_objName)
					);

					// 만약 에셋이 다른 타입일 경우를 처리하기 위해 일단 이렇게 처리
					if (_asset != nullptr)
						SetProperty(this, _asset);
				}

				ImGui::EndDragDropTarget();
			}
		}

		if (_openSeachView)
		{
			SearchView::OpenSeachView(_objName, this, &ObjectButton::SetProperty);
		}

		ImGui::PopStyleColor();

	}

	void ObjectButton::SetProperty(void* thisClass, const shared_ptr<rengine::Object>& obj)
	{
		ObjectButton* _this = reinterpret_cast<ObjectButton*>(thisClass);

		if (_this->m_bIsArray)
		{
			auto _seq = _this->m_data.create_sequential_view();

			// shared_ptr<object> => shared_ptr<해당클래스>
			rttr::variant _var = obj;

			assert(_var.convert(_seq.get_value_type()));

			if (_seq.get_size() == 0)
			{
				_seq.set_size(1);
				_seq.set_value(0, _var);
			}
			else
				_seq.set_value(_this->m_arrayIndex, _var);

			_this->m_setter(_this->m_data);
		}
		else
		{
			_this->m_data = obj;

			_this->m_setter(_this->m_data);
		}
	}
}