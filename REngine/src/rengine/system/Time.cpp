#include <rengine\system\Time.h>

namespace rengine
{
	DEFINE_SINGLETON_CLASS(Time, 
		{

		},
		{

		},
		{

		})

	void Time::Initialize()
	{
		m_startGameTime = std::chrono::system_clock::now();
		m_lastPreTickTime = m_startGameTime;
		m_currentPreTickTime = m_startGameTime;
		m_gameTimeNanos = 0;
		m_deltaTimeNanos = 0;
	}

	void Time::PreTick()
	{
		m_lastPreTickTime = m_currentPreTickTime;

		m_currentPreTickTime = std::chrono::system_clock::now();

		// DeltaTime을 구합니다.
		m_deltaTimeNanos =
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				m_currentPreTickTime - m_lastPreTickTime).count();

		// TODO: 디버깅의 편의를 위해, 한 프레임간의 시간 간격이 매우 길어질 때, delta time을 그대로 사용하지 않고 clamp합니다.
		// 이를 20ms 으로 설정했습니다. 게임의 프레임 루프 횟수가 최소 50fps 이상으로 나오지 않으면 이상해집니다.
		constexpr long long CLAMP_MAX_DELTA_TIME_NANOS = SECOND_TO_NANOS / 50;
		constexpr long long CLAMP_MAX_DELTA_TIME_MILLIS = static_cast<long long>(
			CLAMP_MAX_DELTA_TIME_NANOS * NANOS_TO_MILLIS
			);

		if (m_deltaTimeNanos > CLAMP_MAX_DELTA_TIME_NANOS)
		{
			m_deltaTimeNanos = CLAMP_MAX_DELTA_TIME_MILLIS;

			const auto _startDelayTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
				m_lastPreTickTime + std::chrono::nanoseconds(CLAMP_MAX_DELTA_TIME_NANOS)
			);

			const auto _delayedDuration = m_currentPreTickTime - _startDelayTime;

			m_elapsedDelayedTimeNanos = m_elapsedDelayedTimeNanos + _delayedDuration.count();
		}

		m_gameTimeNanos =
			std::chrono::duration_cast<std::chrono::nanoseconds>(m_currentPreTickTime - m_startGameTime).count() - m_elapsedDelayedTimeNanos;
	}
}