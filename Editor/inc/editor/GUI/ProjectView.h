/**

    @file      ProjectView.h
    @brief     ProjectView class
    @details   프로젝트 디렉토리를 보여주는 창
    @author    LWT
    @date      19.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\GUI\View.h>

namespace editor
{
    class ProjectView : public View
    {
    public:
        ProjectView();

        EDITOR_API ~ProjectView();

        void EDITOR_API Begin() override;

        void EDITOR_API Render() override;

        void EDITOR_API End() override;

    private:
        /**
            @brief Project View Draw
        **/
        void Draw();

        /**
            @brief 폴더들을 TreeNode형식으로 그려줌
        **/
        void DrawFileTreeNode(string path);

        /**
            @brief 현재 디렉토리가 가르키고 있는 디렉토리를 그려줌
        **/
        void DrawDirectory();

        void DrawImageBtn();

        string m_currPath;
    };
}