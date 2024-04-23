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

#include <editor\Widget\Popup.h>
#include <editor\Widget\Menu.h>
#include <editor\Widget\MenuItem.h>
#include <editor\Widget\TextColored.h>
#include <editor\Widget\Button.h>
#include <editor\Widget\Columns.h>
#include <editor\Widget\CheckBox.h>
#include <editor\Widget\ComboBox.h>
#include <editor\Widget\TreeNode.h>
#include <editor\Widget\ColorEdit.h>
#include <editor\Widget\InputText.h>
#include <editor\Widget\DragScalar.h>
#include <editor\Widget\InputScalr.h>
#include <editor\Widget\ObjectButton.h>
#include <editor\Widget\CollapsingHeader.h>

#include <log/log.h>

#ifndef WIDGET_MANAGER
#define WIDGET_MANAGER

namespace editor
{
    class WidgetManager
    {
        DECLARE_SINGLETON_CLASS(WidgetManager)
    
    public:
		// 위젯 매니저가 소유한 위젯들은 보통 고유하다고 가정
		// 추후에 고유하지 않은 경우가 생길경우 로직을 바꿀 필요가 있음
		// ex) component의 경우 component 마다 하나의 CollapsingHeader에 대응 가능함
		TreeNode* GetTreeNodeWidget(const string& name, uint32 flags = 0);

		CollapsingHeader* GetCollapsWidget(const string& name, uint32 flags = 0);


		template<typename T, typename ... Args>
		T* CreateWidget(const string& name, Args... args)
			requires std::is_same_v<T, CollapsingHeader>
			|| std::is_same_v<T, TreeNode>
			|| std::is_same_v<T, Columns<1>>
			|| std::is_same_v<T, Columns<2>>
			|| std::is_same_v<T, Columns<3>>
			|| std::is_same_v<T, Columns<4>>
			|| std::is_same_v<T, Columns<5>>
			|| std::is_same_v<T, WidgetContainer>
		{
			auto _newWidget = make_shared<T>(name, args...);

			if constexpr (std::is_same_v<T, TreeNode>)
			{
				m_treeNodeWidgets.insert(std::make_pair(name, _newWidget));
			}
			else if constexpr (std::is_same_v<T, CollapsingHeader>)
			{
				m_collapsWidgets.insert(std::make_pair(name, _newWidget));
			}
			else if constexpr (std::is_same_v<T, Columns<1>> || std::is_same_v<T, Columns<2>>
				|| std::is_same_v<T, Columns<3>>
				|| std::is_same_v<T, Columns<4>>
				|| std::is_same_v<T, Columns<5>>)
			{
				m_columnWidgets.insert(std::make_pair(name, _newWidget));
			}
			else
			{
				// create widget의 경우 container의 하위로 생성하도록 로직을 변경했음
				//assert(false);
				m_widgets.insert(std::make_pair(name, _newWidget));
			}

			return _newWidget.get();
		}


	private:
		
		std::map<string, shared_ptr<Widget>> m_widgets;
		std::map<string, shared_ptr<Widget>> m_columnWidgets;
		std::map<string, shared_ptr<Widget>> m_collapsWidgets; //하나의 컴포넌트에 대응이 될지도
		std::map<string, shared_ptr<Widget>> m_treeNodeWidgets;
    };
}
#endif