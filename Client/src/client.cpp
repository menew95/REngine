#include <common/common.h>
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <shellapi.h>

#include <rengine_api.h>

void RedirectIOToConsole()
{
	// maximum mumber of lines the output console should have
	static const WORD MAX_CONSOLE_LINES = 500;

	int hConHandle;
	long long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	FILE* fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	coninfo.dwSize.Y = MAX_CONSOLE_LINES;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console

	lStdHandle = (long long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = (long long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "r");

	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = (long long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
	// point to console as well
	std::ios::sync_with_stdio();
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

#ifdef _DEBUG
#ifdef x64
	auto _engineModule = LoadLibrary(_T("REngine_x64_Debug.dll"));
#else
	auto _engineModule = LoadLibrary(_T("REngine_x86_Debug.dll"));
#endif // x64
#else
#ifdef x64
	auto _engineModule = LoadLibrary(_T("REngine_x64_Release.dll"));
#else
	auto _engineModule = LoadLibrary(_T("REngine_x86_Release.dll"));
#endif // x64
#endif

	assert(_engineModule != NULL);

	auto _applicationConstructor = (rengine::ApplicationConstructor)GetProcAddress(_engineModule, "CreateApplication");

	assert(_applicationConstructor != NULL);

	auto _applicationDestructor = (rengine::ApplicationDestructor)GetProcAddress(_engineModule, "DeleteApplication");

	assert(_applicationDestructor != NULL);

	auto* _application = _applicationConstructor();


	{
		if (_application->Initallize(true, hInstance, TEXT("ClientWindow"), TEXT("Client"), 1280, 720) == false)
		{
			return -1;
		}

		while (true)
		{
			MSG _msg;

			if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&_msg);
				DispatchMessage(&_msg);

				if (_msg.message == WM_QUIT)
					break;
			}
			else
			{
				if (_application->Update() == true)
					break;
			}
		}

		_application->Quit();
	}

	_applicationDestructor(_application);

	FreeLibrary(_engineModule);


	return 0;
}
