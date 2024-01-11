#include <rengine\core\Resources.h>

#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\resource\Material.h>

#include <filesystem>

#include <serialize\Serializer.h>

namespace fs = std::filesystem;

inline bool CheckPathExist(tstring& path)
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
			// release
		});

	bool Resources::Initialize()
	{
		LoadAllAsset();

		return true;
	}

	void Resources::LoadAllAsset()
	{
		tstring _assetPath = TEXT("..\\..\\..\\..\\Assets");

		LoadAssert(_assetPath);
	}

	void Resources::LoadAssert(const tstring& path)
	{
		fs::path _path(path);

		if (fs::is_directory(_path))
		{
			for (auto& _p : fs::directory_iterator(_path, fs::directory_options::skip_permission_denied))
			{
				LoadAssert(_p.path().wstring());
			}
		}
		else if (_path.has_extension())
		{
			std::string _extension = _path.extension().string();

			std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

			if (_extension == ".mat")
			{

			}
			else if (_extension == ".mesh")
			{

			}
			else if (_extension == ".anim")
			{

			}
			else if (_extension == ".png" || _extension == ".bmp" || _extension == ".jpeg" || _extension == ".jpg"
				|| _extension == ".dds" || _extension == ".tga" || _extension == ".hdr")
			{

			}
		}
	}

	template<>
	shared_ptr<Mesh> Resources::Load(tstring path)
	{
		if(CheckPathExist(path))
			return nullptr;

		Object* _obj = utility::Serializer::DeSerialize(path);
		Resource* _res;

		if(!(_obj || (_res = dynamic_cast<Resource*>(_obj)) || _res->GetResourceType() != ResourceType::MESH))
			return nullptr;

		return nullptr;
	}

	template<>
	shared_ptr<Material> Resources::Load(tstring path)
	{
		if (CheckPathExist(path))
			return nullptr;

		return nullptr;
	}

	template<>
	shared_ptr<Texture> Resources::Load(tstring path)
	{
		if (CheckPathExist(path))
			return nullptr;

		Object* _obj = utility::Serializer::DeSerialize(path);
		Resource* _res;

		if (!(_obj || (_res = dynamic_cast<Resource*>(_obj)) || _res->GetResourceType() != ResourceType::MESH))
			return nullptr;

		return nullptr;
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
}