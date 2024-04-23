#include <Editor_pch.h>

#include <editor\GUI\MainMenuBar.h>

#include <editor\Core\EventManager.h>

namespace editor
{
	MainMenuBar::MainMenuBar()
		: View("")
	{
		CreateMainMenBar();
	}

	MainMenuBar::~MainMenuBar()
	{
	}

	void MainMenuBar::Begin()
	{

	}

	void MainMenuBar::Render()
	{
		if (ImGui::BeginMainMenuBar())
		{
			RenderChild();

			ImGui::EndMainMenuBar();
		}
	}

	void MainMenuBar::End()
	{

	}

	void MainMenuBar::CreateMainMenBar()
	{
		auto* _edit = AddWidget<Menu>("Edit");

		_edit->AddWidget<MenuItem>("Flag: Save Scene", "", false, false, true, 0)->SetClickEvent(
			[]()
			{
				EventManager::GetInstance()->SaveScene();
			}
		);

		_edit->AddWidget<MenuItem>("Flag: Exit", "", false, false, true, 0)->SetClickEvent(
			[]()
			{
				EventManager::GetInstance()->ExitEditor();
			}
		);

		auto* _view = AddWidget<Menu>("View");

		//auto* _gameView = EventManager::GetInstance()->GetView("Game View");

		_view->AddWidget<MenuItem>("Flag: Game View", "", true, true, true, 0)->SetClickEvent(
			[]()
			{
				auto* _gameView = EventManager::GetInstance()->GetView("Game View");

				_gameView->SetOpen(!_gameView->GetOpen());
			}
		);

		//auto* _hierarchyView = EventManager::GetInstance()->GetView("Hierarchy View");

		_view->AddWidget<MenuItem>("Flag: Hierarchy View", "", true, true, true, 0)->SetClickEvent(
			[]()
			{
				auto* _hierarchyView = EventManager::GetInstance()->GetView("Hierarchy View");

				_hierarchyView->SetOpen(!_hierarchyView->GetOpen());
			}
		);

		//auto* _inspectorView = EventManager::GetInstance()->GetView("Inspector View");

		_view->AddWidget<MenuItem>("Flag: Inspector View", "", true, true, true, 0)->SetClickEvent(
			[]()
			{
				auto* _inspectorView = EventManager::GetInstance()->GetView("Inspector View");

				_inspectorView->SetOpen(!_inspectorView->GetOpen());
			}
		);

		//auto* _projectView = EventManager::GetInstance()->GetView("Project View");

		_view->AddWidget<MenuItem>("Flag: Project View", "", true, true, true, 0)->SetClickEvent(
			[]()
			{
				auto* _projectView = EventManager::GetInstance()->GetView("Project View");

				_projectView->SetOpen(!_projectView->GetOpen());
			}
		);

		auto* _object = AddWidget<Menu>("Object");

		auto* _setting = AddWidget<Menu>("Settings");

		_setting->AddWidget<MenuItem>("Flag: Graphics Setting", "", true, true, true, 0)->SetClickEvent(
			[]()
			{
				auto* _graphicsSetting = EventManager::GetInstance()->GetView("Graphics Setting");

				_graphicsSetting->SetOpen(!_graphicsSetting->GetOpen());
			}
		);

		auto* _layout = AddWidget<Menu>("Layout");
	}
}