#include <Serialize_pch.h>
#include <serialize\ResourceSerializer.h>

#include <serialize\ObjectSerializer.h>

#include <rengine\system\ObjectFactory.h>
#include <rengine\core\Resources.h>

//#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
//#include <boost/iostreams/filtering_stream.hpp>
//#include <boost/iostreams/filter/zlib.hpp>

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

namespace utility
{
	void ResourceSerializer::Serialize(const rengine::Object* object, boost::property_tree::ptree& pt)
	{

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

		shared_ptr<rengine::Object> _object;

		for (auto _iter = pt.begin(); _iter != pt.end(); _iter++)
		{
			if (_iter->first == "TextureImporter")
			{
				auto _node = (*_iter);
				ObjectSerializer::DeSerialize(_node, _object);
			}
		}

		return nullptr;
	}
}