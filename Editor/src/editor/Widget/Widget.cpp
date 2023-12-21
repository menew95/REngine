#include <Editor_pch.h>

#include <editor\Widget\Widget.h>

namespace editor
{
	Widget::Widget(string name, uint32 flags)
		: m_widgetName(name)
		, m_flags(flags)
	{
	}

	Widget::Widget(string name, vector<Widget*> childs, uint32 flags)
		: m_widgetName(name)
		, m_childWidgets(childs)
		, m_flags(flags)
	{
		
	}

	Widget::~Widget()
	{

	}
}