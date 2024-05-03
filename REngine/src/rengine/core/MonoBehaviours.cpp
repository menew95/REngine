#include <rengine\core\MonoBehaviours.h>
#include <rengine\core\component\MonoBehaviour.h>
#include <rengine\core\object\GameObject.h>

namespace rengine
{
	MonoBehaviours::MonoBehaviours(const tstring& name)
	: m_monoBehaviourName(name)
	{

	}

	MonoBehaviours::~MonoBehaviours()
	{

	}

	void MonoBehaviours::AddMonoBehaviour(const std::shared_ptr<MonoBehaviour>& addMonoBehaviour)
	{	
		auto _iter = std::ranges::find_if(m_reserveAddMonoBehaviour,
			[&](auto& mono)
			{
				return mono->GetUUID() == addMonoBehaviour->GetUUID();
			});

		if(_iter != std::end(m_reserveAddMonoBehaviour))
			return;

		m_reserveAddMonoBehaviour.emplace_back(addMonoBehaviour);

		addMonoBehaviour->Awake();
	}

	void MonoBehaviours::DeleteMonoBehaviour(const std::shared_ptr<MonoBehaviour>& deleteMonoBehaviour)
	{
		deleteMonoBehaviour->OnDestroy();

		std::erase_if(m_updateMonoBehaviour, [&](auto& mono)
			{
				return mono->GetUUID() == deleteMonoBehaviour->GetUUID();
			});

		std::erase_if(m_reserveAddMonoBehaviour, [&](auto& mono)
			{
				return mono->GetUUID() == deleteMonoBehaviour->GetUUID();
			});
	}

	void MonoBehaviours::StartMonoBehaviour()
	{
		for (auto _iter = m_reserveAddMonoBehaviour.begin(); _iter != m_reserveAddMonoBehaviour.end();)
		{
			// 컴포넌트가 활성화가 안되었거나 게임 오브젝트가 활성화 상태가 아니라면 스킾
			if (!(*_iter)->GetEnable() && !(*_iter)->GetGameObject().lock()->GetActiveInHierarchy())
				continue;

			// Start를 호출하고 컴포넌트 Update 목록에 추가
			(*_iter)->Start();

			(*_iter)->OnEnable();

			m_updateMonoBehaviour.push_back((*_iter));

			// 추가 대기 목록에서 삭제
			_iter = m_reserveAddMonoBehaviour.erase(_iter);
		}
	}

	void MonoBehaviours::UpdateMonoBehaviour()
	{
		for (auto& _componentIter : m_updateMonoBehaviour)
		{
			if (_componentIter->GetEnable() && _componentIter->GetGameObject().lock()->GetActiveInHierarchy())
			{
				_componentIter->Update();
			}
		}

		for (auto& _componentIter : m_updateMonoBehaviour)
		{
			if (_componentIter->GetEnable() && _componentIter->GetGameObject().lock()->GetActiveInHierarchy())
			{
				_componentIter->LateUpdate();
			}
		}
	}
}
