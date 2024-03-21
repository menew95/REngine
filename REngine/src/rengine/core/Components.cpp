#include <rengine\core\Components.h>

#include <rengine\core\component\Component.h>
#include <rengine\core\object\GameObject.h>

namespace rengine
{
	Components::Components(uint32 order, const tstring& componentName)
		: m_Order(order)
		, m_componentName(componentName)
	{

	}

	Components::~Components()
	{

	}

	void Components::AddComponent(std::shared_ptr<Component>& addComponent)
	{
		m_reserveAddComponents.push(addComponent);
		addComponent->Awake();

		/*if (addComponent->GetEnable() && addComponent->GetGameObject()->GetActiveInHierarchy())
		{
			addComponent->OnEnable();
		}*/
	}

	void Components::DeleteComponent(std::shared_ptr<Component>& deleteComponent)
	{
		auto _find = std::ranges::find(m_reserveDeleteComponents, deleteComponent);

		if (_find == m_reserveDeleteComponents.end())
		{
			m_reserveDeleteComponents.emplace_back(deleteComponent);
		}
	}


	void Components::StartComponents()
	{
		size_t _count = m_reserveAddComponents.size();

		for (size_t i = 0; i < _count; i++)
		{
			auto _component = m_reserveAddComponents.front();

			m_reserveAddComponents.pop();

			if (_component->GetEnable() && _component->GetGameObject().lock()->GetActiveInHierarchy())
			{
				_component->Start();
				m_components.push_back(_component);
			}
			else
			{
				m_reserveAddComponents.push(_component);
			}
		}
	}

	void Components::UpdateComponents()
	{
		for (auto& _componentIter : m_components)
		{
			if (_componentIter->GetEnable() && _componentIter->GetGameObject().lock()->GetActiveInHierarchy())
			{
				_componentIter->Update();
			}
		}

		for (auto& _componentIter : m_components)
		{
			if (_componentIter->GetEnable() && _componentIter->GetGameObject().lock()->GetActiveInHierarchy())
			{
				_componentIter->LateUpdate();
			}
		}
	}

	/*void Components::RenderComponents()
	{
		if (m_IsRender == false)
			return;

		for (auto& _componentIter : m_Components)
		{
			if (_componentIter->GetEnable() && _componentIter->GetGameObject()->GetActiveInHierarchy())
			{
				_componentIter->Render();
			}
		}
	}*/

	void Components::DestroyComponents()
	{
		for (auto& _componentIter : m_reserveDeleteComponents)
		{
			auto _find = std::ranges::find(m_components, _componentIter);

			if (_find != m_components.end())
			{
				(*_find)->OnDestroy();

				m_components.erase(_find);
			}
#ifdef _DEBUG
			else
			{
				// m_Components 안에 없다면 에러
				assert(false);
			}
#endif
		}

		m_reserveDeleteComponents.clear();
	}
}