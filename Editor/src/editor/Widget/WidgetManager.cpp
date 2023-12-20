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
		for (auto* _widget : m_columnWidgets)
		{
			if(_widget != nullptr)
				delete _widget;

			_widget = nullptr;
		}

		m_columnWidgets.clear();
	}

	void WidgetManager::ClearCollapsWidget()
	{
		for (auto* _widget : m_collapsWidgets)
		{
			if (_widget != nullptr)
				delete _widget;

			_widget = nullptr;
		}

		m_columnWidgets.clear();
	}

	void WidgetManager::ClearTreeNodeWidget()
	{
		for (auto* _widget : m_treeNodeWidgets)
		{
			if (_widget != nullptr)
				delete _widget;

			_widget = nullptr;
		}

		m_columnWidgets.clear();
	}
}