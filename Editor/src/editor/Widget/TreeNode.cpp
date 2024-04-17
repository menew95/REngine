#include <Editor_pch.h>

#include <editor\Widget\TreeNode.h>

namespace editor
{
	TreeNode::TreeNode(const string& id, uint32 flags)
		: WidgetContainer(id, flags)
	{
		
	}

	TreeNode::~TreeNode()
	{

	}

	void TreeNode::Render()
	{
		if (ImGui::TreeNodeEx(m_id.c_str(), GetFlags()))
		{
			for (auto* _child : GetChilds())
			{
				_child->Render();
			}

			ImGui::TreePop();
		}
	}
}