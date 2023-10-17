/**

    @file      EditorSystem.h
    @brief     editor dll를 관리하는 클래스
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common\common.h>
#include <common\singleton.h>

namespace editor
{
    class Editor;
}

namespace rengine
{
    class EditorSystem
    {
    DECLARE_SINGLETON_CLASS(EditorSystem)

    public:
        
        void Initialize(void* hwnd, void* device, void* context);

        void Render();

        bool WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        
        /**
            @brief editor dll load
        **/
        void LoadModule();

        class editor::Editor* m_pEditor = nullptr;
    };
}