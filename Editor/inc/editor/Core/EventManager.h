/**

    @file      EventManager.h
    @brief     Editor event manager class
    @details   editor 안에서 일어나는 이벤트들을 모아둔 클래스
    @author    LWT
    @date      21.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    enum class EditorMode
    {
        // Stop Button
        Edit

        // Play Button
        , Play

        // Pause Button
        , Pause

        // Next Button
        , FRAME_BY_FRAME
    };

    class EventManager
    {
        DECLARE_SINGLETON_CLASS(EventManager);

    public:
        
        inline void SetFocusObject(rengine::Object* obj)
        {
            m_curFocusObject = obj;
        }

        inline auto* GetFocusObject() 
        {
            return m_curFocusObject;
        }

        void SaveScene();

        void OpenScene(const tstring& path);

        void SetEditorMode(EditorMode editorMode);

        void StartGame();

        void PauseGame();

        void StopGame();

        void NextFrame();

        void ExitEditor();

        class View* GetView(const string& viewName);

        const EditorMode& GetEditorMode() const { return m_editorMode; }
        void SetEditorMode(const auto& val) { m_editorMode = val; }

    private:
        EditorMode m_editorMode;

        rengine::Object* m_curFocusObject = nullptr;

        class Editor* m_editor = nullptr;

        friend class Editor;
    };
}