#include <Editor_pch.h>

#include <editor/EditorApi.h>

static std::unique_ptr<editor::Editor> g_pEditor = nullptr;

EDITOR_API editor::Editor* CreateEditor()
{
	g_pEditor = std::make_unique<editor::Editor>();

	return g_pEditor.get();
}

void ReleaseEditor(editor::Editor* editor)
{

}

bool UpdateEditor()
{
	return false;
}

editor::Editor* GetEditor()
{
	return g_pEditor.get();
}
