#include <Editor_pch.h>
#include <editor\Widget\Container.h>

namespace editor
{
	Container::Container()
		: Widget()
	{
	
	}
	
	Container::~Container()
	{
	
	}
	
	void Container::Render()
	{
		RenderChild();
	}
}