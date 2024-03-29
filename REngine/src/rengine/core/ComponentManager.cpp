﻿#include <rengine\core\ComponentManager.h>

#include <rengine\core\component\Component.h>
#include <rengine\core\object\GameObject.h>

namespace rengine
{
	DEFINE_SINGLETON_CLASS(ComponentManager,
		{

		},
		{

		},
		{

		});

	void ComponentManager::ReserveAddComponent(std::shared_ptr<Component> component)
	{
		auto _iter = std::ranges::find_if(m_componentsList.begin()
			, m_componentsList.end()
			, [&component](auto& pair)
			{
				return pair.first == component->GetType();
			}
		);

		if (_iter != m_componentsList.end())
		{
			(*_iter).second.AddComponent(component);
		}
		else
		{
			auto _components = make_pair(component->GetType(), Components((uint32)m_componentsList.size(),  component->GetType()));
			
			_components.second.AddComponent(component);

			m_componentsList.push_back(_components);
		}
	}

	void ComponentManager::ReserveDeleteComponent(std::shared_ptr<Component> component)
	{
		auto _iter = std::ranges::find_if(m_componentsList.begin()
			, m_componentsList.end()
			, [&component](auto& pair)
			{
				return pair.first == component->GetType();
			}
		);

		assert(_iter != m_componentsList.end());

		component->GetGameObject().lock()->RemoveComponent(component);

		(*_iter).second.DeleteComponent(component);
	}

	void ComponentManager::UpdateComponent()
	{
		for (auto& _comps : m_componentsList)
		{
			_comps.second.StartComponents();
			_comps.second.UpdateComponents();
		}
	}

	void ComponentManager::DestoryComponent()
	{
		for (auto& _comps : m_componentsList)
		{
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