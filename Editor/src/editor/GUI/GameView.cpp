#include <Editor_pch.h>

#include <editor\GUI\GameView.h>

#include <rengine\core\component\Camera.h>
#include <rengine\core\Resources.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\scene\scene.h>
#include <rengine\core\sceneManager.h>
#include <rengine\system\Time.h>
#include <rengine\system\Input.h>
#include <rengine\system\GraphicsSystem.h>
#include <rengine\system\ObjectFactory.h>

#include <graphics_core\GraphicsEngine.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>

#include <log\log.h>

namespace editor
{
	GameView::GameView()
		: View("Game View")
	{
		CreateGameViewCamera();
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

		ImGui::Image(m_pGameViewCameraBuffer->GetTextureID(), ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{0, 0}, ImVec2{ 1, 1 });

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
		
		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("Deferred Light Pass")));
		
		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("Grid Pass")));
		
		m_pGameViewCameraBuffer->PushRenderPass(graphics::ResourceManager::GetInstance()->GetRenderPass(TEXT("Final Pass")));
	}

	void GameView::GameViewCameraControl()
	{
		graphics::GraphicsEngine::GetInstance()->PushCameraBuffer(m_pGameViewCameraBuffer);

		if (!ImGui::IsWindowFocused())
			return;

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

			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_W))
				_forward += 1.f;
			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_S))
				_forward -= 1.f;

			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_D))
				_right += 1.f;
			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_A))
				_right -= 1.f;

			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_E))
				_up += 1.f;
			if (rengine::Input::GetKeyPress(rengine::EVirtualKey::Key_Q))
				_up -= 1.f;

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

		_info._cameraWorldPos = _camWorld.Translation();
		_info._near = 0.1f;
		_info._far = 1000.f;

		_info._view = math::Matrix::CreateLookAt(_info._cameraWorldPos, _info._cameraWorldPos + _camWorld.Forward(), _camWorld.Up());
		_info._proj = math::Matrix::CreatePerspectiveFieldOfView(90.f * math::Deg2Rad, 1.0f, 0.1f, 1000.f);
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