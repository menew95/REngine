#include <rengine\system\ObjectFactory.h>

#include <rengine\core\ComponentManager.h>

#include <rttr\type.h>

namespace rengine
{
	DEFINE_SINGLETON_CLASS(ObjectFactory,
		{

		},
		{

		},
		{

		});


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
		//Object* _object = _objVar.convert<Object*>();
		//auto _pair = make_pair(_object->GetUUID(), shared_ptr<Object>(_object));

 		m_objectsMap[_object->GetType()].insert(make_pair(_object->GetUUID(), _object));
		//m_objectsMap[_object->GetType()].insert(_pair);

		return _object;
		//return _pair.second;
	}

	void ObjectFactory::ReserveDestroyObject(shared_ptr<Object> deleteObject)
	{
		auto _delMapIter = m_reserveDestroyObjectsQueue.find(deleteObject->GetType());

		if (_delMapIter != m_reserveDestroyObjectsQueue.end()
			&& _delMapIter->second.end() == std::ranges::find_if(_delMapIter->second.begin()
			, _delMapIter->second.end(), 
			[deleteObject](auto _pair)
				{
					return true;
				}))
		{
			// 이미 삭제 대기중임
			return;
		}

		auto _mapIter = m_objectsMap.find(deleteObject->GetType());

		assert(_mapIter != m_objectsMap.end());

		auto _objIter = _mapIter->second.find(deleteObject->GetUUID());

		assert(_objIter != _mapIter->second.end());

		//auto _delMapIter = m_reserveDestroyObjectsQueue.find(deleteObject->GetType());

		m_reserveDestroyObjectsQueue[deleteObject->GetType()].push_back(make_pair(0, deleteObject));

		/*if (_delMapIter != m_reserveDestroyObjectsQueue.end())
		{
			m_reserveDestroyObjectsQueue[deleteObject->GetType()].push_back(make_pair(0, deleteObject));
		}
		else
		{
			m_reserveDestroyObjectsQueue[deleteObject->GetType()].push_back(make_pair(0, deleteObject));
		}*/
	}
}