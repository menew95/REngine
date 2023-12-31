/**

    @file      Module.h
    @brief     module를 로드하고 관리하는 클래스
    @details   ~
    @author    LWT
    @date      17.10.2023

**/
#pragma once

#include <common\common.h>

//#define ProcedureName(namespaceName, className, funcName) "?"#namespaceName"@"#className"@@"#funcName\
//"@@Z";

class Module
{
public:
    Module(const char* moduleName);
    ~Module();

    static std::string GetModuleFilename(const char* moduleName);

    static bool IsAvailable(const char* moduleFilename);

    static std::unique_ptr<Module> Load(const char* moduleFilename);

public:
    inline bool IsValid() { return m_pHandle != nullptr; }

    void* LoadProcedure(const char* procedureName);

    HMODULE m_pHandle = NULL;
};