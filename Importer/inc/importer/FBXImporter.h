﻿/**

    @file      FBXImporter.h
    @brief     fbx importer class
    @details   fbx 파일을 mesh, material, anim등의 리소스로 분리후 임포트
    @author    LWT
    @date      24.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

namespace utility
{
    class FBXImporter
    {
    public:
        /**
            @brief 해당 경로의 fbx 파일로 mesh, anim, material를 생성
            @param path - fbx 파일 경로
        **/
		static void Import(const tstring& path);
    };
}