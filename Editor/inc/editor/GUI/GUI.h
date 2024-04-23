/**

    @file      GUI.h
    @brief     gui base class 
    @details   모든 imgui을 사용하는 class의 base class
    @author    LWT
    @date      23.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

namespace editor
{
    class GUI
    {
    public:
        [[nodiscard]]
        const uint64& GetID() const
        {
            return m_id;
        }

        [[nodiscard]]
        const string& GetIDString() const
        {
            return m_idString;
        }

    protected:
        GUI();

        virtual ~GUI() = default;

        uint64 m_id;

        string m_idString;
    };
}
