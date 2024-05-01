#include <Editor_pch.h>

#include <editor\Widget\WidgetContainer.h>

#include <editor\Widget\WidgetManager.h>

namespace editor
{
	WidgetContainer::WidgetContainer()
	{

	}

	WidgetContainer::~WidgetContainer()
	{
		RemoveAllWidget();
	}

	void WidgetContainer::RenderChild()
	{
		for (auto& _childs : m_childs)
		{
			if(_childs->GetEnable())
				_childs->Render();
		}
	}

	Widget* WidgetContainer::GetChild(const string& lable)
	{
		auto _find = find_if(m_childs.begin(), m_childs.end(), [&lable](auto& childs)
			{
				return childs->GetLable() == lable;
			});

		if (_find != m_childs.end())
		{
			return (*_find).get();
		}

		return nullptr;
	}

	void WidgetContainer::RemoveWidget(Widget* widget)
	{
		for (auto _iter = m_childs.begin(); _iter != m_childs.end(); _iter++)
		{
			if ((*_iter)->GetLable() == widget->GetLable())
			{
				m_childs.erase(_iter);

				break;
			}
		}
	}

	void WidgetContainer::RemoveAllWidget()
	{
		/*for (auto* _child : m_childs)
		{
			if(_child == nullptr)
				continue;

			delete _child;

			_child = nullptr;
		}*/

		m_childs.clear();
	}
}