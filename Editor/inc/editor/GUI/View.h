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
		View();

		virtual EDITOR_API ~View();

		virtual EDITOR_API void Begin();

		virtual EDITOR_API void Render();

		virtual EDITOR_API void End();

	protected:
		std::string m_ViewName;

	};
}