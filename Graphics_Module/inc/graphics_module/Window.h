﻿#pragma once

#include "common\common.h"

#include "graphics_module_dllexport.h"

namespace graphics
{
	struct WindowGraphicsInfo
	{
		uint32 _refrashRate = 60;
		HWND _hwnd = 0;
	};

	class GRAPHICS_DLL_DECLSPEC Window
	{
	public:
		Window();
		~Window();


		void OnResize(uint32 width, uint32 height);

		inline uint32 GetRefrashRate() { return s_WindowInfo._refrashRate; }
		inline HWND GetHwnd() { return s_WindowInfo._hwnd; }

		inline WindowGraphicsInfo& GetWindowGraphicsInfo() { return s_WindowInfo; }

		static std::shared_ptr<Window> Create();

	private:
		WindowGraphicsInfo s_WindowInfo;
	};
}