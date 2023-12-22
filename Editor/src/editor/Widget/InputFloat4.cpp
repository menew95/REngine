#include <Editor_pch.h>
#include <editor\Widget\InputFloat4.h>

namespace editor
{
	InputFloat4::InputFloat4(string name, float* handler, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
	{

	}

	InputFloat4::~InputFloat4()
	{

	}

	void InputFloat4::Render()
	{
		if (ImGui::InputFloat4(GetWidgetName().c_str(), m_pHandler))
		{

		}
	}
}