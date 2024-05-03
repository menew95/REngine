/**

	@file      MonoBehaviours.h
	@brief     MonoBehaviours class
	@details   한 종류의 MonoBehaviour들을 소유하는 클래스
	@author    LWT
	@date      9.01.2024
	@copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

namespace rengine
{
	class MonoBehaviour;

	class MonoBehaviours
	{
	public:
		MonoBehaviours(const tstring& name);
		virtual ~MonoBehaviours();

		bool operator==(const tstring& name)
		{
			return m_monoBehaviourName == name;
		}

		/**
			@brief component를 추가 예약 목록에 등록하고 Awake를 호출
		**/
		virtual void AddMonoBehaviour(const std::shared_ptr<MonoBehaviour>& addMonoBehaviour);

		/**
			@brief 등록된 컴포넌트 또는 추가 대기중이던 목록에서 해당 컴포넌트를 삭제
		**/
		virtual void DeleteMonoBehaviour(const std::shared_ptr<MonoBehaviour>& deleteMonoBehaviour);

		/**
			@brief m_ReserveAddComponents에 예약된 컴포넌트가 활성화 가능하면 Start를 호출하고 컴포넌트 목록으로 이동
		**/
		virtual void StartMonoBehaviour();

		/**
			@brief m_updateMonoBehaviour를 순회하며 활성화된 컴포넌트들을 업데이트 한다.
		**/
		virtual void UpdateMonoBehaviour();

	protected:
		tstring m_monoBehaviourName;

		// 현재 업데이트 루프를 돌고있는 MonoBehaviour 리스트
		std::vector<std::shared_ptr<MonoBehaviour>> m_updateMonoBehaviour;

		// 추가되야할 MonoBehaviour 리스트
		std::list<std::shared_ptr<MonoBehaviour>> m_reserveAddMonoBehaviour;
	};
}