#include <rengine\system\ObjectFactory.h>

#include <rengine\core\ComponentManager.h>
#include <rengine\core\Resources.h>
#include <rengine\core\object\GameObject.h>

#include <rengine\System\Time.h>

#include <rttr\type.h>

namespace rengine
{
	DEFINE_SINGLETON_CLASS(ObjectFactory,
		{

		},
		{

		},
		{
			ObjectFactory::GetInstance()->DeleteAllObjects();
		});


	void ObjectFactory::Update()
	{
		for (auto& _pair : m_reserveDestroyObjectsQueue)
		{
			if(_pair.first == TEXT("GameObject"))	continue;
			
			for (auto _iter = _pair.second.begin(); _iter != _pair.second.end(); )
			{
				_iter->first -= static_cast<float>(Time::GetDeltaTime());
				
				if (_iter->first < 0.001f)
				{
					rttr::variant _var = _iter->second;

					rttr::instance obj = _var;
					rttr::type _derived_type = obj.get_wrapped_instance().get_derived_type();

					if (_derived_type.is_derived_from(rttr::type::get_by_name("Component")))
					{
						auto _comp = static_pointer_cast<Component>(_iter->second);

						ComponentManager::GetInstance()->ReserveDeleteComponent(_comp);
					}
					else if(_derived_type.is_derived_from(rttr::type::get_by_name("Resource")))
					{
						auto _resource = static_pointer_cast<Resource>(_iter->second);

						Resources::GetInstance()->DeleteResource(_resource);
					}

					m_objectsMap[_iter->second->GetType()].erase(_iter->second->GetUUID());

					_iter = _pair.second.erase(_iter);
				}
				else _iter++;
			}
		}

		// 게임 오브젝트는 가장 나중에 파괴가 되어야한다. 그래야 OnDestroy에서 GameObject를 사용 할 수가 있다.
		// 구조 개선이 필요해보임 일단은 이대로 진행
		ComponentManager::GetInstance()->DestoryComponent();

		auto _reserveDestoryGO = m_reserveDestroyObjectsQueue[TEXT("GameObject")];

		for (auto _iter = _reserveDestoryGO.begin(); _iter != _reserveDestoryGO.end(); )
		{
			_iter->first -= static_cast<float>(Time::GetDeltaTime());

			if (_iter->first < 0.001f)
			{
				m_objectsMap[_iter->second->GetType()].erase(_iter->second->GetUUID());

				_iter = _reserveDestoryGO.erase(_iter);
			}
			else _iter++;
		}
	}

	shared_ptr<Object> ObjectFactory::CreateObject(string type, uuid _uuid)
	{
		rttr::type _objType = rttr::type::get_by_name(type);

		if (!_objType.is_valid())
			return nullptr;

		rttr::constructor _ctor = _objType.get_constructor({ rttr::type::get<uuid>() });

		if (!_ctor.is_valid())
			return nullptr;

		rttr::variant _objVar = _ctor.invoke(_uuid);

		if (!_objVar.is_valid() && _objVar.can_convert<shared_ptr<Object>>())
			return nullptr;

		shared_ptr<Object> _object = _objVar.get_value<shared_ptr<Object>>();

		assert(_object != nullptr);

 		m_objectsMap[_object->GetType()].insert(make_pair(_object->GetUUID(), _object));

		if (_objType.is_derived_from(rttr::type::get_by_name("Component")))
		{
			ComponentManager::GetInstance()->ReserveAddComponent(std::dynamic_pointer_cast<Component>(_object));
		}

		return _object;
	}

	void ObjectFactory::ReserveDestroyObject(shared_ptr<Object> deleteObject, float t)
	{
		auto _delMapIter = m_reserveDestroyObjectsQueue.find(deleteObject->GetType());

		if (_delMapIter != m_reserveDestroyObjectsQueue.end()
			&& _delMapIter->second.end() != std::ranges::find_if(_delMapIter->second.begin()
			, _delMapIter->second.end(), 
			[deleteObject](auto _pair)
				{
					return _pair.second == deleteObject;
				}))
		{
			// 이미 삭제 대기중임
			return;
		}

		auto _mapIter = m_objectsMap.find(deleteObject->GetType());

		assert(_mapIter != m_objectsMap.end());

		auto _objIter = _mapIter->second.find(deleteObject->GetUUID());

		assert(_objIter != _mapIter->second.end());

		if (deleteObject->GetType() == TEXT("GameObject"))
		{
			reinterpret_cast<GameObject*>(deleteObject.get())->DestroyGameObject();
		}

		m_reserveDestroyObjectsQueue[deleteObject->GetType()].push_back(make_pair(t, deleteObject));

		/*auto _delMapIter = m_reserveDestroyObjectsQueue.find(deleteObject->GetType());

		if (_delMapIter != m_reserveDestroyObjectsQueue.end())
		{
			m_reserveDestroyObjectsQueue[deleteObject->GetType()].push_back(make_pair(0, deleteObject));
		}
		else
		{
			m_reserveDestroyObjectsQueue[deleteObject->GetType()].push_back(make_pair(0, deleteObject));
		}*/
	}

	shared_ptr<Object> ObjectFactory::Find(uuid uuid)
	{
		for (auto _objMap : m_objectsMap)
		{
			auto _iter = _objMap.second.find(uuid);

			if (_iter != _objMap.second.end())
			{
				return _iter->second;
			}
		}

		return nullptr;
	}
	
	void ObjectFactory::DeleteAllObjects()
	{
		for (auto& _pair : m_objectsMap)
		{
			for (auto& _objPair : _pair.second)
			{
				rttr::variant _var = _objPair.second;

				rttr::type _objType = _var.get_type();

				if (_objType.is_derived_from(rttr::type::get_by_name("Component")))
				{
					auto _comp = static_pointer_cast<Component>(_objPair.second);

					ComponentManager::GetInstance()->ReserveDeleteComponent(_comp);
				}
				else if (_objType.is_derived_from(rttr::type::get_by_name("Resource")))
				{
					auto _resource = static_pointer_cast<Resource>(_objPair.second);

					Resources::GetInstance()->DeleteResource(_resource);
				}

				m_objectsMap[_objPair.second->GetType()].erase(_objPair.second->GetUUID());
			}
		}
	}
}