/**

    @file      Separator.h
    @brief     imgui separator wrapper
    @details   Separator layout를 위젯처럼 사용하기 위한 클래스
    @author    LWT
    @date      23.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

namespace editor
{
    class Separator : public Widget
    {
    public:
        void Render() override;
    };
}