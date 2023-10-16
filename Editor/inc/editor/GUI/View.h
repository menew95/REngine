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
	class EDITOR_API View
	{
	public:
		View();

		virtual ~View();

		virtual void Begin();

		virtual void Render();

		virtual void End();

	protected:
		std::string m_ViewName;

	};
}