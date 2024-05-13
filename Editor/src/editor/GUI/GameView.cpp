#include <Editor_pch.h>

#include <editor\GUI\GameView.h>
#include <editor\Core\AssetManager.h>

#include <rengine\core\component\Camera.h>
#include <rengine\core\Resources.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Transform.h>
#include <rengine\core\scene\scene.h>
#include <rengine\core\sceneManager.h>
#include <rengine\system\Time.h>
#include <rengine\system\Input.h>
#include <rengine\system\GraphicsSystem.h>
#include <rengine\system\ObjectFactory.h>

#include <graphics_core\GraphicsEngine.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_module\Texture.h>

#include <log\log.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace editor
{
	GameView::GameView()
		: View("Game View")
	{
		CreateGameViewCamera();

		CreateGameViewWidget();

		CachingTexture();
	}

	GameView::~GameView()
	{
		graphics::ResourceManager::GetInstance()->RelaseCameraBuffer(TEXT("Game View Camera"));
	}

	void EDITOR_API GameView::Begin()
	{
		__super::Begin();

	}
	void EDITOR_API GameView::Render()
	{
		__super::Render();

		GameViewCameraControl();

		auto _mainCam = rengine::Camera::GetMainTextureID();

		ImVec2 _viportPanelSize = ImGui::GetContentRegionAvail();

		m_viewportSize = { _viportPanelSize.x, _viportPanelSize.y };

		ImTextureID _textureID = m_textureList[static_cast<int>(m_currentViewMode)]->GetTextureID();

		ImGui::Image(_textureID, ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 0}, ImVec2{1, 1});

#pragma region IMGUIZMO
		auto* _object = EventManager::GetInstance()->GetFocusObject();

		if (_object != nullptr && _object->GetType() == TEXT("GameObject"))
		{
			auto* _go = reinterpret_cast<rengine::GameObject*>(_object);

			IMGUIZMO_NAMESPACE::SetOrthographic(false);

			IMGUIZMO_NAMESPACE::SetDrawlist();

			math::Matrix _world = _go->GetTransform()->GetWorld();
			math::Matrix _view = m_pGameViewCameraBuffer->GetView();
			math::Matrix _proj = m_pGameViewCameraBuffer->GetProj();

			IMGUIZMO_NAMESPACE::Manipulate(
				reinterpret_cast<float*>(&_view),
				reinterpret_cast<float*>(&_proj),
				m_currentOperation,
				m_currentMode,
				reinterpret_cast<float*>(&_world)
			);

			ImVec2 _itemMinRect = ImGui::GetItemRectMin();
			ImVec2 _itemMaxRect = ImGui::GetItemRectMax();

			IMGUIZMO_NAMESPACE::SetRect(_itemMinRect.x, _itemMinRect.y, _itemMaxRect.x - _itemMinRect.x, _itemMaxRect.y - _itemMinRect.y);

			if (IMGUIZMO_NAMESPACE::IsUsing())
			{
				// 매트릭스 분해 및 업데이트 (=> 자식 개체들도 ..!)
				_go->GetTransform()->SetWorld(_world);

				return;
			}
		}
#pragma endregion

#pragma region Picking
		if(ImGui::IsItemHovered())
		{
			// 아이템의 왼쪽 상단 모서리 좌표
			ImVec2 _itemMinRect = ImGui::GetItemRectMin();

			auto [mx, my] = ImGui::GetMousePos();
			Vector2 _mousePosInItem = { mx - _itemMinRect.x, my - _itemMinRect.y };

			int mouseX = (int)(mx - _itemMinRect.x);
			int mouseY = (int)(my - _itemMinRect.y);

			Log::Core_Info(std::format("Mouse = {0}, {1}", mouseX, mouseY));

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				ObjectPicking(m_viewportSize, _mousePosInItem);
			}
		}

		if (ImGui::IsWindowFocused() && !rengine::Input::GetKeyPress(rengine::EVirtualKey::MouseRight))
		{
			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_W))
			{
				m_currentOperation = IMGUIZMO_NAMESPACE::OPERATION::TRANSLATE;
			}
			else if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_E))
			{
				m_currentOperation = IMGUIZMO_NAMESPACE::OPERATION::ROTATE;
			}
			else if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_R))
			{
				m_currentOperation = IMGUIZMO_NAMESPACE::OPERATION::SCALE;
			}
			else if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_T))
			{
				m_currentOperation == IMGUIZMO_NAMESPACE::MODE::LOCAL ? m_currentMode = IMGUIZMO_NAMESPACE::MODE::WORLD : m_currentMode = IMGUIZMO_NAMESPACE::MODE::LOCAL;
			}
		}
#pragma endregion

#pragma region asset item drag
		if (ImGui::BeginDragDropTarget())
		{
			auto payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery);

			// Content_Browser_Item 받음.
			if (payload != nullptr && payload->IsDelivery())
			{
				const wchar_t* _wstr = static_cast<const wchar_t*>(payload->Data);

				fs::path _payloadPath(_wstr);

				fs::path _absolutePath = fs::absolute(_payloadPath);

				tstring _extension = _payloadPath.extension().c_str();

				std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

				auto _uuid = AssetManager::GetInstance()->UUIDFromAssetPath(_absolutePath);

				// .DUOL File
				if (_extension == TEXT(".mesh"))
					rengine::SceneManager::GetInstance()->GetCurrentScene()->CreateFromMesh(_uuid);
				else if (_extension == TEXT(".fbx"))
					rengine::SceneManager::GetInstance()->GetCurrentScene()->CreateFromModel(_uuid);
			}

			ImGui::EndDragDropTarget();
		}
#pragma endregion
	}
	void EDITOR_API GameView::End()
	{
		__super::End();

	}
	void GameView::CreateGameViewCamera()
	{
		m_pGameViewCameraBuffer = graphics::ResourceManager::GetInstance()->CreateCameraBuffer(TEXT("Game View Camera"));

		m_pGameViewCameraBuffer->Init();

		m_pGameViewCameraBuffer->SetName("Game View Camera");

		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("SkyBox Pass")));

		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("Deferred Pass")));

		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("Shadow Pass")));
		
		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("Deferred Light Pass")));
		
		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("Grid Pass")));
		
		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("PostProcess Pass")));
		
		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("Final Pass")));
	}

	void GameView::CreateGameViewWidget()
	{
		auto _comboBox = AddWidget<ComboBox>("GBuffer");
		
		vector<string> _items 
		{
			"Default",
			"Depth",
			"Albedo",
			"Normal",
			"World",
			"Emissive",
			"Flag"
		};

		_comboBox->SetComboItems(_items);

		_comboBox->RegisterGetter([&]()
			{
				return static_cast<int>(m_currentViewMode);
			});

		_comboBox->RegisterSetter([&](int& mode)
			{
				m_currentViewMode = static_cast<ViewMode>(mode);
			});
	}

	void GameView::CachingTexture()
	{
		m_textureList[0] = m_pGameViewCameraBuffer->GetColorTexture();
		m_textureList[1] = graphics::ResourceManager::GetInstance()->GetTexture(TEXT("Depth"));
		m_textureList[2] = graphics::ResourceManager::GetInstance()->GetTexture(TEXT("Albedo"));
		m_textureList[3] = graphics::ResourceManager::GetInstance()->GetTexture(TEXT("Normal"));
		m_textureList[4] = graphics::ResourceManager::GetInstance()->GetTexture(TEXT("World"));
		m_textureList[5] = graphics::ResourceManager::GetInstance()->GetTexture(TEXT("Emissive"));
		m_textureList[6] = graphics::ResourceManager::GetInstance()->GetTexture(TEXT("Flag"));
	}

	void GameView::GameViewCameraControl()
	{
		graphics::GraphicsEngine::GetInstance()->PushCameraBuffer(m_pGameViewCameraBuffer);

		if (ImGui::IsWindowFocused() == false)
			return;

		if (rengine::Input::GetKeyPress(rengine::EVirtualKey::LeftShift))
			m_fSpeed = 20.f;
		else if (rengine::Input::GetKeyUp(rengine::EVirtualKey::LeftShift))
			m_fSpeed = 5.f;

		static math::Matrix _camWorld;
		{
			math::Vector3 _rightDir = _camWorld.Right();
			math::Vector3 _upDir = _camWorld.Up();
			math::Vector3 _forwardDir = _camWorld.Forward();

			Vector3 _scale, _translate;
			Quaternion _rotation;

			if (!_camWorld.Decompose(_scale, _rotation, _translate))
				assert(false);

			float _rotationPitch = 0;
			float _rotationY = 0;

			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::MouseRight))
			{
				const auto _mouseDelta = rengine::Input::GetMouseMovementDelta();
				_rotationPitch = _mouseDelta.y * 0.1f * Deg2Rad;
				_rotationY = _mouseDelta.x * 0.1f * Deg2Rad;

				// // (deprecated) Matrix로 회전합니다.
				// if (_rotationPitch != 0)
				//     PitchUsingMatrix(_rotationPitch, _rightDir, _upDir, _forwardDir);
				// if (_rotationY != 0)
				//     RotateYUsingMatrix(_rotationY, _rightDir, _upDir, _forwardDir);

				// Quaternion으로 회전합니다.
				if (_rotationPitch != 0)
					_rotation = _rotation * Quaternion::CreateFromAxisAngle(_rightDir, _rotationPitch);
				if (_rotationY != 0)
					_rotation = _rotation * Quaternion::CreateFromAxisAngle(Vector3::Up, _rotationY);

				_rightDir = Vector3::Transform(Vector3::Right, _rotation);
				_upDir = Vector3::Transform(Vector3::Up, _rotation);
				_forwardDir = Vector3::Transform(Vector3::Forward, _rotation);
			}

			float _forward = 0;
			float _right = 0;
			float _up = 0;

			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::MouseRight))
			{
				if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_W))
					_forward += m_fSpeed * (float)rengine::Time::GetDeltaTime();
				if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_S))
					_forward -= m_fSpeed * (float)rengine::Time::GetDeltaTime();

				if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_D))
					_right += m_fSpeed * (float)rengine::Time::GetDeltaTime();
				if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_A))
					_right -= m_fSpeed * (float)rengine::Time::GetDeltaTime();

				if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_E))
					_up += m_fSpeed * (float)rengine::Time::GetDeltaTime();
				if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_Q))
					_up -= m_fSpeed * (float)rengine::Time::GetDeltaTime();
			}

			if (_forward != 0 || _right != 0 || _up != 0
				|| _rotationPitch != 0 || _rotationY != 0)
			{
				auto _direction = (_forwardDir * _forward + _rightDir * _right + _upDir * _up);
				_direction.Normalize();

				const auto _nextPosition = _translate + _direction * m_fSpeed * static_cast<float>(rengine::Time::GetDeltaTime());

				_camWorld = Matrix{
					_rightDir.x, _rightDir.y, _rightDir.z, 0.f,
					_upDir.x, _upDir.y, _upDir.z, 0.f,
					_forwardDir.x, _forwardDir.y, _forwardDir.z, 0.f,
					_nextPosition.x, _nextPosition.y, _nextPosition.z, 1.f
					};
				/*_camWorld = Matrix{
					_rightDir.x, _rightDir.y, _rightDir.z, 0.f,
					_upDir.x, _upDir.y, _upDir.z, 0.f,
					_forwardDir.x, _forwardDir.y, _forwardDir.z, 0.f,
					0.f, 0.f, 0.f, 1.f
					} * Matrix::CreateTranslation(_nextPosition);*/
			}
		}

		graphics::CameraInfo _info;

		float _aspectRatio =  1.0f;
		float _fov = 90.f * math::Deg2Rad;

		_info._cameraWorldPos = _camWorld.Translation();
		_info._near = 0.1f;
		_info._far = 1000.f;
		_info._aspectRatio = _aspectRatio;
		_info._fov = _fov;

		_info._view = math::Matrix::CreateLookAt(_info._cameraWorldPos, _info._cameraWorldPos + _camWorld.Forward(), _camWorld.Up());
		_info._proj = math::Matrix::CreatePerspectiveFieldOfView(_fov, _aspectRatio, 0.1f, 1000.f);
		_info._projInv = _info._proj.Invert();
		_info._viewToTexSpace = _info._view * _info._proj * math::Matrix(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f
		);
		_info._worldViewProj = math::Matrix::CreateTranslation(_info._cameraWorldPos) * _info._view * _info._proj;

		m_pGameViewCameraBuffer->Update(_info);
	}

	void GameView::ObjectPicking(const Vector2& imageSize, const Vector2& mousePosition)
	{
		uint64 _uuid_hash = rengine::GraphicsSystem::GetInstance()->ObjectPicking(imageSize, mousePosition);

		auto _curScene = rengine::SceneManager::GetInstance()->GetCurrentScene();

		if(_curScene == nullptr)
			return;

		for (auto& _go : _curScene->GetAllGameObjects())
		{
			if (_go->GetHash() == _uuid_hash)
			{
				EventManager::GetInstance()->SetFocusObject(_go.get());
			}
		}
	}
}