#include "Engine/Core/System/Time/Time.h"

#include <windows.h>

namespace Engine {
    Time::Time()
        : m_secondsPerCount(0.0), m_deltaTime(-1.0), m_baseTime(0), m_pauseTime(0)
        , m_stopTime(0), m_prevTime(0), m_currTime(0), m_stopped(false) {
        Int64 countsPerSec;
        QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
        m_secondsPerCount = 1.0 / (Double)countsPerSec;
    }

    Float Time::DeltaTime() const {
        return (Float)m_deltaTime;
    }

    Float Time::TotalTime() const {
        if (m_stopped) {
            return (Float)(((m_stopTime - m_pauseTime) - m_baseTime) * m_secondsPerCount);
        } else {
            return (Float)(((m_currTime - m_pauseTime) - m_baseTime) * m_secondsPerCount);
        }
    }

    void Time::Reset() {
        Int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

        m_baseTime = currTime;
        m_prevTime = currTime;
        m_stopTime = 0;
        m_stopped = false;
    }

    void Time::Stop() {
        if (!m_stopped) {
            Int64 currTime;
            QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

            m_stopTime = currTime;
            m_stopped = true;
        }
    }

    void Time::Start() {
        Int64 startTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

        if (m_stopped) {
            m_pauseTime += (startTime - m_stopTime);

            m_prevTime = m_pauseTime;
            m_stopTime = 0;
            m_stopped = false;
        }
    }

    void Time::Tick() {
        if (m_stopped) {
            m_deltaTime = 0.0;
            return;
        }

        Int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        m_currTime = currTime;

        m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;
        m_prevTime = m_currTime;

        if (m_deltaTime < 0.0) {
            m_deltaTime = 0.0;
        }
    }
}
