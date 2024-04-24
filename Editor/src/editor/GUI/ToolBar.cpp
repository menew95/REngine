#include <Editor_pch.h>
#include <editor\GUI\ToolBar.h>

#include <editor\Widget\ImageButton.h>
#include <editor\Widget\SameLine.h>

#include <rengine\core\resources.h>
#include <rengine\core\resource\Texture.h>

#include <editor\Core\Event.h>

namespace editor
{
	ToolBar::ToolBar()
		: View("Tool Bar")
	{

	}

	ToolBar::~ToolBar()
	{

	}

	void ToolBar::CreateToolBarWidget()
	{
		m_playTexture = rengine::Resources::GetInstance()->GetResource<rengine::Texture>(TEXT("")).get();
		m_stopTexture = rengine::Resources::GetInstance()->GetResource<rengine::Texture>(TEXT("")).get();
		m_pauseTexture = rengine::Resources::GetInstance()->GetResource<rengine::Texture>(TEXT("")).get();
		m_stepTexture = rengine::Resources::GetInstance()->GetResource<rengine::Texture>(TEXT("")).get();

		m_playStopButton = AddWidget<ImageButton>(
			m_playTexture->GetTextureID(),
			"Play\Stop Button",
			Vector2{ 20, 20 }
		);

		m_playStopButton->SetClickEvent(
			[]()
			{
				EventManager::GetInstance()->StartGame();
			}
		);

		
		Event<void> _event;

		_event += std::bind(&EventManager::StartGame, EventManager::GetInstance());

		AddWidget<SameLine>();

		m_pauseButton = AddWidget<ImageButton>(
			m_pauseTexture->GetTextureID(),
			"Pause Button",
			Vector2{ 20, 20 }
		);

		m_pauseButton->SetClickEvent(
			std::bind(&EventManager::PauseGame, EventManager::GetInstance())
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