#include <Editor_pch.h>

#include <editor\Widget\WidgetContainer.h>

#include <editor\Widget\WidgetManager.h>

namespace editor
{
	WidgetContainer::WidgetContainer(const string& id, uint32 flags)
	: Widget(id, flags)
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

	Widget* WidgetContainer::GetChild(const string& name)
	{
		auto _find = find_if(m_childs.begin(), m_childs.end(), [&name](auto& childs)
			{
				return childs->GetID() == name;
			});

		if (_find != m_childs.end())
		{
			return *_find;
		}

		return nullptr;
	}
}