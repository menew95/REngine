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

		// 절대 DeltaTime을 구합니다.
		m_unscaledDeltaTimeNanos =
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				m_currentPreTickTime - m_lastPreTickTime).count();

		// 이번 프레임 절대 시간를 응용 프로그램 시간에 더함
		m_unscaledGameTimeNanos += m_unscaledDeltaTimeNanos;

		if (m_timeScale < 0.0001f)
		{
			m_deltaTimeNanos = 0.f;
		}
		else
		{
			m_deltaTimeNanos = m_unscaledDeltaTimeNanos * m_timeScale;

			m_gameTimeNanos += m_deltaTimeNanos;
		}

		m_frameCount += 1;

		// _unscaledDeltaTime => 프레임 전환에 걸린 시간
		// 따라서, 1초를 이 수로 나누면 현 프레임 기준 FPS 나옴
		m_fps = static_cast<uint32_t>(1.f / m_unscaledDeltaTimeNanos);
	}

	void Time::SetTimeScale(float scale)
	{
		assert(scale >= 0.f);

		m_timeScale = scale;
	}
}