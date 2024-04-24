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

    private:
        void CreateToolBarWidget();

        ImageButton* m_playStopButton;
        ImageButton* m_pauseButton;
        ImageButton* m_stepButton;

        rengine::Texture* m_playTexture;
        rengine::Texture* m_stopTexture;
        rengine::Texture* m_pauseTexture;
        rengine::Texture* m_stepTexture;
    };
}