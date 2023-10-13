/**

    @file      EditorFlags.h
    @brief     editor 관련 설정
    @author    LWT
    @date      12.10.2023

**/
#pragma once

namespace editor
{
    struct EditorDesc
    {
        void* _hwnd;

        void* _device;
        void* _deviceContext;
        void* _renderTarget;
    };
}