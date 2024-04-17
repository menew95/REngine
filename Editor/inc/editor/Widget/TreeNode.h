/**

    @file      TreeNode.h
    @brief     imgui tree node widget
    @details   
    @author    LWT
    @date      20.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetContainer.h>

namespace editor
{
    class TreeNode : public WidgetContainer
    {
    public:
        TreeNode(const string& id, uint32 flags = 0);

        ~TreeNode();

        EDITOR_API void Render() override;
    };
}
