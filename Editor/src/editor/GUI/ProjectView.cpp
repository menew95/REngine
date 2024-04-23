#include <Editor_pch.h>

#include <common\AssetPath.h>

#include <editor\Core\AssetManager.h>
#include <editor\Widget\WidgetManager.h>

#include <editor\GUI\ProjectView.h>
#include <editor\GUI\EditorStyle.h>

#include <filesystem>

#include <editor\editor_api.h>

#include <rengine\core\sceneManager.h>
#include <rengine\core\scene\scene.h>
#include <rengine\core\Resources.h>
#include <rengine\core\resource\Material.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\resource\Mesh.h>

#include <graphics_core\resource\TextureBuffer.h>
#include <graphics_module\TextureFlags.h>

#include <serialize\Serializer.h>
#include <importer\Importer.h>

namespace fs = std::filesystem;

#pragma region icon를 출력하기 위해 임시 방편으로 만듬

HICON GetFileTypeIcon(LPCTSTR path/*, LPWSTR szTypeName*/)
{ 
	SHFILEINFO sfi; 

	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	SHGetFileInfo(path, 0, &sfi, sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_TYPENAME);

	return sfi.hIcon; 
}

HBITMAP icon_to_bitmap(HICON hicon) {
	
	ICONINFO _icon_info;

	GetIconInfo(hicon, &_icon_info);

	return _icon_info.hbmColor;
}

std::vector<unsigned char> ToPixels(HBITMAP BitmapHandle, int& width, int& height, int& pitch)
{
	BITMAP Bmp = { 0 };
	BITMAPINFO Info = { 0 };
	std::vector<unsigned char> Pixels = std::vector<unsigned char>();

	HDC DC = CreateCompatibleDC(NULL);
	std::memset(&Info, 0, sizeof(BITMAPINFO));
	HBITMAP OldBitmap = (HBITMAP)SelectObject(DC, BitmapHandle);
	GetObject(BitmapHandle, sizeof(Bmp), &Bmp);

	Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info.bmiHeader.biWidth = width = Bmp.bmWidth;
	Info.bmiHeader.biHeight = height = Bmp.bmHeight;
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = Bmp.bmBitsPixel;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = ((width * Bmp.bmBitsPixel + 31) / 32) * 4 * height;

	pitch = Bmp.bmWidthBytes;

	Pixels.resize(Info.bmiHeader.biSizeImage);
	GetDIBits(DC, BitmapHandle, 0, height, &Pixels[0], &Info, DIB_RGB_COLORS);
	SelectObject(DC, OldBitmap);

	height = std::abs(height);
	DeleteDC(DC);

	return Pixels;
}

#pragma endregion

namespace editor
{
	ProjectView::ProjectView()
		: View("Project View")
		, m_currPath(g_assetPath)
	{
		m_folderIcon = rengine::Resources::GetInstance()->Load<rengine::Texture>(g_assetPath + TEXT("icon\\folder_icon.png"));
		m_fileIcon = rengine::Resources::GetInstance()->Load<rengine::Texture>(g_assetPath + TEXT("icon\\file_icon.png"));

		CheckMetaFile(m_currPath);

		CreatePopupWidget();
	}

	ProjectView::~ProjectView()
	{

	}

	void EDITOR_API ProjectView::Begin()
	{
		__super::Begin();
	}

	void EDITOR_API ProjectView::Render()
	{
		//__super::Render();

		Draw();

		RenderChild();
	}

	void EDITOR_API ProjectView::End()
	{
		__super::End();
	}

	void ProjectView::Draw()
	{
		tstring _path = g_assetPath;

		ImGui::Columns(2, "Test", true);

		DrawFileTreeNode(_path);

		ImGui::NextColumn();

		DrawDirectory();
	}

	void ProjectView::DrawFileTreeNode(tstring path)
	{
		fs::path _path(path);

		ImGuiTreeNodeFlags nodeFlag = ImGuiTreeNodeFlags_None;
		nodeFlag |= ImGuiTreeNodeFlags_OpenOnArrow;

		if (!fs::is_directory(_path))
		{
			nodeFlag |= ImGuiTreeNodeFlags_Leaf;
		}

		/// 특정 객체를 선택하면 그 객체만 아닌 자식 객체도 모두 색칠되어 일단 주석처리
		if (m_currPath == path)
		{
			ImGui::PushStyleColor(ImGuiCol_Header, EditorStyle::GetColor(ImGuiCol_Header));          // 기본 색상
			ImGui::PushStyleColor(ImGuiCol_Text, EditorStyle::GetColor(ImGuiCol_Text));
		}

		bool open = ImGui::TreeNodeEx(_path.generic_string().c_str(), nodeFlag, _path.stem().generic_string().c_str());

		if (m_currPath == path)
		{
			ImGui::PopStyleColor(2);
		}

		//m_ObjectList.push_back(root);
		ImGui::Spacing();

		if (ImGui::IsItemClicked() && fs::is_directory(_path))
		{
			m_currPath = path;
		}

		if (open)
		{
			if (fs::is_directory(_path))
			{
				for (auto& p : fs::directory_iterator(_path, fs::directory_options::skip_permission_denied))
				{
					DrawFileTreeNode(p.path().wstring());
				}
			}

			ImGui::TreePop();
		}
	}

	void ProjectView::DrawDirectory()
	{
		fs::path _cur_path(m_currPath);

		fs::path _assetPath(g_assetPath);

		if (m_currPath != g_assetPath)
		{
			if (ImGui::Button("<-"))
			{
				// 경로 마지막을 \\를 붙이기 위하여 이렇게 함
				m_currPath = _cur_path.parent_path().parent_path().wstring() + TEXT("\\");

				_cur_path = m_currPath;
			}
		}

		static float _padding = 16.0f;

		static float _thumnailSize = 64.0f;

		float _cellSize = _thumnailSize + _padding;

		float _pannelWidth = ImGui::GetContentRegionAvail().x;

		int _columnsCnt = (int)(_pannelWidth / _cellSize);

		ImGui::Columns(_columnsCnt == 0 ? 1 : _columnsCnt, 0, false);

		ImGui::PushStyleColor(ImGuiCol_ButtonActive, EditorStyle::GetColor(ImGuiCol_ButtonActive));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, EditorStyle::GetColor(ImGuiCol_ButtonHovered));

		for (auto& _directoryEntry : fs::directory_iterator(_cur_path, fs::directory_options::skip_permission_denied))
		{
			const auto& _path = _directoryEntry.path();

			auto _relativePath = fs::relative(_path, _assetPath);

			std::string _fileName = _relativePath.filename().string();

			ImTextureID _texId = nullptr;

			ImVec2 _top = {1, 1}, _bot = {0, 0};

			if (_path.has_extension())
			{
				tstring _extension = _path.extension().wstring();

				std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

				if(_extension == TEXT(".meta") && !m_bDrawMetaFile)
					continue;

				if (_extension == TEXT(".png") || _extension == TEXT(".bmp") || _extension == TEXT(".jpeg") || _extension == TEXT(".jpg")
					|| _extension == TEXT(".dds") || _extension == TEXT(".tga") || _extension == TEXT(".hdr"))
				{
					uuid _uuid;

					auto _metaInfo = utility::Serializer::SerializeMetaInfo(_path);

					shared_ptr<rengine::Texture> _tex = rengine::Resources::GetInstance()->GetResource<rengine::Texture>(_metaInfo._guid);

					_texId = _tex->GetTextureID();

					_top = { 0, 0 };
					_bot = { 1, 1 };
				}
				else
				{
					auto _iter = g_iconTextureMap.find(_path.extension().wstring());

					if (_iter != g_iconTextureMap.end())
					{
						_texId = _iter->second->GetTextureBuffer()->GetTextureID();
					}
					else
					{
						auto* _texture = CreateTextureFromFileIcon(_path.wstring());

						g_iconTextureMap.insert(make_pair(_path.extension().wstring(), _texture));

						_texId = _texture->GetTextureBuffer()->GetTextureID();
					}
				}
			}
			else
			{
				_texId = _directoryEntry.is_directory() ? m_folderIcon->GetTextureBuffer()->GetTextureID() : m_fileIcon->GetTextureBuffer()->GetTextureID();

				_top = { 0, 0 };
				_bot = { 1, 1 };
			}

			if (m_selected.end() != std::find(m_selected.begin(), m_selected.end(), _path.wstring()))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 0.3f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, EditorStyle::GetColor(ImGuiCol_Button));
			}

			ImGui::PushID(_fileName.c_str());

			ImGui::ImageButton(_texId, { _thumnailSize, _thumnailSize }, _top, _bot);

			ImGui::PopStyleColor(1);

			if (ImGui::BeginDragDropSource())
			{
				auto _itemPath = _path.c_str();

				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", _itemPath, wcslen(_itemPath) * sizeof(wchar_t) + 2, ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				auto* _payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");

				if (_payload != nullptr && _directoryEntry.is_directory())
				{
					const wchar_t* _wstr = static_cast<const wchar_t*>(_payload->Data);

					fs::path _payloadPath(_wstr);

					fs::path _newPath = _path / _payloadPath.filename();

					fs::rename(_payloadPath, _newPath);
				}

				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsItemHovered())
			{
				if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (_directoryEntry.is_directory())
					{
						m_selected.clear();

						m_currPath = (fs::path(m_currPath) / _path.filename()).wstring() + TEXT("\\");
					}
					else
					{
						// 디렉토리가 아닌 에셋을 더블 클릭할 경우

						tstring _extension = _path.extension().wstring();

						std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

						if (_extension == TEXT(".mat"))
						{
							// 머티리얼 일경우 인스펙터에 머티리얼 에디터 창을 띄움

							uuid _asset_uuid = AssetManager::GetInstance()->UUIDFromAssetPath(fs::absolute(_path).wstring());

							// 에셋 매니저에 등록되지 않은 에셋 패스임
							assert(_asset_uuid != TEXT("0"));

							const auto& _mat = rengine::Resources::GetInstance()->GetResource<rengine::Material>(_asset_uuid);

							assert(_mat != nullptr);

							EventManager::GetInstance()->SetFocusObject(_mat.get());
						}
					}
				}
				else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
					{
						m_selected.push_back(_path.wstring());
					}
					else
					{
						m_selected.clear();
						m_selected.push_back(_path.wstring());
					}
				}
			}

			ImGui::TextWrapped(_relativePath.stem().string().c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &_thumnailSize, 64, 512);
		ImGui::SliderFloat("Padding Size", &_padding, 0, 32);

		ImGui::PopStyleColor(2);
	}

	void ProjectView::DrawImageBtn()
	{


	}

	void ProjectView::CreatePopupWidget()
	{
		m_popupMenu = AddWidget<Popup>("ProjectView Popup");

		m_popupMenu->SetCheckFunction([]()
			{
				return ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right);
			});

		{
			auto* _menu = m_popupMenu->AddWidget<Menu>("Create", 0);

			auto* _menuItem = _menu->AddWidget<MenuItem>("Folder", "", false, false, 0);

			_menuItem->SetClickEvent([&]() {
					AssetManager::GetInstance()->CreateFolder(m_currPath, TEXT("Folder"));
				});

			_menuItem = _menu->AddWidget<MenuItem>("Scene", "", false, false, 0);

			_menuItem->SetClickEvent([&]() {
					auto _path = AssetManager::MakeNewPath(m_currPath, TEXT("New Scene"), TEXT(".scene"));

					auto _scene = rengine::SceneManager::GetInstance()->CreateScene(TEXT("New Scene"));

					_scene->SetPath(_path);

					AssetManager::GetInstance()->CreateAsset(_scene.get(), _path);
				});

			_menuItem = _menu->AddWidget<MenuItem>("Material", "", false, false, 0);

			_menuItem->SetClickEvent([&]() {
					auto _path = AssetManager::MakeNewPath(m_currPath, TEXT("New Material"), TEXT(".mat"));

					auto _material = rengine::Resources::GetInstance()->CreateResource<rengine::Material>();

					_material->SetPath(_path);

					_material->SetRenderPassID(TEXT("Deferred Pass"));

					_material->SetPipelineID(TEXT("Standard"));

					AssetManager::GetInstance()->CreateAsset(_material.get(), _path);
				});
		}

		{
			auto* _menuItem = m_popupMenu->AddWidget<MenuItem>("Open", "", false, false, 0);

			_menuItem = m_popupMenu->AddWidget<MenuItem>("Delete", "", false, false, 0);
		}
	}

	void ProjectView::CheckMetaFile(const tstring& path)
	{
		fs::path _path(path);

		if (fs::is_directory(_path))
		{
			for (auto& _p : fs::directory_iterator(_path, fs::directory_options::skip_permission_denied))
			{
				CheckMetaFile(_p.path().wstring());
			}
		}
		else if (_path.has_extension())
		{
			std::string _extension = _path.extension().string();

			if(_extension == ".meta")
				return;

			fs::path _metaPath(path + TEXT(".meta"));

			if (fs::exists(_metaPath))
				return;

			utility::Importer::Import(path);
		}
	}

	rengine::Texture* ProjectView::CreateTextureFromFileIcon(const tstring& path)
	{
		HDC hdcScreen = GetDC(NULL);
		HDC hdc = CreateCompatibleDC(hdcScreen);

		HICON hicon = GetFileTypeIcon(path.c_str());

		HBITMAP hbmp = icon_to_bitmap(hicon);

		int _width = 0, _height = 0, _pitch = 0;

		auto _pixel = ToPixels(hbmp, _width, _height, _pitch);

		graphics::TextureDesc _texDesc
		{
			graphics::TextureType::Texture2D,
			graphics::BindFlags::ShaderResource,
			0,
			graphics::Format::B8G8R8A8_UNORM,
			Extent3D{ (uint32)_width, (uint32)_height, 1},
			1,
			1,
			1
		};

		graphics::ImageDesc _imageDesc{ _pixel.data() , (uint32)_pitch, (uint32)(_pitch * _height)};

		auto _texture = rengine::Resources::GetInstance()->CreateResource<rengine::Texture>();

		_texture->GetTextureBuffer()->LoadTexture(_texture->GetUUID(), _texDesc, _imageDesc);

		return _texture.get();
	}
}