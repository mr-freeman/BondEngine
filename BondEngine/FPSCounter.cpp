// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include "FPSCounter.hpp"

namespace be::utils
{
    double clockToMilliseconds(clock_t ticks)
    {
        return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
    }

    void FPSCounter::start_calculation()
    {
        current_ticks = clock();
    }

    void FPSCounter::end_calculation(bool vsync)
    {
        delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene

        if (clockToMilliseconds(delta_ticks) > 1.000)
            fps = CLOCKS_PER_SEC / delta_ticks;
    }
}