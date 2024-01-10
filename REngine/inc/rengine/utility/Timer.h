/**

    @file      Timer.h
    @brief     timer class
    @details   timer utility
    @author    LWT
    @date      10.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <chrono>

#include <rengine\rengine_dllexport.h>

namespace rengine
{
	class RENGINE_API TimerValue
	{
	public:
		const long long m_Nanos;

		[[nodiscard]] double AsSeconds() const { return m_Nanos * NANOS_TO_SECOND; }

		[[nodiscard]] double AsMillis() const { return m_Nanos * NANOS_TO_MILLIS; }

		[[nodiscard]] double AsMicros() const { return m_Nanos * NANOS_TO_MICROS; }

		[[nodiscard]] long long AsNanos() const { return m_Nanos; }

	private:
		static constexpr long long SECOND_TO_MILLIS = 1'000;
		static constexpr long long SECOND_TO_MICROS = 1'000'000;
		static constexpr long long SECOND_TO_NANOS = 1'000'000'000;
		static constexpr double MILLIS_TO_SECOND = 1. / 1'000.;
		static constexpr long long MILLIS_TO_MICROS = 1'000;
		static constexpr long long MILLIS_TO_NANOS = 1'000'000;
		static constexpr double MICROS_TO_SECOND = 1. / 1'000'000.;
		static constexpr double MICROS_TO_MILLIS = 1. / 1'000.;
		static constexpr long long MICROS_TO_NANOS = 1'000;
		static constexpr double NANOS_TO_SECOND = 1. / 1'000'000'000.;
		static constexpr double NANOS_TO_MILLIS = 1. / 1'000'000.;
		static constexpr double NANOS_TO_MICROS = 1. / 1'000.;
	};

	class Timer
	{
	public:
		RENGINE_API void Start()
		{
			m_LapStart = std::chrono::system_clock::now();
		}

		RENGINE_API TimerValue Lap()
		{
			auto _now = std::chrono::system_clock::now();
			std::chrono::nanoseconds _nanos = _now - m_LapStart;
			m_LapStart = _now;

			return { _nanos.count() };
		}

		RENGINE_API void Reset()
		{
			m_LapStart = {};
		}

	private:
		std::chrono::system_clock::time_point m_LapStart;
	};
}