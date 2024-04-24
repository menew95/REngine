#include <rengine\core\ComponentManager.h>

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

	void ComponentManager::ReserveAddComponent(const std::shared_ptr<Component>& component)
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

	void ComponentManager::ReserveDeleteComponent(const std::shared_ptr<Component>& component)
	{
		auto _iter = std::ranges::find_if(m_componentsList.begin()
			, m_componentsList.end()
			, [&component](auto& pair)
			{
				return pair.first == component->GetType();
			}
		);

		assert(_iter != m_componentsList.end());

		auto _gameObject = component->GetGameObject().lock();

		if (_gameObject != nullptr)
		{
			_gameObject->RemoveComponent(component);
		}

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
		/*for (auto& _comps : m_componentsList)
		{
			_comps.second.DestroyComponents();
		}*/
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

	void ComponentManager::DeleteComponent(const std::shared_ptr<Component>& component)
	{
		auto _iter = std::ranges::find_if(m_componentsList.begin()
			, m_componentsList.end()
			, [&component](auto& pair)
			{
				return pair.first == component->GetType();
			}
		);

		assert(_iter != m_componentsList.end());

		auto _gameObject = component->GetGameObject().lock();

		// 만약 아직도 게임오브젝트에 컴포넌트가 등록이 되어있다면 삭제 시켜줌
		if (_gameObject != nullptr)
		{
			_gameObject->RemoveComponent(component);
		}

		(*_iter).second.DeleteComponent(component);
	}
}