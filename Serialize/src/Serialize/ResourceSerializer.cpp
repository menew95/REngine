#include <Serialize_pch.h>
#include <serialize\ResourceSerializer.h>

#include <serialize\ObjectSerializer.h>

#include <rengine\system\ObjectFactory.h>
#include <rengine\core\Resources.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\resource\texture.h>
#include <rengine\core\resource\texture.h>
#include <rengine\core\resource\texture.h>

//#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
//#include <boost/iostreams/filtering_stream.hpp>
//#include <boost/iostreams/filter/zlib.hpp>

#include <filesystem>

namespace fs = std::filesystem;

bool ReadBinary_Mesh(const tstring& path)
{
	std::ifstream ifs(path, std::ios_base::binary);

	if (!ifs.is_open())
		return false;

	/*boost::iostreams::filtering_streambuf<boost::iostreams::input> _buffer;
	_buffer.push(boost::iostreams::zlib_decompressor());
	_buffer.push(ifs);
	boost::archive::binary_iarchive _iaMat(_buffer);

	MeshBinary _mesh_bin;

	_iaMat >> _mat_bin;*/

	return true;
}

bool ReadBinary_Anim(const tstring& path)
{
	std::ifstream ifs(path, std::ios_base::binary);

	if (!ifs.is_open())
		return false;

	/*boost::iostreams::filtering_streambuf<boost::iostreams::input> _buffer;
	_buffer.push(boost::iostreams::zlib_decompressor());
	_buffer.push(ifs);
	boost::archive::binary_iarchive _iaMat(_buffer);

	AnimBinary _anim_bin;

	_iaMat >> _anim_bin;*/

	return true;
}

void TextureSerialize(rengine::Resource* res, boost::property_tree::ptree& pt)
{
	if (res == nullptr)
		return;

	boost::property_tree::ptree _obj_pt;

	string _type = StringHelper::ToString(res->GetType());

	const rttr::type _obj_type = rttr::type::get_by_name(_type);

	for (rttr::property _prop : _obj_type.get_properties())
	{
		if (!_prop.is_valid())
			continue;

		auto _dec_ty = _prop.get_declaring_type();

		if(_dec_ty.get_name() != "Texture")
			continue;


		utility::GetProperty(_obj_pt, _prop, res);
	}

	pt.push_back(make_pair("TextureImporter", _obj_pt));
}

namespace utility
{
	void ResourceSerializer::Serialize(rengine::Object* object, boost::property_tree::ptree& pt)
	{
		shared_ptr<rengine::Resource> _object;

		rengine::Resource* _resource = reinterpret_cast<rengine::Resource*>(object);

		switch (_resource->GetResourceType())
		{
			case rengine::ResourceType::MATERIAL:
			{
				break;
			}
			case rengine::ResourceType::TEXTURE:
			{
				TextureSerialize(_resource, pt);
				//ObjectSerializer::Serialize(object, pt);
				break;
			}
			case rengine::ResourceType::MESH:
			{
				break;
			}
			case rengine::ResourceType::SKINNED_MESH:
			{
				break;
			}
			case rengine::ResourceType::ANMATOR_CONTROLLER:
			{
				break;
			}
			case rengine::ResourceType::ANIMATION_CLIP:
			{
				break;
			}
			case rengine::ResourceType::AUDIO_CLIP:
			{
				break;
			}
			case rengine::ResourceType::UNKNOWN:
			default:
			{
				assert(false);
				break;
			}
		}
	}

	std::shared_ptr<rengine::Object> TextureDeSerialize(const boost::property_tree::ptree& pt)
	{
		return nullptr;
	}

	std::shared_ptr<rengine::Object> ResourceSerializer::DeSerialize(const tstring& path, const rengine::MetaInfo& metaInfo, const boost::property_tree::ptree& pt)
	{
		std::ifstream file(path);

		if (!file.good())
			return nullptr;

		shared_ptr<rengine::Resource> _object;

		for (auto _iter = pt.begin(); _iter != pt.end(); _iter++)
		{
			if (_iter->first == "TextureImporter")
			{
				_object = rengine::Resources::GetInstance()->CreateResource<rengine::Texture>(metaInfo._guid);
				//_object _tex;

				auto _node = (*_iter);
				ObjectSerializer::DeSerialize(_node, _object);
			}
		}

		fs::path _path(path);

		_object->SetName(_path.stem().wstring());

		_object->SetPath(path);

		return _object;
	}
}