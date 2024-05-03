#include <rengine\core\EventManager.h>

#include <common\Event.h>

namespace rengine
{
	using namespace common;

	DEFINE_SINGLETON_CLASS(EventManager,
		{

		},
		{

		},
		{
			Instance.UnInitialize();
		});

	void EventManager::InvokeEvent(const tstring& eventName)
	{
		if (_eventsVoid.contains(eventName))
			_eventsVoid.at(eventName).Invoke();
	}

	EventListenerID EventManager::AddEventFunction(const tstring& eventName,
		std::function<void(void)> functor)
	{
		if (_eventsVoid.contains(eventName))
			return _eventsVoid.at(eventName).AddListener(functor);
		else
		{
			_eventsVoid.insert({ eventName, {} });

			return _eventsVoid.at(eventName).AddListener(functor);
		}
	}

	void EventManager::SendEventMessage(const tstring& eventName)
	{
		_eventMessages.push_back({ eventName, {EventParameterType::Void, {}} });
	}

	void EventManager::Update()
	{
		DispatchEventMessages();
	}

	void EventManager::Initialize()
	{
		// 게임 엔진에서 사용할 이벤트들을 미리 등록합니다.

		// 'std::any' 타입에 대한 Resize event register. => 실제 전달 인자는 DUOLMath::Vector2*
		RegisterEvent<std::any>(TEXT("Resize"));

		// 'void' 타입에 대한 SceneRendering event register.
		RegisterEvent<void>(TEXT("SceneRendering"));

		// 'void' 타입에 대한 SceneLighting event register.
		RegisterEvent<void>(TEXT("SceneLighting"));

		// 'void' 타입에 대한 SceneChanging event register.
		RegisterEvent<void>(TEXT("SceneChanging"));

#pragma region 어떻게 분리할 방법이 .. 없는 것 같다.
		// 'void' 타입에 대한 SceneEditModeUpdating event register.
		RegisterEvent<void>(TEXT("SceneEditModeUpdating"));
#pragma endregion

		// 'void' 타입에 대한 NavMeshChanging event register.
		RegisterEvent<void>(TEXT("NavMeshChanging"));
	}

	void EventManager::UnInitialize()
	{
		// 등록된 이벤트들을 모두 메모리에서 해제합니다.
		_eventsVoid.clear();

		_eventsBool.clear();

		_eventsInt.clear();

		_eventsFloat.clear();

		_eventsTString.clear();

		_eventsAny.clear();

		_eventMessages.clear();
	}

	void EventManager::DispatchEventMessages()
	{
		for (auto& [eventName, eventParameter] : _eventMessages)
		{
			switch (eventParameter._parameterType)
			{
			case EventParameterType::Void:
			{
				InvokeEvent(eventName);

				break;
			}

			case EventParameterType::Bool:
			{
				InvokeEvent(eventName, std::any_cast<bool>(eventParameter._parameter));

				break;
			}

			case EventParameterType::Int:
			{
				InvokeEvent(eventName, std::any_cast<int>(eventParameter._parameter));

				break;
			}

			case EventParameterType::Float:
			{
				InvokeEvent(eventName, std::any_cast<float>(eventParameter._parameter));

				break;
			}

			case EventParameterType::TString:
			{
				InvokeEvent<const tstring&>(eventName, std::any_cast<const tstring&>(eventParameter._parameter));

				break;
			}

			case EventParameterType::Any:
			{
				InvokeEvent(eventName, eventParameter._parameter);

				break;
			}
			}
		}

		_eventMessages.clear();
	}
}