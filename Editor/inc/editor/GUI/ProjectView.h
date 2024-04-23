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

namespace rengine
{
    class Texture;
}

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
        void DrawFileTreeNode(tstring path);

        /**
            @brief 현재 디렉토리가 가르키고 있는 디렉토리를 그려줌
        **/
        void DrawDirectory();

        void DrawImageBtn();

        /**
            @brief 프로젝트 뷰의 팝업을 그려줌
        **/
        void DrawPopup();

        /**
            @brief popup에 사용될 menu widget 생성
        **/
        void CreatePopupWidget();

        /**
            @brief 파일을 순회하며 meta 파일이 없는 파일에 대해 meta 파일을 생성 및 파일 종류에 따라 임포트 등의 행위처리
            @param path - 경로
        **/
        void CheckMetaFile(const tstring& path);

        /**
            @brief 파일 아이콘으로부터 텍스처를 생성함
            @param path - 리소스로 사용할 파일 위치
        **/
        rengine::Texture* CreateTextureFromFileIcon(const tstring& path);

        bool m_bDrawMetaFile = false;

        tstring m_currPath;

        vector<tstring> m_selected;

        shared_ptr<rengine::Texture> m_folderIcon;
        shared_ptr<rengine::Texture> m_fileIcon;

        map<tstring, rengine::Texture*> g_iconTextureMap; // extension, texture

        class Popup* m_popupMenu = nullptr;
    };
}