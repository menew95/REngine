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

#include <ImGui\ImGuizmo.h>

namespace graphics
{
    class CameraBuffer;
    class MaterialBuffer;
    class Texture;
}

namespace editor
{
    enum class ViewMode
    {
        Default = 0,
        Depth = 1,
        Albedo = 2,
        Normal = 3,
        World = 4,
        Emissive = 5,
        Flag = 6,
    };

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

        void CreateGameViewWidget();

        void CachingTexture();

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

        /**
            @brief 현재 view에 렌더링 될 이미지를 교체함(GBuffer Debug 용)
            @param val - 인덱스
        **/
        void SetCurrentView(int val);

        float m_fSpeed = 5.0f;

        graphics::CameraBuffer* m_pGameViewCameraBuffer = nullptr;

        ViewMode m_currentViewMode = ViewMode::Default;

        graphics::Texture* m_textureList[7];

        IMGUIZMO_NAMESPACE::OPERATION m_currentOperation = IMGUIZMO_NAMESPACE::OPERATION::TRANSLATE;

        IMGUIZMO_NAMESPACE::MODE m_currentMode = IMGUIZMO_NAMESPACE::MODE::WORLD;
    };
}