#include <Editor_pch.h>

#include <editor\GUI\SceneView.h>

#include <rengine\System\GraphicsSystem.h>

namespace editor
{
	SceneView::SceneView()
	: View("Scene View")
	{
		m_bOpen = true;
	}

	SceneView::~SceneView()
	{

	}

	void SceneView::Begin()
	{
		__super::Begin();

		// 메인 프레임 버퍼릐 사이즈를 재조절
	}

	void SceneView::Render()
	{
		__super::Render();

		// 
	}

	void SceneView::End()
	{
		__super::End();
	}
}