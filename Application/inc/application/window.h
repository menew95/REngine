﻿#pragma once

#include <common/common.h>
#include <common/singleton.h>

#include <application/application.h>

#include <windows.h>

namespace app
{
	class Window
	{
		DECLARE_SINGLETON_CLASS(Window)
	public:

		HRESULT Initialize(bool showCmd,
			HINSTANCE hInstance,
			const tstring& windowClassName,
			const tstring& windowName,
			UINT width,
			UINT height);

		WindowInfo& GetWindowInfo() { return m_WindowInfo; }

	private:
		ATOM RegisterWindow(HINSTANCE hInstance);

		HRESULT InitializeWindow(bool showCmd);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		std::wstring m_WindowClassNameW;
		tstring m_WindowClassName;
		tstring m_WindowName;

		bool m_IsCmdOn = false;

		WindowInfo m_WindowInfo;
	};
}