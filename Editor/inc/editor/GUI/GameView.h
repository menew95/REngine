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

namespace graphics
{
    class CameraBuffer;
}

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

    private:
        /**
            @brief 게임 뷰 카메라 움직임 제어
        **/
        void GameViewCameraControl();

        float m_fSpeed = 1.0f;

        graphics::CameraBuffer* m_pGameViewCameraBuffer = nullptr;
    };
}