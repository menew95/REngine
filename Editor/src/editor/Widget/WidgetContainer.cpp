#include <Editor_pch.h>

#include <editor\Widget\WidgetContainer.h>

namespace editor
{
	WidgetContainer::WidgetContainer(string name, uint32 flags)
	: Widget(name, flags)
	{

	}

	WidgetContainer::~WidgetContainer()
	{
	
	}

	void WidgetContainer::Render()
	{
		for (auto* _childs : m_childs)
		{
			_childs->Render();
		}
	}
	
	void WidgetContainer::AddWidget(Widget* widget)
	{
		assert(widget != nullptr);

		m_childs.push_back(widget);

	}

	Widget* WidgetContainer::GetChild(string name)
	{
		auto _find = find_if(m_childs.begin(), m_childs.end(), [&name](auto& childs)
			{
				return childs->GetWidgetName() == name;
			});

		if (_find != m_childs.end())
		{
			return *_find;
		}

		return nullptr;
	}
}