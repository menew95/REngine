#include <Editor_pch.h>
#include <editor\GUI\GUI.h>

namespace editor
{
    static uint64 s_id = 0;

    GUI::GUI()
        : m_id(s_id++)
    {
        m_idString = "##" + to_string(m_id);
    }
}