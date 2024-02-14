/**

    @file      rengine.h
    @brief     rengine app
    @details   ~
    @author    LWT
    @date      01.01.2024

**/

#pragma once

#include <common\AppBase.h>

#include <rengine\rengine_flags.h>

#include <rengine\rengine_dllexport.h>

namespace rengine
{
    class REngine : public AppBase
    {
    public:
        RENGINE_API void Initialize(void* desc) override;

        RENGINE_API bool Quit() override;

        RENGINE_API bool Update() override;

        RENGINE_API bool WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

    private:

        REngineDesc m_desc;

        double m_dTickTime = 0;
        double m_dTickCnt = 0;   
    };
}