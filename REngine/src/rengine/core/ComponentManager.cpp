#include <rengine\core\ComponentManager.h>

#include <rengine\core\component\Component.h>

namespace rengine
{
	DEFINE_SINGLETON_CLASS(ComponentManager,
		{

		},
		{

		},
		{

		});

	void ComponentManager::ReserveAddComponent(std::shared_ptr<Component>& component)
	{
		auto _iter = std::ranges::find_if(m_ComponentsList.begin()
			, m_ComponentsList.end()
			, [&component](auto& pair)
			{
				return pair.first == component->GetType();
			}
		);

		if (_iter != m_ComponentsList.end())
		{
			(*_iter).second.AddComponent(component);
		}
		else
		{
			assert(false);
		}
	}

}