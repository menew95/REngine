#include <Editor_pch.h>

#include <common\AssetPath.h>

#include <editor\GUI\ProjectView.h>

#include <editor\GUI\EditorStyle.h>

#include <filesystem>

#include <editor\editor_api.h>


namespace fs = std::filesystem;

#pragma region icon를 출력하기 위해 임시 방편으로 만듬

#include "DirectXTex/DirectXTex.h"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_folderSrv;
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_fileSrv;

map<string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> g_iconMap; // extension, srv

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

union DX11NativeTexture
{
	inline DX11NativeTexture() : _resource{ nullptr }
	{}

	inline DX11NativeTexture(const DX11NativeTexture& texture) : _resource{ texture._resource }
	{}

	inline DX11NativeTexture& operator=(const DX11NativeTexture& texture)
	{
		_resource = texture._resource;
	}

	inline ~DX11NativeTexture()
	{
		_resource.Reset();
	}

	Microsoft::WRL::ComPtr<ID3D11Resource> _resource;
	Microsoft::WRL::ComPtr<ID3D11Texture1D> _tex1D;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _tex2D;
	Microsoft::WRL::ComPtr<ID3D11Texture3D> _tex3D;
};

DX11NativeTexture m_NativeTexture;

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

void BitmapToSrv(LPCTSTR path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv)
{
	HDC hdcScreen = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hdcScreen);

	HICON hicon = GetFileTypeIcon(path);

	HBITMAP hbmp = icon_to_bitmap(hicon);

	int _width = 0, _height = 0, _pitch = 0;

	auto _pixel = ToPixels(hbmp, _width, _height, _pitch);

	D3D11_TEXTURE2D_DESC screenshot_desc = CD3D11_TEXTURE2D_DESC(
		DXGI_FORMAT_B8G8R8A8_UNORM,     // format
		_width,							// width
		_height,						// height
		1,                              // arraySize
		1,                              // mipLevels
		D3D11_BIND_SHADER_RESOURCE,     // bindFlags
		D3D11_USAGE_DEFAULT,            // usage
		D3D11_CPU_ACCESS_WRITE,         // cpuaccessFlags
		1,                              // sampleCount
		0,                              // sampleQuality
		0                               // miscFlags
	);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = _pixel.data();
	data.SysMemPitch = _pitch;
	data.SysMemSlicePitch = _pitch * _height;

	auto* device = (ID3D11Device*)GetEditor()->GetDevice();

	bool hr = device->CreateTexture2D(
		&screenshot_desc,
		&data,
		m_NativeTexture._tex2D.GetAddressOf()
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = screenshot_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MostDetailedMip = screenshot_desc.MipLevels;

	device->CreateShaderResourceView(m_NativeTexture._tex2D.Get(), NULL, srv.ReleaseAndGetAddressOf());
}

uint32 CheckFileFormat(const tstring& path)
{
	if (static_cast<uint32>(path.length()) > 1)
	{
		size_t extensionStartPoint = path.find_last_of('.') + (size_t)1;

		auto fileFormat = path.substr(extensionStartPoint);

		if (fileFormat == TEXT("dds") || fileFormat == TEXT("DDS"))
		{
			return 0;
		}
		else if (fileFormat == TEXT("tga") || fileFormat == TEXT("TGA"))
		{
			return 1;
		}
		else if (fileFormat == TEXT("hdr") || fileFormat == TEXT("HDR"))
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}

	return 3;
}

struct ImageDesc
{
	tstring _filePath;
	void* _data;
};

void CreateImageTest(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, const ImageDesc& srcDesc)
{
	using namespace DirectX;

	auto* device = (ID3D11Device*)GetEditor()->GetDevice();

	if (srcDesc._filePath.size() <= 0)
	{
		AssertMessageBox(false, "filePath is not invaild");
	}

	ScratchImage image;

	switch (CheckFileFormat(srcDesc._filePath))
	{
	case 0:
	{
		LoadFromDDSFile(srcDesc._filePath.c_str(), DDS_FLAGS_NONE, nullptr, image);
		break;
	}
	case 1:
	{
		LoadFromTGAFile(srcDesc._filePath.c_str(), TGA_FLAGS_NONE, nullptr, image);
		break;
	}
	case 2:
	{
		LoadFromHDRFile(srcDesc._filePath.c_str(), nullptr, image);
		break;
	}
	case 3:
	default:
	{
		LoadFromWICFile(srcDesc._filePath.c_str(), WIC_FLAGS_NONE, nullptr, image);
		break;
	}
	}

	AssertMessageBox(image.GetImageCount() != 0, "D3D11Texture LoadFaile Error");

	bool isCubeMap = image.GetMetadata().IsCubemap();

	if (isCubeMap)
	{
		DirectX::ScratchImage mipChain;

		CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), m_NativeTexture._resource.GetAddressOf());

	}
	else
	{
		CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), m_NativeTexture._resource.GetAddressOf());
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	m_NativeTexture._tex2D->GetDesc(&textureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if (isCubeMap)
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	}
	else
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	}

	device->CreateShaderResourceView(m_NativeTexture._resource.Get(), &srvDesc, srv.ReleaseAndGetAddressOf());
}

#pragma endregion

namespace editor
{
	ProjectView::ProjectView()
		: View("Project View")
		, m_currPath(g_assetPath)
	{
		ImageDesc _desc{ TEXT("..\\..\\..\\..\\Assets\\icon\\folder_icon.png"), nullptr};

		CreateImageTest(g_folderSrv, _desc);

		ImageDesc _desc2{ TEXT("..\\..\\..\\..\\\\Assets\\icon\\file_icon.png"), nullptr };

		CreateImageTest(g_fileSrv, _desc2);
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
		__super::Render();

		Draw();
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

		//ImGui::Columns(1);
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
				m_currPath = _cur_path.parent_path().wstring();

				_cur_path = _cur_path.parent_path();
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
			
			ImGui::PushID(_fileName.c_str());

			//tstring _test = StringHelper::StringToWString(_relativePath.wstring().c_str());

			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv;

			if (_path.has_extension())
			{
				auto _iter = g_iconMap.find(_path.extension().string());

				if (_iter != g_iconMap.end())
				{
					_srv = _iter->second;
				}
				else
				{
					BitmapToSrv(_path.wstring().c_str(), _srv);

					g_iconMap.insert(make_pair(_path.extension().string(), _srv));
				}
			}
			else
			{
				_srv = _directoryEntry.is_directory() ? g_folderSrv : g_fileSrv;
			}

			ImTextureID _texId = _srv.Get();//_directoryEntry.is_directory() ? g_folderSrv.Get() : g_fileSrv.Get();


			if (m_selected.end() != std::find(m_selected.begin(), m_selected.end(), _path.wstring()))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 0.3f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, EditorStyle::GetColor(ImGuiCol_Button));
			}

			ImGui::ImageButton(_texId, { _thumnailSize, _thumnailSize }, {0, 0}, {1, 1});

			ImGui::PopStyleColor(1);

			if (ImGui::BeginDragDropSource())
			{
				auto _itemPath = _relativePath.c_str();

				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", _itemPath, wcslen(_itemPath) * sizeof(wchar_t) + 2, ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				auto* _payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");

				if (_payload != nullptr && _directoryEntry.is_directory())
				{
					const wchar_t* _wstr = static_cast<const wchar_t*>(_payload->Data);

					fs::path _payloadPath(g_assetPath);

					_payloadPath /= _wstr;

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

						m_currPath = (fs::path(m_currPath) / _path.filename()).wstring();
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

			ImGui::TextWrapped(_fileName.c_str());

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
}