/**

    @file      Document.h
    @brief     document class
    @details   하위에 도킹된 view들을 소유한다.
    @author    LWT
    @date      3.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <editor\Editor_dllexport.h>

namespace editor
{
    class Document
    {
    public:
        Document(std::string name);

        virtual EDITOR_API ~Document();

        virtual EDITOR_API void Begin();

        virtual EDITOR_API void Render();

        virtual EDITOR_API void End();

    protected:
        vector<class View*> m_childs;

        std::string m_documentName;

        uint32 m_flags = 0;
    };
}