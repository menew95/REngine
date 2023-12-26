#include <Editor_pch.h>
#include <editor\Widget\InputFloat3.h>

namespace editor
{
	InputFloat3::InputFloat3(string name, float* handler, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, _handler{handler[0], handler[1], handler[2]}
	{

	}

	InputFloat3::~InputFloat3()
	{

	}

	void InputFloat3::Render()
	{
		if (ImGui::InputFloat3(GetWidgetName().c_str(), _handler, "%.3f", GetFlags()))
		{

		}
	}
}