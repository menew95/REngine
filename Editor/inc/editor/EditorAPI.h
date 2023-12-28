#pragma once

#include <editor\Editor.h>

extern "C"
{
	EDITOR_API editor::Editor* CreateEditor();
	EDITOR_API void ReleaseEditor(editor::Editor*);

	EDITOR_API bool UpdateEditor();

	using EditorConstructor = editor::Editor * (*)();
	using EditorDestructor = void (*)(editor::Editor*);
}

editor::Editor* GetEditor();