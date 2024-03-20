#include <Editor_pch.h>
#include <editor\Core\AssetManager.h>
#include <common\AssetPath.h>
#include <importer\Importer.h>
#include <rengine\core\resources.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace editor
{
	DEFINE_SINGLETON_CLASS(AssetManager,
		{

		}, 
		{

		}, 
		{

		});


	void AssetManager::Initialze()
	{
		LoadAssetData();

		//Refresh();
	}
	
	void AssetManager::CreateAsset(rengine::Object* objec, const tstring& path)
	{
	
	}
	
	void AssetManager::ImportAsset(const tstring& path)
	{
		utility::Importer::Import(path);


	}

	void AssetManager::LoadAsset(const tstring& path)
	{
		fs::path _path(path);

		std::string _extension = _path.extension().string();

		std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

		if (_extension == ".mat")
		{
			rengine::Resources::GetInstance()->Load<rengine::Material>(_path.wstring());
		}
		else if (_extension == ".mesh")
		{
			rengine::Resources::GetInstance()->Load<rengine::Mesh>(_path.wstring());
		}
		else if (_extension == ".anim")
		{
			rengine::Resources::GetInstance()->Load<rengine::AnimationClip>(_path.wstring());
		}
		else if (_extension == ".png" || _extension == ".bmp" || _extension == ".jpeg" || _extension == ".jpg"
			|| _extension == ".dds" || _extension == ".tga" || _extension == ".hdr")
		{
			rengine::Resources::GetInstance()->Load<rengine::Texture>(_path.wstring());
		}
	}
	
	void AssetManager::MoveAsset(const tstring& oldPath, const tstring& newPath)
	{
	
	}
	
	void AssetManager::FindAsset(const tstring& filter)
	{
	
	}
	
	void AssetManager::Refresh()
	{
		CheckMetaFile(g_assetPath);
	}

	void AssetManager::CheckMetaFile(const tstring& path)
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

			if (_extension == ".meta")
				return;

			fs::path _metaPath(path + TEXT(".meta"));

			if (fs::exists(_metaPath))
				return;

			ImportAsset(path);
		}
	}

	void AssetManager::LoadAssetData()
	{
		fs::path _path(g_editorPath + TEXT("Library\\AssetData"));

		if (!fs::exists(_path))
		{
			// asset data file이 없을 경우 생성
			std::wofstream _ofs(g_editorPath + TEXT("Library\\AssetData"));

			_ofs << TEXT("Assets:\n");

			_ofs.close();
		}

		std::wifstream _ifs(g_editorPath + TEXT("Library\\AssetData"));

		assert(_ifs.is_open());

		pair<uuid, tstring> _pa;

		while (!_ifs.eof())
		{
			tstring _buf;
			std::getline(_ifs, _buf, L':');
			if (_buf.find(TEXT("UUID")) != wstring::npos)
			{
				std::getline(_ifs, _buf);

				_buf.erase(0, 1);

				_pa.first = _buf;
			}
			else if (_buf.find(TEXT("Path")) != wstring::npos)
			{
				std::getline(_ifs, _buf);

				_buf.erase(0, 1);

				_pa.second = _buf;

				m_assetList.insert(_pa);
			}
		}

		_ifs.close();

		for (auto& _assetPath : m_assetList)
		{
			LoadAsset(_assetPath.second);
		}
	}

	void AssetManager::SaveAssetData()
	{
		std::wofstream _ofs(g_editorPath + TEXT("Library\\AssetData"));

		for (auto& _pair : m_assetList)
		{
			_ofs << TEXT("  -\tUUID: ") + _pair.first + TEXT("\n");

			_ofs << TEXT("\tPath: ") + _pair.second + TEXT("\n");
		}

		_ofs.close();
	}
}