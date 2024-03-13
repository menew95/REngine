#include <Editor_pch.h>
#include <editor\Core\AssetManager.h>

#include <common\AssetPath.h>

#include <importer\Importer.h>

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
		Refresh();
	}
	
	void AssetManager::CreateAsset(rengine::Object* objec, const tstring& path)
	{
	
	}
	
	void AssetManager::ImportAsset(const tstring& path)
	{
		utility::Importer::Import(path);

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
}