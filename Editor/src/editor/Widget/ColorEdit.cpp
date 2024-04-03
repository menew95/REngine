#include <Editor_pch.h>

#include <editor\Widget\ColorEdit.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	ColorEdit::ColorEdit(string name, rengine::Object* handler, rttr::property& prop, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, m_prop(prop)
	{

	}

	ColorEdit::~ColorEdit()
	{

	}

	void ColorEdit::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		float _cor[4];

		if (_val.can_convert<math::Color>())
		{
			math::Color _color = _val.convert<math::Color>();

			_cor[0] = _color.x;
			_cor[1] = _color.y;
			_cor[2] = _color.z;
			_cor[3] = _color.w;
		}
		else if (_val.can_convert<math::Vector4>())
		{
			math::Vector4 _color = _val.convert<math::Vector4>();

			_cor[0] = _color.x;
			_cor[1] = _color.y;
			_cor[2] = _color.z;
			_cor[3] = _color.w;
		}
		
		ImGui::PushID(m_widgetName.c_str());

		ImGui::Text(m_widgetName.c_str());
		//ImGui::SameLine();
		ImGui::NextColumn();

		if (ImGui::ColorEdit4("", _cor))
		{
			math::Color _color{ _cor[0], _cor[1], _cor[2], _cor[3] };

			m_prop.set_value(m_pHandler, _color);
		}

		ImGui::PopID();
	}
}