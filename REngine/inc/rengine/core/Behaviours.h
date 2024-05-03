/**

	@file      Components.h
	@brief     components class
	@details   한 종류의 컴포넌트들을 소유하는 클래스
	@author    LWT
	@date      9.01.2024
	@copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

namespace rengine
{
	class Behaviour;

	class Behaviours
	{
	public:
		Behaviours(const tstring& name);
		virtual ~Behaviours();

		bool operator==(const tstring& name)
		{
			return m_behaviourName == name;
		}

		/**
			@brief Behaviour를 추가 예약 목록에 등록하고 Awake를 호출
		**/
		virtual void AddBehaviours(const std::shared_ptr<Behaviour>& addBehaviour);

		/**
			@brief 등록된 Behaviour 또는 추가 대기중이던 목록에서 해당 Behaviour를 삭제
		**/
		virtual void DeleteBehaviours(const std::shared_ptr<Behaviour>& deleteBehaviour);

		/**
			@brief m_reserveAddBehaviours에 예약된 Behaviour가 활성화 가능하면 Start를 호출하고 Behaviour 목록으로 이동
		**/
		virtual void StartBehaviours();

	protected:

		tstring m_behaviourName;

		// 현재 활성화된 Behaviour 리스트
		std::vector<std::shared_ptr<Behaviour>> m_behaviours;

		// 추가되야할 Behaviour 리스트
		std::list<std::shared_ptr<Behaviour>> m_reserveAddBehaviours;
	};
}