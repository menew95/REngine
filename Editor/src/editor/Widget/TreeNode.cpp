#include <Editor_pch.h>

#include <editor\Widget\TreeNode.h>

namespace editor
{
	TreeNode::TreeNode(string name, vector<Widget*> childs, uint32 flags)
		: Widget(name, childs, flags)
	{
		
	}

	TreeNode::~TreeNode()
	{

	}

	void TreeNode::Render()
	{
		if (ImGui::TreeNodeEx(GetWidgetName().c_str()), GetFlags())
		{
			for (auto* _child : GetChilds())
			{
				_child->Render();
			}

			ImGui::TreePop();
		}
	}
}