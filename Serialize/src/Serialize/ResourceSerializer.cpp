#include <Serialize_pch.h>
#include <serialize\ResourceSerializer.h>

#include <serialize\ObjectSerializer.h>

#include <rengine\system\ObjectFactory.h>
#include <rengine\core\Resources.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Material.h>

#include <serialize\binary\AnimBin.h>
#include <serialize\binary\MaterialBin.h>
#include <serialize\binary\MeshBin.h>

#include <boost\serialization\vector.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>
#include <boost\iostreams\filtering_stream.hpp>
#include <boost\iostreams\filter\zlib.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace utility
{
	template<typename T>
	void DeserializeBinary(const tstring& path)
	{
		std::ifstream _ifs(path, std::ios_base::binary);

		if (!_ifs.is_open())
			return false;

		boost::iostreams::filtering_stream<boost::iostreams::input> _buffer;
		_buffer.push(boost::iostreams::zlib_decompressor());
		_buffer.push(_ifs);
		boost::archive::binary_iarchive _ia(_buffer);

		T _bin;
		_ia >> _bin;

		return true;
	}

	template<typename T>
	void SerializeBinary(const tstring& path, T& resource)
	{
		std::ofstream _ofs(path, std::ios_base::binary);
		boost::iostreams::filtering_stream<boost::iostreams::output> _buffer;
		_buffer.push(boost::iostreams::zlib_compressor());
		_buffer.push(_ofs);

		boost::archive::binary_oarchive _oa(_buffer);
		_oa << resource;
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

			if (_dec_ty.get_name() != "Texture")
				continue;

			GetProperty(_obj_pt, _prop, res);
		}

		pt.push_back(make_pair("TextureImporter", _obj_pt));
	}

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
				break;
			}
			case rengine::ResourceType::MESH:
			case rengine::ResourceType::SKINNED_MESH:
			{
				rengine::Mesh* _mesh = reinterpret_cast<rengine::Mesh*>(object);

				MeshBin _bin;

				_bin._name = _mesh->GetNameStr();

				_bin._vertices = _mesh->GetVertices();
				_bin._indices = _mesh->Getindices();
				_bin._boundingMinBox = _mesh->GetBoundingBoxMin();
				_bin._boundingMaxBox = _mesh->GetBoundingBoxMax();

				_bin._boneName = StringHelper::WStringToString(_mesh->GetBoneName());
				_bin._isSkinned = _mesh->GetIsSkinned();

				SerializeBinary<MeshBin>(_resource->GetPath(), _bin);
				break;
			}
			case rengine::ResourceType::ANMATOR_CONTROLLER:
			{
				break;
			}
			case rengine::ResourceType::ANIMATION_CLIP:
			{
				rengine::AnimationClip* _clip = reinterpret_cast<rengine::AnimationClip*>(object);

				AnimationClipBin _bin;

				_bin._clipName = _clip->GetNameStr();

				_bin._frameRate = _clip->GetFrameRate();
				_bin._tickPerFrame = _clip->GetTickPerFrame();
				_bin._totalKeyFrame = _clip->GetTotalKeyFrame();
				_bin._startKeyFrame = _clip->GetStartKeyFrame();
				_bin._endKeyFrame = _clip->GetEndKeyFrame();

				_bin._snapList.resize(_clip->GetSnaps().size());

				for (size_t i = 0; i < _bin._snapList.size(); i++)
				{
					_bin._snapList[i] = _clip->GetSnaps()[i];
				}

				SerializeBinary<AnimationClipBin>(_resource->GetPath(), _bin);

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