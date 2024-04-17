#include <Editor_pch.h>
#include <editor\Widget\InputFloat3.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	InputFloat3::InputFloat3(const string& id, rengine::Object* handler, rttr::property& prop, uint32 flags)
		: Widget(id, flags)
		, m_pHandler(handler)
		, m_prop(prop)
	{

	}

	InputFloat3::~InputFloat3()
	{

	}

	void InputFloat3::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if(!_val.can_convert<math::Vector3>())
			assert(false);

		math::Vector3 _vec = _val.convert<math::Vector3>();

		float* _handler[3] = { &_vec.x, &_vec.y, &_vec.z };

		if (ImGui::InputFloat3(m_id.c_str(), *_handler, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _vec);
		}
	}
}