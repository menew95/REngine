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

namespace editor
{
    class Widget; 

    class WidgetManager
    {
        DECLARE_SINGLETON_CLASS(WidgetManager)
    
    public:
		void ClearColumnWidget();
		void ClearCollapsWidget();
		void ClearTreeNodeWidget();

		template<typename T, typename ... Args>
		T& CreateWidget(Args&&... args)
		{
			
			return nullptr;
		}

		/*template<typename T, typename ... Args>
		T* CreateColumnsWidget(Args&&... args)
		{
			_columnWidgets.emplace_back(new T(args...));
			T* ins = reinterpret_cast<T*>(_columnWidgets.back());
			return ins;
		}

		template<typename T, typename ... Args>
		T* CreateCollapsWidget(Args&&... args)
		{
			_collapsWidgets.emplace_back(new T(args...));
			T& ins = reinterpret_cast<T*>(_collapsWidgets.back());
			return ins;
		}*/

		class TreeNode;

		template<typename ... Args>
		TreeNode* CreateWidget(Args&&... args)
		{
			m_treeNodeWidgets.emplace_back(new TreeNode(args...));
			TreeNode* ins = reinterpret_cast<TreeNode*>(m_treeNodeWidgets.back());
			return ins;
		}

	private:
		std::vector<Widget*> m_columnWidgets;
		std::vector<Widget*> m_collapsWidgets;
		std::vector<Widget*> m_treeNodeWidgets;
    };
}