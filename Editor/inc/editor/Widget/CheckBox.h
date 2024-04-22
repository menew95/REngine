/**

    @file      CheckBox.h
    @brief     imgui Check Box wrapper
    @details   imgui Check Box 관리하는 widget component나 gamobject의 active 컨트롤 들에 사용됨
    @author    LWT
    @date      27.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetData.h>

namespace editor
{
    class CheckBox : public WidgetData<bool>
    {
    public:
        CheckBox(const string& id, uint32 flags = 0);

        ~CheckBox();

    private:
        void Draw() override;

    protected:
        
    };
}