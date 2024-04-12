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
        void CreateGameViewCamera();

        /**
            @brief 게임 뷰 카메라 움직임 제어
        **/
        void GameViewCameraControl();

        /**
            @brief 그래픽스 엔진으로부터 클릭한 마우스 좌표에 해당하는 값을 읽어옴
            @param imageSize     - image view size
            @param mousePosition - click position in image view
        **/
        void ObjectPicking(const Vector2& imageSize, const Vector2& mousePosition);

        float m_fSpeed = 1.0f;

        graphics::CameraBuffer* m_pGameViewCameraBuffer = nullptr;
    };
}