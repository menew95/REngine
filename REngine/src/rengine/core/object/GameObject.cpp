﻿#include <rengine\core\object\GameObject.h>
#include <rengine\core\scene\scene.h>
#include <rengine\core\component\Transform.h>

#include <rengine\System\ObjectFactory.h>

#include <rttr\registration.h>

#include <rengine\core\sceneManager.h>

std::weak_ptr<rengine::GameObject> converter_func(const weak_ptr<rengine::Object>& value, bool& ok)
{
	ok = true;

	std::weak_ptr<rengine::GameObject> _ret = std::static_pointer_cast<rengine::GameObject>(value.lock());

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::GameObject>("GameObject")
	.constructor<const uuid&>()
	.property("Active Self", &rengine::GameObject::m_bActiveSelf, rttr::detail::private_access())
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::BOOL),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL)
	)
	.property("Tag", &rengine::GameObject::GetTag, &rengine::GameObject::SetTag)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::WSTRING),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::WSTRING)
	)
	.property("Layer", &rengine::GameObject::GetLayer, &rengine::GameObject::SetLayer)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::UINT32),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	.property("Static", &rengine::GameObject::GetIsStatic, &rengine::GameObject::SetIsStatic)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::BOOL),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL)
	)
	.property("Components", &rengine::GameObject::GetComponents, &rengine::GameObject::SetComponents)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	);

	rttr::type::register_converter_func(converter_func);
}

namespace rengine
{
	RENGINE_API shared_ptr<GameObject> GameObject::Instantiate(shared_ptr<Transform> parent)
	{
		//auto _newGO = make_shared<GameObject>();
		
		auto _newGO = ObjectFactory::GetInstance()->CreateObject<GameObject>();

		rengine::SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(_newGO);

		auto _trans = _newGO->AddComponent<Transform>();

		_newGO->SetTransform(_trans);

		if(parent != nullptr)
		{
			_trans->SetParent(parent);
		}
		else
		{
			rengine::SceneManager::GetInstance()->GetCurrentScene()->AddRootGameObject(_newGO);
		}

		return _newGO;
	}

	GameObject::GameObject(const uuid& uuid)
		: Object(uuid, TEXT("Game Object"), TEXT("GameObject"))
	{
	}

	GameObject::~GameObject()
	{
		PreDestroy();
	}

	void GameObject::AddComponent(string type)
	{
		// 이미 소유한 컴포넌트를 다시 추가하지 못하게 막음
		for (auto& _weak : m_Components)
		{
			auto _comp = _weak.lock();

			if(_comp == nullptr)
				continue;

			if(_comp->GetTypeStr() == type)
				return;
		}

		auto _obj = ObjectFactory::GetInstance()->CreateObject(type, UUIDGenerator::Generate());

		assert(_obj != nullptr);

		auto _newComponent = static_pointer_cast<Component>(_obj);

		assert(_newComponent != nullptr);

		auto _this = shared_from_this();

		_newComponent->SetGameObject(_this);

		// 위치가 이게 맞는지 모르겠다.
		//_newComponent->SetEnable(true);

		m_Components.emplace_back(_newComponent);
	}

	void GameObject::PreDestroy()
	{
		// 등록된 컴포넌트들을 삭제
		for (auto& _comp : m_Components)
		{
			DestroyImmediate(_comp.lock());

			_comp.reset();
		}

		m_pTransform.reset();


		/*if(m_pScene.lock() != nullptr)
			m_pScene.lock()->RemoveGameObject(shared_from_this());*/
	}

	bool GameObject::RemoveComponent(tstring type)
	{
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			if (type == m_Components[i].lock()->GetType())
			{
				m_Components.erase(m_Components.begin() + i);

				return true;
			}
		}

		return false;
	}

	void GameObject::RemoveComponent(const shared_ptr<Component>& component)
	{
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			if (component == m_Components[i].lock())
			{
				m_Components.erase(m_Components.begin() + i);

				return;
			}
		}
	}

	void GameObject::RemoveComponent(Component* compoent)
	{
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			if (compoent->GetUUID() == m_Components[i].lock()->GetUUID())
			{
				m_Components.erase(m_Components.begin() + i);

				return;
			}
		}
	}

	void GameObject::DestroyGameObject()
	{
		// 자식 게임오브젝트 먼저 삭제 예약을 한다음 소유하고있는 컴포넌트를 삭제 예약
		for (auto& _ptr : m_pTransform.lock()->GetChilds())
		{
			auto _childs = _ptr.lock();

			Destroy(_childs->GetGameObject().lock());
		}

		for (auto& _ptr : m_Components)
		{
			auto _comp = _ptr.lock();
			if (_comp == nullptr)
				continue;

			Destroy(_comp);
		}
	}

	/*inline void GameObject::SetScene(const std::shared_ptr<Scene>& scene)
	{
		auto _scene = m_pScene.lock();

		if (_scene != nullptr)
		{
			_scene->RemoveGameObject(shared_from_this());
		}
		
		if (scene != nullptr)
		{
			scene->AddGameObject(shared_from_this());
		}

		m_pScene = scene;
	}*/

	void GameObject::SetComponents(std::vector<std::weak_ptr<Component>> comps)
	{
		m_Components = comps;

		for(auto& _comp : m_Components)
		{
			if (auto _ptr = _comp.lock())
			{
				_ptr->SetGameObject(shared_from_this());

				if (_ptr->GetType() == TEXT("Transform"))
				{
					SetTransform(std::static_pointer_cast<Transform>(_ptr));
				}
			}
		}
	}

	inline void GameObject::SetActive(bool val)
	{
		auto _trans = m_pTransform.lock();

		auto _parentTrans = _trans->GetParent();

		m_bActiveSelf = val;

		if (_parentTrans != nullptr)
		{
			// 
			m_bActiveInHierarchy = _parentTrans->GetGameObject().lock()->GetActiveInHierarchy();
			
			auto _trans = m_pTransform.lock();

			for (auto i = 0u; i < _trans->GetChildSize(); i++)
			{
				auto _go = _trans->GetChild(i)->GetGameObject().lock();

				if (_go == nullptr)
					continue;

				_go->SetActive(val);
			}
		}
		else
		{
			// 부모 게임 오브젝트가 없을 경우 바로 계층 구조에서 활성화 상태를 바꾸고 자식에게 적용
			m_bActiveInHierarchy = val;

			auto _trans = m_pTransform.lock();

			for (auto i = 0u; i < _trans->GetChildSize(); i++)
			{
				auto _go = _trans->GetChild(i)->GetGameObject().lock();

				if (_go == nullptr)
					continue;

				_go->SetActive(val);
			}
		}
	}
}