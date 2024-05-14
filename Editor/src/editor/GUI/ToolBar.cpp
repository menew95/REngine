#include <Editor_pch.h>
#include <editor\GUI\ToolBar.h>

#include <editor\Widget\ImageButton.h>
#include <editor\Widget\SameLine.h>

#include <rengine\core\resources.h>
#include <rengine\core\resource\Texture.h>

#include <common\AssetPath.h>

namespace editor
{
	ToolBar::ToolBar()
		: View("Tool Bar")
	{
		m_windowSettings._notitleBar = true;
		m_windowSettings._noCollapse = true;
		m_windowSettings._noResize = true;
		m_windowSettings._noScrollbar = true;

		CreateToolBarWidget();
	}

	ToolBar::~ToolBar()
	{

	}

	void ToolBar::Begin()
	{
		__super::Begin();

		ImGuiStyle& style = ImGui::GetStyle();

		float size = ImGui::CalcTextSize("Tool Bar").x + style.FramePadding.x * 2.0f;
		float avail = ImGui::GetContentRegionAvail().x;

		float off = (avail - size) * 0.5f;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

	void ToolBar::CreateToolBarWidget()
	{
		m_playTexture = rengine::Resources::GetInstance()->CreateResource<rengine::Texture>(
			TEXT("editor_toolbar_play_button"), g_editorLibraryPath + TEXT("bin\\editor\\play.png")).get();
		m_stopTexture = rengine::Resources::GetInstance()->CreateResource<rengine::Texture>(
			TEXT("editor_toolbar_stop_button"), g_editorLibraryPath + TEXT("bin\\editor\\stop.png")).get();
		m_pauseTexture = rengine::Resources::GetInstance()->CreateResource<rengine::Texture>(
			TEXT("editor_toolbar_pause_button"), g_editorLibraryPath + TEXT("bin\\editor\\pause.png")).get();
		m_stepTexture = rengine::Resources::GetInstance()->CreateResource<rengine::Texture>(
			TEXT("editor_toolbar_step_button"), g_editorLibraryPath + TEXT("bin\\editor\\step.png")).get();

		m_playStopButton = AddWidget<ImageButton>(
			m_playTexture->GetTextureID(),
			"Play\Stop Button",
			Vector2{ 20, 20 }
		);

		m_playStopButton->SetClickEvent(
			[this]()
			{
				if (EventManager::GetInstance()->GetEditorMode() == EditorMode::Edit)
				{
					m_playStopButton->SetTextureID(m_stopTexture->GetTextureID());

					EventManager::GetInstance()->StartGame();
				}
				else
				{
					m_playStopButton->SetTextureID(m_playTexture->GetTextureID());

					m_pauseButton->SetTextureID(m_pauseTexture->GetTextureID());

					EventManager::GetInstance()->StopGame();
				}
			}
		);

		AddWidget<SameLine>();

		m_pauseButton = AddWidget<ImageButton>(
			m_pauseTexture->GetTextureID(),
			"Pause Button",
			Vector2{ 20, 20 }
		);

		m_pauseButton->SetClickEvent(
			[this]()
			{
				if (EventManager::GetInstance()->GetEditorMode() == EditorMode::Play)
				{
					m_pauseButton->SetTextureID(m_playTexture->GetTextureID());
				}
				else if(EventManager::GetInstance()->GetEditorMode() == EditorMode::Pause)
				{
					m_pauseButton->SetTextureID(m_pauseTexture->GetTextureID());
				}

				EventManager::GetInstance()->PauseGame();
			}
		);

		AddWidget<SameLine>();

		m_stepButton = AddWidget<ImageButton>(
			m_stepTexture->GetTextureID(),
			"Step Button",
			Vector2{ 20, 20 }
		);

		m_stepButton->SetClickEvent(
			std::bind(&EventManager::NextFrame, EventManager::GetInstance())
		);
	}
}