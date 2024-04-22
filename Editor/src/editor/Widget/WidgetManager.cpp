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

	TreeNode* WidgetManager::GetTreeNodeWidget(const string& name, uint32 flags)
	{
		auto _find = m_treeNodeWidgets.find(name);

		if (_find != m_treeNodeWidgets.end())
			return reinterpret_cast<TreeNode*>(_find->second);

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
			return reinterpret_cast<CollapsingHeader*>(_find->second);

		if (CollapsingHeader* _ret = CreateWidget<CollapsingHeader>(name, flags))
		{
			return _ret;
		}

		return nullptr;
	}

	void WidgetManager::RegistWidget(Widget* widget)
	{
		if (dynamic_cast<TreeNode*>(widget) != nullptr)
		{
			m_treeNodeWidgets.insert(std::make_pair(widget->GetID(), widget));
		}
		else if (dynamic_cast<CollapsingHeader*>(widget) != nullptr)
		{
			m_collapsWidgets.insert(std::make_pair(widget->GetID(), widget));
		}
		else if (dynamic_cast<Columns<1>*>(widget) != nullptr
			|| dynamic_cast<Columns<2>*>(widget) != nullptr
			|| dynamic_cast<Columns<3>*>(widget) != nullptr
			|| dynamic_cast<Columns<4>*>(widget) != nullptr
			|| dynamic_cast<Columns<5>*>(widget) != nullptr)
		{
			m_columnWidgets.insert(std::make_pair(widget->GetID(), widget));
		}
		else
		{
			m_widgets.insert(std::make_pair(widget->GetID(), widget));
		}
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