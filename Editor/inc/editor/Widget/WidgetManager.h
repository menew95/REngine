/**

    @file      WidgetManager.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      20.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <editor\Widget\Button.h>
#include <editor\Widget\TreeNode.h>
#include <editor\Widget\InputText.h>
#include <editor\Widget\InputFloat.h>
#include <editor\Widget\InputFloat2.h>
#include <editor\Widget\InputFloat3.h>
#include <editor\Widget\InputFloat4.h>
#include <editor\Widget\CollapsingHeader.h>

namespace editor
{
    class WidgetManager
    {
        DECLARE_SINGLETON_CLASS(WidgetManager)
    
    public:
		void ClearColumnWidget();
		void ClearCollapsWidget();
		void ClearTreeNodeWidget();

		Widget* GetColumnWidget(string name);

		CollapsingHeader* GetCollapsWidget(string name);

		template<typename T, typename ... Args>
		Widget* CreateWidget(string name, Args&&... args)
		{
			Widget* _newWidget = new T(args...);

			return _newWidget;
		}

		template<typename ... Args>
		Widget* CreateWidget(string name, Args&&... args)
		{
			TreeNode* _newWidget = new TreeNode(args...);
			m_treeNodeWidgets.insert(std::make_pair(name, _newWidget));

			return _newWidget;
		}

		template<typename ... Args>
		Widget* CreateWidget(string name, Args&&... args)
		{
			CollapsingHeader* _newWidget = new CollapsingHeader(args...);
			m_collapsWidgets.insert(std::make_pair(name, _newWidget));

			return _newWidget;
		}

	private:
		std::map<string, Widget*> m_columnWidgets;
		std::map<string, Widget*> m_collapsWidgets; //하나의 컴포넌트에 대응이 될지도
		std::map<string, Widget*> m_treeNodeWidgets;
    };
}