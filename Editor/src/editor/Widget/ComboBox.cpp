#include <Editor_pch.h>

#include <editor\Widget\ComboBox.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	ComboBox::ComboBox(const string& id, rengine::Object* handler, rttr::property& prop, uint32 flags)
		: Widget(id, flags)
		, m_pHandler(handler)
		, m_prop(prop)
	{
		/*
			enumeration enum_align = enum_type.get_enumeration();
			std::string name = enum_align.value_to_name(E_Alignment::AlignHCenter);
			std::cout << name; // prints "AlignHCenter"
			variant var = enum_align.name_to_value(name);
			E_Alignment value = var.get_value<E_Alignment>(); // stores value 'AlignHCenter'
		*/

		// char*를 캐싱 해두어야 하기때문에 string list를 미리 생성할 필요가 있다.
		rttr::type _enumType = rttr::type::get_by_name(m_prop.get_name());

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

	void ComboBox::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		m_item_current = _val.convert<uint32>();

		vector<const char*> _items;

		for (auto& _name : m_items)
		{
			_items.push_back(_name.c_str());
		}

		if (ImGui::Combo(m_id.c_str(), &m_item_current, _items.data(), static_cast<int>(_items.size())))
		{
			m_prop.set_value(m_pHandler, (uint32)m_item_current);
		}
	}
}