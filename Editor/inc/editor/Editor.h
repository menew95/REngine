/**

    @file      Editor.h
    @brief     Editor a
    @details   ~
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common/common.h>

#include <common/AppBase.h>

#include <filesystem>

#include <editor/Editor_dllexport.h>
#include <editor/EditorFlags.h>

namespace editor
{
    class View;
    class Document;
    
    class Editor : public AppBase
    {
    public:
        /**
            @brief editor 초기화
            @param desc - editor 설정
        **/
        EDITOR_API void Initialize(void* option = nullptr) override;

        EDITOR_API bool Update() override;

        EDITOR_API bool Quit() override;

        EDITOR_API bool WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

        void* GetDevice() { return m_pDevice; };

    private:
        
        void Begin();

        void Render();

        void End();

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

        Document* m_pEditorDocment;

        EditorDesc m_desc;
    };
}