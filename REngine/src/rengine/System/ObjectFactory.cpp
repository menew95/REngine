#include <rengine\system\ObjectFactory.h>

#include <rengine\core\ComponentManager.h>

namespace rengine
{
	DEFINE_SINGLETON_CLASS(ObjectFactory,
		{

		},
		{

		},
		{

		});


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