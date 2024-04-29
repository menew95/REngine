#include <Editor_pch.h>

#include <editor\Widget\Widget.h>

namespace editor
{
	Widget::Widget(const string& lable, uint32 flags)
		: m_lable(lable)
		, m_flags(flags)
	{
	}

	Widget::~Widget()
	{

	}

	void Widget::Draw()
	{
		if (m_isEnable)
		{
			Render();
		}

	}
}