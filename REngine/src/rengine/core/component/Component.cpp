﻿#include <rengine\core\component\Component.h>

#include <rengine\core\object\GameObject.h>

#include <rttr\registration.h>

std::weak_ptr<rengine::Component> converter_func(const weak_ptr<rengine::Object>& value, bool& ok)
{
	ok = true;

	std::weak_ptr<rengine::Component> _ret = std::static_pointer_cast<rengine::Component>(value.lock());

	return _ret;
}

std::vector<weak_ptr<rengine::Component>> converter_func_container(const vector<weak_ptr<rengine::Object>>& value, bool& ok)
{
	ok = true;

	std::vector<weak_ptr<rengine::Component>> _ret;

	for (auto& _ptr : value)
	{
		_ret.push_back(std::static_pointer_cast<rengine::Component>(_ptr.lock()));
	}

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Component>("Component")
	.constructor<const uuid&>()
	.property("m_bEnable", &rengine::Component::GetEnable, &rengine::Component::SetEnable)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL),
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::BOOL)
	)
	.property("m_pGameObject", &rengine::Component::GetGameObject, &rengine::Component::SetGameObject)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	);

	rttr::type::register_converter_func(converter_func);
	rttr::type::register_converter_func(converter_func_container);
}

namespace rengine
{
	Component::Component(const uuid& uuid, const tstring& type)
	: Object(uuid, type, type)
	{

	}

	Component::~Component()
	{
		PreDestroy();
	}

	std::shared_ptr<Transform> Component::GetTransform()
	{
		if (auto _go = m_pGameObject.lock())
		{
			return _go->GetTransform();
		}

		return nullptr;
	}

	void Component::SetEnable(bool value)
	{
		if (m_pGameObject.lock() && m_pGameObject.lock()->GetActiveInHierarchy() && m_bEnable != value)
		{
			m_bEnable = value;

			if (m_bEnable)
			{
				OnEnable();
			}
			else
			{
				OnDisable();
			}
		}
	}

	void Component::PreDestroy()
	{
		// 게임 오브젝트에 등록되어 있을 경우 게임오브젝트 목록으로 부터 삭제
		auto _gameObject = m_pGameObject.lock();

		if (_gameObject != nullptr)
		{
			_gameObject->RemoveComponent(this);
		}
	}
}