#include <Editor_pch.h>

#include <editor\Widget\InputFloat.h>

namespace editor
{
	InputFloat::InputFloat(string name, float* handler, uint32 flags)
	: Widget(name, flags)
	, m_pHandler(handler)
	{

	}

	InputFloat::~InputFloat()
	{

	}

	void InputFloat::Render()
	{
		if (ImGui::InputFloat(GetWidgetName().c_str(), m_pHandler))
		{
			
		}
	}
}