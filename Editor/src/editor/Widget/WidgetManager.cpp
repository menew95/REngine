#include <Editor_pch.h>

#include <editor\Widget\WidgetManager.h>

namespace editor
{
	DEFINE_SINGLETON_CLASS(WidgetManager,
		{
		},
		{

		},
		{
			/*ClearColumnWidget();
			ClearCollapsWidget();
			ClearTreeNodeWidget();*/
		});

	TreeNode* WidgetManager::GetTreeNodeWidget(const string& name, uint32 flags)
	{
		auto _find = m_treeNodeWidgets.find(name);

		if (_find != m_treeNodeWidgets.end())
			return reinterpret_cast<TreeNode*>(_find->second.get());

		if (TreeNode* _ret = CreateWidget<TreeNode>(name, flags))
		{
			return _ret;
		}

		return nullptr;
	}

	CollapsingHeader* WidgetManager::GetCollapsWidget(const string& name, uint32 flags)
	{
		auto _find = m_collapsWidgets.find(name);

		if (_find != m_collapsWidgets.end())
			return reinterpret_cast<CollapsingHeader*>(_find->second.get());

		if (CollapsingHeader* _ret = CreateWidget<CollapsingHeader>(name, flags))
		{
			return _ret;
		}

		return nullptr;
	}
}