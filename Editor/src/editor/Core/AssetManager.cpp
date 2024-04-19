#include <Editor_pch.h>
#include <editor\Core\AssetManager.h>
#include <common\AssetPath.h>
#include <importer\Importer.h>
#include <rengine\core\resources.h>
#include <rengine\System\ObjectFactory.h>
#include <Serialize\Serializer.h>
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
		// 지금은 마땅히 하드 코딩 말곤 생각이 안난다.
		m_componentList.push_back("Camera");
		m_componentList.push_back("MeshRenderer");
		m_componentList.push_back("Transform");
		m_componentList.push_back("Light");
		m_componentList.push_back("TestComponent");

		LoadAssetData();

		Refresh();
	}
	
	void AssetManager::CreateAsset(rengine::Object* object, const tstring& path)
	{
		m_bIsDirty = true;
		
	}
	
	void AssetManager::ImportAsset(const tstring& path)
	{
		auto _obj = utility::Importer::Import(path);

		if(_obj == nullptr)
			return;

		auto _resource = static_pointer_cast<rengine::Resource>(_obj);

		if(_resource == nullptr)
			return;

		m_bIsDirty = true;
		m_assetList.insert(make_pair(_resource->GetPath(), _resource->GetUUID()));
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
		m_bIsDirty = true;

		fs::path _oldPath(oldPath);

		fs::path _newPath(newPath);

		fs::rename(_oldPath, _newPath);
	}
	
	void AssetManager::FindAsset(const tstring& filter)
	{
		
	}
	
	uuid AssetManager::UUIDFromAssetPath(const tstring& path)
	{
		auto _iter = find_if(begin(m_assetList), end(m_assetList), [&path](const auto& _pair)
			{
				return _pair.first == path;
			});

		if(_iter == end(m_assetList))
			return TEXT("0");

		return _iter->second;
	}

	shared_ptr<rengine::Object> AssetManager::AssetFromPath(const tstring& path, const tstring& type)
	{
		auto _uuid = UUIDFromAssetPath(path);

		if(_uuid == TEXT("0"))
			return nullptr;

		shared_ptr<rengine::Object> _object;

		if(type == TEXT(""))
			return rengine::ObjectFactory::GetInstance()->Find(_uuid);
		else
		{
			for (auto [_object_uuid, _object] : rengine::ObjectFactory::GetInstance()->FindObjectsOfType(type))
			{
				if (_uuid == _object_uuid)
				{
					return _object;
				}
			}
		}

		return nullptr;
	}

	void AssetManager::Refresh()
	{
		CheckMetaFile(g_assetPath);

		SaveAssetData();
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
			{
				// asset list에 등록이 되어있는지 확인후 안되어 있으면 추가
				auto _iter = m_assetList.find(fs::absolute(_path));

				auto _metainfo = utility::Serializer::SerializeMetaInfo(path);

				if (_iter != m_assetList.end())
				{
					// 이미 해당 경로의 uuid가 존재함

					if (_iter->second != _metainfo._guid)
					{
						// 그런데 이전에 등록된 uuid와 다를 경우 새로운 uuid로 교체함
						m_assetList[fs::absolute(_path)] = _metainfo._guid;

						m_bIsDirty = true;
					}
				}
				else
				{
					// 해당 경로에 해당하는 uuid가 없음
					m_assetList.insert(make_pair(fs::absolute(_path), _metainfo._guid));

					m_bIsDirty = true;
				}

				return;
			}

			// 메타파일이 존재하지 않을경우 메타파일을 생성하고 에디터에 임포트 해줌
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

		pair<tstring, uuid> _pa;

		while (!_ifs.eof())
		{
			tstring _buf;
			std::getline(_ifs, _buf, L':');
			if (_buf.find(TEXT("UUID")) != wstring::npos)
			{
				std::getline(_ifs, _buf);

				_buf.erase(0, 1);

				_pa.second = _buf;
			}
			else if (_buf.find(TEXT("Path")) != wstring::npos)
			{
				std::getline(_ifs, _buf);

				_buf.erase(0, 1);

				_pa.first = _buf;

				m_assetList.insert(_pa);
			}
		}

		_ifs.close();

		for (auto& [_assetPath, _uuid] : m_assetList)
		{
			LoadAsset(_assetPath);
		}
	}

	void AssetManager::SaveAssetData()
	{
		if(!m_bIsDirty) return;

		std::wofstream _ofs(g_editorPath + TEXT("Library\\AssetData"));

		_ofs << TEXT("Assets:\n");

		for (auto& _pair : m_assetList)
		{
			_ofs << TEXT("  -\tUUID: ") + _pair.second + TEXT("\n");

			_ofs << TEXT("\tPath: ") + _pair.first + TEXT("\n");
		}

		_ofs.close();
	}
}