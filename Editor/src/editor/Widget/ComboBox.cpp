#include <Editor_pch.h>

#include <editor\Widget\ComboBox.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	ComboBox::ComboBox(const string& id, rttr::property& prop, uint32 flags)
	: WidgetData<int>(id, flags)
	{
		/*
			enumeration enum_align = enum_type.get_enumeration();
			std::string name = enum_align.value_to_name(E_Alignment::AlignHCenter);
			std::cout << name; // prints "AlignHCenter"
			variant var = enum_align.name_to_value(name);
			E_Alignment value = var.get_value<E_Alignment>(); // stores value 'AlignHCenter'
		*/

		// char*를 캐싱 해두어야 하기때문에 string list를 미리 생성할 필요가 있다.
		rttr::type _enumType = rttr::type::get_by_name(prop.get_name());

		rttr::enumeration _enums = _enumType.get_enumeration();

		auto _names = _enums.get_names();

		for (auto& _name : _names)
		{
			m_items.push_back(_name.to_string());
		}

	}

	ComboBox::~ComboBox()
	{

	}

	void ComboBox::Draw()
	{
		if (ImGui::BeginCombo(m_id.c_str(), m_items[m_data].c_str()))
		{
			for (size_t i = 0; i < m_items.size(); i++)
			{
				bool selected = (i == m_data);

				if (ImGui::Selectable(m_items[i].c_str(), selected))
				{
					if (!selected)
					{
						ImGui::SetItemDefaultFocus();

						m_data = i;

						m_isValChange = true;
					}
				}
			}

			ImGui::EndCombo();
		}
	}
}