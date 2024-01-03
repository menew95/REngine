#pragma once

#include <editor\Editor.h>

extern "C"
{
	EDITOR_API editor::Editor* CreateEditor();

	EDITOR_API bool QuitEditor();

	EDITOR_API bool UpdateEditor();

	using EditorConstructor = editor::Editor * (*)();

	using EditorDestructor = bool (*)();
}

editor::Editor* GetEditor();