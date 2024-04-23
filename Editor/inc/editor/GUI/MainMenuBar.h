/**

    @file      MainMenuBar.h
    @brief     imgui main menu bar wrapper
    @details   imgui main menu bar
    @author    LWT
    @date      23.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\GUI\View.h>

namespace editor
{
    class MainMenuBar : public View
    {
    public:
        MainMenuBar();

        virtual EDITOR_API ~MainMenuBar();

        virtual EDITOR_API void Begin();

        virtual EDITOR_API void Render();

        virtual EDITOR_API void End();

    private:
        void CreateMainMenBar();
    };
}