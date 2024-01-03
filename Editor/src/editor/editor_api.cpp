#include <Editor_pch.h>

#include <editor\editor_api.h>

static std::unique_ptr<editor::Editor> g_pEditor = nullptr;

EDITOR_API editor::Editor* CreateEditor()
{
	g_pEditor = std::make_unique<editor::Editor>();

	return g_pEditor.get();
}

EDITOR_API bool QuitEditor()
{
	return g_pEditor->Quit();
}

EDITOR_API bool UpdateEditor()
{
	return g_pEditor->Update();
}

editor::Editor* GetEditor()
{
	return g_pEditor.get();
}
