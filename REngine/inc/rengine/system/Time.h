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

        void SetTimeScale(float scale);

    private:
        std::chrono::system_clock::time_point m_startGameTime;

        std::chrono::system_clock::time_point m_lastPreTickTime;

        std::chrono::system_clock::time_point m_currentPreTickTime;

    private:
      
        /**
         * \brief 응용 프로그램을 시작하고 현재까지 흐른 상대 시간
         */
        long long m_gameTimeNanos;

        /**
         * \brief 응용 프로그램을 시작하고 현재까지 흐른 절대 시간
         */
        long long m_unscaledGameTimeNanos;

        /**
         * \brief 지난 프레임과 이번 프레임 사이의 상대 시간
         */
        long long m_deltaTimeNanos;

        /**
         * \brief 지난 프레임과 이번 프레임 사이의 절대 시간
         */
        long long m_unscaledDeltaTimeNanos = 0;

        /**
         * \brief 응용 프로그램 시작 후 총 프레임 수
         */
        uint64_t m_frameCount = 0;

        /**
         * \brief 현재 프레임 기준 1초 당 프레임 수
         */
        uint32_t m_fps = 0;

        /**
         * \brief 엔진의 시간 배수 계수(0보다 아래 값은 불가능하다.)
         */
        double m_timeScale = 1.0;

    public:
        inline RENGINE_API static double GetGameTimeMillis() { return Instance.m_gameTimeNanos * NANOS_TO_MILLIS; }

        inline RENGINE_API static double GetDeltaTimeMillis() { return Instance.m_deltaTimeNanos * NANOS_TO_MILLIS; }

        inline RENGINE_API static double GetGameTime() { return Instance.m_gameTimeNanos * NANOS_TO_SECOND; }

        inline RENGINE_API static double GetDeltaTime() { return Instance.m_deltaTimeNanos * NANOS_TO_SECOND; }

        inline RENGINE_API static auto GetGameTimePoint() { return Instance.m_currentPreTickTime; }

        inline RENGINE_API float GetFPS() const { return static_cast<float>(m_fps); }

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