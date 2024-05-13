#include <rengine\core\Resources.h>

#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\resource\Material.h>
#include <rengine\core\resource\AnimationClip.h>
#include <rengine\core\resource\Model.h>

#include <rengine\system\ObjectFactory.h>

#include <filesystem>

#include <serialize\Serializer.h>

#include <rengine\utility\DefaultResourceBuilder.h>

namespace fs = std::filesystem;

inline bool CheckPathExist(const tstring& path)
{
	fs::path _path(path);

	return fs::exists(_path) && fs::is_regular_file(_path);
}

namespace rengine
{
	DEFINE_SINGLETON_CLASS(Resources,
		{
			// constructor
		},
		{
			// destructor
		},
		{
			Resources::GetInstance()->UnloadAllResource();
		});

	bool Resources::Initialize()
	{
		LoadAllAsset();

		DefaultResourceBuilder::DefaultMeshBuild();

		DefaultResourceBuilder::DefaultMaterialBuild();

		return true;
	}

	bool Resources::DeleteResource(const shared_ptr<Resource>& res)
	{
		assert(res != nullptr);

		if (res->GetIsLoad() && !res->UnLoadMemory())
			assert(false);

		switch (res->GetResourceType())
		{
			case ResourceType::MATERIAL:
			{
				m_materialMap.erase(res->GetUUID());
				break;
			}
			case ResourceType::TEXTURE:
			{
				m_textureMap.erase(res->GetUUID());
				break;
			}
			case ResourceType::MESH:
			case ResourceType::SKINNED_MESH:
			{
				m_meshMap.erase(res->GetUUID());
				break;
			}
			case ResourceType::ANIMATOR_CONTROLLER:
			{
				break;
			}
			case ResourceType::ANIMATION_CLIP:
			{
				m_animationClipMap.erase(res->GetUUID());
				break;
			}
			case ResourceType::AUDIO_CLIP:
			{
				break;
			}
			case ResourceType::UNKNOWN:
			default:
			{
				assert(false);
				return false;
			}
		}

		return true;
	}

	void Resources::LoadAllAsset()
	{
		tstring _assetPath = TEXT("..\\..\\..\\..\\Assets");

		map<tstring, vector<tstring>> _gather;

		GatherAsset(_assetPath, _gather);

		LoadAssets(_gather);

		//LoadAsset(_assetPath);
	}

	void rengine::Resources::UnloadAllResource()
	{
		for (auto& [_uuid, _mesh] : m_meshMap)
		{
			assert(_mesh->UnLoadMemory());
		}

		m_meshMap.clear();

		for (auto& [_uuid, _texture] : m_textureMap)
		{
			assert(_texture->UnLoadMemory());
		}

		m_textureMap.clear();

		for (auto& [_uuid, _material] : m_materialMap)
		{
			assert(_material->UnLoadMemory());
		}

		m_materialMap.clear();

		for (auto& [_uuid, _anim] : m_animationClipMap)
		{
			assert(_anim->UnLoadMemory());
		}

		m_animationClipMap.clear();
	}

	void rengine::Resources::GatherAsset(const tstring& path, map<tstring, vector<tstring>>& gather)
	{
		fs::path _path(path);

		if (fs::is_directory(_path))
		{
			for (auto& _p : fs::directory_iterator(_path, fs::directory_options::skip_permission_denied))
			{
				GatherAsset(_p.path().wstring(), gather);
			}
		}
		else if (_path.has_extension())
		{
			std::string _extension = _path.extension().string();

			std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

			if (_extension == ".mat")
			{
				gather[TEXT("material")].push_back(_path.wstring());
			}
			else if (_extension == ".mesh")
			{
				gather[TEXT("mesh")].push_back(_path.wstring());
			}
			else if (_extension == ".anim")
			{
				gather[TEXT("anim")].push_back(_path.wstring());
			}
			else if (_extension == ".png" || _extension == ".bmp" || _extension == ".jpeg" || _extension == ".jpg"
				|| _extension == ".dds" || _extension == ".tga" || _extension == ".hdr")
			{
				gather[TEXT("texture")].push_back(_path.wstring());
			}
			else if (_extension == ".fbx")
			{
				gather[TEXT("model")].push_back(_path.wstring());
			}
		}
	}

	void rengine::Resources::LoadAssets(map<tstring, vector<tstring>>& gather)
	{
		for (const auto& _path : gather[TEXT("mesh")])
		{
			Load<Mesh>(_path);
		}

		for (const auto& _path : gather[TEXT("anim")])
		{
			Load<AnimationClip>(_path);
		}

		for (const auto& _path : gather[TEXT("texture")])
		{
			Load<Texture>(_path);
		}

		for (const auto& _path : gather[TEXT("material")])
		{
			Load<Material>(_path);
		}

		for (const auto& _path : gather[TEXT("model")])
		{
			Load<Model>(_path);
		}
	}

	void Resources::LoadAsset(const tstring& path)
	{
		fs::path _path(path);

		if (fs::is_directory(_path))
		{
			for (auto& _p : fs::directory_iterator(_path, fs::directory_options::skip_permission_denied))
			{
				LoadAsset(_p.path().wstring());
			}
		}
		else if (_path.has_extension())
		{
			std::string _extension = _path.extension().string();

			std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

			if (_extension == ".mat")
			{
				Load<Material>(_path.wstring());
			}
			else if (_extension == ".mesh")
			{
				Load<Mesh>(_path.wstring());
			}
			else if (_extension == ".anim")
			{
				Load<AnimationClip>(_path.wstring());
			}
			else if (_extension == ".png" || _extension == ".bmp" || _extension == ".jpeg" || _extension == ".jpg"
				|| _extension == ".dds" || _extension == ".tga" || _extension == ".hdr")
			{
				Load<Texture>(_path.wstring());
			}
			else if (_extension == ".fbx")
			{
				Load<Model>(_path.wstring());
			}
		}
	}

	template<>
	shared_ptr<Mesh> Resources::Load(tstring path)
	{
		if(!CheckPathExist(path))
			return nullptr;

		auto _metaInfo = utility::Serializer::SerializeMetaInfo(path);

		if (m_meshMap.find(_metaInfo._guid) != m_meshMap.end())
			return m_meshMap[_metaInfo._guid];

		auto _obj = utility::Serializer::DeSerialize(path);
		shared_ptr<Mesh> _res;

		if(!_obj || !(_res = dynamic_pointer_cast<Mesh>(_obj)) || (_res->GetResourceType() != ResourceType::MESH && _res->GetResourceType() != ResourceType::SKINNED_MESH))
			return nullptr;

		_res->LoadMemory();

		m_meshMap.insert(make_pair(_obj->GetUUID(), _res));

		return _res;
	}

	template<>
	shared_ptr<Material> Resources::Load(tstring path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _metaInfo = utility::Serializer::SerializeMetaInfo(path);

		if (m_materialMap.find(_metaInfo._guid) != m_materialMap.end())
			return m_materialMap[_metaInfo._guid];

		auto _obj = utility::Serializer::DeSerialize(path);
		shared_ptr<Material> _res;

		if (!_obj || !(_res = dynamic_pointer_cast<Material>(_obj)) || _res->GetResourceType() != ResourceType::MATERIAL)
			return nullptr;

		_res->LoadMemory();

		m_materialMap.insert(make_pair(_obj->GetUUID(), _res));

		return _res;
	}

	template<>
	shared_ptr<Texture> Resources::Load(tstring path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _metaInfo = utility::Serializer::SerializeMetaInfo(path);

		if (m_textureMap.find(_metaInfo._guid) != m_textureMap.end())
			return m_textureMap[_metaInfo._guid];

		auto _obj = utility::Serializer::DeSerialize(path);
		shared_ptr<Texture> _res;

		if (_obj == nullptr|| (_res = dynamic_pointer_cast<Texture>(_obj)) == nullptr || _res->GetResourceType() != ResourceType::TEXTURE)
			return nullptr;

		_res->LoadMemory();

		m_textureMap.insert(make_pair(_obj->GetUUID(), _res));

		return _res;
	}

	template<>
	shared_ptr<AnimationClip> Resources::Load(tstring path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _metaInfo = utility::Serializer::SerializeMetaInfo(path);

		if(m_animationClipMap.find(_metaInfo._guid) != m_animationClipMap.end())
			return m_animationClipMap[_metaInfo._guid];

		auto _obj = utility::Serializer::DeSerialize(path);
		shared_ptr<AnimationClip> _res;

		if (_obj == nullptr || (_res = dynamic_pointer_cast<AnimationClip>(_obj)) == nullptr || _res->GetResourceType() != ResourceType::ANIMATION_CLIP)
			return nullptr;

		_res->LoadMemory();

		m_animationClipMap.insert(make_pair(_obj->GetUUID(), _res));

		return _res;
	}

	template<>
	shared_ptr<Model> Resources::Load(tstring path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _metaInfo = utility::Serializer::SerializeMetaInfo(path);

		if (m_modelMap.find(_metaInfo._guid) != m_modelMap.end())
			return m_modelMap[_metaInfo._guid];

		auto _obj = utility::Serializer::DeSerialize(path);
		shared_ptr<Model> _res;

		if (_obj == nullptr || (_res = dynamic_pointer_cast<Model>(_obj)) == nullptr || _res->GetResourceType() != ResourceType::MODEL)
			return nullptr;

		_res->LoadMemory();

		m_modelMap.insert(make_pair(_obj->GetUUID(), _res));

		return _res;
	}

	template<>
	shared_ptr<Mesh> Resources::GetResource(uuid uuid)
	{
		auto _iter = m_meshMap.find(uuid);

		if(_iter == m_meshMap.end())
			return nullptr;

		return _iter->second;
	}

	template<>
	shared_ptr<Material> Resources::GetResource(uuid uuid)
	{
		auto _iter = m_materialMap.find(uuid);

		if (_iter == m_materialMap.end())
			return nullptr;

		return _iter->second;
	}

	template<>
	shared_ptr<Texture> Resources::GetResource(uuid uuid)
	{
		auto _iter = m_textureMap.find(uuid);

		if (_iter == m_textureMap.end())
			return nullptr;

		return _iter->second;
	}

	template<>
	shared_ptr<AnimationClip> Resources::GetResource(uuid uuid)
	{
		auto _iter = m_animationClipMap.find(uuid);

		if (_iter == m_animationClipMap.end())
			return nullptr;

		return _iter->second;
	}

	template<>
	shared_ptr<Model> Resources::GetResource(uuid uuid)
	{
		auto _iter = m_modelMap.find(uuid);

		if (_iter == m_modelMap.end())
			return nullptr;

		return _iter->second;
	}

	template<>
	shared_ptr<Mesh> Resources::CreateResource()
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<Mesh>();

		assert(_ret != nullptr);

		m_meshMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Material> Resources::CreateResource()
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<Material>();

		assert(_ret != nullptr);

		m_materialMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Texture> Resources::CreateResource()
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<Texture>();

		assert(_ret != nullptr);

		m_textureMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<AnimationClip> Resources::CreateResource()
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<AnimationClip>();

		assert(_ret != nullptr);

		m_animationClipMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Model> Resources::CreateResource()
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<Model>();

		assert(_ret != nullptr);

		m_modelMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Mesh> Resources::CreateResource(uuid uuid)
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<Mesh>(uuid);

		assert(_ret != nullptr);

		m_meshMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Material> Resources::CreateResource(uuid uuid)
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<Material>(uuid);

		assert(_ret != nullptr);

		m_materialMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Texture> Resources::CreateResource(uuid uuid)
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<Texture>(uuid);

		assert(_ret != nullptr);

		m_textureMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<AnimationClip> Resources::CreateResource(uuid uuid)
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<AnimationClip>(uuid);

		assert(_ret != nullptr);

		m_animationClipMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Model> Resources::CreateResource(uuid uuid)
	{
		auto _ret = ObjectFactory::GetInstance()->CreateObject<Model>(uuid);

		assert(_ret != nullptr);

		m_modelMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Mesh> Resources::CreateResource(uuid uuid, const tstring& path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _ret = ObjectFactory::GetInstance()->CreateObject<Mesh>(uuid);

		assert(_ret != nullptr);

		_ret->SetPath(path);

		_ret->LoadMemory();

		m_meshMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Material> Resources::CreateResource(uuid uuid, const tstring& path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _ret = ObjectFactory::GetInstance()->CreateObject<Material>(uuid);

		assert(_ret != nullptr);

		_ret->SetPath(path);

		_ret->LoadMemory();

		m_materialMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Texture> Resources::CreateResource(uuid uuid, const tstring& path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _ret = ObjectFactory::GetInstance()->CreateObject<Texture>(uuid);

		assert(_ret != nullptr);

		_ret->SetPath(path);

		_ret->LoadMemory();

		m_textureMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<AnimationClip> Resources::CreateResource(uuid uuid, const tstring& path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _ret = ObjectFactory::GetInstance()->CreateObject<AnimationClip>(uuid);

		assert(_ret != nullptr);

		_ret->SetPath(path);

		_ret->LoadMemory();

		m_animationClipMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}

	template<>
	shared_ptr<Model> Resources::CreateResource(uuid uuid, const tstring& path)
	{
		if (!CheckPathExist(path))
			return nullptr;

		auto _ret = ObjectFactory::GetInstance()->CreateObject<Model>(uuid);

		assert(_ret != nullptr);

		_ret->SetPath(path);

		_ret->LoadMemory();

		m_modelMap.insert(make_pair(_ret->GetUUID(), _ret));

		return _ret;
	}
}