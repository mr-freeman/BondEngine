// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <Timer.hpp>

#include <chrono>
    
using namespace std::chrono;

namespace be::utils
{

    Timer::Timer()
    {
        Restart();
    }

    void Timer::Restart()
    {
        m_StartTime = high_resolution_clock().now();
    }

    template <typename T>
    T getElapsedTime(high_resolution_clock::time_point StartTime)
    {
        auto CurrTime = std::chrono::high_resolution_clock::now();
        auto time_span = std::chrono::duration_cast<duration<T>>(CurrTime - StartTime);

        return time_span.count();
    }

    double Timer::GetElapsedTime() const
    {
        return getElapsedTime<double>(m_StartTime);
    }

    float Timer::GetElapsedTimef() const
    {

        return getElapsedTime<float>(m_StartTime);
    }
    /*
    void Timer::start()
    {
        running = true;
        start_time = std::chrono::system_clock::now();
    }

    void Timer::stop()
    {
        end_time = std::chrono::system_clock::now();
        running = false;
    }

    double Timer::getElapsedMiliSeconds()
    {
        std::chrono::time_point<std::chrono::system_clock> end_time_loc;

        if (running)
            end_time_loc = std::chrono::system_clock::now();
        else
            end_time_loc = end_time;

        return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end_time_loc - start_time).count());
    }

    double Timer::getElapsedSeconds()
    {
        return getElapsedMiliSeconds() / 1000.0;
    }*/
}