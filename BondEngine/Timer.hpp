// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <chrono>

using namespace std::chrono;

namespace be::utils
{
    class Timer
    {
    public:
        
        Timer();

        void Restart();

        double GetElapsedTime() const;
        float GetElapsedTimef() const;

    private:
        std::chrono::high_resolution_clock::time_point m_StartTime;

        bool running = false;
    };
}