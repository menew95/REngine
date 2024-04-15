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
#include <editor\Widget\CheckBox.h>
#include <editor\Widget\ComboBox.h>
#include <editor\Widget\TreeNode.h>
#include <editor\Widget\ColorEdit.h>
#include <editor\Widget\InputText.h>
#include <editor\Widget\DragFloat.h>
#include <editor\Widget\DragFloat2.h>
#include <editor\Widget\DragFloat3.h>
#include <editor\Widget\DragFloat4.h>
#include <editor\Widget\InputFloat.h>
#include <editor\Widget\InputFloat2.h>
#include <editor\Widget\InputFloat3.h>
#include <editor\Widget\InputFloat4.h>
#include <editor\Widget\ObjectButton.h>
#include <editor\Widget\CollapsingHeader.h>

#include <log/log.h>

namespace editor
{
    class WidgetManager
    {
        DECLARE_SINGLETON_CLASS(WidgetManager)
    
    public:
		void ClearColumnWidget();
		void ClearCollapsWidget();
		void ClearTreeNodeWidget();

		Widget* GetColumnWidget(string name, uint32 flags = 0);

		TreeNode* GetTreeNodeWidget(string name, uint32 flags = 0);

		CollapsingHeader* GetCollapsWidget(string name, uint32 flags = 0);

		template<typename T, typename ... Args>
		T* CreateWidget(string name, Args... args)
		{
			T* _newWidget = new T(name, args...);

			if constexpr (std::is_same_v<T, TreeNode>)
			{
				m_treeNodeWidgets.insert(std::make_pair(name, _newWidget));
			}
			else if constexpr (std::is_same_v<T, CollapsingHeader>)
			{
				m_collapsWidgets.insert(std::make_pair(name, _newWidget));
			}
			else
			{
				m_widgets.insert(std::make_pair(name, _newWidget));
			}

			return _newWidget;
		}

	private:
		std::map<string, Widget*> m_widgets;
		std::map<string, Widget*> m_columnWidgets;
		std::map<string, Widget*> m_collapsWidgets; //하나의 컴포넌트에 대응이 될지도
		std::map<string, Widget*> m_treeNodeWidgets;
    };
}