/**

    @file      EditorSystem.h
    @brief     editor dll를 관리하는 클래스
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common/singleton.h>

namespace rengine
{
    class EditorSystem
    {
    DECLARE_SINGLETON_CLASS(EditorSystem)

    public:

        void Initialize(void* hwnd, void* device, void* context);

    private:
        
        /**
            @brief editor dll load
        **/
        void LoadModule();

    };
}