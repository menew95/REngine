#include <Editor_pch.h>

#include <editor\Widget\CheckBox.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	CheckBox::CheckBox(const string& id, rengine::Object* handler, rttr::property& prop, uint32 flags)
		: Widget(id, flags)
		, m_pHandler(handler)
		, m_prop(prop)
	{

	}

	CheckBox::~CheckBox()
	{

	}

	void CheckBox::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<bool>())
			assert(false);

		bool _bool = _val.convert<bool>();

		if (ImGui::Checkbox(m_id.c_str(), &_bool))
		{
			m_prop.set_value(m_pHandler, _bool);
		}
	}
}