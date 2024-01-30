#include <rengine\core\component\Renderer.h>

#include <rengine\core\resource\Material.h>

#include <rttr\registration.h>

std::vector<weak_ptr<rengine::Material>> converter_func_weak_container(const vector<weak_ptr<rengine::Object>>& value, bool& ok)
{
	ok = true;

	std::vector<weak_ptr<rengine::Material>> _ret;

	for (auto& _ptr : value)
	{
		_ret.push_back(std::static_pointer_cast<rengine::Material>(_ptr.lock()));
	}

	return _ret;
}


RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Renderer>("Renderer")
	.constructor<tstring, tstring>()
	.property("Materials", &rengine::Renderer::GetMaterials, &rengine::Renderer::SetMaterials)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	)
	;

	rttr::type::register_converter_func(converter_func_weak_container);
}

namespace rengine
{
	Renderer::Renderer(uuid uuid, tstring type)
		: Component(uuid, type)
	{

	}

	Renderer::~Renderer()
	{

	}
}