/**

    @file      Editor_dllexport.h
    @brief     export
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#ifdef EDITOR_EXPORT
#ifndef EDITOR_API
#define EDITOR_API __declspec(dllexport)
#endif
#else
#ifndef EDITOR_API
#define EDITOR_API __declspec(dllimport)
#endif
#endif