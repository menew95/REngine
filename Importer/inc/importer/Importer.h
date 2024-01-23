/**

    @file      Importer.h
    @brief     asset import class
    @details   에셋을 임포트 하면 meta file과 에셋 종류에 따라 임포팅
    @author    LWT
    @date      23.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <importer\ImporterAPI.h>

namespace utility
{
    class Importer
    {
    public:
        IMPORTER_API static void Import(const tstring& path);
    };
}