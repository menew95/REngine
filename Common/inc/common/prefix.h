﻿#pragma once
#define dll_export      __declspec(dllexport)
#define dll_import      __declspec(dllimport)

#if _MSC_VER <= 1600
#define thread_local    __declspec(thread)
#endif

#if !defined(dimof)
#define dimof(x) (sizeof(x)/sizeof(*x))
#endif

#pragma warning(disable:4481)   //  warning C4481: nonstandard extension used: override specifier 'override'

static const unsigned MaxPath = 260;    /* max. length of full pathname */
static const unsigned MaxDrive = 3;    /* max. length of drive component */
static const unsigned MaxDir = 256;    /* max. length of path component */
static const unsigned MaxFilename = 256;    /* max. length of file name component */
static const unsigned MaxExtension = 256;    /* max. length of extension component */


typedef void* RHandle;

//#if defined(DEBUG) | defined(_DEBUG)
//
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
//
//#if defined(DEBUG_ENABLE_VLD)
//
//#include <vld.h>
//
//#if defined(_WIN64)
//#pragma comment(lib, "C:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win64\\vld.lib")
//#elif
//#pragma comment(lib, "C:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win32\\vld.lib")
//#endif
//
//#endif
//
//#endif

#if !defined(foreach)
#define foreach(iteratorName, ContainerType, container)                                         \
        for (auto iteratorName=container.begin(); iteratorName!=container.end(); ++iteratorName)    \
        /**/
#endif

#if !defined(foreach_const)
#define foreach_const(iteratorName, ContainerType, container)                                   \
        for (auto iteratorName=container.cbegin(); iteratorName!=container.cend(); ++iteratorName)  \
        /**/
#endif

#ifdef _DEBUG
#define MessageBoxRet(ret, msg, type) auto ret = MessageBoxA(NULL, msg, "Assert", type);
#define DebugMessageBox(msg) { MessageBoxA(NULL, msg, "Assert", MB_OK); }
#define AssertMessageBox(val, msg) if(!(val)) { MessageBoxA(NULL, msg, "Assert", MB_OK); DebugBreak(); }
#define FatalError(msg){ MessageBoxA(NULL, msg, "Fatal error", MB_OK); DebugBreak(); assert(false); }
#define NullCheck(val) { if(val == nullptr) { MessageBoxA(NULL, TO_STRING(val is null), "NullCheck Message", MB_OK); DebugBreak(); } }
#define SAFE_RELEASE(x) if(x != nullptr) { x->release(); x = nullptr; }
#define SAFE_DELETE(x) if(x != nullptr) { delete x; x = nullptr; }

#define SAFE_INSERT_MAP(val, container, key) auto item = container.find(key);\
if (item == container.end()){ \
    AssertMessageBox(false, TO_STRING(key insert failed)); }\
else {\
    container.insert(std::make_pair(key, val));\
}

#define SAFE_DELETE_MAP(container, key) auto item = container.find(key);\
if (item != container.end())\
{\
    container.erase(item);\
}\
else\
{\
    MessageBoxA(NULL, TO_STRING(key is not exist in container), "Assert", MB_OK); DebugBreak();\
}
#else
#define DebugMessageBox(msg);
#define AssertMessageBox(val, msg) assert(val)
#define NullCheck(val) if(val == nullptr) { assert(false); }
#define SAFE_RELEASE(x) x->release();
#define SAFE_DELETE(x) { delete x; x = nullptr; }
#define SAFE_INSERT_MAP(val, container, key) auto item = container.find(key);\
if (item == container.end()){ \
    AssertMessageBox(false, TO_STRING(key insert failed)); }\
else {\
    container.insert(std::make_pair(key, val));\
}

#define SAFE_DELETE_MAP(container, key) auto item = container.find(key);\
if (item != container.end())\
{\
    container.erase(item);\
}
#endif // _DEBUG


template<bool B> struct constant_expression { static bool result() { return true; } };
template<> struct constant_expression<false> { static bool result() { return false; } };