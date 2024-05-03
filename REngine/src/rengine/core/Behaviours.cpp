#include <rengine\core\Behaviours.h>

#include <rengine\core\component\Behaviour.h>

#include <rengine\core\object\GameObject.h>

namespace rengine
{
	Behaviours::Behaviours(const tstring& name)
		: m_behaviourName(name)
	{

	}

	Behaviours::~Behaviours()
	{

	}

	void Behaviours::AddBehaviours(const std::shared_ptr<Behaviour>& addBehaviour)
	{
		m_reserveAddBehaviours.emplace_back(addBehaviour);

		addBehaviour->Awake();
	}

	void Behaviours::DeleteBehaviours(const std::shared_ptr<Behaviour>& deleteBehaviour)
	{
		deleteBehaviour->OnDestroy();

		auto _updateiter = std::ranges::find_if(std::begin(m_behaviours), std::end(m_behaviours),
			[&](auto& comp)
			{
				return deleteBehaviour == comp;
			});

		if (_updateiter != std::end(m_behaviours)) m_behaviours.erase(_updateiter);

		auto _additer = std::ranges::find_if(std::begin(m_reserveAddBehaviours), std::end(m_reserveAddBehaviours),
			[&](auto& comp)
			{
				return deleteBehaviour == comp;
			});

		if (_additer != std::end(m_reserveAddBehaviours)) m_reserveAddBehaviours.erase(_additer);
	}

	void Behaviours::StartBehaviours()
	{
		for (auto _iter = m_reserveAddBehaviours.begin(); _iter != m_reserveAddBehaviours.end();)
		{
			// 컴포넌트가 활성화가 안되었거나 게임 오브젝트가 활성화 상태가 아니라면 스킾
			if (!(*_iter)->GetEnable() && !(*_iter)->GetGameObject().lock()->GetActiveInHierarchy())
				continue;

			// Start를 호출하고 컴포넌트 Update 목록에 추가
			(*_iter)->Start();

			(*_iter)->OnEnable();

			m_behaviours.push_back((*_iter));

			// 추가 대기 목록에서 삭제
			_iter = m_reserveAddBehaviours.erase(_iter);
		}
	}
}