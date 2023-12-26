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
			ClearColumnWidget();
			ClearCollapsWidget();
			ClearTreeNodeWidget();
		});

	void WidgetManager::ClearColumnWidget()
	{
		for (auto& _pair : m_columnWidgets)
		{
			if(_pair.second != nullptr)
				delete _pair.second;

			_pair.second = nullptr;
		}

		m_columnWidgets.clear();
	}

	void WidgetManager::ClearCollapsWidget()
	{
		for (auto& _pair : m_collapsWidgets)
		{
			if (_pair.second != nullptr)
				delete _pair.second;

			_pair.second = nullptr;
		}

		m_columnWidgets.clear();
	}

	void WidgetManager::ClearTreeNodeWidget()
	{
		for (auto& _pair : m_treeNodeWidgets)
		{
			if (_pair.second != nullptr)
				delete _pair.second;

			_pair.second = nullptr;
		}

		m_columnWidgets.clear();
	}

	Widget* WidgetManager::GetColumnWidget(string name)
	{
		return nullptr;
	}

	TreeNode* WidgetManager::GetTreeNodeWidget(string name)
	{
		auto _find = m_treeNodeWidgets.find(name);

		if (_find != m_treeNodeWidgets.end())
			return reinterpret_cast<TreeNode*>(_find->second);

		if (TreeNode* _ret = CreateWidget<TreeNode>(name))
		{
			return _ret;
		}

		return nullptr;
	}

	CollapsingHeader* WidgetManager::GetCollapsWidget(string name)
	{
		auto _find = m_collapsWidgets.find(name);

		if (_find != m_collapsWidgets.end())
			return reinterpret_cast<CollapsingHeader*>(_find->second);

		if (CollapsingHeader* _ret = CreateWidget<CollapsingHeader>(name))
		{
			return _ret;
		}

		return nullptr;
	}

	/*template<>
	TreeNode* WidgetManager::CreateWidget(string name, uint32 flags)
	{
		TreeNode* _newWidget = new TreeNode(name, flags);
		m_treeNodeWidgets.insert(std::make_pair(name, _newWidget));

		return _newWidget;
	}

	template<>
	CollapsingHeader* WidgetManager::CreateWidget(string name, uint32 flags)
	{
		CollapsingHeader* _newWidget = new CollapsingHeader(name, flags);
		m_collapsWidgets.insert(std::make_pair(name, _newWidget));

		return _newWidget;
	}*/
}