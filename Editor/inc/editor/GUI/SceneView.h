/**

    @file      SceneView.h
    @brief     editor scene view class
    @details   에디터에서 scene를 편집하기 위한 렌더링를 하는 view
    @author    LWT
    @date      3.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <editor/GUI/View.h>

namespace editor
{
    class SceneView : public View
    {

    public:
        SceneView();

        ~SceneView();

        void Begin() override;

        void Render() override;

        void End() override;

    private:
        
    };
}