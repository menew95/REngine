#include <Importer_pch.h>
#include <importer\Importer.h>

#include <importer\FBXImporter.h>

#include <rengine\core\resource\Material.h>
#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\resource\AnimationClip.h>
#include <rengine\core\Resources.h>

#include <serialize\Serializer.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace utility
{
	shared_ptr<rengine::Object> Importer::Import(const tstring& path)
	{
		fs::path _path(path);

		if(!fs::exists(_path) || !_path.has_extension())
			return nullptr;

		auto _extension = _path.extension().string();

		std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

		shared_ptr<rengine::Object> _object;

		if (_extension == ".fbx")
		{
			FBXImporter::Import(path);
		}
		else if (_extension == ".mat")
		{
			// 해당 경로에 있는 파일을 역직렬화해야함
			assert(false);

			auto _importMat = rengine::Resources::GetInstance()->CreateResource<rengine::Material>();

			_importMat->SetPath(path);

			_object = _importMat;

		}
		else if (_extension == ".mesh")
		{
			// 해당 경로에 있는 파일을 역직렬화해야함
			assert(false);
			auto _importMesh = rengine::Resources::GetInstance()->CreateResource<rengine::Mesh>();

			_importMesh->SetPath(path);

			_object = _importMesh;
		}
		else if (_extension == ".anim")
		{
			// 해당 경로에 있는 파일을 역직렬화해야함
			assert(false);
			auto _importClip = rengine::Resources::GetInstance()->CreateResource<rengine::AnimationClip>();

			_importClip->SetPath(path);

			_object = _importClip;
		}
		else if (_extension == ".png" || _extension == ".bmp" || _extension == ".jpeg" || _extension == ".jpg"
			|| _extension == ".dds" || _extension == ".tga" || _extension == ".hdr")
		{
			// 해당 경로에 있는 파일을 역직렬화해야함
			assert(false);

			auto _importTex = rengine::Resources::GetInstance()->CreateResource<rengine::Texture>();

			_importTex->SetPath(path);

			_object = _importTex;
		}

		if (_object != nullptr)
			utility::Serializer::CreateMetaInfo(path, _object.get());

		return _object;
	}
}
