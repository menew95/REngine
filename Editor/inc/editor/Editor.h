/**

    @file      Editor.h
    @brief     Editor a
    @details   ~
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common/common.h>

#include <filesystem>

#include <editor/Editor_dllexport.h>
#include <editor/EditorFlags.h>

namespace editor
{
    class View;
    
    class EDITOR_API Editor
    {
    public:
        /**
            @brief editor 초기화
            @param desc - editor 설정
        **/
        void Initialize(const EditorDesc& desc);

        void Release();

        void Begin();

        void Render();

        void End();

        bool WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    private:
        
        void InitImGui();

        void InitResource();

        void LoadInFile(const char* file);


        //fs::path GetEditorDir();
        void GetEditorDir(void* path);

        void LoadLayout(std::string aLayout, void* someContext);

        HWND m_hwnd;
        int m_Height;
        int m_Width;

        void* m_pDevice;
        void* m_pContext;

        View* m_pEditorView;
    };
}