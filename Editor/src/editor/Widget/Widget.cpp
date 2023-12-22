#include <Editor_pch.h>

#include <editor\Widget\Widget.h>

namespace editor
{
	Widget::Widget(string name, uint32 flags)
		: m_widgetName(name)
		, m_flags(flags)
	{
	}

	Widget::~Widget()
	{

	}
}