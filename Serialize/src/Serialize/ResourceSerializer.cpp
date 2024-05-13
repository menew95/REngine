#include <Serialize_pch.h>
#include <serialize\ResourceSerializer.h>

#include <serialize\ObjectSerializer.h>

#include <rengine\system\ObjectFactory.h>
#include <rengine\core\Resources.h>
#include <rengine\core\resource\Texture.h>
#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Material.h>
#include <rengine\core\resource\Model.h>

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

	void PropertySerialize(const rengine::MaterialProperty& prop, boost::property_tree::ptree& pt)
	{
		auto _propName = StringHelper::WStringToString(prop.GetName());

		switch (prop.GetPropType())
		{
			case rengine::MaterialProperty::PropType::Color:
			{
				math::Color _val = prop.GetColor();

				serializeConfig(_val, _propName, pt);
				break;
			}
			case rengine::MaterialProperty::PropType::Float:
			{
				pt.put(_propName, prop.GetFloat());
				break;
			}
			case rengine::MaterialProperty::PropType::Int:
			{
				pt.put(_propName, prop.GetInt());
				break;
			}
			case rengine::MaterialProperty::PropType::Range:
			{
				auto _val = prop.GetRange();

				serializeConfig(_val, _propName, pt);
				break;
			}
			case rengine::MaterialProperty::PropType::Texture:
			{
				auto _val = prop.GetTexture();

				string _uuid = _val != nullptr ? _val->GetUUIDStr() : "";

				pt.put(_propName, _uuid);

				break;
			}
			case rengine::MaterialProperty::PropType::Vector:
			{
				auto _val = prop.GetVector4();

				serializeConfig(_val, _propName, pt);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}
	}

	void MaterialSerialize(rengine::Resource* res, boost::property_tree::ptree& pt)
	{
		if (res == nullptr)
			return;

		rengine::Material* _mat = reinterpret_cast<rengine::Material*>(res);

		boost::property_tree::ptree _obj_pt;

		_mat->GetRenderPassID();

		_obj_pt.put("m_renderPassID", StringHelper::WStringToString(_mat->GetRenderPassID()));
		_obj_pt.put("m_pipelineID", StringHelper::WStringToString(_mat->GetPipelineID()));

		for (auto& _pair : _mat->GetProperties())
		{
			boost::property_tree::ptree _property_pt;

			for (auto& _prop : _pair.second)
			{
				PropertySerialize(_prop, _property_pt);
			}

			string _type;

			switch (_pair.first)
			{
				case rengine::MaterialProperty::PropType::Color:
				{
					_type = "m_Colors";
					break;
				}
				case rengine::MaterialProperty::PropType::Float:
				{
					_type = "m_Floats";
					break;
				}
				case rengine::MaterialProperty::PropType::Int:
				{
					_type = "m_Ints";
					break;
				}
				case rengine::MaterialProperty::PropType::Range:
				{
					_type = "m_Ranges";
					break;
				}
				case rengine::MaterialProperty::PropType::Texture:
				{
					_type = "m_TexEnvs";
					break;
				}
				case rengine::MaterialProperty::PropType::Vector:
				{
					_type = "m_Vectors";
					break;
				}
				default:
				{
					assert(false);
					break;
				}
			}

			_obj_pt.push_back(make_pair(_type, _property_pt));
		}

		pt.push_back(make_pair("MaterialImporter", _obj_pt));
	}

	void ModelSerialize(rengine::Resource* res, boost::property_tree::ptree& pt)
	{
		if (res == nullptr && res->GetResourceType() != rengine::ResourceType::MODEL)
			return;

		rengine::Model* _model = reinterpret_cast<rengine::Model*>(res);

		boost::property_tree::ptree _node_pt;
		boost::property_tree::ptree _mesh_pt;
		boost::property_tree::ptree _bone_pt;
		boost::property_tree::ptree _material_pt;
		boost::property_tree::ptree _anim_pt;

		for (auto& _node : _model->GetObjectNodes())
		{
			boost::property_tree::ptree _child;

			string _world = "world";

			serializeConfig<Matrix>(_node._world, _world, _child);

			_child.put("hasParent", _node._hasParent);
			_child.put("parent", StringHelper::WStringToString(_node._parent));

			_child.put("hasMesh", _node._hasMesh);
			_child.put("isSkinned", _node._hasBone);
			_child.put("mesh", StringHelper::WStringToString(_node._mesh));


			boost::property_tree::ptree _materials;

			for (auto& _material : _node._materials)
			{
				boost::property_tree::ptree _matpt;

				_matpt.put("", StringHelper::WStringToString(_material));

				_materials.push_back(std::make_pair("", _matpt));
			}

			_child.add_child("materials", _materials);

			_node_pt.push_back(std::make_pair(StringHelper::WStringToString(_node._name), _child));
		}

		pt.add_child("node", _node_pt);

		for (auto& _uuid : _model->GetMeshUUIDs())
		{
			boost::property_tree::ptree _child;

			_child.put("", StringHelper::WStringToString(_uuid));

			_mesh_pt.push_back(std::make_pair("", _child));
		}

		pt.add_child("mesh", _mesh_pt);

		for (auto& _uuid : _model->GetBoneUUIDs())
		{
			boost::property_tree::ptree _child;

			_child.put("", StringHelper::WStringToString(_uuid));

			_bone_pt.push_back(std::make_pair("", _child));
		}

		pt.add_child("bone", _bone_pt);

		for (auto& _uuid : _model->GetMaterialUUIDs())
		{
			boost::property_tree::ptree _child;

			_child.put("", StringHelper::WStringToString(_uuid));

			_material_pt.push_back(std::make_pair("", _child));
		}

		pt.add_child("material", _material_pt);

		for (auto& _uuid : _model->GetClipUUIDs())
		{
			boost::property_tree::ptree _child;

			_child.put("", StringHelper::WStringToString(_uuid));

			_anim_pt.push_back(std::make_pair("", _child));
		}

		pt.add_child("animation", _anim_pt);
	}

	void ResourceSerializer::Serialize(rengine::Object* object, boost::property_tree::ptree& pt)
	{
		shared_ptr<rengine::Resource> _object;

		rengine::Resource* _resource = reinterpret_cast<rengine::Resource*>(object);

		switch (_resource->GetResourceType())
		{
			case rengine::ResourceType::MATERIAL:
			{
				MaterialSerialize(_resource, pt);

				std::ofstream file(_resource->GetPath());

				assert(file.is_open());

				boost::property_tree::write_json(file, pt);

				file.close();

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
			case rengine::ResourceType::ANIMATOR_CONTROLLER:
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
			case rengine::ResourceType::MODEL:
			{
				ModelSerialize(_resource, pt);

				std::ofstream file(_resource->GetPath() + TEXT(".meta"));

				assert(file.is_open());

				boost::property_tree::write_json(file, pt);

				file.close();
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

	template<typename T>
	T DeserializeBinary(const tstring& path)
	{
		std::ifstream _ifs(path, std::ios_base::binary);

		assert(_ifs.is_open());

		boost::iostreams::filtering_streambuf<boost::iostreams::input> _buffer;
		_buffer.push(boost::iostreams::zlib_decompressor());
		_buffer.push(_ifs);

		T _bin;

		boost::archive::binary_iarchive _ia(_buffer);

		_ia >> _bin;

		_ifs.close();

		return _bin;
	}

	rengine::MaterialProperty MaterialPropertyDeserialize(rengine::MaterialProperty::PropType type, tstring name, boost::property_tree::ptree& pt)
	{
		switch (type)
		{
			case rengine::MaterialProperty::PropType::Color:
			{
				math::Color _val = parseConfig<math::Color>(pt);
				return rengine::MaterialProperty(name, _val);
			}
			case rengine::MaterialProperty::PropType::Float:
			{
				float _val = pt.get<float>("");

				return rengine::MaterialProperty(name, _val);
			}
			case rengine::MaterialProperty::PropType::Int:
			{
				int _val = pt.get<int>("");

				return rengine::MaterialProperty(name, _val);
			}
			case rengine::MaterialProperty::PropType::Range:
			{
				math::Vector2 _val = parseConfig<math::Vector2>(pt);

				return rengine::MaterialProperty(name, _val);
			}
			case rengine::MaterialProperty::PropType::Texture:
			{
				uuid _uuid = StringHelper::StringToWString(pt.get<string>(""));

				auto _tex = rengine::Resources::GetInstance()->GetResource<rengine::Texture>(_uuid);

				return rengine::MaterialProperty(name, _tex);
			}
			case rengine::MaterialProperty::PropType::Vector:
			{
				math::Vector4 _val = parseConfig<math::Vector4>(pt);

				return rengine::MaterialProperty(name, _val);
			}
			default:
			{
				assert(false);
				break;
			}
		}

		return rengine::MaterialProperty();
	}

	std::shared_ptr<rengine::Material> MaterialDeserialize(const rengine::MetaInfo& info, const boost::property_tree::ptree& pt)
	{
		shared_ptr<rengine::Material> _resource;

		_resource = rengine::Resources::GetInstance()->CreateResource<rengine::Material>(info._guid);

		auto _iter = pt.find("MaterialImporter");

		vector<rengine::MaterialProperty> _properties;

		for (auto& _pair : _iter->second)
		{
			rengine::MaterialProperty::PropType _type;

			if (_pair.first == "m_renderPassID")
			{
				auto _passID = StringHelper::StringToWString(_pair.second.get<string>(""));

				_resource->SetRenderPassID(_passID);

				continue;
			}
			else if (_pair.first == "m_pipelineID")
			{
				auto _pipeID = StringHelper::StringToWString(_pair.second.get<string>(""));

				_resource->SetPipelineID(_pipeID);

				continue;
			}

			if (_pair.first == "m_Colors")			_type = rengine::MaterialProperty::PropType::Color;
			else if(_pair.first == "m_Vectors")		_type = rengine::MaterialProperty::PropType::Vector;
			else if (_pair.first == "m_Floats")		_type = rengine::MaterialProperty::PropType::Float;
			else if (_pair.first == "m_Ranges")		_type = rengine::MaterialProperty::PropType::Range;
			else if (_pair.first == "m_TexEnvs")	_type = rengine::MaterialProperty::PropType::Texture;
			else if (_pair.first == "m_Ints")		_type = rengine::MaterialProperty::PropType::Int;

			_properties.reserve(_properties.size() + _pair.second.size());

			for (auto _propPair : _pair.second)
			{
				tstring _propName = StringHelper::StringToWString(_propPair.first);

				_properties.push_back(
					MaterialPropertyDeserialize(_type, _propName, _propPair.second));
			}
		}

		_resource->SetPropertyBlock(_properties);

		return _resource;
	}

	std::shared_ptr<rengine::Texture> TextureDeSerialize(const rengine::MetaInfo& info, const boost::property_tree::ptree& pt)
	{
		shared_ptr<rengine::Texture> _resource;

		auto _iter = pt.find("TextureImporter");

		_resource = rengine::Resources::GetInstance()->CreateResource<rengine::Texture>(info._guid);

		auto _node = (*_iter);
		ObjectSerializer::DeSerialize(_node, _resource);

		return _resource;
	}

	std::shared_ptr<rengine::Model> ModelDeserialize(const rengine::MetaInfo& info, const boost::property_tree::ptree& pt)
	{
		shared_ptr<rengine::Model> _resource;

		_resource = rengine::Resources::GetInstance()->CreateResource<rengine::Model>(info._guid);

		auto _iter = pt.find("node");

		bool _isSkinned = false;

		for (auto& _pair : _iter->second)
		{
			rengine::ObjectNode _objectNode;

			_objectNode._name = StringHelper::StringToWString(_pair.first);

			_objectNode._hasParent = _pair.second.get<bool>("hasParent");

			if (_objectNode._hasParent)
				_objectNode._parent = StringHelper::StringToWString(_pair.second.get<string>("parent"));

			_objectNode._hasMesh = _pair.second.get<bool>("hasMesh");

			if (_objectNode._hasMesh)
			{
				_objectNode._mesh = StringHelper::StringToWString(_pair.second.get<string>("mesh"));

				_objectNode._hasBone = _pair.second.get<bool>("isSkinned");

				if (_objectNode._hasBone) _isSkinned = true;

				for (auto& _matPair : _pair.second.get_child("materials"))
				{
					_objectNode._materials.emplace_back(StringHelper::StringToWString(_matPair.second.get<string>("")));
				}
			}

			_resource->AddNode(_objectNode);
		}

		_iter = pt.find("mesh");

		for (auto& _pair : _iter->second)
		{
			_resource->AddMeshUUID(StringHelper::StringToWString(_pair.second.get<string>("")));
		}

		if(_isSkinned)
		{
			_resource->SetIsSkinnedModel(_isSkinned);

			_iter = pt.find("bone");

			for (auto& _pair : _iter->second)
			{
				_resource->AddBoneUUID(StringHelper::StringToWString(_pair.second.get<string>("")));
			}
		}

		_iter = pt.find("material");

		for (auto& _pair : _iter->second)
		{
			_resource->AddMaterialUUID(StringHelper::StringToWString(_pair.second.get<string>("")));
		}

		_iter = pt.find("animation");

		for (auto& _pair : _iter->second)
		{
			_resource->AddClipUUID(StringHelper::StringToWString(_pair.second.get<string>("")));
		}

		return _resource;
	}

	std::shared_ptr<rengine::Object> ResourceSerializer::DeSerialize(const tstring& path, const rengine::MetaInfo& metaInfo, const boost::property_tree::ptree& pt)
	{
		std::ifstream file(path);

		shared_ptr<rengine::Resource> _object;

		if (!file.good())
			return _object;

		fs::path _path(path);

		if (!_path.has_extension())
			return _object;

		std::string _extension = _path.extension().string();

		std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

		if (_extension == ".mat")
		{
			boost::property_tree::ptree _pt;

			std::ifstream file(path);

			assert(file.good());

			try
			{
				boost::property_tree::read_json(file, _pt);

				file.close();
			}
			catch (const boost::property_tree::json_parser_error& e)
			{
				auto error = e.what();
			}

			_object = MaterialDeserialize(metaInfo, _pt);
		}
		else if (_extension == ".fbx")
		{
			boost::property_tree::ptree _pt;

			std::ifstream file(path + TEXT(".meta"));

			assert(file.good());

			try
			{
				boost::property_tree::read_json(file, _pt);

				file.close();
			}
			catch (const boost::property_tree::json_parser_error& e)
			{
				auto error = e.what();
			}

			_object = ModelDeserialize(metaInfo, _pt);
		}
		else if (_extension == ".mesh")
		{
			MeshBin _bin = DeserializeBinary<MeshBin>(path);

			auto _mesh = rengine::Resources::GetInstance()->CreateResource<rengine::Mesh>(metaInfo._guid);

			_mesh->SetNameStr(_bin._name);

			_mesh->SetVertices(_bin._vertices);
			_mesh->Setindices(_bin._indices);
			_mesh->SetBoundingBoxMin(_bin._boundingMinBox);
			_mesh->SetBoundingBoxMax(_bin._boundingMaxBox);

			_mesh->SetBoneName(StringHelper::StringToWString(_bin._boneName));
			_mesh->SetIsSkinned(_bin._isSkinned);

			_object = _mesh;
		}
		else if (_extension == ".anim")
		{
			AnimationClipBin _bin = DeserializeBinary<AnimationClipBin>(path);

			auto _clip = rengine::Resources::GetInstance()->CreateResource<rengine::AnimationClip>(metaInfo._guid);

			_clip->SetNameStr(_bin._clipName);

			_clip->SetFrameRate(_bin._frameRate);
			_clip->SetTickPerFrame(_bin._tickPerFrame);
			_clip->SetTotalKeyFrame(_bin._totalKeyFrame);
			_clip->SetStartKeyFrame(_bin._startKeyFrame);
			_clip->SetEndKeyFrame(_bin._endKeyFrame);

			vector<rengine::AnimationSnap> _snaps(_bin._snapList.size());

			for (size_t i = 0; i < _bin._snapList.size(); i++)
			{
				_snaps[i] = _bin._snapList[i].Convert();
			}

			_clip->SetSnaps(_snaps);

			_object = _clip;
		}
		else if (_extension == ".png" || _extension == ".bmp" || _extension == ".jpeg" || _extension == ".jpg"
			|| _extension == ".dds" || _extension == ".tga" || _extension == ".hdr")
		{
			_object = TextureDeSerialize(metaInfo, pt);
		}

		_object->SetName(_path.stem().wstring());

		_object->SetPath(path);

		return _object;
	}
}