#include <rengine\system\ObjectFactory.h>

#include <rengine\core\ComponentManager.h>
#include <rengine\core\Resources.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\scene\scene.h>

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
		for (auto _iter = m_reserveDestroyObjectsQueue.begin(); _iter != m_reserveDestroyObjectsQueue.end();)
		{
			(*_iter).first -= static_cast<float>(Time::GetDeltaTime());

			// 대기 시간이 0 밑으로 내려가면 삭제 시작
			if ((*_iter).first < 0.f)
			{
				auto _mapIter = m_objectsMap.find((*_iter).second->GetType());

				assert(_mapIter != m_objectsMap.end());

				auto _objectIter = _mapIter->second.find((*_iter).second->GetUUID());

				assert(_objectIter != _mapIter->second.end());

				// variant 에 shared_ptr이 물고 있어서 스코프를 하나 더 늘렸다.
				{
					auto _object = _objectIter->second;

					rttr::variant _var = _object;

					rttr::instance obj = _var;
					rttr::type _derived_type = obj.get_wrapped_instance().get_derived_type();

					if (_derived_type.is_derived_from(rttr::type::get_by_name("GameObject")))
					{
						auto _gameObject = static_pointer_cast<GameObject>(_object);

						if (_gameObject->GetScene() != nullptr)
						{
							_gameObject->GetScene()->RemoveGameObject(_gameObject);
						}
					}
					else if (_derived_type.is_derived_from(rttr::type::get_by_name("Component")))
					{
						auto _comp = static_pointer_cast<Component>(_object);

						ComponentManager::GetInstance()->ReserveDeleteComponent(_comp);
					}
					else if (_derived_type.is_derived_from(rttr::type::get_by_name("Resource")))
					{
						auto _resource = static_pointer_cast<Resource>(_object);

						Resources::GetInstance()->DeleteResource(_resource);
					}

				}

				// 오브젝트 맵에서 삭제
				_mapIter->second.erase(_objectIter);

				// 삭제 대기 리스트에서 삭제
				_iter = m_reserveDestroyObjectsQueue.erase(_iter);
			}
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

		// 생성한 오브젝트가 컴포넌트 일 경우에는 컴포넌트 매니저에 등록
		if (_objType.is_derived_from(rttr::type::get_by_name("Component")))
		{
			ComponentManager::GetInstance()->ReserveAddComponent(std::dynamic_pointer_cast<Component>(_object));
		}

		return _object;
	}

	void ObjectFactory::ReserveDestroyObject(const shared_ptr<Object>& deleteObject, float t)
	{
		if(deleteObject == nullptr)
			return;

		auto _iter = std::ranges::find_if(std::begin(m_reserveDestroyObjectsQueue), std::end(m_reserveDestroyObjectsQueue)
			, [&deleteObject](auto pair)
			{
				return deleteObject->GetUUID() == pair.second->GetUUID();
			});

		// 현재 이미 등록 되어있음
		if (_iter != std::end(m_reserveDestroyObjectsQueue))
		{
			// 더 적은 시간을 적용
			// 새로 들어온 시간으로 교체를 해야하는지 아니면 더 적은 시간을 적용해야하는지 잘 모르겠음
			_iter->first = _iter->first < t ? _iter->first : t;

			return;
		}

#ifdef _DEBUG
		// 오브젝트 팩토리에서 생성한적 없는 타입임
		auto _mapIter = m_objectsMap.find(deleteObject->GetType());

		assert(_mapIter != m_objectsMap.end());


		// 오브젝트 팩토리에서 생성한적 없는 오브젝트임
		auto _objIter = _mapIter->second.find(deleteObject->GetUUID());

		assert(_objIter != _mapIter->second.end());
#endif // _DEBUG

		m_reserveDestroyObjectsQueue.emplace_back(make_pair(t, deleteObject.get()));
	}

	void rengine::ObjectFactory::DestroyImmediate(const shared_ptr<Object>& deleteObject)
	{
		if(deleteObject == nullptr)
			return;

		// 만약 삭제 대기중이라면 삭제 대기 리스트에서 삭제함
		auto _reserveObjIter = std::ranges::find_if(std::begin(m_reserveDestroyObjectsQueue), std::end(m_reserveDestroyObjectsQueue),
			[&](auto& _pair)
			{
				return _pair.second->GetUUID() == deleteObject->GetUUID();
			});

		if(_reserveObjIter != std::end(m_reserveDestroyObjectsQueue))
			m_reserveDestroyObjectsQueue.erase(_reserveObjIter);

		
		auto _mapIter = m_objectsMap.find(deleteObject->GetType());

		assert(_mapIter != m_objectsMap.end());

		auto _objectIter = _mapIter->second.find(deleteObject->GetUUID());

		assert(_objectIter != _mapIter->second.end());

		// variant 에 shared_ptr이 물고 있어서 스코프를 하나 더 늘렸다.
		{
			auto _object = _objectIter->second;

			rttr::variant _var = _object;

			rttr::instance obj = _var;
			rttr::type _derived_type = obj.get_wrapped_instance().get_derived_type();

			if (_derived_type.is_derived_from(rttr::type::get_by_name("GameObject")))
			{
				auto _gameObject = static_pointer_cast<GameObject>(_object);

				if (_gameObject->GetScene() != nullptr)
				{
					_gameObject->GetScene()->RemoveGameObject(_gameObject);
				}
			}
			else if (_derived_type.is_derived_from(rttr::type::get_by_name("Component")))
			{
				auto _comp = static_pointer_cast<Component>(_object);

				ComponentManager::GetInstance()->ReserveDeleteComponent(_comp);
			}
			else if (_derived_type.is_derived_from(rttr::type::get_by_name("Resource")))
			{
				auto _resource = static_pointer_cast<Resource>(_object);

				Resources::GetInstance()->DeleteResource(_resource);
			}

		}

		// 오브젝트 맵에서 삭제
		_mapIter->second.erase(_objectIter);
		
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
		for (auto& [_type, _objMap] : m_objectsMap)
		{
			for (auto& [_uuid, _object] : _objMap)
			{
				// variant 에 shared_ptr이 물고 있어서 스코프를 하나 더 늘렸다.
				{
					rttr::variant _var = _object;

					rttr::instance obj = _var;
					rttr::type _derived_type = obj.get_wrapped_instance().get_derived_type();

					if (_derived_type.is_derived_from(rttr::type::get_by_name("GameObject")))
					{
						auto _gameObject = static_pointer_cast<GameObject>(_object);

						if (_gameObject->GetScene() != nullptr)
						{
							_gameObject->GetScene()->RemoveGameObject(_gameObject);
						}
					}
					else if (_derived_type.is_derived_from(rttr::type::get_by_name("Component")))
					{
						auto _comp = static_pointer_cast<Component>(_object);

						ComponentManager::GetInstance()->ReserveDeleteComponent(_comp);
					}
					else if (_derived_type.is_derived_from(rttr::type::get_by_name("Resource")))
					{
						auto _resource = static_pointer_cast<Resource>(_object);

						Resources::GetInstance()->DeleteResource(_resource);
					}
				}
			}

			_objMap.clear();
		}

		m_objectsMap.clear();
	}
}