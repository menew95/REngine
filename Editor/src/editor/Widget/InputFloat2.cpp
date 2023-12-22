#include <Editor_pch.h>
#include <editor\Widget\InputFloat2.h>

namespace editor
{
	InputFloat2::InputFloat2(string name, float* handler, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
	{

	}

	InputFloat2::~InputFloat2()
	{

	}

	void InputFloat2::Render()
	{
		if (ImGui::InputFloat2(GetWidgetName().c_str(), m_pHandler))
		{

		}
	}
}