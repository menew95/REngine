/**

    @file      Time.h
    @brief     engine time class
    @details   엔진 안에서 타임을 관리하는 클래스
    @author    LWT
    @date      10.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <rengine\rengine_dllexport.h>

namespace rengine
{
    class Time
    {
        DECLARE_SINGLETON_CLASS_EXPORT(RENGINE_API, Time)

    public:
        void Initialize();

        void PreTick();

    private:
        std::chrono::system_clock::time_point m_startGameTime;

        std::chrono::system_clock::time_point m_lastPreTickTime;

        std::chrono::system_clock::time_point m_currentPreTickTime;

    private:
        long long m_gameTimeNanos;

        long long m_deltaTimeNanos;

        long long m_elapsedDelayedTimeNanos;

    public:
        inline RENGINE_API static double GetGameTimeMillis() { return Instance.m_gameTimeNanos * NANOS_TO_MILLIS; }

        inline RENGINE_API static double GetDeltaTimeMillis() { return Instance.m_deltaTimeNanos * NANOS_TO_MILLIS; }

        inline RENGINE_API static double GetGameTime() { return Instance.m_gameTimeNanos * NANOS_TO_SECOND; }

        inline RENGINE_API static double GetDeltaTime() { return Instance.m_deltaTimeNanos * NANOS_TO_SECOND; }

        inline RENGINE_API static auto GetGameTimePoint() { return Instance.m_currentPreTickTime; }

    public:
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
}