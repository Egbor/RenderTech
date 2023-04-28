#ifndef TIME_H
#define TIME_H

#include "Engine/EngineDef.h"

namespace Engine {
    class Time {
    private:
        Double m_secondsPerCount;
        Double m_deltaTime;

        Int64 m_baseTime;
        Int64 m_pauseTime;
        Int64 m_stopTime;
        Int64 m_prevTime;
        Int64 m_currTime;

        bool m_stopped;

    public:
        Time();
        Time(const Time&) = default;
        ~Time() = default;

        Float TotalTime() const;
        Float DeltaTime() const;

        void Reset();
        void Start();
        void Stop();
        void Tick();
    };
}

#endif // TIME_H
