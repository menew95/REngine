#include <Editor_pch.h>

#include <editor\GUI\ProjectView.h>

#include <filesystem>

#include <editor/EditorAPI.h>

#include "DirectXTex/DirectXTex.h"

namespace fs = std::filesystem;

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_folderSrv;
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_fileSrv;

//HICON GetFileTypeIcon(LPCTSTR szFileType, LPWSTR szTypeName)
//{ 
//	SHFILEINFO sfi; 
//
//	ZeroMemory(&sfi, sizeof(SHFILEINFO));
//	SHGetFileInfo(szFileType, 0, &sfi, sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_TYPENAME);
//	lstrcpy(szTypeName, sfi.szTypeName); 
//
//	return sfi.hIcon; 
//}


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


namespace editor
{
	ProjectView::ProjectView()
		: View("Project View")
		, m_currPath("C:\\Users\\IK-LPC-020\\Documents\\GitHub\\REngine")
	{
		ImageDesc _desc{ TEXT("C:\\Users\\IK-LPC-020\\Documents\\GitHub\\REngine\\Assets\\icon\\folder_icon.png"), nullptr};

		CreateImageTest(g_folderSrv, _desc);

		ImageDesc _desc2{ TEXT("C:\\Users\\IK-LPC-020\\Documents\\GitHub\\REngine\\Assets\\icon\\file_icon.png"), nullptr };

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
		string _path = "C:\\Users\\IK-LPC-020\\Documents\\GitHub\\REngine";

		ImGui::Columns(2, "Test", true);

		DrawFileTreeNode(_path);

		ImGui::NextColumn();

		DrawDirectory();

		ImGui::Columns(1);
	}

	void ProjectView::DrawFileTreeNode(string path)
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

		//if (!ImGui::GetDragDropPayload() && ImGui::BeginDragDropSource()) {
		//	// Some processing...
		//	ImGui::EndDragDropSource();
		//}

		//if (ImGui::BeginDragDropTarget()) {
		//	// Some processing...
		//	ImGui::EndDragDropTarget();
		//}

		//ImGui::PushID(gameObj->GetUUID().c_str());
		//if (ImGui::BeginPopupContextItem()) {
		//	// Some processing...
		//	ImGui::EndPopup();
		//}
		//ImGui::PopID();

		if (open)
		{
			if (fs::is_directory(_path))
			{
				for (auto& p : fs::directory_iterator(_path, fs::directory_options::skip_permission_denied))
				{
					DrawFileTreeNode(p.path().generic_string());
				}
			}

			ImGui::TreePop();
		}
	}

	void ProjectView::DrawDirectory()
	{
		fs::path _cur_path(m_currPath);

		fs::path _assetPath("C:\\Users\\IK-LPC-020\\Documents\\GitHub\\REngine");

		if (m_currPath != "C:\\Users\\IK-LPC-020\\Documents\\GitHub\\REngine")
		{
			if (ImGui::Button("<-"))
			{
				m_currPath = _cur_path.parent_path().string();

				_cur_path = _cur_path.parent_path();
			}
		}


		static float _padding = 16.0f;

		static float _thumnailSize = 64.0f;

		float _cellSize = _thumnailSize + _padding;

		float _pannelWidth = ImGui::GetContentRegionAvail().x;

		int _columnsCnt = (int)(_pannelWidth / _cellSize);

		ImGui::Columns(_columnsCnt == 0 ? 1 : _columnsCnt, 0, false);

		for (auto& _directoryEntry : fs::directory_iterator(_cur_path, fs::directory_options::skip_permission_denied))
		{
			const auto& _path = _directoryEntry.path();

			auto _relativePath = fs::relative(_path, _assetPath);

			std::string _fileName = _relativePath.filename().string();

			//ImGui::Button(_fileName.c_str(), { _thumnailSize, _thumnailSize });

			ImTextureID _texId = _directoryEntry.is_directory() ? g_folderSrv.Get() : g_fileSrv.Get();

			ImGui::ImageButton(_texId, { _thumnailSize, _thumnailSize }, {0, 0}, {1, 1});

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (_directoryEntry.is_directory())
				{
					m_currPath = (fs::path(m_currPath) / _path.filename()).string();
				}
			}

			ImGui::TextWrapped(_fileName.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &_thumnailSize, 64, 512);
		ImGui::SliderFloat("Padding Size", &_padding, 0, 32);
	}

	void ProjectView::DrawImageBtn()
	{


	}
}