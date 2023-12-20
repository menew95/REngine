/**

    @file      View.h
    @brief     view base
    @author    LWT
    @date      16.10.2023

**/
#pragma once

#include <common\common.h>

#include <editor\Editor_dllexport.h>

namespace editor
{
	class View
	{
	public:
		View(std::string name);

		virtual EDITOR_API ~View();

		virtual EDITOR_API void Begin();

		virtual EDITOR_API void Render();

		virtual EDITOR_API void End();

		inline bool GetOpen() { return m_bOpen; }
		inline void SetOpen(bool val) { m_bOpen = val; }

		virtual void LoadSetting();

	protected:
	bool _test = false;
		std::string m_ViewName;

		bool m_bOpen = false;

		uint32 m_flags = 0;
	};
}