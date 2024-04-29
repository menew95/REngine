/**

    @file      EventManager.h
    @brief     engine event manager class
    @details   엔진안에서 호출되는 이벤트들을 관리하는 클래스
    @author    LWT
    @date      29.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\singleton.h>

#include <common\Event.h>

namespace rengine
{
	enum class EventParameterType
	{
		Void
		, Bool
		, Float
		, Int
		, TString
		, Any
	};

	struct EventParameter
	{
		EventParameterType	_parameterType;

		std::any			_parameter;
	};

	struct EventMessage
	{
		tstring _eventName;

		EventParameter _eventParameter;
	};

    class EventManager
    {
        DECLARE_SINGLETON_CLASS(EventManager);

    public:
        bool Initialize();

        void DispatchEventMessages();

        void Update();

        template <typename TParam>
        void RegisterEvent(const tstring& eventName);

        template <typename TParam>
        common::EventListenerID AddEventFunction(const tstring& eventName,
            std::function<void(TParam)> functor);

        common::EventListenerID AddEventFunction(const tstring& eventName,
            std::function<void(void)> functor);

        template <typename TParam>
        bool RemoveEventFunction(const tstring& eventName, common::EventListenerID id);

        template <typename TParam>
        void SendEventMessage(const tstring& eventName, TParam eventParameter);

        void SendEventMessage(const tstring& eventName);

        template <typename TParam>
        void InvokeEvent(const tstring& eventName, TParam eventParameter);

        void InvokeEvent(const tstring& eventName);

    private:
		/**
		 * \brief 매개 변수를 받지 않는 이벤트들의 목록
		 */
		std::unordered_map<tstring, common::Event<void>>								_eventsVoid;

		/**
		 * \brief 'bool' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<tstring, common::Event<void, bool>>							_eventsBool;

		/**
		 * \brief 'int' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<tstring, common::Event<void, int>>							_eventsInt;

		/**
		 * \brief 'float' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<tstring, common::Event<void, float>>							_eventsFloat;

		/**
		 * \brief 'tstring' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<tstring, common::Event<void, const tstring&>>	_eventsTString;

		/**
		 * \brief 'std::any' 매개 변수를 받는 이벤트들의 목록
		 */
		std::unordered_map<tstring, common::Event<void, std::any>>						_eventsAny;

		/**
		 * \brief 함수의 호출 요청과 호출 시점을 분리하기 위해서 중간 매개자로 Queue를 둡니다.
		 */
		std::vector<EventMessage> _eventMessages;
    };

	template <typename TParam>
	void EventManager::InvokeEvent(const tstring& eventName, TParam eventParameter)
	{
		static_assert(std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> ||
			std::is_same_v<float, TParam> || std::is_same_v<const tstring&, TParam> || std::is_same_v<std::any, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<TParam, bool>)
		{
			if (_eventsBool.contains(eventName))
				_eventsBool.at(eventName).Invoke(eventParameter);
		}
		else if constexpr (std::is_same_v<TParam, int>)
		{
			if (_eventsInt.contains(eventName))
				_eventsInt.at(eventName).Invoke(eventParameter);
		}
		else if constexpr (std::is_same_v<TParam, float>)
		{
			if (_eventsFloat.contains(eventName))
				_eventsFloat.at(eventName).Invoke(eventParameter);
		}
		else if constexpr (std::is_same_v<TParam, const tstring&>)
		{
			if (_eventsTString.contains(eventName))
				_eventsTString.at(eventName).Invoke(eventParameter);
		}
		else if constexpr (std::is_same_v<TParam, std::any>)
		{
			if (_eventsAny.contains(eventName))
				_eventsAny.at(eventName).Invoke(eventParameter);
		}
	}

	template <typename TParam>
	void EventManager::RegisterEvent(const tstring& eventName)
	{
		static_assert(std::is_same_v<void, TParam> || std::is_same_v<bool, TParam> || std::is_same_v<int, TParam>
			|| std::is_same_v<float, TParam> || std::is_same_v<const tstring&, TParam> || std::is_same_v<std::any, TParam>, "That type isn't supported.");

		if constexpr (std::is_same_v<void, TParam>)
		{
			if (!_eventsVoid.contains(eventName))
				_eventsVoid.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<bool, TParam>)
		{
			if (!_eventsBool.contains(eventName))
				_eventsBool.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<int, TParam>)
		{
			if (!_eventsInt.contains(eventName))
				_eventsInt.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<float, TParam>)
		{
			if (!_eventsFloat.contains(eventName))
				_eventsFloat.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<const tstring&, TParam>)
		{
			if (!_eventsTString.contains(eventName))
				_eventsTString.insert({ eventName, {} });
		}
		else if constexpr (std::is_same_v<std::any, TParam>)
		{
			if (!_eventsAny.contains(eventName))
				_eventsAny.insert({ eventName, {} });
		}
	}

	template <typename TParam>
	common::EventListenerID EventManager::AddEventFunction(const tstring& eventName, std::function<void(TParam)> functor)
	{
		static_assert(std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> || std::is_same_v<float, TParam>
			|| std::is_same_v<const tstring&, TParam> || std::is_same_v<std::any, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<bool, TParam>)
		{
			if (_eventsBool.contains(eventName))
				return _eventsBool.at(eventName).AddListener(functor);
			else
			{
				_eventsBool.insert({ eventName, {} });

				return _eventsBool.at(eventName).AddListener(functor);
			}
		}
		else if constexpr (std::is_same_v<int, TParam>)
		{
			if (_eventsInt.contains(eventName))
				return _eventsInt.at(eventName).AddListener(functor);
			else
			{
				_eventsInt.insert({ eventName, {} });

				return _eventsInt.at(eventName).AddListener(functor);
			}
		}
		else if constexpr (std::is_same_v<float, TParam>)
		{
			if (_eventsFloat.contains(eventName))
				return _eventsFloat.at(eventName).AddListener(functor);
			else
			{
				_eventsFloat.insert({ eventName, {} });

				return _eventsFloat.at(eventName).AddListener(functor);
			}
		}
		else if constexpr (std::is_same_v<const tstring&, TParam>)
		{
			if (_eventsTString.contains(eventName))
				return _eventsTString.at(eventName).AddListener(functor);
			else
			{
				_eventsTString.insert({ eventName, {} });

				return _eventsTString.at(eventName).AddListener(functor);
			}
		}
		else if constexpr (std::is_same_v<std::any, TParam>)
		{
			if (_eventsAny.contains(eventName))
				return _eventsAny.at(eventName).AddListener(functor);
			else
			{
				_eventsAny.insert({ eventName, {} });

				return _eventsAny.at(eventName).AddListener(functor);
			}
		}
		else
			return false;
	}

	template <typename TParam>
	bool EventManager::RemoveEventFunction(const tstring& eventName, common::EventListenerID id)
	{
		static_assert(std::is_same_v<void, TParam> || std::is_same_v<bool, TParam> || std::is_same_v<int, TParam>
			|| std::is_same_v<TParam, float> || std::is_same_v<TParam, const tstring&> || std::is_same_v<std::any, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<void, TParam>)
			return _eventsVoid.contains(eventName) ? _eventsVoid.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<bool, TParam>)
			return _eventsBool.contains(eventName) ? _eventsBool.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<float, TParam>)
			return _eventsFloat.contains(eventName) ? _eventsFloat.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<int, TParam>)
			return _eventsInt.contains(eventName) ? _eventsInt.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<const tstring&, TParam>)
			return _eventsTString.contains(eventName) ? _eventsTString.at(eventName).RemoveListener(id) : false;
		else if constexpr (std::is_same_v<std::any, TParam>)
			return _eventsAny.contains(eventName) ? _eventsAny.at(eventName).RemoveListener(id) : false;
		else
			return false;
	}

	template <typename TParam>
	void EventManager::SendEventMessage(const tstring& eventName, TParam eventParameter)
	{
		static_assert(std::is_same_v<bool, TParam> || std::is_same_v<int, TParam> || std::is_same_v<float, TParam>
			|| std::is_same_v< const tstring&, TParam> || std::is_same_v<std::any, TParam>, "That type is not supported.");

		if constexpr (std::is_same_v<bool, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::Bool, {eventParameter}} });
		}
		else if constexpr (std::is_same_v<int, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::Int, {eventParameter}} });
		}
		else if constexpr (std::is_same_v<float, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::Float, {eventParameter}} });
		}
		else if constexpr (std::is_same_v<const tstring&, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::TString, {eventParameter}} });
		}
		else if constexpr (std::is_same_v<std::any, TParam>)
		{
			_eventMessages.push_back({ eventName, {EventParameterType::Any, {eventParameter}} });
		}
	}
}