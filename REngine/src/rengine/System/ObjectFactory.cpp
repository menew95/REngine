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


	/*template<typename T, std::enable_if<std::is_base_of<Component, T>::value, T>::type>
	void ObjectFactory::ReserveComponent(shared_ptr<T> component)
	{
		ComponentManager::GetInstance()->ReserveAddComponent(component);
	}*/
}