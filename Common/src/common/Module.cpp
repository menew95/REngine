#include <common\Module.h>
#include <strsafe.h>

static HMODULE LoadLibrarySafe(LPCSTR filename)
{
    /* Disable dialog error messages */
    UINT prevMode = SetErrorMode(0);
    SetErrorMode(prevMode | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);

    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");

    /* Load library */
    HMODULE module = LoadLibraryA(filename);

    /* Reset previous error mode */
    SetErrorMode(prevMode);

    return module;
}

Module::Module(const char* moduleName)
{
    m_pHandle = LoadLibrarySafe(moduleName);

    auto _error = GetLastError();

    if (_error == 126)
    {
        auto lpszFunction = TEXT("");

        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;
        DWORD dw = GetLastError();

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
        StringCchPrintf((LPTSTR)lpDisplayBuf,
            LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s failed with error %d: %s"),
            lpszFunction, dw, lpMsgBuf);
        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

        DebugBreak();
    }

    /* Check if loading has failed */
    assert(m_pHandle != NULL);
}

Module::~Module()
{
    if (IsValid())
    {
        FreeLibrary(m_pHandle);
    }
}

std::string Module::GetModuleFilename(const char* moduleName)
{
	std::string s(moduleName);

#ifdef x64
	s += "_x64";
#else
	s += "_x84";
#endif

#ifdef _DEBUG
	s += "_Debug";
#else
	s += "_Release";
#endif

    s += ".dll";
    return s;
}

bool Module::IsAvailable(const char* moduleFilename)
{
    /* Check if Win32 dynamic link library can be loaded properly */
    if (HMODULE handle = LoadLibrarySafe(moduleFilename))
    {
        FreeLibrary(handle);
        return true;
    }
    return false;
}

std::unique_ptr<Module> Module::Load(const char* moduleFilename)
{
    std::unique_ptr<Module> module = std::make_unique<Module>(moduleFilename);

    return (module->IsValid() ? std::move(module) : nullptr);
}

void* Module::LoadProcedure(const char* procedureName)
{
    FARPROC procAddr = GetProcAddress(m_pHandle, procedureName);

    assert(procAddr != NULL);

    return reinterpret_cast<void*>(procAddr);
}
