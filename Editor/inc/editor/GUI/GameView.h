/**

    @file      GameView.h
    @brief     GameView
    @details   ~
    @author    LWT
    @date      19.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\GUI\View.h>

namespace editor
{
    class GameView : public View
    {
    public:
        GameView();

        EDITOR_API ~GameView();

        void EDITOR_API Begin() override;

        void EDITOR_API Render() override;

        void EDITOR_API End() override;
    };
}