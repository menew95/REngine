/**

    @file      AppBase.h
    @brief     App interface
    @details   ~
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common\common.h>

class AppBase
{
public:
    virtual void Initialize(void* desc) abstract;

    virtual bool Quit() abstract;

    virtual bool Update() abstract;

    virtual bool WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) abstract;
};