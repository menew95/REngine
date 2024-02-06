#include <Editor_pch.h>

#include <editor\GUI\GameView.h>

#include <rengine\core\component\Camera.h>
#include <rengine\core\Resources.h>
#include <rengine\core\resource\Texture.h>

namespace editor
{
	GameView::GameView()
		: View("Game View")
	{

	}

	GameView::~GameView()
	{

	}

	void EDITOR_API GameView::Begin()
	{
		__super::Begin();

	}
	void EDITOR_API GameView::Render()
	{
		__super::Render();

		auto _mainCam = rengine::Camera::GetMainTextureID();

		ImGui::Image(_mainCam, ImGui::GetCurrentWindow()->Size);
	}
	void EDITOR_API GameView::End()
	{
		__super::End();

	}
}