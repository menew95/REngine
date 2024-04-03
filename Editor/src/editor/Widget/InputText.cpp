﻿#include <Editor_pch.h>

#include <editor\Widget\InputText.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	/*InputText::InputText(string name, string& text, uint32 flags, Event<rengine::Object, void, string> event)
	: Widget(name, flags)
	, m_text(text)
	, m_label("##")
	, m_event(event)
	{
		m_label.append(name);
	}*/

	InputText::InputText(string name, rengine::Object* handler, rttr::property& prop, uint32 flags)
	: Widget(name, flags)
	, m_pHandler(handler)
	, m_prop(prop)
	{

	}

	InputText::~InputText()
	{
	}

	void InputText::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<tstring>())
			assert(false);

		tstring _wstr = _val.convert<tstring>();

		string _str = StringHelper::WStringToString(_wstr);

		strcpy_s(m_pInputText, _str.c_str());

		ImGui::PushID(m_widgetName.c_str());

		ImGui::Text(m_widgetName.c_str());
		//ImGui::SameLine();
		ImGui::NextColumn();

		if (ImGui::InputTextEx("", "GameObject Name", m_pInputText, IM_ARRAYSIZE(m_pInputText), ImVec2(.0f, 0.f), GetFlags()))
		{
			_str = m_pInputText;

			_wstr = StringHelper::StringToWString(_str);

			m_prop.set_value(m_pHandler, _wstr);
		}

		ImGui::PopID();

		/*strcpy_s(m_pInputText, m_text.c_str());

		if (ImGui::InputTextEx(m_label.c_str(), "GameObject Name", m_pInputText, IM_ARRAYSIZE(m_pInputText), ImVec2(.0f, 0.f), GetFlags()))
		{
			m_event.Invoke(m_pInputText);
		}*/
	}
}