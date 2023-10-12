/**

    @file      Editor.h
    @brief     Editor a
    @details   ~
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common/common.h>

#include <editor/Editor_dllexport.h>
#include <editor/EditorFlags.h>

namespace editor
{
    class EDITOR_API Editor
    {
    public:
        /**
            @brief editor 초기화
            @param desc - editor 설정
        **/
        void Initialize(const EditorDesc& desc);

    private:
        
        void InitImGui();

        HWND m_hwnd;
        void* m_pDevice;
        void* m_pContext;
    };
}