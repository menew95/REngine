#include <Editor_pch.h>

#include <editor\Widget\Widget.h>

namespace editor
{
	Widget::Widget(const string& id, uint32 flags)
		: m_id(id)
		, m_flags(flags)
	{
	}

	Widget::~Widget()
	{

	}
}