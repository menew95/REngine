#include <Editor_pch.h>

#include <editor\Widget\CollapsingHeader.h>

namespace editor
{
	CollapsingHeader::CollapsingHeader(string name, uint32 flags)
	: WidgetContainer(name, flags)
	{
	
	}
	
	CollapsingHeader::~CollapsingHeader()
	{
	
	}

	void CollapsingHeader::Render()
	{
		if (ImGui::CollapsingHeader(GetWidgetName().c_str()))
		{
			for (auto& _widget : GetChilds())
			{
				_widget->Render();
			}
		}
	}
}