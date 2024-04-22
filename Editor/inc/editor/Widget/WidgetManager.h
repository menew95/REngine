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
#include <editor\Widget\DragFloat.h>
#include <editor\Widget\DragFloat2.h>
#include <editor\Widget\DragFloat3.h>
#include <editor\Widget\DragFloat4.h>
#include <editor\Widget\DragScalar.h>
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

		template<size_t Size>
		Columns<Size>* GetColumnWidget(const string& name, uint32 flags = 0)
		{
			auto _find = m_treeNodeWidgets.find(name);

			if (_find != m_treeNodeWidgets.end())
				return reinterpret_cast<Columns<Size>*>(_find->second);

			if (Columns<Size>* _ret = CreateWidget<Columns<Size>>(name, flags))
			{
				return _ret;
			}

			return nullptr;
		}

		TreeNode* GetTreeNodeWidget(const string& name, uint32 flags = 0);

		CollapsingHeader* GetCollapsWidget(const string& name, uint32 flags = 0);


		template<typename T, typename ... Args>
		T* CreateWidget(const string& name, Args... args)
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
			else if constexpr (std::is_same_v<T, Columns<1>> || std::is_same_v<T, Columns<2>>
				|| std::is_same_v<T, Columns<3>>
				|| std::is_same_v<T, Columns<4>>
				|| std::is_same_v<T, Columns<5>>)
			{
				m_columnWidgets.insert(std::make_pair(name, _newWidget));
			}
			else
			{
				m_widgets.insert(std::make_pair(name, _newWidget));
			}

			return _newWidget;
		}


	private:
		void RegistWidget(Widget* widget);
		
		std::map<string, Widget*> m_widgets;
		std::map<string, Widget*> m_columnWidgets;
		std::map<string, Widget*> m_collapsWidgets; //하나의 컴포넌트에 대응이 될지도
		std::map<string, Widget*> m_treeNodeWidgets;
    };
}