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
		auto _iter = std::ranges::find_if(m_componentsList.begin()
			, m_componentsList.end()
			, [&component](auto& pair)
			{
				return pair.first == component->GetType();
			}
		);

		assert(_iter != m_componentsList.end());

		(*_iter).second.AddComponent(component);
	}

	void ComponentManager::ReserveDeleteComponent(std::shared_ptr<Component>& component)
	{
		auto _iter = std::ranges::find_if(m_componentsList.begin()
			, m_componentsList.end()
			, [&component](auto& pair)
			{
				return pair.first == component->GetType();
			}
		);

		assert(_iter != m_componentsList.end());

		(*_iter).second.DeleteComponent(component);
	}

	void ComponentManager::UpdateComponent()
	{
		for (auto& _comps : m_componentsList)
		{
			_comps.second.StartComponents();
			_comps.second.UpdateComponents();
			_comps.second.DestroyComponents();
		}
	}

	void ComponentManager::RenderComponent()
	{
		for (auto& _comps : m_renderComponentsList)
		{
			_comps.second.UpdateComponents();
		}
	}

	void ComponentManager::ClearComponentsList()
	{
		m_componentsList.clear();
	}
}