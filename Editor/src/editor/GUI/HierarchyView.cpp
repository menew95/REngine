#include <Editor_pch.h>

#include <editor\GUI\HierarchyView.h>


std::shared_ptr<GameObject> g_pRoot = std::make_shared<GameObject>("Root");

namespace editor
{
	HierarchyView::HierarchyView()
	{
		m_ViewName = "HierarchyView";

		int idx = 0;

		for (int i = 0; i < 4; i++)
		{
			std::string _name = "GameObject" + idx;
			idx++;

			std::shared_ptr<GameObject> _obj = std::make_shared<GameObject>(_name.c_str());

			for (int j = 4 - i; j > 0; j--)
			{
				std::string _name2 = "GameObject" + idx;
				idx++;

				std::shared_ptr<GameObject> _obj2 = std::make_shared<GameObject>(_name2.c_str());

				_obj->AddChilds(_obj2);
			}

			g_pRoot->AddChilds(_obj);
		}
	}

	HierarchyView::~HierarchyView()
	{

	}

	void HierarchyView::Begin()
	{
		__super::Begin();

	}

	void HierarchyView::Render()
	{
		__super::Render();

		DrawTree(g_pRoot);
	}

	void HierarchyView::End()
	{
		__super::End();

	}

	void HierarchyView::SetScene()
	{

	}

	void HierarchyView::DrawTree(std::shared_ptr<GameObject> gameObj)
	{

		// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.

		bool node_open = ImGui::TreeNode(gameObj->GetName().c_str());

		if (node_open)
		{
			for (auto& child : gameObj->GetChilds())
			{
				DrawTree(child);
			}

			ImGui::TreePop();
		}
	}
}