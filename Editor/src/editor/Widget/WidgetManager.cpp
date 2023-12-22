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

	CollapsingHeader* WidgetManager::GetCollapsWidget(string name)
	{
		auto _find = m_collapsWidgets.find(name);

		if (_find != m_collapsWidgets.end())
			return reinterpret_cast<CollapsingHeader*>(_find->second);

		if (Widget* _ret = CreateWidget<CollapsingHeader>(name))
		{
			return (CollapsingHeader*)_ret;
		}

		return nullptr;
	}
}