#include <Editor_pch.h>

#include <editor\Widget\TreeNode.h>

namespace editor
{
	TreeNode::TreeNode(const string& lable, uint32 flags)
		: Widget(lable, flags)
	{
		
	}

	TreeNode::~TreeNode()
	{

	}

	void TreeNode::Render()
	{
		if (ImGui::TreeNodeEx((m_lable + m_idString).c_str(), GetFlags()))
		{
			RenderChild();

			ImGui::TreePop();
		}
	}
}