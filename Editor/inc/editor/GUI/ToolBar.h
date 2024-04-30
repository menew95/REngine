/**

    @file      ToolBar.h
    @brief     tool bar
    @details   ~
    @author    LWT
    @date      24.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\GUI\View.h>

namespace rengine
{
    class Texture;
}

namespace editor
{
    class ImageButton;

    class ToolBar : public View
    {
    public:
        ToolBar();
        ~ToolBar();

        void Begin() override;

    private:
        void CreateToolBarWidget();

        ImageButton* m_playStopButton = nullptr;
        ImageButton* m_pauseButton = nullptr;
        ImageButton* m_stepButton = nullptr;

        rengine::Texture* m_playTexture = nullptr;
        rengine::Texture* m_stopTexture = nullptr;
        rengine::Texture* m_pauseTexture = nullptr;
        rengine::Texture* m_stepTexture = nullptr;
    };
}