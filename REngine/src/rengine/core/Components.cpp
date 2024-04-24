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

	void Components::AddComponent(const std::shared_ptr<Component>& addComponent)
	{
		m_reserveAddComponents.emplace_back(addComponent);

		addComponent->Awake();

		/*if (addComponent->GetEnable() && addComponent->GetGameObject()->GetActiveInHierarchy())
		{
			addComponent->OnEnable();
		}*/
	}

	void Components::DeleteComponent(const std::shared_ptr<Component>& deleteComponent)
	{
		deleteComponent->OnDestroy();

		auto _updateiter = std::ranges::find_if(std::begin(m_updateComponents), std::end(m_updateComponents), 
			[&](auto& comp)
			{
				return deleteComponent == comp;
			});

		if(_updateiter != std::end(m_updateComponents)) m_updateComponents.erase(_updateiter);

		auto _additer = std::ranges::find_if(std::begin(m_reserveAddComponents), std::end(m_reserveAddComponents),
			[&](auto& comp)
			{
				return deleteComponent == comp;
			});

		if (_additer != std::end(m_reserveAddComponents)) m_reserveAddComponents.erase(_additer);
	}


	void Components::StartComponents()
	{
		for (auto _iter = m_reserveAddComponents.begin(); _iter != m_reserveAddComponents.end();)
		{
			// 컴포넌트가 활성화가 안되었거나 게임 오브젝트가 활성화 상태가 아니라면 스킾
			if (!(*_iter)->GetEnable() && !(*_iter)->GetGameObject().lock()->GetActiveInHierarchy())
				continue;

			// Start를 호출하고 컴포넌트 Update 목록에 추가
			(*_iter)->Start();
			m_updateComponents.push_back((*_iter));

			// 추가 대기 목록에서 삭제
			_iter = m_reserveAddComponents.erase(_iter);
		}
	}

	void Components::UpdateComponents()
	{
		for (auto& _componentIter : m_updateComponents)
		{
			if (_componentIter->GetEnable() && _componentIter->GetGameObject().lock()->GetActiveInHierarchy())
			{
				_componentIter->Update();
			}
		}

		for (auto& _componentIter : m_updateComponents)
		{
			if (_componentIter->GetEnable() && _componentIter->GetGameObject().lock()->GetActiveInHierarchy())
			{
				_componentIter->LateUpdate();
			}
		}
	}
}